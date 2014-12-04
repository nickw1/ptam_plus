
#include "cvd/timer.h"
#include <chrono>

namespace CVD {

long long get_time_of_day_ns()
{
  return (long long)std::chrono::high_resolution_clock::to_time_t(std::chrono::high_resolution_clock::now());
}

}
