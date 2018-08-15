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

#include "io/automaton/utf16be.h"
#include "io/encoding.h"

namespace io {
    encoding utf16be::get_encoding() const {
        return encoding::UTF16BE;
    }
    
    bool utf16be::add_char(const int c) {
        if (current == -1) {
            current = (c << 8);
            return true;
        } else {
            bool alive = add_unicode(current | c);
            current = -1;
            return alive;;
        }
    }
}
