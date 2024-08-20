#pragma once

#include <ruby.h>

#include "AppDefs.h"
#include "StringUtils.hpp"
#include "ValueType.hpp"
#include "debugger/DebugUtils.hpp"
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
    ss << "globalVars:" << StringUtils::length(str) << '|' << str;

    groupStream.str("");
    groupStream.clear();

    serializeLocalVarsLayer(groupStream);
    str = groupStream.str();
    ss << "localVars:" << StringUtils::length(str) << '|' << str;

    if (ValueTypeUtils::isFalse(self)) {
      return;
    }

    groupStream.str("");
    groupStream.clear();

    serializeObjectLayer(groupStream, self);
    str = groupStream.str();
    ss << "instanceVars:" << StringUtils::length(str) << '|' << str;
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

    ss << "className:" << (StringUtils::length(className) + 1) << '|' << className << '|';
    ss << "classPath:" << (StringUtils::length(classPath) + 1) << '|' << classPath << '|';
    ss << "classRId|" << classObj << '|';

    String name = "self";
    String type = "object";
    String value = StringUtils::format("<%s>", className.c_str());

    ss << "name:" << (StringUtils::length(name) + 1) << '|' << name << '|';
    ss << "type:" << (StringUtils::length(type) + 1) << '|' << type << '|';
    ss << "value:" << (StringUtils::length(value) + 1) << '|' << value << '|';

    serializeClassVars(groupStream, classObj);
    str = groupStream.str();
    ss << "classVars:" << StringUtils::length(str) << '|' << str;

    groupStream.str("");
    groupStream.clear();

    serializeObjectVars(groupStream, obj);
    str = groupStream.str();

    ss << "instanceRId|" << obj << '|';
    ss << "instanceVars:" << StringUtils::length(str) << '|' << str;
  }

  static void serializeArrayLayer(StrStream &ss, VALUE obj)
  {
    StrStream groupStream;
    String str;

    String className = DebugUtils::getClassNameOf(obj);
    String classPath = DebugUtils::getClassPathOf(obj);
    VALUE classObj = rb_class_of(obj);

    ss << "className:" << (StringUtils::length(className) + 1) << '|' << className << '|';
    ss << "classPath:" << (StringUtils::length(classPath) + 1) << '|' << classPath << '|';
    ss << "classRId|" << classObj << '|';

    String name = "self";
    String type = "object";
    String value = StringUtils::format("<%s>", className.c_str());

    ss << "name:" << (StringUtils::length(name) + 1) << '|' << name << '|';
    ss << "type:" << (StringUtils::length(type) + 1) << '|' << type << '|';
    ss << "value:" << (StringUtils::length(value) + 1) << '|' << value << '|';

    serializeArrayEntries(groupStream, obj);
    str = groupStream.str();

    ss << "instanceRId|" << obj << '|';
    ss << "instanceVars:" << StringUtils::length(str) << '|' << str;
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
      VALUE value = DebugUtils::getInstanceVariable(obj, name.c_str());
      ss << name << '|';
      ss << value << '|';
      serializeSimpleValue(ss, value);
    }
  }

  static void serializeArrayEntries(StrStream &ss, VALUE array)
  {
    VectorPtr<VALUE> entries = DebugUtils::getArrayEntries(array);
    for (int i = 0; i < entries->size(); i++) {
      VALUE entry = entries->at(i);
      ss << '[' << i << "]|";
      ss << entry << '|';
      serializeSimpleValue(ss, entry);
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
        ss << ':' << StringUtils::length(value) << '|' << value << '|';
        break;
      }
      case ValueType::OBJECT: {
        String className = DebugUtils::getClassNameOf(var);
        String value = StringUtils::format("<%s>", className.c_str());
        ss << ':' << StringUtils::length(value) << '|' << value << '|';
        break;
      }
      case ValueType::ARRAY: {
        long length = DebugUtils::getArrayLength(var);
        String value = StringUtils::format("<Array:%i>", length);
        ss << ':' << StringUtils::length(value) << '|' << value << '|';
        break;
      }
      default: {
        VALUE anyStr = rb_any_to_s(var);
        String value = Convert::toCStr(anyStr);
        ss << ':' << StringUtils::length(value) << '|' << value << '|';
        break;
      }
    }
  }
};

}  // namespace dbg