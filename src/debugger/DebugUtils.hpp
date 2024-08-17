#pragma once

#include <ostream>
#include <sstream>
#include <string>

#include "AppDefs.h"
#include "Log.hpp"
#include "ValueType.hpp"
#include "integrator/Convert.hpp"
#include "st.h"

namespace dbg {

using app::CStr;
using app::OutStrStream;
using app::String;
using app::UPtr;
using app::Vector;
using std::to_string;

struct DebugUtils {
  static const char* getClassName(VALUE classObj)
  {
    return Convert::toCStr(
      rb_class_name(classObj)
    );
  }

  static const char* getClassPath(VALUE classObj)
  {
    return Convert::toCStr(
      rb_class_path(classObj)
    );
  }

  static const char* getClassNameOf(VALUE value)
  {
    VALUE classObj = rb_class_of(value);
    return getClassName(classObj);
  }

  static const char* getClassPathOf(VALUE value)
  {
    VALUE classObj = rb_class_of(value);
    return getClassPath(classObj);
  }

  static const char* getMethodName(ID mid)
  {
    return (mid != 0) ? rb_id2name(mid) : "unknown";
  }

  static UPtr<Vector<String>> instanceVariables(VALUE instanceObj)
  {
    VALUE varsArr = rb_obj_instance_variables(instanceObj);
    return Convert::toCStringVector2(varsArr);
  }

  static UPtr<Vector<String>> classVariables(VALUE instanceObj)
  {
    VALUE varsArr = rb_mod_class_variables(instanceObj);
    return Convert::toCStringVector2(varsArr);
  }

  static UPtr<Vector<String>> moduleConstants(VALUE instanceObj)
  {
    VALUE varsArr = rb_mod_constants(instanceObj);
    return Convert::toCStringVector2(varsArr);
  }

  static UPtr<Vector<String>> globalVariables()
  {
    VALUE varsArr = rb_f_global_variables();
    return Convert::toCStringVector2(varsArr);
  }

  static int ivarTableForeach(st_data_t key, st_data_t value, st_data_t arg)
  {
    auto ivName = rb_id2name((ID)key);
    auto ivValueStr = Convert::toCStr(rb_any_to_s(value));

    printf("%s: %s\n", ivName, ivValueStr);

    return ST_CONTINUE;
  }

  static void printIvTable(VALUE instance)
  {
    struct st_table* iv_table = rb_generic_ivar_table(instance);
    st_data_t* data;

    typedef int (*func_ptr_t)(...);
    st_foreach(iv_table, (func_ptr_t)ivarTableForeach, 0);
  }

  static void printLocalVariables()
  {
    VALUE result = rb_funcall(Qnil, rb_intern("local_variables"), 0);
    UPtr<Vector<String>> names = Convert::toCStringVector2(result);

    // VALUE val = rb_funcall(Qfalse, rb_intern("[]"), 1, ID2SYM(rb_ary_entry(result, 0)));
    // rb_p(val);
    // VALUE first_var_name = rb_ary_entry(result, 0);

    // rb_p(rb_to_id(first_var_name));
    // rb_p(rb_intern(names->at(0).c_str()));
    // rb_p(ID2SYM(rb_to_id(first_var_name)));

    // VALUE var_value = rb_funcall(instance, rb_intern("[]"), 1, ID2SYM(rb_to_id(first_var_name)));
    // rb_p(var_value);

    // Log::out() << Convert::toCStr(ID2SYM(rb_to_id(first_var_name)));

    // VALUE v1 = rb_eval_string("me");
    // rb_p(v1);

    for (const auto& name : *names) {
      VALUE varValue = rb_eval_string(name.c_str());
      Log::out(false) << name << ": ";
      rb_p(varValue);
    }
  }

  static UPtr<Vector<String>> localVariables()
  {
    VALUE result = rb_funcall(Qnil, rb_intern("local_variables"), 0);
    return Convert::toCStringVector2(result);
  }

  static VALUE getLocalVariable(CStr name)
  {
    return rb_eval_string(name);
  }

  static VALUE getInstanceVariable(VALUE instance, CStr name)
  {
    return rb_iv_get(instance, name);
  }

  static VALUE getClassVariable(VALUE instance, CStr name)
  {
    return rb_cv_get(instance, name);
  }

  static VALUE getGlobalVariable(CStr name)
  {
    return rb_gv_get(name);
  }

  static void printType(VALUE obj)
  {
    CStr str = ValueTypeUtils::getTypeStr(obj);
    Log::out() << str;
  }

  static String getDebugVars(VALUE self, ID mid, VALUE classObj)
  {
    OutStrStream outStream;
    OutStrStream metaOut;

    String globalVars = getGlobalVariablesDebug();
    outStream << "globalVars:" << globalVars.size() << '|' << globalVars;

    String localVars = getLocalVariablesDebug();
    outStream << "localVars:" << localVars.size() << '|' << localVars;

    if (TYPE(classObj) != T_FALSE) {
      String className = DebugUtils::getClassName(classObj);
      String classPath = DebugUtils::getClassPath(classObj);
      String classVars = getClassVariablesDebug(classObj);

      metaOut << "className:" << className << '|';
      metaOut << "classPath:" << classPath << '|';
      metaOut << "classObject:" << classObj << '|';

      outStream << "classVars:" << classVars.size() << '|' << classVars;
    }

    if (TYPE(self) != T_FALSE) {
      metaOut << "instanceObject:" << self << '|';

      String instVars = getInstanceVariablesDebug(self);
      outStream << "instanceVars:" << instVars.size() << '|' << instVars;
    }

    String objectsString = metaOut.str();
    outStream << "metadata:" << objectsString.size() << '|' << objectsString;

    return outStream.str();
  }

