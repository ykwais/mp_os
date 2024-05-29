#include "gtest/gtest.h"

#include <algorithm>
#include <iostream>
#include <list>
#include <random>
#include <vector>

#include <b_tree.h>
#include <client_logger_builder.h>
#include <logger_builder.h>
#include <search_tree.h>
#include <unordered_map>

namespace comparison
{

    class int_comparer final
    {

    public:

        int operator()(
            int const &left,
            int const &right) const noexcept
        {
            return left - right;
        }

    };

    class stdstring_comparer final
    {

    public:

        int operator()(
            std::string const &first,
            std::string const &second) const noexcept
        {
            if (first == second)
            {
                return 0;
            }

            if (first > second)
            {
                return 1;
            }

            return -1;
        }

    };

    class ac_kvp_int_stdstring_comparer final
    {

    public:

        int operator()(
            typename associative_container<int, std::string>::key_value_pair const &first,
            typename associative_container<int, std::string>::key_value_pair const &second)
        {
            auto keys_comparison_result = int_comparer()(first.key, second.key);
            if (keys_comparison_result != 0) return keys_comparison_result;
            return stdstring_comparer()(first.value, second.value);
        }

    };

}

template<
    typename tkey,
    typename tvalue>
bool compare_results(
    std::vector<typename search_tree<tkey, tvalue>::key_value_pair> const &expected,
    std::vector<typename search_tree<tkey, tvalue>::key_value_pair> const &actual,
    std::function<int(tkey const &, tkey const &)> keys_comparer,
    std::function<int(tvalue const &, tvalue const &)> values_comparer)
{
    if (expected.size() != actual.size())
    {
        return false;
    }

    for (size_t i = 0; i < expected.size(); ++i)
    {
        if (keys_comparer(expected[i].key, actual[i].key) != 0)
        {
            return false;
        }

        if (values_comparer(expected[i].value, actual[i].value) != 0)
        {
            return false;
        }
    }

    return true;
}

template<
    typename tvalue>
bool compare_obtain_results(
    std::vector<tvalue> const &expected,
    std::vector<tvalue> const &actual,
    std::function<int(tvalue const &, tvalue const &)> values_comparer)
{
    if (expected.size() != actual.size())
    {
        return false;
    }

    for (size_t i = 0; i < expected.size(); ++i)
    {
        if (values_comparer(expected[i], actual[i]) != 0)
        {
            return false;
        }
    }

    return true;
}

logger *create_logger(
    std::vector<std::pair<std::string, logger::severity>> const &output_file_streams_setup,
    bool use_console_stream = true,
    logger::severity console_stream_severity = logger::severity::debug)
{
    return client_logger_builder().build();

    logger_builder *builder = new client_logger_builder();

    if (use_console_stream)
    {
        builder->add_console_stream(console_stream_severity);
    }

    for (auto &output_file_stream_setup: output_file_streams_setup)
    {
        builder->add_file_stream(output_file_stream_setup.first, output_file_stream_setup.second);
    }

    logger *built_logger = builder->build();

    delete builder;

    return built_logger;
}

template<
    typename tkey,
    typename tvalue>
bool infix_const_iterator_test(
    b_tree<tkey, tvalue> const &tree,
    std::vector<std::pair<tkey, tvalue>> const &expected_result,
    std::function<int(tkey const &, tkey const &)> keys_comparer,
    std::function<int(tvalue const &, tvalue const &)> values_comparer)
{
    auto end_infix = tree.cend_infix();
    auto it = tree.cbegin_infix();

    for (auto const &item: expected_result)
    {
        auto tmp = *it;
        auto key = tmp.first;
        auto data = tmp.second;
        auto array_key = item.first;
        auto array_value = item.second;


//        if (std::get<0>(data) != std::get<0>(item) ||
//            std::get<1>(data) != std::get<1>(item) ||
//            keys_comparer(std::get<2>(data), std::get<2>(item)) != 0 ||
//            values_comparer(std::get<3>(data), std::get<3>(item)) != 0)
//        {
//            return false;
//        }

        if (keys_comparer(key, array_key) != 0 || values_comparer(data, array_value) != 0)
        {
            return false;
        }

        ++it;
    }

    return true;

//    auto end_infix = tree.cend_infix();/////////////////
//    auto it = tree.cbegin_infix();/////////////////////

//    for (auto const &item: expected_result)
//    {
//        if ((*it)->depth != item.depth || (*it)->key != item.key || (*it)->value != item.value)
//        {
//            return false;
//        }
//
//        ++it;
//    }
//
//    return true;
}

