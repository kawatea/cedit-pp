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

#include <functional>
#include <string>
#include "gtest/gtest.h"
#include "parser/default_converter.h"
#include "parser/parser_exception.h"

TEST(default_converter, test_convert_string) {
    std::function<std::string(const std::string&)> converter = parser::default_converter<std::string>;
    EXPECT_EQ("test", converter("test"));
    EXPECT_EQ("a b c", converter("a b c"));
    EXPECT_EQ("", converter(""));
}

TEST(default_converter, test_convert_int) {
    std::function<int(const std::string&)> converter = parser::default_converter<int>;
    EXPECT_EQ(100, converter("100"));
    EXPECT_EQ(100, converter("+100"));
    EXPECT_EQ(-100, converter("-100"));
    EXPECT_THROW(converter("test"), parser::parser_exception);
    EXPECT_THROW(converter("1 1"), parser::parser_exception);
}

TEST(default_converter, test_convert_bool) {
    std::function<bool(const std::string&)> converter = parser::default_converter<bool>;
    EXPECT_TRUE(converter("1"));
    EXPECT_FALSE(converter("0"));
    EXPECT_THROW(converter("test"), parser::parser_exception);
    EXPECT_THROW(converter("1 1"), parser::parser_exception);
}
