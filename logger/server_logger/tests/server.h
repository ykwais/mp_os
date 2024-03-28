

#ifndef MP_OS_SERVER_H
#define MP_OS_SERVER_H

#include <crow.h>
#include <unordered_map>
#include "logger.h"
#include <mutex>

class server
{
private:

    std::mutex _mtx;

    std::unordered_map<int, std::unordered_map<logger::severity, std::pair<bool, std::string>>> _all_streams;

    crow::SimpleApp _app;

public:

    explicit server(uint64_t port = 8080);

    server(const server&) = delete;

    server& operator=(const server&) = delete;

    server(server&&) = delete;

    server& operator=(server&&) = delete;

    ~server() = default;

};


#endif //MP_OS_SERVER_H
