#ifndef SERVICE_HH
#define SERVICE_HH

#include <string>
#include <unistd.h>

#include "util/embed/compressed_data.hh"
#include "arguments.hh"

#define SERVICE_NAME static constexpr std::string_view name

class Service {
public:
    virtual ~Service() = default;

    virtual void init() {}
    virtual void run() = 0;

protected:
    static void create_file_if_it_doesnt_exist(std::string const& path, CompressedData const& data);

    bool should_exit() const { return getppid() == 1; }
};

#endif //SERVICE_HH
