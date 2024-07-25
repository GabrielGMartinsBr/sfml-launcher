#pragma once

#include <iostream>
#include <sstream>

class Log {
  struct Out {
    std::ostringstream* stream = nullptr;
    bool lb;

    Out(bool lb)
    {
      this->lb = lb;
    }

    template <typename T>
    Out& operator<<(const T& value)
    {
      if (stream == nullptr) {
        stream = new std::ostringstream;
      }
      (*stream) << value;
      return *this;
    }

    ~Out()
    {
      std::cout << (*stream).str();
      if (lb) {
        std::cout << std::endl;
      }
      delete stream;
    }
  };

  struct Err {
    std::ostringstream* stream = nullptr;
    bool lb;

    Err(bool lb)
    {
      this->lb = lb;
    }

    template <typename T>
    Err& operator<<(const T& value)
    {
      if (stream == nullptr) {
        stream = new std::ostringstream;
      }
      (*stream) << value;
      return *this;
    }

    ~Err()
    {
      std::cerr << (*stream).str();
      if (lb) {
        std::cerr << std::endl;
      }
      delete stream;
    }
  };

 public:
  static Out out(bool lb = true) { return Out(lb); }
  static Err err(bool lb = true) { return Err(lb); }
};
