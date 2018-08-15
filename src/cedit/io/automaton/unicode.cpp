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

#include "io/automaton/unicode.h"

namespace io {
    bool unicode::add_unicode(const int c) {
        if (surrogate) {
            if (c < 0xdc00 || c > 0xdfff) return false;
            surrogate = false;
        } else {
            if (c == 0x0a00 || (c >= 0xdc00 && c <= 0xdfff)) {
                return false;
            } else if (c >= 0xd800 && c <= 0xdbff) {
                surrogate = true;
                score *= 0.8;
            } else if (c >= 0x0080 && c <= 0x07ff) {
                score *= 0.9;
            } else if (c >= 0xa000 && c <= 0xf8ff) {
                score *= 0.8;
            }
        }
        
        return true;
    }
}
