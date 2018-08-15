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

#include "io/automaton/sjis.h"
#include "io/encoding.h"

namespace io {
    encoding sjis::get_encoding() const {
        return encoding::SJIS;
    }
    
    bool sjis::add_char(const int c) {
        if (multi) {
            if (c < 0x40 || c == 0x7f || c > 0xfc) return false;
            multi = false;
        } else {
            if (c >= 0x80) {
                if (c == 0x80 || c == 0xa0 || c > 0xfc) return false;
                if (c >= 0xa1 && c <= 0xdf) {
                    score *= 0.9;
                } else {
                    multi = true;
                    if (c >= 0xf0) score *= 0.9;
                }
            } else if (c == 0x00) {
                score *= 0.8;
            }
        }
        return true;
    }
}
