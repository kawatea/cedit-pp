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
#include <string>
#include <glibmm/ustring.h>
#include "gtest/gtest.h"
#include "io/encoding.h"
#include "io/file.h"

TEST(file, test_get_simple_name) {
    EXPECT_EQ("", io::file("").get_simple_name());
    EXPECT_EQ("", io::file("test/").get_simple_name());
    EXPECT_EQ("", io::file("test\\").get_simple_name());
    EXPECT_EQ("test", io::file("test").get_simple_name());
    EXPECT_EQ("test2", io::file("test1/test2").get_simple_name());
    EXPECT_EQ("test2", io::file("test1\\test2").get_simple_name());
    EXPECT_EQ("test1\ttest2", io::file("test1\ttest2").get_simple_name());
}

TEST(file, test_get_title_name) {
    {
        io::file file("");
        EXPECT_EQ("Untitled", file.get_title_name());
        file.set_edited(true);
        EXPECT_EQ("* Untitled", file.get_title_name());
    }
    {
        io::file file("test");
        EXPECT_EQ("test", file.get_title_name());
        file.set_edited(true);
        EXPECT_EQ("* test", file.get_title_name());
    }
    {
        io::file file("test1/test2");
        EXPECT_EQ("test2", file.get_title_name());
        file.set_edited(true);
        EXPECT_EQ("* test2", file.get_title_name());
    }
    {
        io::file file("test1\\test2");
        EXPECT_EQ("test2", file.get_title_name());
        file.set_edited(true);
        EXPECT_EQ("* test2", file.get_title_name());
    }
}

TEST(file, test_exists) {
    const std::string dir = "test/resource/file";
    
    EXPECT_TRUE(io::file(".").exists());
    EXPECT_TRUE(io::file(dir).exists());
    EXPECT_TRUE(io::file(dir + "/").exists());
    EXPECT_TRUE(io::file(dir + "/input.txt").exists());
    EXPECT_FALSE(io::file(dir + "/dummy.txt").exists());
    EXPECT_FALSE(io::file(dir + "/dummy/").exists());
}

TEST(file, test_can_open) {
    const std::string dir = "test/resource/file";
    
    EXPECT_TRUE(io::file(dir + "/input.txt").can_open());
    EXPECT_TRUE(io::file(dir + "/dummy.txt").can_open());
    EXPECT_FALSE(io::file(".").can_open());
    EXPECT_FALSE(io::file(dir).can_open());
    EXPECT_FALSE(io::file(dir + "/").can_open());
    EXPECT_FALSE(io::file(dir + "/dummy/").can_open());
}

TEST(file, test_read) {
    const std::string dir = "test/resource/file/";
    const Glib::ustring text = "abcde\nあいうえお\n12345\n";
    
    EXPECT_EQ(text, io::file(dir + "jis.txt", io::encoding::JIS).read());
    EXPECT_EQ(text, io::file(dir + "sjis.txt", io::encoding::SJIS).read());
    EXPECT_EQ(text, io::file(dir + "euc.txt", io::encoding::EUC).read());
    EXPECT_EQ(text, io::file(dir + "utf8.txt", io::encoding::UTF8).read());
    EXPECT_EQ(text, io::file(dir + "utf16_1.txt", io::encoding::UTF16).read());
    EXPECT_EQ(text, io::file(dir + "utf16_2.txt", io::encoding::UTF16).read());
    EXPECT_EQ(text, io::file(dir + "utf16be.txt", io::encoding::UTF16BE).read());
    EXPECT_EQ(text, io::file(dir + "utf16le.txt", io::encoding::UTF16LE).read());
}

TEST(file, test_write) {
    const std::string dir = "tmp/";
    const std::string name = dir + "test.txt";
    const Glib::ustring text = "abcde\nあいうえお\n12345\n";
    
    #ifdef _WIN32
    #define mkdir(path, mode) mkdir(path)
    #endif
    mkdir(dir.c_str(), ACCESSPERMS);
    
    for (const io::encoding& encoding : io::encoding::all_encodings()) {
        const io::file file(name, encoding);
        file.write(text);
        EXPECT_EQ(text, file.read());
        std::remove(name.c_str());
    }
    
    rmdir(dir.c_str());
}
