#ifndef CHRONO_HH
#define CHRONO_HH

#include <chrono>

using namespace std::chrono_literals;
using hr = std::chrono::high_resolution_clock;
using duration_ms = hr::duration<std::chrono::milliseconds>;

#endif //CHRONO_HH
