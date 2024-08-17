#pragma once

#include <ruby.h>

#include <stdexcept>

#include "AppDefs.h"

enum struct ValueType {
  NONE,
  NIL,
  OBJECT,
  CLASS,
  I_CLASS,
  MODULE,
  STRING,
  REGEXP,
  BOOLEAN,
  FIX_NUM,
  BIG_NUM,
  FLOAT,
  STRUCT,
  ARRAY,
  HASH,
  FILE,
  DATA,
  MATCH,
  BLK_TAG,
  UNDEF,
  VAR_MAP,
  SYMBOL,
  SCOPE,
  NODE
};

struct ValueTypeUtils {
  static inline void checkType(VALUE var, ValueType expected)
  {
    ValueType type = getType(var);
    if (type != expected) {
      throw std::runtime_error("Invalid type.");
    }
  }
  static inline void checkIsNumber(VALUE var)
  {
    if (!isNumber(var)) {
      throw std::runtime_error("Invalid type.");
    }
  }
  static inline void checkIsBoolean(VALUE var)
  {
    if (!isBoolean(var)) {
      throw std::runtime_error("Invalid type.");
    }
  }

  static inline bool isNil(VALUE var)
  {
    return TYPE(var) == T_NIL;
  }

  static inline bool isBoolean(VALUE var)
  {
    int type = TYPE(var);
    return isBoolean(type);
  }

  static inline bool isBoolean(int type)
  {
    return type == T_FALSE || type == T_TRUE;
  }

  static inline bool isNumber(VALUE var)
  {
    int type = TYPE(var);
    return isNumber(type);
  }

  static inline bool isNumber(int type)
  {
    return type == T_FIXNUM || type == T_BIGNUM || type == T_FLOAT;
  }

  static inline bool isFixNumber(VALUE var)
  {
    int type = TYPE(var);
    return isFixNumber(type);
  }

  static inline bool isFixNumber(int type)
  {
    return type == T_FIXNUM;
  }

  static inline bool isBigNumber(VALUE var)
  {
    int type = TYPE(var);
    return isBigNumber(type);
  }
  static inline bool isBigNumber(int type)
  {
    return type == T_BIGNUM;
  }

  static inline bool isFloat(VALUE var)
  {
    int type = TYPE(var);
    return isFloat(type);
  }

  static inline bool isFloat(int type)
  {
    return type == T_FLOAT;
  }

  static inline bool isString(VALUE var)
  {
    return TYPE(var) == T_STRING;
  }

  static inline bool isClass(VALUE var)
  {
    return TYPE(var) == T_CLASS;
  }

  static inline bool isStruct(VALUE var)
  {
    return TYPE(var) == T_STRUCT;
  }

  static inline bool isObject(VALUE var)
  {
    return TYPE(var) == T_OBJECT;
  }

  static inline bool isFalse(VALUE var)
  {
    return TYPE(var) == T_FALSE;
  }

  static inline bool isTrue(VALUE var)
  {
    return TYPE(var) == T_TRUE;
  }

  static ValueType getType(VALUE value)
  {
    return getType(TYPE(value));
  }

  static ValueType getType(int type)
  {
    switch (type) {
      case T_NIL:
        return ValueType::NIL;
      case T_OBJECT:
        return ValueType::OBJECT;
      case T_CLASS:
        return ValueType::CLASS;
      case T_ICLASS:
        return ValueType::I_CLASS;
      case T_MODULE:
        return ValueType::MODULE;
      case T_FLOAT:
        return ValueType::FLOAT;
      case T_STRING:
        return ValueType::STRING;
      case T_REGEXP:
        return ValueType::REGEXP;
      case T_ARRAY:
        return ValueType::ARRAY;
      case T_FIXNUM:
        return ValueType::FIX_NUM;
      case T_HASH:
        return ValueType::HASH;
      case T_STRUCT:
        return ValueType::STRUCT;
      case T_BIGNUM:
        return ValueType::BIG_NUM;
      case T_FILE:
        return ValueType::FILE;
      case T_TRUE:
      case T_FALSE:
        return ValueType::BOOLEAN;
      case T_DATA:
        return ValueType::DATA;
      case T_MATCH:
        return ValueType::MATCH;
      case T_SYMBOL:
        return ValueType::SYMBOL;
      case T_BLKTAG:
        return ValueType::BLK_TAG;
      case T_UNDEF:
        return ValueType::UNDEF;
      case T_VARMAP:
        return ValueType::VAR_MAP;
      case T_SCOPE:
        return ValueType::SCOPE;
      case T_NODE:
        return ValueType::NONE;
      case T_NONE:
        return ValueType::NODE;

      default: {
        throw std::runtime_error("Variable type conversion failed, unexpected variable type.");
      }
    }
  }

  static app::CStr getTypeStr(VALUE var)
  {
    int type = TYPE(var);
    return getTypeStr(type);
  }

  static app::CStr getTypeStr(int type)
  {
    switch (type) {
      case T_NIL:
        return "nil";
      case T_OBJECT:
        return "object";
      case T_CLASS:
        return "class";
      case T_ICLASS:
        return "i_class";
      case T_MODULE:
        return "module";
      case T_FLOAT:
        return "float";
      case T_STRING:
        return "string";
      case T_REGEXP:
        return "regexp";
      case T_ARRAY:
        return "array";
      case T_FIXNUM:
        return "fix";
      case T_HASH:
        return "hash";
      case T_STRUCT:
        return "struct";
      case T_BIGNUM:
        return "big";
      case T_FILE:
        return "file";
      case T_TRUE:
      case T_FALSE:
        return "bool";
      case T_DATA:
        return "data";
      case T_MATCH:
        return "match";
      case T_SYMBOL:
        return "symbol";
      case T_BLKTAG:
        return "blk_tag";
      case T_UNDEF:
        return "undef";
      case T_VARMAP:
        return "var_map";
      case T_SCOPE:
        return "scope";
      case T_NODE:
        return "node";
      case T_NONE:
        return "none";
      default: {
        throw std::runtime_error("Variable type conversion failed, unexpected variable type.");
      }
    }
  }
};
