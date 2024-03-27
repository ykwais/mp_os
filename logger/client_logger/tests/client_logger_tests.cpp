#include <gtest/gtest.h>
#include "../include/client_logger.h"
#include "../include/client_logger_builder.h"



int main(
    int argc,
    char *argv[])
{
    testing::InitGoogleTest(&argc, argv);

    client_logger_builder builder;
//
//    builder.add_file_stream("1.txt", logger::severity::debug)
//    ->add_file_stream("2.txt", logger::severity::information)
//    ->add_console_stream(logger::severity::debug)
//    ->set_format("&& %d %t {%s} %m  ** %%%%");
//
//    std::unique_ptr<logger> log(builder.build());
//
//    log->information("hi");
//    builder.clear()->set_format("%t {}{}}{ %m")->add_console_stream(logger::severity::debug);//?????????????????????????
//    std::unique_ptr<logger> log2(builder.build());
//    log2->debug("111");
//    log2->debug("211");
//    log2->warning("oops");

    builder.add_file_stream("a.txt" ,logger::severity::trace)->
            add_file_stream("b.txt" ,logger::severity::trace)->
            add_file_stream("b.txt" ,logger::severity::trace)->
            add_file_stream("a.txt", logger::severity::debug)->
            add_file_stream("../tests/a.txt", logger::severity::debug)->
            add_console_stream(logger::severity::trace)->
            set_format("[%d %t][%s] %m");

   builder.transform_with_configuration("config.json", "log");

//    std::cout << std::filesystem::current_path();

    std::unique_ptr<logger> log(builder.build());

    log->trace("it is a very long message!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    log->debug("dd");

    return RUN_ALL_TESTS();
}