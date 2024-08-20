#pragma once

#include <ruby.h>

#include <stdexcept>

#include "AppDefs.h"
#include "StringUtils.hpp"
#include "ValueType.hpp"
#include "debugger/DebugUtils.hpp"
#include "debugger/DebugVariableScope.h"
#include "integrator/Convert.hpp"

namespace dbg {

using app::StrStream;
using app::StrVector;
using app::VectorPtr;

struct SerializeUtils {
  static void serializeFirstLayer(StrStream &ss, VALUE self, VALUE mid, VALUE classObject)
  {
    StrStream groupStream;
    String str;

    serializeGlobalVarsLayer(groupStream);
    str = groupStream.str();
    ss << "globalVars:" << str.size() << '|' << str;

    groupStream.str("");
    groupStream.clear();

    serializeLocalVarsLayer(groupStream);
    str = groupStream.str();
    ss << "localVars:" << str.size() << '|' << str;

    if (ValueTypeUtils::isFalse(self)) {
      return;
    }

    groupStream.str("");
    groupStream.clear();

    serializeObjectLayer(groupStream, self);
    str = groupStream.str();
    ss << "instanceVars:" << str.size() << '|' << str;
  }

  static void serializeFetchVariable(StrStream &strStream, String &scope, CStr name, VALUE parent = 0)
  {
    VALUE value = 0;

    if (DebugVariableScope::GLOBAL == scope) {
      value = DebugUtils::getGlobalVariable(name);
    } else if (DebugVariableScope::LOCAL == scope) {
      value = DebugUtils::getLocalVariable(name);
    } else if (DebugVariableScope::CLASS == scope) {
      if (parent == 0) {
        throw std::runtime_error("Invalid parent object id.");
      }
      value = DebugUtils::getClassVariable(parent, name);
    } else if (DebugVariableScope::INSTANCE == scope) {
      if (parent == 0) {
        throw std::runtime_error("Invalid parent object id.");
      }
      value = DebugUtils::getInstanceVariable(parent, name);
    }

    serializeObjectLayer(strStream, value);
  }

  static void serializeGlobalVarsLayer(StrStream &ss)
  {
    StrVector names = DebugUtils::globalVariables();
    for (const String &name : *names) {
      ss << name << '|';
      VALUE value = DebugUtils::getGlobalVariable(name.c_str());
      ss << value << '|';
      serializeSimpleValue(ss, value);
    }
  }

  static void serializeLocalVarsLayer(StrStream &ss)
  {
    StrVector names = DebugUtils::localVariables();
    for (const String &name : *names) {
      ss << name << '|';
      VALUE value = DebugUtils::getLocalVariable(name.c_str());
      ss << value << '|';
      serializeSimpleValue(ss, value);
    }
  }

  static void serializeObjectLayer(StrStream &ss, VALUE obj)
  {
    StrStream groupStream;
    String str;

    String className = DebugUtils::getClassNameOf(obj);
    String classPath = DebugUtils::getClassPathOf(obj);
    VALUE classObj = rb_class_of(obj);

    ss << "className:" << (className.size() + 1) << '|' << className << '|';
    ss << "classPath:" << (classPath.size() + 1) << '|' << classPath << '|';
    ss << "classRId|" << classObj << '|';

    String name = "self";
    String type = "object";
    String value = StringUtils::format("<%s>", className.c_str());

    ss << "name:" << (name.size() + 1) << '|' << name << '|';
    ss << "type:" << (type.size() + 1) << '|' << type << '|';
    ss << "value:" << (value.size() + 1) << '|' << value << '|';

    serializeClassVars(groupStream, classObj);
    str = groupStream.str();
    ss << "classVars:" << str.size() << '|' << str;

    groupStream.str("");
    groupStream.clear();

    serializeObjectVars(groupStream, obj);
    str = groupStream.str();

    ss << "instanceRId|" << obj << '|';
    ss << "instanceVars:" << str.size() << '|' << str;
  }

  static void serializeClassVars(StrStream &ss, VALUE classObj)
  {
    StrVector names = DebugUtils::classVariables(classObj);
    for (const String &name : *names) {
      ss << name << '|';
      VALUE value = DebugUtils::getInstanceVariable(classObj, name.c_str());
      ss << value << '|';
      serializeSimpleValue(ss, value);
    }
  }

  static void serializeObjectVars(StrStream &ss, VALUE obj)
  {
    StrVector names = DebugUtils::instanceVariables(obj);
    for (const String &name : *names) {
      ss << name << '|';
      VALUE value = DebugUtils::getInstanceVariable(obj, name.c_str());
      ss << value << '|';
      serializeSimpleValue(ss, value);
    }
  }

  static void serializeSimpleValue(StrStream &ss, VALUE var)
  {
    ValueType type = ValueTypeUtils::getType(var);

    String typeStr = ValueTypeUtils::getTypeStr(var);
    ss << typeStr;

    switch (type) {
      case ValueType::NIL: {
        ss << '|';
        break;
      }
      case ValueType::FIX_NUM: {
        int value = Convert::toCInt(var);
        ss << '|' << value << '|';
        break;
      }
      case ValueType::BIG_NUM: {
        long value = Convert::toCLong(var);
        ss << '|' << value << '|';
        break;
      }
      case ValueType::FLOAT: {
        double value = Convert::toCDouble(var);
        ss << '|' << value << '|';
        break;
      }
      case ValueType::BOOLEAN: {
        double value = Convert::toCBool(var);
        ss << '|' << value << '|';
        break;
      }
      case ValueType::STRING: {
        String value = Convert::toCStr(var);
        ss << ':' << value.size() << '|' << value << '|';
        break;
      }
      case ValueType::OBJECT: {
        String className = DebugUtils::getClassNameOf(var);
        String value = StringUtils::format("<%s>", className.c_str());
        ss << ':' << value.size() << '|' << value << '|';
        break;
      }
      case ValueType::ARRAY: {
        long length = DebugUtils::getArrayLength(var);
        String value = StringUtils::format("<Array:%i>", length);
        ss << ':' << value.size() << '|' << value << '|';
        break;
      }
      default: {
        VALUE anyStr = rb_any_to_s(var);
        String value = Convert::toCStr(anyStr);
        ss << ':' << value.size() << '|' << value << '|';
        break;
      }
    }
  }
};

}  // namespace dbg