#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H

#include <logger.h>
#include <forward_list>
#include <unordered_map>
#include <fstream>

class client_logger_builder;

class client_logger final:
    public logger
{

private:

    static std::unordered_map<std::string, std::pair<size_t, std::ofstream>> _all_streams;

    std::string _format;

    friend client_logger_builder;

    std::string make_format(const std::string& message, logger::severity severity) const;


    class refcount_stream final
    {
    public:

        std::pair<std::string, std::ofstream*> _stream;

    public:

        explicit refcount_stream(const std::string& path);

        refcount_stream(refcount_stream const &other);

        refcount_stream &operator=(refcount_stream const &other);

        refcount_stream(refcount_stream &&other) noexcept;

        refcount_stream &operator=(refcount_stream &&other) noexcept;

        ~refcount_stream();

    };

    std::unordered_map<logger::severity, std::pair<std::forward_list<refcount_stream>, bool>> _out_streams;//int a = 8; int b = 9;  a = b; int c = b;

    client_logger(const std::unordered_map<logger::severity, std::pair<std::forward_list<refcount_stream>, bool>>& current_streams, std::string format) : _out_streams(current_streams), _format(std::move(format)) {}


    enum class type
    {
        DATE,
        TIME,
        MESS,
        SEVER,
        NAF
    };

    static type check_char(char c) noexcept;



public:

//    client_logger(
//        client_logger const &other);
//
//    client_logger &operator=(
//        client_logger const &other);
//
//    client_logger(
//        client_logger &&other) noexcept;
//
//    client_logger &operator=(
//        client_logger &&other) noexcept;
//
//    ~client_logger() noexcept final;

public:

    [[nodiscard]] logger const *log(
        const std::string &message,
        logger::severity severity) const noexcept override;

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H