#ifndef ARGUMENTS_HH
#define ARGUMENTS_HH

#include <string>

struct Arguments {
    Arguments(int argc, char* argv[]);

    const std::string program_name;
    std::string       data_dir;
    std::string       service = "orchestrator";

private:
    static void print_help(std::string const& program_name);
};

Arguments const& args(int argc=0, char* argv[]={});

#endif //ARGUMENTS_HH
