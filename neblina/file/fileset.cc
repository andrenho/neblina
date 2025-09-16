#include "fileset.hh"

#include "arguments.hh"
#include "gz.hh"
#include "whole_file.hh"
#include "util/filesystem.hh"

void deploy_fileset(FileSet const& fileset, std::string const& directory)
{
    for (auto const& kv: fileset) {
        auto path = fs::path(args().data_dir + "/" + kv.first);
        fs::create_directories(path.parent_path());

        File const& f = kv.second;
        std::vector<uint8_t> contents = f.is_compressed ? gz::gunzip(f.contents, f.compressed_sz) : std::vector<uint8_t> { f.contents, f.contents + f.uncompressed_sz };
        whole_file::write(path, contents);
    }
}
