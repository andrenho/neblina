#ifndef ARGUMENTS_HH
#define ARGUMENTS_HH

#include <string>

struct Arguments {
    Arguments(int argc, char* argv[]);

    std::string       service = "watchdog";
    std::string       config_file_path;
    const std::string program_name;

private:
    static void print_help(std::string const& program_name);
};

#endif //ARGUMENTS_HH
