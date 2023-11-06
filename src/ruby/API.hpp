#pragma once

#include <iostream>

class API {

 public:

  void printVersion()
  {
    std::cout << "SFML Launcher 0.0.0\n";
  }

  int add(int a, int b)
  {
    return a + b;
  }
};

struct Utils {
  static void init()
  {
    std::cout << "Version: 0.0.0\n";
  }
};
