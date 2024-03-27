#include <not_implemented.h>

#include "../include/client_logger_builder.h"
#include <filesystem>
#include <forward_list>

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
            throw std::ios_base::failure("problem with emplace in hash_map\n");
        }
        iter = insert.first;
    }

    iter->second.first.emplace_front(std::filesystem::weakly_canonical(stream_file_path).string());
    return this;


    //throw not_implemented("logger_builder *client_logger_builder::add_file_stream(std::string const &stream_file_path, logger::severity severity)", "your code should be here...");
}

logger_builder* client_logger_builder::add_console_stream(logger::severity severity)
{
    auto iter = _current_streams.find(severity);
    if(iter == _current_streams.end())
    {
        auto insert = _current_streams.emplace(severity, std::make_pair(std::forward_list<client_logger::refcount_stream>(), true));
        if(!insert.second)
        {
            throw std::ios_base::failure("there is no such severity\n");
        }
        iter = insert.first;
    }

    iter->second.second = true;

    return this;

}

void client_logger_builder::parsing_severity(logger::severity severity, nlohmann::json &js)
{
    if(js.empty() || !js.is_object())
    {
        return;
    }

    auto it = _current_streams.find(severity);
    if(it == _current_streams.end())
    {
        auto insert = _current_streams.emplace(severity, std::make_pair(std::forward_list<client_logger::refcount_stream>(), false));
        if(!insert.second)
        {
            throw std::ios_base::failure("there is no such severity\n");
        }
        it = insert.first;
    }

    auto console_iter = js.find("console");
    if(console_iter != js.end() && console_iter->is_boolean())
    {
        it->second.second = console_iter->get<bool>();
    }

    auto path_iter = js.find("path");
    if(path_iter != js.end() && path_iter->is_array())
    {
        nlohmann::json arr = *path_iter;
        for(const nlohmann::json& obj : arr)
        {
            if(!obj.is_string() || obj.empty())
            {
                continue;
            }
            std::string path_file = obj.get<std::string>();
            it->second.first.emplace_front(std::filesystem::weakly_canonical(path_file).string());
        }
    }


}


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

//logger_builder *client_logger_builder::add_file_stream(
//    std::string const &stream_file_path,
//    logger::severity severity)
//{
//    auto iter = _current_streams.find(severity);
//    if(iter == _current_streams.end())
//    {
//        auto insert = _current_streams.emplace(severity, std::make_pair(std::forward_list<client_logger::refcount_stream>(), false));
//        if(!insert.second)
//        {
//            throw std::ios_base::failure("bad_alloc");
//        }
//        iter = insert.first;
//    }
//
//    iter->second.first.emplace_front(std::filesystem::absolute(stream_file_path).string());
//
//    return this;
//
//
//    //throw not_implemented("logger_builder *client_logger_builder::add_file_stream(std::string const &stream_file_path, logger::severity severity)", "your code should be here...");
//}

//logger_builder *client_logger_builder::add_console_stream(
//    logger::severity severity)
//{
//    auto iter = _current_streams.find(severity);
//    if(iter == _current_streams.end())
//    {
//        auto insert = _current_streams.emplace(severity, std::make_pair(std::forward_list<client_logger::refcount_stream>(), true));
//        if(!insert.second)
//        {
//            throw std::ios_base::failure("bad_alloc");
//        }
//        iter = insert.first;
//    }
//
//    iter->second.second = true;
//
//    return this;
//    //throw not_implemented("logger_builder *client_logger_builder::add_console_stream(logger::severity severity)", "your code should be here...");
//}

logger_builder* client_logger_builder::transform_with_configuration(
    std::string const &configuration_file_path,
    std::string const &configuration_path)
{
    std::ifstream file_json(configuration_file_path);
    if(!file_json.is_open())
    {
        throw std::ios_base::failure("fIle" + configuration_file_path + "is not opened");
    }

    nlohmann::json full_data = nlohmann::json::parse(file_json);
    file_json.close();

    auto iter = full_data.find(configuration_path);
    if(iter == full_data.end() || !iter->is_object())
    {
        return this;
    }

    parsing_severity(logger::severity::information, (*iter)["information"]);
    parsing_severity(logger::severity::warning, (*iter)["warning"]);
    parsing_severity(logger::severity::error, (*iter)["error"]);
    parsing_severity(logger::severity::critical, (*iter)["critical"]);
    parsing_severity(logger::severity::debug, (*iter)["debug"]);
    parsing_severity(logger::severity::trace, (*iter)["trace"]);

    auto format = (*iter)["format"];
    if(!format.empty() && format.is_string())
    {
        _format = format.get<std::string>();
    }

    return this;

    //throw not_implemented("logger_builder* client_logger_builder::transform_with_configuration(std::string const &configuration_file_path, std::string const &configuration_path)", "your code should be here...");
}

logger_builder *client_logger_builder::clear()
{
    _format = "%m";
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