TEST(bTreePositiveTests, test0)
{
    std::function<int(int const &, int const &)> keys_comparer = comparison::int_comparer();
    std::function<int(std::string const &, std::string const &)> values_comparer = comparison::stdstring_comparer();

    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
    {
        { "b_tree_tests_logs.txt", logger::severity::trace }
    });

    logger->trace("bTreePositiveTests.test0 started");

    std::vector<std::pair<int, std::string>> expected_result =
    {

    };

    search_tree<int, std::string> *tree = new b_tree<int, std::string>(1024, [](int const &left, int const &right) { return left - right; },
                                                                       nullptr, nullptr);
    //b_tree<int, std::string> tree(1024, [](int const &left, int const &right) { return left - right; });
    EXPECT_TRUE(infix_const_iterator_test(*reinterpret_cast<b_tree<int, std::string> const *>(tree), expected_result, keys_comparer, values_comparer));

    logger->trace("bTreePositiveTests.test0 finished");

    delete tree;
}

TEST(bTreePositiveTests, test1)
{
    std::function<int(int const &, int const &)> keys_comparer = comparison::int_comparer();
    std::function<int(std::string const &, std::string const &)> values_comparer = comparison::stdstring_comparer();

    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
    {
        { "b_tree_tests_logs.txt", logger::severity::trace }
    });

    logger->trace("bTreePositiveTests.test1 started");

    std::vector<std::pair<int, std::string>> expected_result =
    {
        std::pair<int, std::string>(1, "ab"),
        std::pair<int, std::string>(2, "b"),
        std::pair<int, std::string>(3, "d"),
        std::pair<int, std::string>(4, "e"),
        std::pair<int, std::string>(15, "c"),
        std::pair<int, std::string>(27, "f")
    };

    search_tree<int, std::string> *tree = new b_tree<int, std::string>(3, keys_comparer, nullptr, logger);

    tree->insert(1, std::string("ab"));
    tree->insert(2, std::string("b"));
    tree->insert(15, std::string("c"));
    tree->insert(3, std::string("d"));
    tree->insert(4, std::string("e"));
    tree->insert(27, std::string("f"));

    EXPECT_TRUE(infix_const_iterator_test(*reinterpret_cast<b_tree<int, std::string> const *>(tree), expected_result, keys_comparer, values_comparer));

    logger->trace("bTreePositiveTests.test1 finished");

    delete tree;
}

TEST(bTreePositiveTests, test2)
{
    std::function<int(int const &, int const &)> keys_comparer = comparison::int_comparer();
    std::function<int(std::string const &, std::string const &)> values_comparer = comparison::stdstring_comparer();

    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
    {
        { "b_tree_tests_logs.txt", logger::severity::trace }
    });

    logger->trace("bTreePositiveTests.test2 started");

    std::vector<std::pair<int, std::string>> expected_result =
    {
        std::pair<int, std::string>(1, "a"),
        std::pair<int, std::string>(2, "b"),
        std::pair<int, std::string>(3, "d"),
        std::pair<int, std::string>(4, "e"),
        std::pair<int, std::string>(15, "c"),
        std::pair<int, std::string>(24, "g"),
        std::pair<int, std::string>(45, "k"),
        std::pair<int, std::string>(100, "f"),
        std::pair<int, std::string>(101, "j"),
        std::pair<int, std::string>(193, "l"),
        std::pair<int, std::string>(456, "h"),
        std::pair<int, std::string>(534, "m")
    };

    search_tree<int, std::string> *tree = new b_tree<int, std::string>(5, keys_comparer, nullptr, logger);

    tree->insert(1, std::string("a"));
    tree->insert(2, std::string("b"));
    tree->insert(15, std::string("c"));
    tree->insert(3, std::string("d"));
    tree->insert(4, std::string("e"));
    tree->insert(100, std::string("f"));
    tree->insert(24, std::string("g"));
    tree->insert(456, std::string("h"));
    tree->insert(101, std::string("j"));
    tree->insert(45, std::string("k"));
    tree->insert(193, std::string("l"));
    tree->insert(534, std::string("m"));

    EXPECT_TRUE(infix_const_iterator_test(*reinterpret_cast<b_tree<int, std::string> const *>(tree), expected_result, keys_comparer, values_comparer));

    logger->trace("bTreePositiveTests.test2 finished");

    delete tree;
}

