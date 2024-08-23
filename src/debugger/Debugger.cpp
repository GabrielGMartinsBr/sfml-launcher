#include "Debugger.h"

#include <boost/asio/io_context.hpp>
#include <chrono>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <stdexcept>

#include "AppDefs.h"
#include "Log.hpp"
#include "TcpServer.h"
#include "ValueType.hpp"
#include "debugger/Breakpoints.h"
#include "debugger/DebugUtils.hpp"
#include "debugger/ParentVarLocation.hpp"
#include "debugger/SerializeUtils.hpp"
#include "engnine/Engine.h"

namespace dbg {

using app::StrStream;
using Eng::Engine;
using json = nlohmann::json;

constexpr int PORT = 3333;

#define rb_sourceFile() (ruby_current_node ? ruby_current_node->nd_file : 0)
#define rb_sourceLine() (ruby_current_node ? nd_line(ruby_current_node) : 0)

Debugger& Debugger::getInstance()
{
  static Debugger instance;
  return instance;
}

Debugger::Debugger() :
    breakpoints(Breakpoints::getInstance())
{
  state = DebuggerState::WAITING;
  running = false;
  attached = false;
  shouldStop = false;
}

void Debugger::setState(DebuggerState value)
{
  std::lock_guard<std::mutex> lock(mtx);
  state = value;
}

void Debugger::start()
{
  serverThread = std::make_unique<std::thread>(&Debugger::startServerThread, this);
  rb_add_event_hook(trace_function, RUBY_EVENT_LINE);

  running = true;
}

void Debugger::stop()
{
  io_context.stop();
}

bool Debugger::isRunning()
{
  return running;
}

bool Debugger::isAttached()
{
  return attached;
}

void Debugger::attach()
{
  attached = true;
  setState(DebuggerState::RUNNING);
}

void Debugger::handleContinue()
{
  setState(DebuggerState::RUNNING);
}

void Debugger::handlePause()
{
  setState(DebuggerState::SHOULD_PAUSE);
}

void Debugger::handleStop()
{
  shouldStop = true;
  Engine::Engine::getInstance().stop();
}

void Debugger::handleFetchVariable(VALUE var)
{
  StrStream strStream;

  ValueType type = ValueTypeUtils::getType(var);
  switch (type) {
    case ValueType::OBJECT: {
      SerializeUtils::serializeObjectLayer(strStream, var);
      break;
    }
    case ValueType::ARRAY: {
      SerializeUtils::serializeArrayLayer(strStream, var);
      break;
    }
    case ValueType::HASH: {
      SerializeUtils::serializeHashLayer(strStream, var);
      break;
    }
    default: {
      return;
    }
  }

  String outStr = strStream.str();
  sendDebugVariable(outStr);
}

void Debugger::handleSetVariableValue(ParentVarLocation location, VALUE parent, CStr name, CStr type, CStr value)
{
  VALUE resultValue = setVariableValue(location, parent, name, type, value);
  String resultValueStr = SerializeUtils::getValueString(resultValue);
  emitVariableChangedValue(location, parent, name, type, resultValueStr.c_str());
}

void Debugger::sendIsPaused()
{
  if (server == nullptr) {
    throw std::runtime_error("TcpServer pointer is null.");
  }
  server->sendIsPaused(state == DebuggerState::PAUSED);
}

void Debugger::sendCurrentLine(UInt line)
{
  if (server == nullptr) {
    throw std::runtime_error("TcpServer pointer is null.");
  }
  server->sendCurrentLine(line);
}

void Debugger::sendDebugVariable(String& data)
{
  if (server == nullptr) {
    throw std::runtime_error("TcpServer pointer is null.");
  }
  server->sendDebugVariable(data);
}

void Debugger::sendDebugState(VALUE self, VALUE mid, VALUE classObject)
{
  if (server == nullptr) {
    throw std::runtime_error("TcpServer pointer is null.");
  }
  StrStream ss;
  SerializeUtils::serializeFirstLayer(ss, self, mid, classObject);
  String data = ss.str();
  server->sendDebugState(data);
}

/*
  ------------------------------------------------------

    ⇩⇩⇩⇩⇩⇩⇩⇩⇩ Private ⇩⇩⇩⇩⇩⇩⇩⇩⇩

  ------------------------------------------------------
*/

void Debugger::trace_function(rb_event_t event, NODE* node, VALUE self, ID mid, VALUE classObj)
{
  Debugger& instance = getInstance();
  Engine& engine = Engine::getInstance();

  if (instance.shouldStop) {
    return;
  }

  while (instance.state == DebuggerState::WAITING) {
    if (instance.shouldStop) return;
    engine.update();
    std::this_thread::sleep_for(std::chrono::milliseconds(33));
  }

  if (nd_type(node) != NODE_NEWLINE) {
    return;
  }

  if (instance.state == DebuggerState::PAUSED) {
    return;
  }

  UInt currLine;

  if (instance.state == DebuggerState::SHOULD_PAUSE) {
    instance.setState(DebuggerState::PAUSED);

    Log::out() << "here again?";

    currLine = rb_sourceLine() + 1;

    instance.sendCurrentLine(currLine);
    instance.sendIsPaused();
    instance.sendDebugState(self, mid, classObj);

    while (instance.state == DebuggerState::PAUSED) {
      engine.update();
      std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }

    instance.sendIsPaused();
    instance.sendCurrentLine(0);
  }

  static Breakpoints& breakpoints = Breakpoints::getInstance();
  if (breakpoints.isEmpty()) {
    return;
  }

  auto fileName = rb_sourceFile();
  if (strcmp(fileName, "(eval)") != 0) {
    // Ignore events if line is not from scripts code
    return;
  }

  currLine = rb_sourceLine() + 1;

  if (breakpoints.contains(currLine)) {
    instance.setState(DebuggerState::PAUSED);

    std::cout.flush();
    std::cerr.flush();

    instance.sendIsPaused();
    instance.sendCurrentLine(currLine);
    instance.sendDebugState(self, mid, classObj);

    while (instance.state == DebuggerState::PAUSED) {
      if (instance.shouldStop) return;
      engine.update();
      std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }

    instance.sendIsPaused();
    instance.sendCurrentLine(0);
  }
}

void Debugger::startServerThread()
{
  try {
    server = std::make_unique<TcpServer>(io_context, PORT);
    io_context.run();
  } catch (std::exception& e) {
    Log::err() << "Debugger server error: " << e.what();
  }
}

VALUE Debugger::setVariableValue(ParentVarLocation location, VALUE parent, CStr name, CStr type, CStr valueStr)
{
  VALUE value = DebugUtils::createValue(type, valueStr);

  switch (location) {
    case ParentVarLocation::LOCAL: {
      DebugUtils::setLocalVariable(name, valueStr);
      return DebugUtils::getLocalVariable(name);
    }
    case ParentVarLocation::GLOBAL: {
      DebugUtils::setGlobalVariable(name, value);
      return DebugUtils::getGlobalVariable(name);
    }
    case ParentVarLocation::CLASS: {
      if (parent == 0) {
        throw std::runtime_error("Invalid class object received.");
      }
      DebugUtils::setClassVariable(parent, name, value);
      return DebugUtils::getClassVariable(parent, name);
    }
    case ParentVarLocation::OBJECT: {
      if (parent == 0) {
        throw std::runtime_error("Invalid parent object received.");
      }
      DebugUtils::setInstanceVariable(parent, name, value);
      return DebugUtils::getInstanceVariable(parent, name);
    }
    case ParentVarLocation::ARRAY: {
      if (parent == 0) {
        throw std::runtime_error("Invalid array object received.");
      }
      long index = static_cast<VALUE>(std::stol(name));
      if (index > -1) {
        DebugUtils::setArrayEntry(parent, index, value);
        return DebugUtils::getArrayEntry(parent, index);
      }
      throw std::runtime_error("Invalid array index received.");
    }
    case ParentVarLocation::HASH: {
      if (parent == 0) {
        throw std::runtime_error("Invalid array object received.");
      }
      VALUE key = DebugUtils::parseHashKey(name);
      DebugUtils::setHashEntry(parent, key, value);
      return DebugUtils::lookupHashEntry(parent, key);
    }
  }
}

void Debugger::emitVariableChangedValue(ParentVarLocation location, VALUE parent, CStr name, CStr type, CStr value)
{
  if (server == nullptr) {
    throw std::runtime_error("TcpServer pointer is null.");
  }
  json obj;
  obj["parentLocation"] = location;
  obj["parentVar"] = parent;
  obj["name"] = name;
  obj["type"] = type;
  obj["value"] = value;

  String data = obj.dump();
  server->sendVariableChangedValue(data);
}

}  // namespace dbg