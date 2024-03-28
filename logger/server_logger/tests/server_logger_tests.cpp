#include <gtest/gtest.h>
#include <server_logger_builder.h>
#include <server_logger.h>


int main(
    int argc,
    char *argv[])
{
    //testing::InitGoogleTest(&argc, argv);
    server_logger_builder builder;

    builder.add_file_stream("a.txt", logger::severity::trace)->add_file_stream("b.txt", logger::severity::debug)->
            add_console_stream(logger::severity::trace);

    std::unique_ptr<logger> log(builder.build());

    log->trace("good")->debug("debug");

    log->trace("IT is a very long strange message !!!!!!!!!!%%%%%%%%\tzdtjhdjh");
    //return RUN_ALL_TESTS();
}