TEST(bTreePositiveTests, test3)
{
    std::function<int(int const &, int const &)> keys_comparer = comparison::int_comparer();
    std::function<int(std::string const &, std::string const &)> values_comparer = comparison::stdstring_comparer();

    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
    {
        { "b_tree_tests_logs.txt", logger::severity::trace }
    });

    logger->trace("bTreePositiveTests.test3 started");

    std::vector<std::pair<int, std::string>> expected_result =
    {
        std::pair<int, std::string>(1, "a"),
        std::pair<int, std::string>(2, "b"),
        std::pair<int, std::string>(3, "d"),
        std::pair<int, std::string>(4, "e"),
        std::pair<int, std::string>(15, "c"),
        std::pair<int, std::string>(24, "g"),
        std::pair<int, std::string>(45, "k"),
        std::pair<int, std::string>(100, "f"),
        std::pair<int, std::string>(101, "j"),
        std::pair<int, std::string>(193, "l"),
        std::pair<int, std::string>(456, "h"),
        std::pair<int, std::string>(534, "m")
    };

    search_tree<int, std::string> *tree = new b_tree<int, std::string>(7, keys_comparer, nullptr, logger);

    tree->insert(1, std::string("a"));
    tree->insert(2, std::string("b"));
    tree->insert(15, std::string("c"));
    tree->insert(3, std::string("d"));
    tree->insert(4, std::string("e"));
    tree->insert(100, std::string("f"));
    tree->insert(24, std::string("g"));
    tree->insert(456, std::string("h"));
    tree->insert(101, std::string("j"));
    tree->insert(45, std::string("k"));
    tree->insert(193, std::string("l"));
    tree->insert(534, std::string("m"));

    EXPECT_TRUE(infix_const_iterator_test(*reinterpret_cast<b_tree<int, std::string> const *>(tree), expected_result, keys_comparer, values_comparer));

    logger->trace("bTreePositiveTests.test3 finished");

    delete tree;
}

TEST(bTreePositiveTests, test4)
{
    std::function<int(int const &, int const &)> keys_comparer = comparison::int_comparer();
    std::function<int(std::string const &, std::string const &)> values_comparer = comparison::stdstring_comparer();

    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
    {
        { "b_tree_tests_logs.txt", logger::severity::trace }
    });

    logger->trace("bTreePositiveTests.test4 started");

    std::vector<std::pair<int, std::string>> expected_result =
    {
        std::pair<int, std::string>(1, "a"),
        std::pair<int, std::string>(2, "b"),
        std::pair<int, std::string>(3, "d"),
        std::pair<int, std::string>(4, "e"),
        std::pair<int, std::string>(15, "c"),
        std::pair<int, std::string>(24, "g"),
        std::pair<int, std::string>(45, "k"),
        std::pair<int, std::string>(100, "f"),
        std::pair<int, std::string>(101, "j"),
        std::pair<int, std::string>(193, "l"),
        std::pair<int, std::string>(456, "h"),
        std::pair<int, std::string>(534, "m")
    };

    search_tree<int, std::string> *tree = new b_tree<int, std::string>(3, keys_comparer, nullptr, logger);

    tree->insert(1, std::string("a"));
    tree->insert(2, std::string("b"));
    tree->insert(15, std::string("c"));
    tree->insert(3, std::string("d"));
    tree->insert(4, std::string("e"));
    tree->insert(100, std::string("f"));
    tree->insert(24, std::string("g"));
    tree->insert(456, std::string("h"));
    tree->insert(101, std::string("j"));
    tree->insert(45, std::string("k"));
    tree->insert(193, std::string("l"));
    tree->insert(534, std::string("m"));

    EXPECT_TRUE(infix_const_iterator_test(*reinterpret_cast<b_tree<int, std::string> const *>(tree), expected_result, keys_comparer, values_comparer));

    logger->trace("bTreePositiveTests.test4 finished");

    delete tree;
    delete logger;
}

