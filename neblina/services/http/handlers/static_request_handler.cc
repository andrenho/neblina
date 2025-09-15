#include "static_request_handler.hh"

HttpResponse StaticRequestHandler::get(const HttpRequest& request, const URLParameters& url_parameters,
    const QueryParameters& query_parameters) const
{
    // remove query parameters from resource
    std::string resource = request.resource();
    size_t j = resource.find('?');
    if (j != std::string::npos)
        resource = resource.substr(0, j-1);

    if (resource == "/")
        resource = "/index.html";

    // split resource by '.'
    std::vector<std::string> file_path;
    for (;;) {
        size_t i = resource.find('/');
        if (i == std::string::npos)
            break;
        if (i == 0) {
            resource = resource.substr(1);
            continue;
        }
        if (resource.substr(0, i) == "..")
            throw BadRequestException();
        file_path.emplace_back(resource.substr(0, i));
        resource = resource.substr(i + 1);
    }
    file_path.emplace_back(resource);

    // find extension
    j = resource.find('.');
    std::string extension;
    if (j != std::string::npos)
        extension = resource.substr(j);

    // load file, or return 404
    auto contents = file_contents(file_path);
    if (contents)
        return HttpResponse(200, ContentType::best_content_type(extension), *contents);
    else
        throw NotFoundException();
}