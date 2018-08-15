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

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include "io/automaton/automaton.h"
#include "io/automaton/euc.h"
#include "io/automaton/jis.h"
#include "io/automaton/sjis.h"
#include "io/automaton/utf16.h"
#include "io/automaton/utf16be.h"
#include "io/automaton/utf16le.h"
#include "io/automaton/utf8.h"
#include "io/encoding.h"

namespace io {
    const encoding encoding::NONE = encoding({""}, "");
    const encoding encoding::JIS = encoding({"JIS"}, "ISO-2022-JP");
    const encoding encoding::SJIS = encoding({"Shift_JIS", "SJIS", "CP932"}, "CP932");
    const encoding encoding::EUC = encoding({"EUC-JP", "EUC"}, "EUC-JP");
    const encoding encoding::UTF8 = encoding({"UTF-8", "UTF8"}, "UTF-8");
    const encoding encoding::UTF16 = encoding({"UTF-16", "UTF16"}, "UTF-16");
    const encoding encoding::UTF16BE = encoding({"UTF-16BE", "UTF16BE"}, "UTF-16BE");
    const encoding encoding::UTF16LE = encoding({"UTF-16LE", "UTF16LE"}, "UTF-16LE");
    
    const std::vector<encoding> encoding::encodings = {encoding::JIS, encoding::SJIS, encoding::EUC, encoding::UTF8, encoding::UTF16, encoding::UTF16BE, encoding::UTF16LE};
    
    encoding::encoding(const std::vector<std::string>& names, const std::string& iconv_name) : names(names), iconv_name(iconv_name) {}
    
    bool encoding::operator==(const encoding& e) const {
        return iconv_name == e.iconv_name;
    }
    
    bool encoding::operator!=(const encoding& e) const {
        return !(*this == e);
    }
    
    const std::vector<encoding>& encoding::all_encodings() {
        return encodings;
    }
    
    encoding encoding::from_name(const std::string& name) {
        for (const encoding& encoding : encodings) {
            for (const std::string& encoding_name : encoding.names) {
                if (std::equal(name.begin(), name.end(), encoding_name.begin(), encoding_name.end(), [](char c1, char c2) { return tolower(c1) == tolower(c2); })) return encoding;
            }
        }
        throw std::invalid_argument(name + " can't be converted into proper encoding");
    }
    
    const std::string& encoding::get_name() const {
        return names[0];
    }
    
    const std::string& encoding::get_iconv_name() const {
        return iconv_name;
    }
    
    encoding detect_encoding(const std::string& file_name) {
        std::vector<std::unique_ptr<automaton>> current, next;
        current.emplace_back(new utf8());
        current.emplace_back(new sjis());
        current.emplace_back(new euc());
        current.emplace_back(new utf16());
        current.emplace_back(new utf16be());
        current.emplace_back(new utf16le());
        current.emplace_back(new jis());
        
        std::ifstream stream(file_name);
        while (true) {
            const int c = stream.get();
            if (c == EOF) break;
            
            for (auto&& automaton : current) {
                if (automaton->add_char(c)) {
                    if (automaton->is_definite()) return automaton->get_encoding();
                    if (automaton->get_score() >= 0.001) next.push_back(std::move(automaton));
                }
            }
            
            if (next.empty()) return encoding::NONE;
            if (next.size() == 1) return next[0]->get_encoding();
            current.swap(next);
            next.clear();
        }
        
        return (*std::max_element(current.begin(), current.end(), [](const std::unique_ptr<automaton>& a, const std::unique_ptr<automaton>& b) { return a->get_score() < b->get_score(); }))->get_encoding();
    }
}