TEST(bTreePositiveTests, test5)
{
    std::function<int(int const &, int const &)> keys_comparer = comparison::int_comparer();
    std::function<int(std::string const &, std::string const &)> values_comparer = comparison::stdstring_comparer();

    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
    {
        { "b_tree_tests_logs.txt", logger::severity::trace }
    });

    logger->trace("bTreePositiveTests.test5 started");

    std::vector<std::tuple<size_t, size_t, int, std::string>> expected_result =
    {

    };

//    search_tree<int, std::string> *tree = new b_tree<int, std::string>(2, keys_comparer, nullptr, logger);
//
//    tree->insert(1, std::string("a"));
//    tree->insert(2, std::string("b"));
//    tree->insert(15, std::string("c"));
//    tree->insert(3, std::string("d"));
//    tree->insert(4, std::string("e"));

    search_tree<int, int> *tree = new b_tree<int, int>(2, keys_comparer, nullptr, logger);

//    tree->insert(1, 1);
//    tree->insert(2, 2);
//    tree->insert(15, 3);
//    tree->insert(3, 4);
//    tree->insert(4, 5);

    tree->insert(10, 1);
    tree->insert(20, 2);
    tree->insert(30, 3);
    tree->insert(40, 4);
    tree->insert(50, 5);
    tree->insert(60, 1);
    tree->insert(70, 2);
    tree->insert(80, 3);
    tree->insert(90, 4);
    tree->insert(75, 5);
    tree->insert(110, 1);
    tree->insert(120, 2);
    tree->insert(130, 3);
    tree->insert(140, 4567);
    tree->insert(150, 45);

    std::cout << tree->obtain(140) << std::endl;

//    tree->insert(10, 1);
//    tree->insert(20, 2);
//    tree->insert(30, 3);
//    tree->insert(40, 4);
//    tree->insert(50, 5);
//    tree->insert(35, 78);



    auto first_disposed = tree->dispose(70);
    auto second_disposed = tree->dispose(20);
    auto third_disposed = tree->dispose(140);
    auto four_disposed = tree->dispose(150);
    auto five_disposed = tree->dispose(75);
    auto six_disposed = tree->dispose(30);
    auto seven_disposed = tree->dispose(60);
    auto eight_disposed = tree->dispose(10);
    auto nine_disposed = tree->dispose(80);
    auto ten_disposed = tree->dispose(40);
    auto eleven_disposed = tree->dispose(120);
    auto twelve_disposed = tree->dispose(50);
    auto thirteen_disposed = tree->dispose(110);
    auto for_disposed = tree->dispose(130);
    auto f_disposed = tree->dispose(90);









    //EXPECT_TRUE(infix_const_iterator_test(*reinterpret_cast<b_tree<int, std::string> const *>(tree), expected_result, keys_comparer, values_comparer));

    logger->trace("bTreePositiveTests.test5 finished");

    delete tree;
    delete logger;
}

