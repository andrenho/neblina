#ifndef SERVICE_HH
#define SERVICE_HH

#include <unistd.h>

#include "file/fileset.hh"

#define SERVICE_NAME static constexpr std::string_view name

extern bool termination_requested;

class Service {
public:
    virtual ~Service() = default;

    virtual void init() {}
    virtual void run() = 0;

protected:
    static void create_file_if_it_doesnt_exist(std::filesystem::path const& path, File const& file);

    [[nodiscard]] static bool should_exit() { return getppid() == 1 || termination_requested; }
};

#endif //SERVICE_HH
