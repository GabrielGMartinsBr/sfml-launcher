#include "Debugger.h"

#include <boost/asio/io_context.hpp>
#include <chrono>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "Log.hpp"
#include "TcpServer.h"
#include "debugger/Breakpoints.h"
#include "debugger/DebugUtils.hpp"
#include "debugger/DebugVariableScope.h"
#include "engnine/Engine.h"

namespace dbg {

using Eng::Engine;

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
  running = false;
  attached = false;
  isPaused = false;
  sentIsPaused = false;
  sentCurrentLine = false;
  shouldPause = false;
  shouldContinue = false;
  shouldStop = false;
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
}

void Debugger::handleContinue()
{
  shouldContinue = true;
}

void Debugger::handlePause()
{
  shouldPause = true;
}

void Debugger::handleStop()
{
  shouldStop = true;
  Engine::Engine::getInstance().stop();
}

void Debugger::handleFetchVariable(String& scope, String& name, VALUE rubyObject)
{
  std::ostringstream strStream;
  strStream << scope.size() << "|" << scope << "|" << name << "|";

  if (DebugVariableScope::LOCAL == scope) {
    DebugUtils::getDebugLocalVariable(strStream, name.c_str());
  }

  if (DebugVariableScope::GLOBAL == scope) {
    DebugUtils::getDebugGlobalVariable(strStream, name.c_str());
  }

  if (DebugVariableScope::CLASS == scope) {
    if (rubyObject == 0) {
      throw std::runtime_error("Invalid class object received.");
    }
    DebugUtils::getDebugGlobalVariable(strStream, rubyObject, name.c_str());
  }

  if (DebugVariableScope::INSTANCE == scope) {
    if (rubyObject == 0) {
      throw std::runtime_error("Invalid instance object received.");
    }
    DebugUtils::getDebugInstanceVariable(strStream, rubyObject, name.c_str());
  }

  String outStr = strStream.str();
  sendDebugVariable(outStr);
}

void Debugger::sendIsPaused()
{
  if (server == nullptr) {
    throw std::runtime_error("TcpServer pointer is null.");
  }
  server->sendIsPaused(isPaused);
  sentIsPaused = true;
}

void Debugger::sendCurrentLine(UInt line)
{
  if (server == nullptr) {
    throw std::runtime_error("TcpServer pointer is null.");
  }
  server->sendCurrentLine(line);
  sentCurrentLine = true;
}

void Debugger::sendDebugVariable(String& data)
{
  if (server == nullptr) {
    throw std::runtime_error("TcpServer pointer is null.");
  }
  server->sendDebugVariable(data);
}

/*
  ------------------------------------------------------

    ⇩⇩⇩⇩⇩⇩⇩⇩⇩ Private ⇩⇩⇩⇩⇩⇩⇩⇩⇩

  ------------------------------------------------------
*/

void Debugger::startServerThread()
{
  try {
    server = std::make_unique<TcpServer>(io_context, PORT);
    io_context.run();
  } catch (std::exception& e) {
    Log::err() << "Debugger server error: " << e.what();
  }
}

void Debugger::sendDebugState(VALUE self, VALUE mid, VALUE classObj)
{
  if (server == nullptr) {
    throw std::runtime_error("TcpServer pointer is null.");
  }
  String data = DebugUtils::getDebugVars(self, mid, classObj);
  server->sendDebugState(data);
}

void Debugger::trace_function(rb_event_t event, NODE* node, VALUE self, ID mid, VALUE classObj)
{
  Debugger& instance = getInstance();
  Engine& engine = Engine::getInstance();

  if (instance.shouldStop) {
    return;
  }

  while (!instance.attached) {
    if (instance.shouldStop) return;
    engine.update();
    std::this_thread::sleep_for(std::chrono::milliseconds(33));
  }

  if (nd_type(node) != NODE_NEWLINE) {
    return;
  }

  UInt currLine;

  if (instance.shouldPause) {
    instance.shouldPause = false;
    instance.shouldContinue = false;
    instance.isPaused = true;

    currLine = rb_sourceLine() + 1;

    instance.sendCurrentLine(currLine);
    instance.sendIsPaused();
    instance.sendDebugState(self, mid, classObj);

    while (!instance.shouldContinue) {
      engine.update();
      std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }

    instance.shouldContinue = false;
    instance.isPaused = false;

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
    // VALUE mod_constants = rb_mod_constants(classObj);
    // rb_p(mod_constants);
    // rb_p(rb_sym_all_symbols());
    // VALUE vars = rb_mod_constants(classObj);

    instance.sendDebugState(self, mid, classObj);

    instance.shouldContinue = false;
    instance.isPaused = true;
    instance.sentIsPaused = false;
    instance.sentCurrentLine = false;

    std::cout.flush();
    std::cerr.flush();

    while (!instance.shouldContinue) {
      if (instance.shouldStop) return;
      if (!instance.sentIsPaused) {
        instance.sendIsPaused();
      }
      if (!instance.sentCurrentLine) {
        instance.sendCurrentLine(currLine);
      }
      engine.update();
      std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }
    instance.isPaused = false;

    instance.sendIsPaused();
    instance.sendCurrentLine(0);
  }
}

}  // namespace dbg