TEST(bTreePositiveTests, test6)
{
    std::function<int(int const &, int const &)> keys_comparer = comparison::int_comparer();
    std::function<int(std::string const &, std::string const &)> values_comparer = comparison::stdstring_comparer();

    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
    {
        { "b_tree_tests_logs.txt", logger::severity::trace }
    });

    logger->trace("bTreePositiveTests.test6 started");

    std::vector<std::pair<int, std::string>> expected_result =
    {
        std::pair<int, std::string>(2, "b"),
        std::pair<int, std::string>(3, "d"),
        std::pair<int, std::string>(4, "e"),
        std::pair<int, std::string>(15, "c"),
        std::pair<int, std::string>(45, "k"),
        std::pair<int, std::string>(101, "j"),
        std::pair<int, std::string>(456, "h"),
        std::pair<int, std::string>(534, "m")
    };

    search_tree<int, std::string> *tree = new b_tree<int, std::string>(4, keys_comparer, nullptr, logger);

    tree->insert(1, std::string("a"));
    tree->insert(2, std::string("b"));
    tree->insert(15, std::string("c"));
    tree->insert(3, std::string("d"));
    tree->insert(4, std::string("e"));
    tree->insert(100, std::string("f"));
    tree->insert(24, std::string("g"));
    tree->insert(456, std::string("h"));
    tree->insert(101, std::string("j"));
    tree->insert(45, std::string("k"));
    tree->insert(193, std::string("l"));
    tree->insert(534, std::string("m"));

    auto first_disposed = std::move(tree->dispose(1));
    auto second_disposed = std::move(tree->dispose(100));
    auto third_disposed = std::move(tree->dispose(193));
    auto fourth_disposed = std::move(tree->dispose(24));

    EXPECT_TRUE(infix_const_iterator_test(*reinterpret_cast<b_tree<int, std::string> const *>(tree), expected_result, keys_comparer, values_comparer));

    EXPECT_TRUE(values_comparer(first_disposed, "a") == 0);
    EXPECT_TRUE(values_comparer(second_disposed, "f") == 0);
    EXPECT_TRUE(values_comparer(third_disposed, "l") == 0);
    EXPECT_TRUE(values_comparer(fourth_disposed, "g") == 0);

    logger->trace("bTreePositiveTests.test6 finished");

    delete tree;
    delete logger;
}

TEST(bTreePositiveTests, test7)
{
    std::function<int(int const &, int const &)> keys_comparer = comparison::int_comparer();
    std::function<int(std::string const &, std::string const &)> values_comparer = comparison::stdstring_comparer();

    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
    {
        { "b_tree_tests_logs.txt", logger::severity::trace }
    });

    logger->trace("bTreePositiveTests.test7 started");

    std::vector<std::string> expected_result =
    {
        "g",
        "d",
        "e",
        " ",
        "l",
        "a",
        "b",
        "y"
    };

    search_tree<int, std::string> *tree = new b_tree<int, std::string>(5, keys_comparer, nullptr, logger);

    tree->insert(1, std::move(std::string("a")));
    tree->insert(2, std::move(std::string("b")));
    tree->insert(15, std::move(std::string("c")));
    tree->insert(3, std::move(std::string("d")));
    tree->insert(4, std::move(std::string("e")));
    tree->insert(100, std::move(std::string(" ")));
    tree->insert(24, std::move(std::string("g")));
    tree->insert(-456, std::move(std::string("h")));
    tree->insert(101, std::move(std::string("j")));
    tree->insert(-45, std::move(std::string("k")));
    tree->insert(-193, std::move(std::string("l")));
    tree->insert(534, std::move(std::string("m")));
    tree->insert(1000, std::move(std::string("y")));

    std::vector<std::string> actual_result =
    {
        tree->obtain(24),
        tree->obtain(3),
        tree->obtain(4),
        tree->obtain(100),
        tree->obtain(-193),
        tree->obtain(1),
        tree->obtain(2),
        tree->obtain(1000)
    };

    EXPECT_TRUE(compare_obtain_results(expected_result, actual_result, values_comparer));

    logger->trace("bTreePositiveTests.test7 finished");

    delete tree;
    delete logger;
}

