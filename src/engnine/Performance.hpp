#pragma once

#include <chrono>

#include "AppDefs.h"
#include "Log.hpp"

namespace Eng {

using std::chrono::high_resolution_clock;
using std::chrono::time_point;
namespace chrono = std::chrono;

struct Performance {
  high_resolution_clock::time_point startPoint;
  high_resolution_clock::time_point endPoint;
  app::Vector<double> averages;

  Performance() :
      startPoint(now()) { }

  ~Performance()
  {
    stop();
  }

  void start()
  {
    startPoint = now();
  }

  double stop()
  {
    endPoint = now();

    auto startMs = chrono::time_point_cast<chrono::microseconds>(startPoint).time_since_epoch();
    auto endMs = chrono::time_point_cast<chrono::microseconds>(endPoint).time_since_epoch();

    auto duration = chrono::duration_cast<chrono::microseconds>(endMs - startMs);
    double ms = duration.count() * 0.001;

    return ms;
  }

  void average()
  {
    averages.push_back(stop());
    start();
    if (averages.size() % 100 == 99) {
      double t = 0;
      for (double v : averages) {
        t += v;
      }
      Log::out() << "Average: " << (t / averages.size());
      averages.clear();
    }
  }

  static high_resolution_clock::time_point now()
  {
    return high_resolution_clock::now();
  }
};

}  // namespace Eng