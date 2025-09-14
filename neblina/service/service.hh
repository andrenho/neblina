#ifndef SERVICE_HH
#define SERVICE_HH

#include <string>

#include "util/embed/compressed_data.hh"

#define SERVICE_NAME static constexpr std::string_view name

class Service {
public:
    virtual ~Service() = default;

    virtual void init() {}
    [[noreturn]] virtual void run() = 0;

protected:
    static void create_file_if_it_doesnt_exist(std::string const& path, CompressedData const& data);
};

#endif //SERVICE_HH
