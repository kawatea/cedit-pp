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

#include "io/automaton/jis.h"
#include "io/encoding.h"

namespace io {
    encoding jis::get_encoding() const {
        return encoding::JIS;
    }
    
    bool jis::add_char(const int c) {
        if (c >= 0x80) return false;
        if (escape && (c == 0x24 || c == 0x28)) definite = true;
        escape = (c == 0x1b);
        if (c == 0x00) score *= 0.8;
        return true;
    }
}
