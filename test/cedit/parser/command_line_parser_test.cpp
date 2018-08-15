//    This file is part of cedit++.
//    Copyright (C) 2018 kawatea
//
//    cedit++ is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    cedit++ is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with cedit++.  If not, see <https://www.gnu.org/licenses/>.

#include <string>
#include <functional>
#include "gtest/gtest.h"
#include "parser/command_line_parser.h"
#include "parser/parser_exception.h"

TEST(command_line_parser, test_parse_nothing) {
    const int argc = 1;
    const char* argv[] = {"test"};
    parser::command_line_parser parser("test", "version");
    parser.parse(argc, argv);
}

TEST(command_line_parser, test_parse_help) {
    const int argc = 2;
    const char* argv[] = {"test", "-h"};
    parser::command_line_parser parser("test", "version");
    EXPECT_EXIT(parser.parse(argc, argv), ::testing::ExitedWithCode(0), ".*");
}

TEST(command_line_parser, test_parse_version) {
    const int argc = 2;
    const char* argv[] = {"test", "-v"};
    parser::command_line_parser parser("test", "version");
    EXPECT_EXIT(parser.parse(argc, argv), ::testing::ExitedWithCode(0), "version\n");
}

TEST(command_line_parser, test_parse_bool_with_short_name) {
    const int argc = 2;
    const char* argv[] = {"test", "-a"};
    parser::command_line_parser parser("test", "version");
    parser.add_property<bool>('a');
    parser.parse(argc, argv);
    EXPECT_TRUE(parser.get<bool>('a'));
}

TEST(command_line_parser, test_parse_string_with_short_name) {
    const int argc = 3;
    const char* argv[] = {"test", "-a", "value"};
    parser::command_line_parser parser("test", "version");
    parser.add_property<std::string>('a');
    parser.parse(argc, argv);
    EXPECT_EQ("value", parser.get<std::string>('a'));
}

TEST(command_line_parser, test_parse_int_with_short_name) {
    const int argc = 3;
    const char* argv[] = {"test", "-a", "123"};
    parser::command_line_parser parser("test", "version");
    parser.add_property<int>('a');
    parser.parse(argc, argv);
    EXPECT_EQ(123, parser.get<int>('a'));
}

TEST(command_line_parser, test_parse_bool_with_long_name) {
    const int argc = 2;
    const char* argv[] = {"test", "--aa"};
    parser::command_line_parser parser("test", "version");
    parser.add_property<bool>('\0', "aa");
    parser.parse(argc, argv);
    EXPECT_TRUE(parser.get<bool>("aa"));
}

TEST(command_line_parser, test_parse_string_with_long_name) {
    const int argc = 3;
    const char* argv[] = {"test", "--aa", "value"};
    parser::command_line_parser parser("test", "version");
    parser.add_property<std::string>('\0', "aa");
    parser.parse(argc, argv);
    EXPECT_EQ("value", parser.get<std::string>("aa"));
}

TEST(command_line_parser, test_parse_int_with_long_name) {
    const int argc = 3;
    const char* argv[] = {"test", "--aa", "123"};
    parser::command_line_parser parser("test", "version");
    parser.add_property<int>('\0', "aa");
    parser.parse(argc, argv);
    EXPECT_EQ(123, parser.get<int>("aa"));
}

TEST(command_line_parser, test_parse_with_equal) {
    const int argc = 2;
    const char* argv[] = {"test", "-a=123"};
    parser::command_line_parser parser("test", "version");
    parser.add_property<int>('a');
    parser.parse(argc, argv);
    EXPECT_EQ(123, parser.get<int>('a'));
}

TEST(command_line_parser, test_parse_default_value) {
    const int argc = 1;
    const char* argv[] = {"test"};
    parser::command_line_parser parser("test", "version");
    parser.add_property<int>('a', "", "", false, 123);
    parser.parse(argc, argv);
    EXPECT_EQ(123, parser.get<int>('a'));
}

TEST(command_line_parser, test_parse_custom_converter) {
    const int argc = 3;
    const char* argv[] = {"test", "-a", "value"};
    parser::command_line_parser parser("test", "version");
    parser.add_property<std::string>('a', "", "", false, "", [](const std::string& value) { return "value:" + value; });
    parser.parse(argc, argv);
    EXPECT_EQ("value:value", parser.get<std::string>('a'));
}

TEST(command_line_parser, test_parse_potisional_argument) {
    const int argc = 2;
    const char* argv[] = {"test", "value"};
    parser::command_line_parser parser("test", "version");
    parser.add_positional_argument<std::string>("key");
    parser.parse(argc, argv);
    EXPECT_EQ("value", parser.get_positional_argument<std::string>());
}

TEST(command_line_parser, test_add_property_without_name) {
    parser::command_line_parser parser("test", "version");
    EXPECT_THROW(parser.add_property<std::string>(), parser::parser_exception);
}

TEST(command_line_parser, test_add_duplicate_property) {
    parser::command_line_parser parser("test", "version");
    parser.add_property<std::string>('a');
    EXPECT_THROW(parser.add_property<std::string>('a'), parser::parser_exception);
}

TEST(command_line_parser, test_add_positional_argument_without_name) {
    parser::command_line_parser parser("test", "version");
    EXPECT_THROW(parser.add_positional_argument<std::string>(""), parser::parser_exception);
}

TEST(command_line_parser, test_add_duplicate_positional_argument) {
    parser::command_line_parser parser("test", "version");
    parser.add_positional_argument<std::string>("key1");
    EXPECT_THROW(parser.add_positional_argument<std::string>("key2"), parser::parser_exception);
}

