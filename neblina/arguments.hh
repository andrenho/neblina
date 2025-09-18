#ifndef ARGUMENTS_HH
#define ARGUMENTS_HH

#include <string>

#include "util/chrono.hh"
#include "util/filesystem.hh"

struct Arguments {
    Arguments(int argc, char* argv[]);

    const fs::path    program_name;
    fs::path          data_dir;
    std::string       service = "orchestrator";
    int               port = 0;
    bool              open_to_world = false;
    ms                frequency = 1s;
    std::string       logging_color = "0";

    [[nodiscard]] std::string config_dir() const { return data_dir/"config"; }

private:
    static void print_help(std::string const& program_name);
};

Arguments const& args(int argc=0, char* argv[]={});

#endif //ARGUMENTS_HH
