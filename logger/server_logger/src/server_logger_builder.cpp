#include <not_implemented.h>

#include "../include/server_logger_builder.h"

//server_logger_builder::server_logger_builder()
//{
//    throw not_implemented("server_logger_builder::server_logger_builder()", "your code should be here...");
//}

//server_logger_builder::server_logger_builder(
//    server_logger_builder const &other)
//{
//    throw not_implemented("server_logger_builder::server_logger_builder(server_logger_builder const &other)", "your code should be here...");
//}
//
//server_logger_builder &server_logger_builder::operator=(
//    server_logger_builder const &other)
//{
//    throw not_implemented("server_logger_builder &server_logger_builder::operator=(server_logger_builder const &other)", "your code should be here...");
//}
//
//server_logger_builder::server_logger_builder(
//    server_logger_builder &&other) noexcept
//{
//    throw not_implemented("server_logger_builder::server_logger_builder(server_logger_builder &&other) noexcept", "your code should be here...");
//}
//
//server_logger_builder &server_logger_builder::operator=(
//    server_logger_builder &&other) noexcept
//{
//    throw not_implemented("server_logger_builder &server_logger_builder::operator=(server_logger_builder &&other) noexcept", "your code should be here...");
//}
//
//server_logger_builder::~server_logger_builder() noexcept
//{
//    throw not_implemented("server_logger_builder::~server_logger_builder() noexcept", "your code should be here...");
//}

logger_builder *server_logger_builder::add_file_stream(
    std::string const &stream_file_path,
    logger::severity severity)
{
    auto iter = _streams.find(severity);

    if(iter == _streams.end())
    {
        auto insert = _streams.emplace(severity, std::make_pair(false, std::string()));

        if(!insert.second)
        {
            throw std::ios_base::failure("bad_alloc");
        }

        iter = insert.first;
    }

    iter->second.second = stream_file_path;

    return this;


    //throw not_implemented("logger_builder *server_logger_builder::add_file_stream(std::string const &stream_file_path, logger::severity severity)", "your code should be here...");
}

logger_builder *server_logger_builder::add_console_stream(
    logger::severity severity)
{
    auto iter = _streams.find(severity);

    if(iter == _streams.end())
    {
        auto insert = _streams.emplace(severity, std::make_pair(true, std::string()));

        if(!insert.second)
        {
            throw std::ios_base::failure("bad_alloc");
        }
        iter = insert.first;
    }

    iter->second.first = true;

    return this;


    //throw not_implemented("logger_builder *server_logger_builder::add_console_stream(logger::severity severity)", "your code should be here...");
}

logger_builder* server_logger_builder::transform_with_configuration(
    std::string const &configuration_file_path,
    std::string const &configuration_path)
{
    throw not_implemented("logger_builder* server_logger_builder::transform_with_configuration(std::string const &configuration_file_path, std::string const &configuration_path)", "invalid");
}

logger_builder *server_logger_builder::clear()
{
    _streams.clear();
    _dest = "http://127.0.0.1:8080";
    return this;
    //throw not_implemented("logger_builder *server_logger_builder::clear()", "your code should be here...");
}

logger *server_logger_builder::build() const
{
    return new server_logger(_dest, _streams);
    //throw not_implemented("logger *server_logger_builder::build() const", "your code should be here...");
}

logger_builder* server_logger_builder::set_dest(const std::string& str)
{
    _dest = str;
    return this;
}

logger_builder* server_logger_builder::set_format(const std::string& str)
{
    throw not_implemented("logger_builder* server_logger_builder::set_format(const std::string& str)", "invalid");
}