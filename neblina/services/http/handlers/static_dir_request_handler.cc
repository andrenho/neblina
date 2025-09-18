#include "static_dir_request_handler.hh"

#include <fstream>

#include "arguments.hh"
#include "util/filesystem.hh"

HttpResponse StaticDirRequestHandler::file_contents(std::vector<std::string> const& file_path, std::string const& cached_etag) const
{
    // create file path
    std::string path = args().data_dir / dir_name_;
    for (std::string const& p: file_path)
        path += "/" + p;

    // figure out ETag
    std::string etag;
    try {
        fs::file_time_type last_write_time = fs::last_write_time(path);
        etag = std::format("{}", last_write_time);
    } catch (std::filesystem::filesystem_error&) {
        throw NotFoundException();
    }

    // check by the If-None-Match header if the resource is cached
    if (etag == cached_etag)
        return HttpResponse(304);

    // read file
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file)
        return {};
    std::streamsize sz = file.tellg();
    file.seekg(0, std::ios::beg);

    // respond to the request
    std::string buffer(sz, '\0');
    if (file.read(buffer.data(), sz)) {
        HttpResponse response(200, ContentType::best_content_type(fs::path(path).extension().string()), buffer);
        if (!etag.empty())
            response.headers["ETag"] = etag;
        return response;
    } else {
        throw InternalServerErrorException();
    }
}
