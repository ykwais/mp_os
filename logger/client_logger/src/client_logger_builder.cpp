#include <not_implemented.h>

#include "../include/client_logger_builder.h"
#include <filesystem>
#include <forward_list>

//client_logger_builder::client_logger_builder()
//{
//    throw not_implemented("client_logger_builder::client_logger_builder()", "your code should be here...");
//}
//
//client_logger_builder::client_logger_builder(
//    client_logger_builder const &other)
//{
//    throw not_implemented("client_logger_builder::client_logger_builder(client_logger_builder const &other)", "your code should be here...");
//}
//
//client_logger_builder &client_logger_builder::operator=(
//    client_logger_builder const &other)
//{
//    throw not_implemented("client_logger_builder &client_logger_builder::operator=(client_logger_builder const &other)", "your code should be here...");
//}
//
//client_logger_builder::client_logger_builder(
//    client_logger_builder &&other) noexcept
//{
//    throw not_implemented("client_logger_builder::client_logger_builder(client_logger_builder &&other) noexcept", "your code should be here...");
//}
//
//client_logger_builder &client_logger_builder::operator=(
//    client_logger_builder &&other) noexcept
//{
//    throw not_implemented("client_logger_builder &client_logger_builder::operator=(client_logger_builder &&other) noexcept", "your code should be here...");
//}
//
//client_logger_builder::~client_logger_builder() noexcept
//{
//    throw not_implemented("client_logger_builder::~client_logger_builder() noexcept", "your code should be here...");
//}

logger_builder *client_logger_builder::add_file_stream(
    std::string const &stream_file_path,
    logger::severity severity)
{
    auto iter = _current_streams.find(severity);
    if(iter == _current_streams.end())
    {
        auto insert = _current_streams.emplace(severity, std::make_pair(std::forward_list<client_logger::refcount_stream>(), false));
        if(!insert.second)
        {
            throw std::ios_base::failure("bad_alloc");
        }
        iter = insert.first;
    }

    iter->second.first.emplace_front(std::filesystem::absolute(stream_file_path).string());

    return this;


    //throw not_implemented("logger_builder *client_logger_builder::add_file_stream(std::string const &stream_file_path, logger::severity severity)", "your code should be here...");
}

logger_builder *client_logger_builder::add_console_stream(
    logger::severity severity)
{
    auto iter = _current_streams.find(severity);
    if(iter == _current_streams.end())
    {
        auto insert = _current_streams.emplace(severity, std::make_pair(std::forward_list<client_logger::refcount_stream>(), true));
        if(!insert.second)
        {
            throw std::ios_base::failure("bad_alloc");
        }
        iter = insert.first;
    }

    iter->second.second = true;

    return this;
    //throw not_implemented("logger_builder *client_logger_builder::add_console_stream(logger::severity severity)", "your code should be here...");
}

logger_builder* client_logger_builder::transform_with_configuration(
    std::string const &configuration_file_path,
    std::string const &configuration_path)
{
    //json
    throw not_implemented("logger_builder* client_logger_builder::transform_with_configuration(std::string const &configuration_file_path, std::string const &configuration_path)", "your code should be here...");
}

logger_builder *client_logger_builder::clear()
{
    _format.clear();
    _current_streams.clear();
    return this;
    //throw not_implemented("logger_builder *client_logger_builder::clear()", "your code should be here...");
}

logger *client_logger_builder::build() const
{
    return new client_logger(_current_streams, _format);
   //throw not_implemented("logger *client_logger_builder::build() const", "your code should be here...");
}

logger_builder* client_logger_builder::set_format(const std::string &str)
{

    _format = str;
    return this;
}