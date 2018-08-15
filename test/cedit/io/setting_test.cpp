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

#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <map>
#include <string>
#include <vector>
#include <glibmm/ustring.h>
#include "gtest/gtest.h"
#include "io/file.h"
#include "io/setting.h"

TEST(setting, test_get) {
    io::setting::create("test/resource/setting/setting");
    const io::setting& setting = io::setting::get();
    
    EXPECT_EQ(true, setting.get_boolean("bool"));
    EXPECT_EQ(false, setting.get_boolean("c++", "bool"));
    EXPECT_EQ(true, setting.get_boolean("java", "bool"));
    EXPECT_EQ(true, setting.get_boolean("python", "bool"));
    EXPECT_EQ(true, setting.get_boolean("", "bool"));
    EXPECT_EQ(false, setting.get_boolean("boolean"));
    EXPECT_EQ(false, setting.get_boolean("c++", "boolean"));
    EXPECT_EQ(false, setting.get_boolean("java", "boolean"));
    EXPECT_EQ(false, setting.get_boolean("python", "boolean"));
    EXPECT_EQ(false, setting.get_boolean("", "boolean"));
    
    EXPECT_EQ(1, setting.get_integer("int"));
    EXPECT_EQ(2, setting.get_integer("c++", "int"));
    EXPECT_EQ(1, setting.get_integer("java", "int"));
    EXPECT_EQ(1, setting.get_integer("python", "int"));
    EXPECT_EQ(1, setting.get_integer("", "int"));
    EXPECT_EQ(0, setting.get_integer("integer"));
    EXPECT_EQ(0, setting.get_integer("c++", "integer"));
    EXPECT_EQ(0, setting.get_integer("java", "integer"));
    EXPECT_EQ(0, setting.get_integer("python", "integer"));
    EXPECT_EQ(0, setting.get_integer("", "integer"));
    
    EXPECT_EQ("test", setting.get_string("string"));
    EXPECT_EQ("c++", setting.get_string("c++", "string"));
    EXPECT_EQ("test", setting.get_string("java", "string"));
    EXPECT_EQ("test", setting.get_string("python", "string"));
    EXPECT_EQ("test", setting.get_string("", "string"));
    EXPECT_EQ("", setting.get_string("str"));
    EXPECT_EQ("", setting.get_string("c++", "str"));
    EXPECT_EQ("", setting.get_string("java", "str"));
    EXPECT_EQ("", setting.get_string("python", "str"));
    EXPECT_EQ("", setting.get_string("", "str"));
    
    io::setting::destroy();
}

TEST(setting, test_set) {
    const std::string dir = "tmp/";
    const std::string name = dir + "setting";
    
    #ifdef _WIN32
    #define mkdir(path, mode) mkdir(path)
    #endif
    mkdir(dir.c_str(), ACCESSPERMS);
    io::file(name).write("");
    io::setting::create(name);
    io::setting& setting = io::setting::get();
    
    EXPECT_EQ(false, setting.get_boolean("bool"));
    EXPECT_EQ(0, setting.get_integer("int"));
    EXPECT_EQ("", setting.get_string("string"));
    
    setting.set_boolean("bool", true);
    setting.set_integer("int", 1);
    setting.set_string("string", "string");
    EXPECT_EQ(true, setting.get_boolean("bool"));
    EXPECT_EQ(1, setting.get_integer("int"));
    EXPECT_EQ("string", setting.get_string("string"));
    
    setting.set_boolean("bool", false);
    setting.set_integer("int", 2);
    setting.set_string("string", "");
    EXPECT_EQ(false, setting.get_boolean("bool"));
    EXPECT_EQ(2, setting.get_integer("int"));
    EXPECT_EQ("", setting.get_string("string"));
    
    setting.set_boolean("c++", "bool", true);
    setting.set_integer("c++", "int", 3);
    setting.set_string("c++", "string", "string");
    EXPECT_EQ(false, setting.get_boolean("bool"));
    EXPECT_EQ(2, setting.get_integer("int"));
    EXPECT_EQ("", setting.get_string("string"));
    EXPECT_EQ(true, setting.get_boolean("c++", "bool"));
    EXPECT_EQ(3, setting.get_integer("c++", "int"));
    EXPECT_EQ("string", setting.get_string("c++", "string"));
    
    setting.set_boolean("", "bool", true);
    setting.set_integer("", "int", 4);
    setting.set_string("", "string", "str");
    EXPECT_EQ(true, setting.get_boolean("bool"));
    EXPECT_EQ(4, setting.get_integer("int"));
    EXPECT_EQ("str", setting.get_string("string"));
    EXPECT_EQ(true, setting.get_boolean("c++", "bool"));
    EXPECT_EQ(3, setting.get_integer("c++", "int"));
    EXPECT_EQ("string", setting.get_string("c++", "string"));
    EXPECT_EQ(true, setting.get_boolean("", "bool"));
    EXPECT_EQ(4, setting.get_integer("", "int"));
    EXPECT_EQ("str", setting.get_string("", "string"));
    
    io::setting::destroy();
    std::remove(name.c_str());
    rmdir(dir.c_str());
}

TEST(setting, test_accelerators) {
    const std::string dir = "tmp/";
    const std::string name = dir + "setting";
    
    #ifdef _WIN32
    #define mkdir(path, mode) mkdir(path)
    #endif
    mkdir(dir.c_str(), ACCESSPERMS);
    io::file(name).write("");
    io::setting::create(name);
    io::setting& setting = io::setting::get();
    
    {
        const std::map<Glib::ustring, std::vector<Glib::ustring>> accelerators = setting.get_accelerators();
        EXPECT_EQ(0, accelerators.size());
    }
    
    {
        setting.set_accelerators({{"key1", {"value1"}}, {"key2", {"value2a", "value2b"}}});
        const std::map<Glib::ustring, std::vector<Glib::ustring>> accelerators = setting.get_accelerators();
        EXPECT_EQ(2, accelerators.size());
        EXPECT_EQ(std::vector<Glib::ustring>({"value1"}), accelerators.at("key1"));
        EXPECT_EQ(std::vector<Glib::ustring>({"value2a", "value2b"}), accelerators.at("key2"));
    }
    
    {
        setting.set_accelerators({{"key1", {"value0"}}, {"key2", {}}, {"key3", {"value3"}}});
        const std::map<Glib::ustring, std::vector<Glib::ustring>> accelerators = setting.get_accelerators();
        EXPECT_EQ(3, accelerators.size());
        EXPECT_EQ(std::vector<Glib::ustring>({"value0"}), accelerators.at("key1"));
        EXPECT_EQ(std::vector<Glib::ustring>(), accelerators.at("key2"));
        EXPECT_EQ(std::vector<Glib::ustring>({"value3"}), accelerators.at("key3"));
    }
    
    io::setting::destroy();
    std::remove(name.c_str());
    rmdir(dir.c_str());
}
