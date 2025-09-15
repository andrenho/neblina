#include "static_dir_request_handler.hh"

#include <fstream>

#include "arguments.hh"

std::optional<std::string> StaticDirRequestHandler::file_contents(const std::vector<std::string>& file_path) const
{
    std::string path = args().data_dir + "/" + dir_name_;
    for (std::string const& p: file_path)
        path += "/" + p;

    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file)
        return {};

    std::streamsize sz = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string buffer(sz, '\0');
    if (file.read(buffer.data(), sz))
        return buffer;
    else
        return {};

}
