#include <not_implemented.h>

#include "../include/client_logger.h"
#include <sstream>

std::unordered_map<std::string, std::pair<size_t, std::ofstream>> client_logger::_all_streams;


//client_logger::client_logger(
//    client_logger const &other)
//{
//    throw not_implemented("client_logger::client_logger(client_logger const &other)", "your code should be here...");
//}
//
//client_logger &client_logger::operator=(
//    client_logger const &other)
//{
//    throw not_implemented("client_logger &client_logger::operator=(client_logger const &other)", "your code should be here...");
//}
//
//client_logger::client_logger(
//    client_logger &&other) noexcept
//{
//    throw not_implemented("client_logger::client_logger(client_logger &&other) noexcept", "your code should be here...");
//}
//
//client_logger &client_logger::operator=(
//    client_logger &&other) noexcept
//{
//    throw not_implemented("client_logger &client_logger::operator=(client_logger &&other) noexcept", "your code should be here...");
//}
//
//client_logger::~client_logger() noexcept
//{
//    throw not_implemented("client_logger::~client_logger() noexcept", "your code should be here...");
//}

client_logger::type client_logger::check_char(char c) noexcept
{
    switch(c)
    {
        case 't':
            return type::TIME;
        case 'd':
            return type::DATE;
        case 's':
            return type::SEVER;
        case 'm':
            return type::MESS;
        default:
            return type::NAF;
    }
}

logger const *client_logger::log(
    const std::string &text,
    logger::severity severity) const noexcept
{
    std::string res = make_format(text, severity);

    auto iter = _out_streams.find(severity);

    if(iter != _out_streams.end())
    {
        if(iter->second.second)
        {
            std::cout << res << std::endl;
        }

        for(auto &elem : iter->second.first)
        {
            *(elem._stream.second) << res << std::endl;
        }
    }

    return this;
    //throw not_implemented("logger const *client_logger::log(const std::string &text, logger::severity severity) const noexcept", "your code should be here...");
}

std::string client_logger::make_format(const std::string &message, logger::severity severity) const
{

    std::stringstream res;
    for(auto iter = _format.begin(), end = _format.end(); iter != end; ++iter)
    {
        type check = type::NAF;

        if(*iter == '%')
        {
            check = check_char(*(iter+1));

        }
        if(check != type::NAF)
        {
            switch(check)
            {
                case type::DATE:
                    res << current_date_to_string();
                    break;
                case type::TIME:
                    res << current_time_to_string();
                    break;
                case type::MESS:
                    res << message;
                    break;
                default:
                    res << severity_to_string(severity);
                    break;
            }

            ++iter;

        }
        else
        {
            res << *iter;
        }
    }
    return res.str();

    //throw not_implemented("std::string client_logger::make_format(const std::string &message)", "your code should be here...");
}

client_logger::refcount_stream::refcount_stream(const std::string &path) : _stream(std::make_pair(path, nullptr)) {}

client_logger::refcount_stream::refcount_stream(const client_logger::refcount_stream &other) : _stream(std::make_pair(other._stream.first, nullptr))
{
    auto iter = _all_streams.find(_stream.first);

    if(iter != _all_streams.end())
    {
        ++iter->second.first;
        _stream.second = &iter->second.second;
    }
    else {
        auto insert = _all_streams.emplace(_stream.first, std::make_pair(1, std::ofstream(_stream.first)));//вот здесь мы открываем поток вывода
        if (!insert.second || !insert.first->second.second)
        {
            if(!insert.second)
            {
                _all_streams.erase(insert.first);
            }
            throw std::runtime_error("File "+ other._stream.first + " could not be opened");
        }
        _stream.second = &insert.first->second.second;

    }

}

client_logger::refcount_stream& client_logger::refcount_stream::operator=(const client_logger::refcount_stream &other)
{
    if(this == &other)
    {
        return *this;
    }

    if(_stream.second != nullptr)
    {
        auto iter = _all_streams.find(_stream.first);

        if(iter != _all_streams.end())
        {
            iter->second.first--;

            if(iter->second.first == 0)
            {
                iter->second.second.close();
                _all_streams.erase(iter);
            }
        }

    }

    _stream.first = other._stream.first;
    _stream.second = other._stream.second;

    if(_stream.second != nullptr)
    {
        auto it = _all_streams.find(_stream.first);
        it->second.first++;
    }

    return *this;


}

client_logger::refcount_stream::refcount_stream(client_logger::refcount_stream &&other) noexcept
{
    std::swap(_stream.first, other._stream.first);
    _stream.second = other._stream.second;//exchange!!!
    other._stream.second = nullptr;
}

client_logger::refcount_stream &client_logger::refcount_stream::operator=(client_logger::refcount_stream &&other) noexcept
{
    if(this == &other)
    {
        return *this;
    }

    std::swap(_stream.first, other._stream.first);
    std::swap(_stream.second, other._stream.second);

    return *this;

}

client_logger::refcount_stream::~refcount_stream()
{
    if(_stream.second != nullptr)
    {
        auto iter = _all_streams.find(_stream.first);
        if(iter != _all_streams.end())
        {
            iter->second.first--;
            if(iter->second.first == 0)
            {
                iter->second.second.close();
                _all_streams.erase(_stream.first);
            }
        }

    }

}

