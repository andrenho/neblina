#include "service.hh"

#include "util/filesystem.hh"

void Service::create_file_if_it_doesnt_exist(std::filesystem::path const& path, File const& file)
{
    deploy_file(file, path);
}