TEST(bTreePositiveTests, test8)
{
    std::function<int(int const &, int const &)> keys_comparer = comparison::int_comparer();
    std::function<int(std::string const &, std::string const &)> values_comparer = comparison::stdstring_comparer();

    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
    {
        { "b_tree_tests_logs.txt", logger::severity::trace }
    });

    logger->trace("bTreePositiveTests.test8 started");

    std::vector<std::string> expected_result =
    {
        "y",
        "l",
        "a",
        "g",
        "k",
        "b",
        "c",
        "h"
    };

    search_tree<int, std::string> *tree = new b_tree<int, std::string>(4, keys_comparer, nullptr, logger);

    tree->insert(1, std::string("a"));
    tree->insert(2, std::string("b"));
    tree->insert(15, std::string("c"));
    tree->insert(3, std::string("d"));
    tree->insert(4, std::string("e"));
    tree->insert(100, std::string(" "));
    tree->insert(24, std::string("g"));
    tree->insert(-456, std::string("h"));
    tree->insert(101, std::string("j"));
    tree->insert(-45, std::string("k"));
    tree->insert(-193, std::string("l"));
    tree->insert(534, std::string("m"));
    tree->insert(1000, std::string("y"));

    std::vector<std::string> actual_result =
    {
        tree->obtain(1000),
        tree->obtain(-193),
        tree->obtain(1),
        tree->obtain(24),
        tree->obtain(-45),
        tree->obtain(2),
        tree->obtain(15),
        tree->obtain(-456)
    };

    EXPECT_TRUE(compare_obtain_results(expected_result, actual_result, values_comparer));

    logger->trace("bTreePositiveTests.test8 finished");

    delete tree;
    delete logger;
}

TEST(bTreePositiveTests, test9)
{
    std::function<int(int const &, int const &)> keys_comparer = comparison::int_comparer();
    std::function<int(typename associative_container<int, std::string>::key_value_pair const &, typename associative_container<int, std::string>::key_value_pair const &)> values_comparer = comparison::ac_kvp_int_stdstring_comparer();

    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
    {
        { "b_tree_tests_logs.txt", logger::severity::trace }
    });

    logger->trace("bTreePositiveTests.test9 started");

    std::vector<typename associative_container<int, std::string>::key_value_pair> expected_result =
    {
        { 4, "e" },
        { 15, "c" },
        { 24, "g" },
        { 45, "k" },
        { 100, "f" },
        { 101, "j" },
    };

    search_tree<int, std::string> *tree = new b_tree<int, std::string>(5, keys_comparer, nullptr, logger);

    tree->insert(1, std::string("a"));
    tree->insert(2, std::string("b"));
    tree->insert(15, std::string("c"));
    tree->insert(3, std::string("d"));
    tree->insert(4, std::string("e"));
    tree->insert(100, std::string("f"));
    tree->insert(24, std::string("g"));
    tree->insert(456, std::string("h"));
    tree->insert(101, std::string("j"));
    tree->insert(45, std::string("k"));
    tree->insert(193, std::string("l"));
    tree->insert(534, std::string("m"));

    std::vector<typename associative_container<int, std::string>::key_value_pair> actual_result = tree->obtain_between(4, 101, true, true);

    EXPECT_TRUE(compare_obtain_results(expected_result, actual_result, values_comparer));

    logger->trace("bTreePositiveTests.test9 finished");

    delete tree;
    delete logger;
}

TEST(bTreeNegativeTests, test1)
{
    std::function<int(int const &, int const &)> keys_comparer = comparison::int_comparer();
    std::function<int(std::string const &, std::string const &)> values_comparer = comparison::stdstring_comparer();

    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
    {
        { "b_tree_tests_logs.txt", logger::severity::trace }
    });

    logger->trace("bTreeNegativeTests.test1 started");

    search_tree<int, std::string> *tree = new b_tree<int, std::string>(3, keys_comparer, nullptr, logger);

    tree->insert(1, std::string("a"));
    tree->insert(2, std::string("b"));
    tree->insert(15, std::string("c"));
    tree->insert(3, std::string("d"));
    tree->insert(4, std::string("e"));

    ASSERT_THROW(tree->dispose(45), std::logic_error);

    logger->trace("bTreeNegativeTests.test1 finished");

    delete tree;
    delete logger;
}

