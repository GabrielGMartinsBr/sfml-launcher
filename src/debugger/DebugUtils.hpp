#pragma once

#include <string>

#include "AppDefs.h"
#include "Log.hpp"
#include "ValueType.hpp"
#include "integrator/Convert.hpp"
#include "st.h"

namespace dbg {

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

  static long getArrayLength(VALUE array)
  {
    long length = RARRAY_LEN(array);
    return length;
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
};

}  // namespace dbg