  static String getVariableDebugValue(VALUE var)
  {
    ValueType type = ValueTypeUtils::getType(var);
    OutStrStream ss;

    switch (type) {
      case ValueType::NIL: {
        serializeNil(ss, var);
        break;
      }
      case ValueType::BOOLEAN: {
        serializeBool(ss, var);
        break;
      }
      case ValueType::FIX_NUM:
      case ValueType::BIG_NUM:
      case ValueType::FLOAT: {
        serializeNumber(ss, var);
        break;
      }
      case ValueType::STRING: {
        serializeString(ss, var);
        break;
      }
      case ValueType::OBJECT: {
        serializeObject(ss, var);
        break;
      }
      default: {
        serializeAny(ss, var);
      }
    }

    return ss.str();
  }

  static String getLocalVariablesDebug()
  {
    auto names = localVariables();
    std::ostringstream strStream;
    for (const String& name : *names) {
      strStream << name << '|';
      VALUE value = getLocalVariable(name.c_str());
      strStream << getVariableDebugValue(value);
    }
    return strStream.str();
  }

  static String getInstanceVariablesDebug(VALUE instance)
  {
    std::ostringstream strStream;
    auto names = instanceVariables(instance);
    for (const String& name : *names) {
      strStream << name << '|';
      VALUE value = getInstanceVariable(instance, name.c_str());
      strStream << getVariableDebugValue(value);
    }
    return strStream.str();
  }

  static String getClassVariablesDebug(VALUE classObj)
  {
    std::ostringstream strStream;
    auto names = classVariables(classObj);
    for (const String& name : *names) {
      strStream << name << '|';
      VALUE value = getClassVariable(classObj, name.c_str());
      strStream << getVariableDebugValue(value);
    }
    return strStream.str();
  }

  static String getGlobalVariablesDebug()
  {
    auto names = globalVariables();
    std::ostringstream strStream;
    for (const String& name : *names) {
      strStream << name << '|';
      VALUE value = getGlobalVariable(name.c_str());
      strStream << getVariableDebugValue(value);
    }
    return strStream.str();
  }

  static void getDebugGlobalVariable(OutStrStream& strStream, CStr name)
  {
    VALUE value = DebugUtils::getGlobalVariable(name);
    getDebugVariable(strStream, value);
  }

  static void getDebugLocalVariable(OutStrStream& strStream, CStr name)
  {
    VALUE value = DebugUtils::getLocalVariable(name);
    getDebugVariable(strStream, value);
  }

  static void getDebugInstanceVariable(OutStrStream& strStream, VALUE instance, CStr name)
  {
    VALUE value = DebugUtils::getInstanceVariable(instance, name);
    getDebugVariable(strStream, value);
  }

  static void getDebugGlobalVariable(OutStrStream& strStream, VALUE classObj, CStr name)
  {
    VALUE value = DebugUtils::getClassVariable(classObj, name);
    getDebugVariable(strStream, value);
  }

  static void getDebugVariable(OutStrStream& strStream, VALUE value)
  {
    int type = TYPE(value);

    if (type != T_OBJECT) {
      strStream << getVariableDebugValue(value);
      return;
    }

    String className = getClassNameOf(value);

    strStream << "object" << '|' << className.size() << '|' << className << '|';

    strStream << getInstanceVariablesDebug(value);
  }

  /*
    Serialize variables
  */

  static void serializeNil(OutStrStream& ss, VALUE var)
  {
    ValueTypeUtils::checkType(var, ValueType::NIL);

    ss << "nil|";
  }

  static void serializeNumber(OutStrStream& ss, VALUE var)
  {
    int type = TYPE(var);

    if (ValueTypeUtils::isFixNumber(type)) {
      int val = Convert::toCInt(var);
      ss << "fix|" << val << '|';
      return;
    }

    if (ValueTypeUtils::isBigNumber(type)) {
      long val = Convert::toCLong(var);
      ss << "big|" << val << '|';
      return;
    }

    if (ValueTypeUtils::isFloat(type)) {
      double val = Convert::toCDouble(var);
      ss << "float|" << val << '|';
      return;
    }
  }

  static void serializeBool(OutStrStream& ss, VALUE var)
  {
    ValueTypeUtils::checkIsBoolean(var);

    bool value = Convert::toCBool(var);
    if (value) {
      ss << "bool|true|";
    } else {
      ss << "bool|false|";
    }
  }

  static void serializeString(OutStrStream& ss, VALUE var)
  {
    String value = Convert::toCStr(var);
    ss << "string:" << value.size() << '|' << value << '|';
  }

  static void serializeObject(OutStrStream& ss, VALUE var)
  {
    ValueTypeUtils::checkType(var, ValueType::OBJECT);

    VALUE valueStr = rb_any_to_s(var);
    String value = Convert::toCStr(valueStr);
    ss << "object:" << value.size() << '|' << value << '|';
  }

  static void serializeAny(OutStrStream& ss, VALUE var)
  {
    VALUE valueStr = rb_any_to_s(var);
    String value = Convert::toCStr(valueStr);
    ss << "any:" << value.size() << '|' << value << '|';
  }
  
};

}  // namespace dbg