#ifndef LOG_HH
#define LOG_HH

#include <string>

#include <format>
#include <iostream>
#include <utility>

#include "arguments.hh"

template <typename... Args>
void DBG(std::format_string<Args...> fmt, Args&&... a) {
    if (!args().verbose)
        return;
    std::cout << std::format("\e[{}m{:16}: ", args().logging_color, args().service) + std::format(fmt, std::forward<Args>(a)...) + "\e[0m\n";
}

template <typename... Args>
void LOG(std::format_string<Args...> fmt, Args&&... a) {
    std::cout << std::format("\e[{}m{:16}: ", args().logging_color, args().service) + std::format(fmt, std::forward<Args>(a)...) + "\e[0m\n";
}

template <typename... Args>
void ERR(std::format_string<Args...> fmt, Args&&... a) {
    std::cerr << args().service + ": " + std::format(fmt, std::forward<Args>(a)...) + "\n";
}

#endif //LOG_HH
