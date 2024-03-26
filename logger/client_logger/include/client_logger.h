#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H

#include <logger.h>
#include <list>
#include <unordered_map>
#include <fstream>

class client_logger_builder;

class client_logger final:
    public logger
{

private:

    static std::unordered_map<std::string, std::pair<size_t, std::ofstream>> _all_streams;

    std::unordered_map<logger::severity, std::pair<std::list<std::pair<std::string, std::ofstream*>>, bool>> _out_streams;

    std::string _format;

    friend client_logger_builder;

    std::string make_format(const std::string& message);

    client_logger(std::unordered_map<logger::severity, std::pair<std::list<std::pair<std::string, std::ofstream*>>, bool>>& current_streams, std::string format);

    class refcount_stream final
    {
    private:

        std::pair<std::string, std::ofstream*> _stream;

    public:

        refcount_stream(std::string path);

        refcount_stream(refcount_stream const &other);

        refcount_stream &operator=(refcount_stream const &other);

        refcount_stream(refcount_stream &&other) noexcept;

        refcount_stream &operator=(refcount_stream &&other) noexcept;

        ~refcount_stream();

    };

public:

    client_logger(
        client_logger const &other);

    client_logger &operator=(
        client_logger const &other);

    client_logger(
        client_logger &&other) noexcept;

    client_logger &operator=(
        client_logger &&other) noexcept;

    ~client_logger() noexcept final;

public:

    [[nodiscard]] logger const *log(
        const std::string &message,
        logger::severity severity) const noexcept override;

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H