TEST(command_line_parser, test_parse_unknown_property) {
    const int argc = 2;
    const char* argv[] = {"test", "-a"};
    parser::command_line_parser parser("test", "version");
    EXPECT_EXIT(parser.parse(argc, argv), ::testing::ExitedWithCode(1), "\"-a\" is undefined\n.*");
}

TEST(command_line_parser, test_parse_unknown_positional_argument) {
    const int argc = 2;
    const char* argv[] = {"test", "a"};
    parser::command_line_parser parser("test", "version");
    EXPECT_EXIT(parser.parse(argc, argv), ::testing::ExitedWithCode(1), "positional argument is undefined\n.*");
}

TEST(command_line_parser, test_parse_missing_value) {
    const int argc = 2;
    const char* argv[] = {"test", "-a"};
    parser::command_line_parser parser("test", "version");
    parser.add_property<std::string>('a');
    EXPECT_EXIT(parser.parse(argc, argv), ::testing::ExitedWithCode(1), "\"-a\" needs value\n.*");
}

TEST(command_line_parser, test_parse_missing_value_with_equal) {
    const int argc = 2;
    const char* argv[] = {"test", "-a="};
    parser::command_line_parser parser("test", "version");
    parser.add_property<std::string>('a');
    EXPECT_EXIT(parser.parse(argc, argv), ::testing::ExitedWithCode(1), "\"-a\" needs value\n.*");
}

TEST(command_line_parser, test_parse_unnecessary_value) {
    const int argc = 2;
    const char* argv[] = {"test", "-a=true"};
    parser::command_line_parser parser("test", "version");
    parser.add_property<bool>('a');
    EXPECT_EXIT(parser.parse(argc, argv), ::testing::ExitedWithCode(1), "\"-a\" doesn't need value\n.*");
}

TEST(command_line_parser, test_parse_duplicate_property) {
    const int argc = 3;
    const char* argv[] = {"test", "-a", "-a"};
    parser::command_line_parser parser("test", "version");
    parser.add_property<bool>('a');
    EXPECT_EXIT(parser.parse(argc, argv), ::testing::ExitedWithCode(1), "\"-a\" is already parsed\n.*");
}

TEST(command_line_parser, test_parse_duplicate_property_with_different_name) {
    const int argc = 3;
    const char* argv[] = {"test", "-a", "--aa"};
    parser::command_line_parser parser("test", "version");
    parser.add_property<bool>('a', "aa");
    EXPECT_EXIT(parser.parse(argc, argv), ::testing::ExitedWithCode(1), "\"-a, --aa\" is already parsed\n.*");
}

TEST(command_line_parser, test_parse_duplicate_positional_argument) {
    const int argc = 3;
    const char* argv[] = {"test", "value1", "value2"};
    parser::command_line_parser parser("test", "version");
    parser.add_positional_argument<std::string>("key");
    EXPECT_EXIT(parser.parse(argc, argv), ::testing::ExitedWithCode(1), "\"key\" is already parsed\n.*");
}

TEST(command_line_parser, test_parse_missing_required_property) {
    const int argc = 1;
    const char* argv[] = {"test"};
    parser::command_line_parser parser("test", "version");
    parser.add_property<std::string>('a', "", "", true);
    EXPECT_EXIT(parser.parse(argc, argv), ::testing::ExitedWithCode(1), "\"-a\" is required\n.*");
}

TEST(command_line_parser, test_parse_missing_required_positional_argument) {
    const int argc = 1;
    const char* argv[] = {"test"};
    parser::command_line_parser parser("test", "version");
    parser.add_positional_argument<std::string>("key", "", true);
    EXPECT_EXIT(parser.parse(argc, argv), ::testing::ExitedWithCode(1), "\"key\" is required\n.*");
}

TEST(command_line_parser, test_parse_invalid_conversion) {
    const int argc = 3;
    const char* argv[] = {"test", "-a", "value"};
    parser::command_line_parser parser("test", "version");
    parser.add_property<int>('a');
    EXPECT_EXIT(parser.parse(argc, argv), ::testing::ExitedWithCode(1), "\"value\" can't be converted into int\n.*");
}

TEST(command_line_parser, test_get_unknown_property) {
    const int argc = 1;
    const char* argv[] = {"test"};
    parser::command_line_parser parser("test", "version");
    parser.parse(argc, argv);
    EXPECT_THROW(parser.get<std::string>('a'), parser::parser_exception);
}

TEST(command_line_parser, test_get_wrong_property) {
    const int argc = 1;
    const char* argv[] = {"test"};
    parser::command_line_parser parser("test", "version");
    parser.add_property<std::string>('a');
    parser.parse(argc, argv);
    EXPECT_THROW(parser.get<int>('a'), parser::parser_exception);
}

TEST(command_line_parser, test_get_unknown_positional_argument) {
    const int argc = 1;
    const char* argv[] = {"test"};
    parser::command_line_parser parser("test", "version");
    parser.parse(argc, argv);
    EXPECT_THROW(parser.get_positional_argument<std::string>(), parser::parser_exception);
}

TEST(command_line_parser, test_get_wrong_positional_argument) {
    const int argc = 1;
    const char* argv[] = {"test"};
    parser::command_line_parser parser("test", "version");
    parser.add_positional_argument<std::string>("key");
    parser.parse(argc, argv);
    EXPECT_THROW(parser.get_positional_argument<int>(), parser::parser_exception);
}
