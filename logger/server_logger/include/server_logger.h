#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_SERVER_LOGGER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_SERVER_LOGGER_H

#include <logger.h>
#include <unordered_map>
#include <httplib.h>


class server_logger_builder;

class server_logger final:
    public logger
{
private:


    mutable httplib::Client _client;

    friend server_logger_builder;

    server_logger(const std::string& dest, const std::unordered_map<logger::severity, std::pair<bool, std::string>>& streams);

    static int getter_pid();

public:

    server_logger(
        server_logger const &other) = delete;

    server_logger &operator=(
        server_logger const &other) = delete;

    server_logger(
        server_logger &&other) noexcept = default;

    server_logger &operator=(
        server_logger &&other) noexcept = delete;

    ~server_logger() noexcept final;

public:

    [[nodiscard]] logger const *log(
        const std::string &message,
        logger::severity severity) const noexcept override;

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_SERVER_LOGGER_H