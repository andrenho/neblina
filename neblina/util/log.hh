#ifndef LOG_HH
#define LOG_HH

#include <string>

#include <format>
#include <iostream>
#include <utility>

#include "arguments.hh"

template <typename... Args>
void log(std::format_string<Args...> fmt, Args&&... a) {
    std::cout << "\e[" + args().logging_color + "m" + args().service + ": " + std::format(fmt, std::forward<Args>(a)...) + "\e[0m\n";
}

template <typename... Args>
void err(std::format_string<Args...> fmt, Args&&... a) {
    std::cerr << args().service + ": " + std::format(fmt, std::forward<Args>(a)...) + "\n";
}

#endif //LOG_HH
