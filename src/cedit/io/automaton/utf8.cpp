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

#include "io/automaton/utf8.h"
#include "io/encoding.h"

namespace io {
    encoding utf8::get_encoding() const {
        return encoding::UTF8;
    }
    
    bool utf8::add_char(const int c) {
        if (count == 0) {
            if (c >= 0x80) {
                if (c >= 0xc2 && c <= 0xdf) {
                    count = 1;
                    minimum = 0x80;
                    maximum = 0xbf;
                    score *= 0.9;
                } else if (c >= 0xe0 && c <= 0xef) {
                    count = 2;
                    minimum = ((c == 0xe0) ? 0xa0 : 0x80);
                    maximum = ((c == 0xed) ? 0x9f : 0xbf);
                    if (c >= 0xea && c <= 0xee) score *= 0.8;
                } else if (c >= 0xf0 && c <= 0xf4) {
                    count = 3;
                    minimum = ((c == 0xf0) ? 0x90 : 0x80);
                    maximum = ((c == 0xf4) ? 0x8f : 0xbf);
                    score *= 0.8;
                } else {
                    return false;
                }
            } else if (c == 0x00) {
                score *= 0.8;
            }
        } else {
            if (c < minimum || c > maximum) return false;
            count--;
            minimum = 0x80;
            maximum = 0xbf;
        }
        return true;
    }
}
