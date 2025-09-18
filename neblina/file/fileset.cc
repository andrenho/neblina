#include "fileset.hh"

#include "arguments.hh"
#include "gz.hh"
#include "whole_file.hh"
#include "util/filesystem.hh"

void deploy_file(File const& file, fs::path const& path)
{
    fs::create_directories(path.parent_path());

    std::vector<uint8_t> contents = file.is_compressed
        ? gz::gunzip(file.contents, file.compressed_sz)
        : std::vector<uint8_t> { file.contents, file.contents + file.uncompressed_sz };
    whole_file::write(path, contents);
}

void deploy_fileset(FileSet const& fileset, fs::path const& directory)
{
    for (auto const& kv: fileset)
        deploy_file(kv.second, directory / kv.first);
}
