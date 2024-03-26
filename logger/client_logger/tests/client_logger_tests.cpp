#include <gtest/gtest.h>
#include "../include/client_logger.h"
#include "../include/client_logger_builder.h"



int main(
    int argc,
    char *argv[])
{
    testing::InitGoogleTest(&argc, argv);

    client_logger_builder builder;

    builder.add_file_stream("1.txt", logger::severity::debug)
    ->add_file_stream("2.txt", logger::severity::information)
    ->set_format("%d %m");

    std::unique_ptr<logger> log(builder.build());

    log->information("hi");

    return RUN_ALL_TESTS();
}