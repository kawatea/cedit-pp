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

#ifndef CEDIT_IO_AUTOMATON_UTF16LE_H_
#define CEDIT_IO_AUTOMATON_UTF16LE_H_

#include "io/automaton/unicode.h"
#include "io/encoding.h"

namespace io {
    class utf16le final : public unicode {
        public:
        utf16le() = default;
        ~utf16le() = default;
        
        encoding get_encoding() const override;
        bool add_char(const int c) override;
        
        private:
        int current = -1;
    };
}

#endif
