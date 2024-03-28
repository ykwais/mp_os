

#include "server.h"
#include "logger_builder.h"
#include <iostream>
#include <fstream>

server::server(uint64_t port)
{

    CROW_ROUTE( _app, "/init")([&](const crow::request &req){

        std::lock_guard lock(_mtx);

        std::string string_of_pid = req.url_params.get("pid");

        std::string string_of_severity = req.url_params.get("severity");

        std::string string_of_console = req.url_params.get("console");

        std::string string_of_path = req.url_params.get("path_to_file");

        int pid_int = std::stoi(string_of_pid);

        logger::severity sever = logger_builder::string_to_severity(string_of_severity);

        bool console_bool = string_of_console == "1";


        auto iter = _all_streams.find(pid_int);

        if(iter == _all_streams.end())
        {
            auto insert = _all_streams.emplace(pid_int, std::unordered_map<logger::severity, std::pair<bool, std::string>>());

            if(!insert.second)
            {
                throw std::ios_base::failure("bad_alloc");
            }

            iter = insert.first;
        }

        auto iter_map = iter->second.find(sever);

        if(iter_map == iter->second.end())
        {
            auto insert = iter->second.emplace(sever, std::make_pair(console_bool, string_of_path));
            iter_map = insert.first;
        }

        iter_map->second.first = console_bool;
        iter_map->second.second = std::move(string_of_path);
        if(!iter_map->second.second.empty())
        {
            std::ofstream temporary(iter_map->second.second);/////////////////////////////////////////
        }

        return 0;
    });

    CROW_ROUTE( _app, "/log")([&](const crow::request &req){

        std::lock_guard lock(_mtx);

        std::string string_of_pid = req.url_params.get("pid");

        std::string string_of_severity = req.url_params.get("severity");

        std::string string_of_message = req.url_params.get("message");

        int pid_int = std::stoi(string_of_pid);

        logger::severity sever = logger_builder::string_to_severity(string_of_severity);

        auto iter = _all_streams.find(pid_int);

        if(iter != _all_streams.end())
        {
            auto iter_map = iter->second.find(sever);

            if(iter_map != iter->second.end())
            {
                if(iter_map->second.first)
                {
                    std::cout << string_of_message << std::endl;
                }
                if(!iter_map->second.second.empty())
                {
                    std::ofstream file(iter_map->second.second);
                    file << string_of_message << std::endl;
                }

            }
        }

        return 0;
    });

    CROW_ROUTE(_app, "/kill")([&](const crow::request &req){

        std::lock_guard lock(_mtx);

        std::string string_of_pid = req.url_params.get("pid");

        int pid_int = std::stoi(string_of_pid);

        _all_streams.erase(pid_int);

        return 0;
    });

    _app.port(port).multithreaded().run();
}

