//#include <not_implemented.h>
#include <httplib.h>
#include "../include/server_logger.h"

#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#endif

//server_logger::server_logger(
//    server_logger const &other)
//{
//    throw not_implemented("server_logger::server_logger(server_logger const &other)", "your code should be here...");
//}
//
//server_logger &server_logger::operator=(
//    server_logger const &other)
//{
//    throw not_implemented("server_logger &server_logger::operator=(server_logger const &other)", "your code should be here...");
//}

//server_logger::server_logger(
//    server_logger &&other) noexcept
//{
//    throw not_implemented("server_logger::server_logger(server_logger &&other) noexcept", "your code should be here...");
//}
//
//server_logger &server_logger::operator=(
//    server_logger &&other) noexcept
//{
//    throw not_implemented("server_logger &server_logger::operator=(server_logger &&other) noexcept", "your code should be here...");
//}

server_logger::~server_logger() noexcept
{
    httplib::Params parametrs;
    parametrs.emplace("pid", std::to_string(getter_pid()));
    _client.Get("/kill", parametrs, httplib::Headers());
}

logger const *server_logger::log(
    const std::string &text,
    logger::severity severity) const noexcept
{
    std::stringstream str_str;

    str_str << "<" << current_date_to_string() << ">" << " [" << current_time_to_string() << "] {"
    << severity_to_string(severity) << "}" << text;

    httplib::Params parametrs;
    parametrs.emplace("pid", std::to_string(getter_pid()));
    parametrs.emplace("severity", severity_to_string(severity));
    parametrs.emplace("message", str_str.str());

    _client.Get("/log", parametrs, httplib::Headers());

    return this;

}

server_logger::server_logger(const std::string& dest, const std::unordered_map<logger::severity, std::pair<bool, std::string>>& streams) : _client(dest)
{
    for(auto& elem : streams)
    {
        httplib::Params parametrs;
        parametrs.emplace("pid", std::to_string(getter_pid()));
        parametrs.emplace("severity", server_logger::severity_to_string(elem.first));
        parametrs.emplace("console", elem.second.first ? "1" : "0");
        parametrs.emplace("path_to_file", elem.second.second);

        _client.Get("/init", parametrs, httplib::Headers());
    }
}

int server_logger::getter_pid()
{
#ifdef _WIN32
    return _getpid();
#else
    return getpid();
#endif
}