TEST(bTreeNegativeTests, test2)
{
    std::function<int(int const &, int const &)> keys_comparer = comparison::int_comparer();
    std::function<int(std::string const &, std::string const &)> values_comparer = comparison::stdstring_comparer();

    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
    {
        { "b_tree_tests_logs.txt", logger::severity::trace }
    });

    logger->trace("bTreeNegativeTests.test2 started");

    std::vector<std::string> expected_result =
    {
        "a",
        "b",
        "c",
        "d",
        "e"
    };

    search_tree<int, std::string> *tree = new b_tree<int, std::string>(4, keys_comparer, nullptr, logger);

    tree->insert(1, std::string("a"));
    tree->insert(2, std::string("b"));
    tree->insert(15, std::string("c"));
    tree->insert(3, std::string("d"));
    tree->insert(4, std::string("e"));
    tree->insert(100, std::string(" "));
    tree->insert(24, std::string("g"));
    tree->insert(-456, std::string("h"));
    tree->insert(101, std::string("j"));
    tree->insert(-45, std::string("k"));
    tree->insert(-193, std::string("l"));
    tree->insert(534, std::string("m"));
    tree->insert(1000, std::string("y"));

    std::vector<std::string> actual_result;

    EXPECT_THROW(actual_result = std::move(std::vector<std::string>
    {
        tree->obtain(1000),
        tree->obtain(193),
        tree->obtain(1),
        tree->obtain(24),
    }), std::logic_error);

    EXPECT_FALSE(compare_obtain_results(expected_result, actual_result, values_comparer));

    logger->trace("bTreeNegativeTests.test2 finished");

    delete tree;
    delete logger;
}

TEST(bTreeNegativeTests, test3)
{
    std::function<int(int const &, int const &)> keys_comparer = comparison::int_comparer();
    std::function<int(std::string const &, std::string const &)> values_comparer = comparison::stdstring_comparer();

    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
    {
        { "b_tree_tests_logs.txt", logger::severity::trace }
    });

    logger->trace("bTreeNegativeTests.test3 started");

    search_tree<int, std::string> *tree = new b_tree<int, std::string>(4, keys_comparer, nullptr, logger);

    tree->insert(1, std::string("a"));
    tree->insert(2, std::string("b"));
    tree->insert(15, std::string("c"));
    tree->insert(3, std::string("d"));
    tree->insert(4, std::string("e"));
    tree->insert(100, std::string(" "));
    tree->insert(24, std::string("g"));
    tree->insert(-456, std::string("h"));
    tree->insert(101, std::string("j"));
    tree->insert(-45, std::string("k"));
    tree->insert(-193, std::string("l"));
    tree->insert(534, std::string("m"));
    tree->insert(1000, std::string("y"));

    ASSERT_THROW(tree->obtain(1001), std::logic_error);

    logger->trace("bTreeNegativeTests.test3 finished");

    delete tree;
    delete logger;
}


TEST(own, test1)
{

    std::function<int(int const &, int const &)> keys_comparer = comparison::int_comparer();
    std::function<int(std::string const &, std::string const &)> values_comparer = comparison::stdstring_comparer();

    search_tree<int, int> *tree = new b_tree<int, int>(4, keys_comparer);
    std::unordered_map<int, int> map;

    size_t iterations = 1000000;

    srand(time(nullptr));

    for(size_t i = 0; i < iterations; ++i)
    {
        switch(rand() % 3)
        {
            case 0:
            case 1:
            {
                int tmp = rand();
                try
                {

                    if (map.find(tmp) == map.end())
                    {
                        map.insert(std::make_pair(tmp, 1));
                        tree->insert(tmp, 1);
                    }
                } catch (std::logic_error& er)
                {
                    std::cout << er.what() << std::endl;
                }
                break;
            }
            case 2:
            {
                if (!map.empty())
                {
                    auto it = map.begin();

                    //std::advance(it, rand() % map.size());

                    tree->dispose(it->first);
                    map.erase(it);
                } else
                {
                    std::cout << "Empty" << std::endl;
                }
            }
                break;
        }
    }

    //EXPECT_EQ(tree.size(), map.size());

    while(!map.empty())
    {
        auto it = map.begin();
        //std::advance(it, rand() % map.size());

        tree->dispose(it->first);
        map.erase(it);
    }
    //EXPECT_EQ(tree.size(), 0);
}

int main(
    int argc,
    char **argv)
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}