// File: GlobalAPI.hpp

#pragma once

#include <iostream>

#include "./API.hpp"
#include "ruby.h"

// Function to create an instance of the API class
inline VALUE api_create(VALUE self)
{
  API *api = new API();
  return Data_Wrap_Struct(self, 0, free, api);
}

// Function to call the printVersion method of the API class
inline VALUE api_printVersion(VALUE self)
{
  API *api;
  Data_Get_Struct(self, API, api);
  api->printVersion();
  return Qnil;
}

// Function to call the add method of the API class
inline VALUE api_add(VALUE self, VALUE a, VALUE b)
{
  API *api;
  Data_Get_Struct(self, API, api);
  int result = api->add(FIX2INT(a), FIX2INT(b));
  return INT2FIX(result);  // Convert the result to a Ruby integer
}


inline VALUE utils_init(VALUE self)
{
  Utils::init();
  return Qnil;
}

inline void initRM_API()
{
  std::cout << "initRM_API\n";

  VALUE rb_cAPI = rb_define_class("API", rb_cObject);

  rb_define_alloc_func(rb_cAPI, api_create);
  rb_define_method(rb_cAPI, "printVersion", RUBY_METHOD_FUNC(api_printVersion), 0);
  rb_define_method(rb_cAPI, "add", RUBY_METHOD_FUNC(api_add), 2);

  VALUE rb_UtilsClass = rb_define_class("Utils", rb_cObject);

  rb_define_module_function(rb_UtilsClass, "init", RUBY_METHOD_FUNC(utils_init), 0);

}