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
#include "gtest/gtest.h"
#include "parser/parser_util.h"

class test_class {};

TEST(parser_util, test_get_type_name) {
    EXPECT_EQ("int", parser::get_type_name<int>());
    EXPECT_EQ("string", parser::get_type_name<std::string>());
    EXPECT_EQ("test_class", parser::get_type_name<test_class>());
}

TEST(parser_util, test_quote) {
    EXPECT_EQ("\"test\"", parser::quote("test"));
    EXPECT_EQ("\"a b c\"", parser::quote("a b c"));
}

TEST(parser_util, test_to_short_option) {
    EXPECT_EQ("-a", parser::to_short_option('a'));
}

TEST(parser_util, test_to_long_option) {
    EXPECT_EQ("--a", parser::to_long_option("a"));
    EXPECT_EQ("--test", parser::to_long_option("test"));
}
