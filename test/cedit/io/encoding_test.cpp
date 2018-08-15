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

#include <stdexcept>
#include "gtest/gtest.h"
#include "io/encoding.h"

TEST(encoding, test_from_name) {
    EXPECT_EQ(io::encoding::JIS, io::encoding::from_name("jis"));
    EXPECT_EQ(io::encoding::JIS, io::encoding::from_name("JIS"));
    EXPECT_EQ(io::encoding::SJIS, io::encoding::from_name("shift_jis"));
    EXPECT_EQ(io::encoding::SJIS, io::encoding::from_name("Shift_JIS"));
    EXPECT_EQ(io::encoding::SJIS, io::encoding::from_name("sjis"));
    EXPECT_EQ(io::encoding::SJIS, io::encoding::from_name("SJIS"));
    EXPECT_EQ(io::encoding::SJIS, io::encoding::from_name("cp932"));
    EXPECT_EQ(io::encoding::SJIS, io::encoding::from_name("CP932"));
    EXPECT_EQ(io::encoding::EUC, io::encoding::from_name("euc-jp"));
    EXPECT_EQ(io::encoding::EUC, io::encoding::from_name("EUC-JP"));
    EXPECT_EQ(io::encoding::EUC, io::encoding::from_name("euc"));
    EXPECT_EQ(io::encoding::EUC, io::encoding::from_name("EUC"));
    EXPECT_EQ(io::encoding::UTF8, io::encoding::from_name("utf-8"));
    EXPECT_EQ(io::encoding::UTF8, io::encoding::from_name("UTF-8"));
    EXPECT_EQ(io::encoding::UTF8, io::encoding::from_name("utf8"));
    EXPECT_EQ(io::encoding::UTF8, io::encoding::from_name("UTF8"));
    EXPECT_EQ(io::encoding::UTF16, io::encoding::from_name("utf-16"));
    EXPECT_EQ(io::encoding::UTF16, io::encoding::from_name("UTF-16"));
    EXPECT_EQ(io::encoding::UTF16, io::encoding::from_name("utf16"));
    EXPECT_EQ(io::encoding::UTF16, io::encoding::from_name("UTF16"));
    EXPECT_EQ(io::encoding::UTF16BE, io::encoding::from_name("utf-16be"));
    EXPECT_EQ(io::encoding::UTF16BE, io::encoding::from_name("UTF-16BE"));
    EXPECT_EQ(io::encoding::UTF16BE, io::encoding::from_name("utf16be"));
    EXPECT_EQ(io::encoding::UTF16BE, io::encoding::from_name("UTF16BE"));
    EXPECT_EQ(io::encoding::UTF16LE, io::encoding::from_name("utf-16le"));
    EXPECT_EQ(io::encoding::UTF16LE, io::encoding::from_name("UTF-16LE"));
    EXPECT_EQ(io::encoding::UTF16LE, io::encoding::from_name("utf16le"));
    EXPECT_EQ(io::encoding::UTF16LE, io::encoding::from_name("UTF16LE"));
    EXPECT_THROW(io::encoding::from_name(""), std::invalid_argument);
    EXPECT_THROW(io::encoding::from_name("none"), std::invalid_argument);
    EXPECT_THROW(io::encoding::from_name("NONE"), std::invalid_argument);
    EXPECT_THROW(io::encoding::from_name("shiftjis"), std::invalid_argument);
    EXPECT_THROW(io::encoding::from_name("shift-jis"), std::invalid_argument);
    EXPECT_THROW(io::encoding::from_name("eucjp"), std::invalid_argument);
    EXPECT_THROW(io::encoding::from_name("euc_jp"), std::invalid_argument);
    EXPECT_THROW(io::encoding::from_name("utf_8"), std::invalid_argument);
    EXPECT_THROW(io::encoding::from_name("utf_16"), std::invalid_argument);
    EXPECT_THROW(io::encoding::from_name("utf_16be"), std::invalid_argument);
    EXPECT_THROW(io::encoding::from_name("utf_16le"), std::invalid_argument);
    EXPECT_THROW(io::encoding::from_name("test"), std::invalid_argument);
}

TEST(encoding, test_get_name) {
    EXPECT_EQ("", io::encoding::NONE.get_name());
    EXPECT_EQ("JIS", io::encoding::JIS.get_name());
    EXPECT_EQ("Shift_JIS", io::encoding::SJIS.get_name());
    EXPECT_EQ("EUC-JP", io::encoding::EUC.get_name());
    EXPECT_EQ("UTF-8", io::encoding::UTF8.get_name());
    EXPECT_EQ("UTF-16", io::encoding::UTF16.get_name());
    EXPECT_EQ("UTF-16BE", io::encoding::UTF16BE.get_name());
    EXPECT_EQ("UTF-16LE", io::encoding::UTF16LE.get_name());
}

TEST(encoding, test_get_iconv_name) {
    EXPECT_EQ("", io::encoding::NONE.get_iconv_name());
    EXPECT_EQ("ISO-2022-JP", io::encoding::JIS.get_iconv_name());
    EXPECT_EQ("CP932", io::encoding::SJIS.get_iconv_name());
    EXPECT_EQ("EUC-JP", io::encoding::EUC.get_iconv_name());
    EXPECT_EQ("UTF-8", io::encoding::UTF8.get_iconv_name());
    EXPECT_EQ("UTF-16", io::encoding::UTF16.get_iconv_name());
    EXPECT_EQ("UTF-16BE", io::encoding::UTF16BE.get_iconv_name());
    EXPECT_EQ("UTF-16LE", io::encoding::UTF16LE.get_iconv_name());
}

TEST(encoding, test_detect_encoding) {
    const std::string dir = "test/resource/file/";
    
    EXPECT_EQ(io::encoding::JIS, io::detect_encoding(dir + "jis.txt"));
    EXPECT_EQ(io::encoding::SJIS, io::detect_encoding(dir + "sjis.txt"));
    EXPECT_EQ(io::encoding::EUC, io::detect_encoding(dir + "euc.txt"));
    EXPECT_EQ(io::encoding::UTF8, io::detect_encoding(dir + "utf8.txt"));
    EXPECT_EQ(io::encoding::UTF16, io::detect_encoding(dir + "utf16_1.txt"));
    EXPECT_EQ(io::encoding::UTF16, io::detect_encoding(dir + "utf16_2.txt"));
    EXPECT_EQ(io::encoding::UTF16BE, io::detect_encoding(dir + "utf16be.txt"));
    EXPECT_EQ(io::encoding::UTF16LE, io::detect_encoding(dir + "utf16le.txt"));
}
