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

#include "io/automaton/utf16.h"
#include "io/encoding.h"

namespace io {
    encoding utf16::get_encoding() const {
        return encoding::UTF16;
    }
    
    bool utf16::add_char(const int c) {
        if (c < 0xfe || first == c) return false;
        if (first == -1) {
            first = c;
        } else {
            definite = true;
        }
        return true;
    }
}
