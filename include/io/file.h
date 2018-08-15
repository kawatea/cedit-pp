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

#ifndef CEDIT_IO_FILE_H_
#define CEDIT_IO_FILE_H_

#include <string>
#include <glibmm/ustring.h>
#include "io/encoding.h"

namespace io {
    class file final {
        public:
        file(const std::string& name = "", const encoding& encode = encoding::NONE);
        ~file() = default;
        
        const std::string& get_full_name() const;
        const std::string& get_simple_name() const;
        std::string get_title_name() const;
        const encoding& get_encoding() const;
        void set_encoding(const encoding& encode);
        bool is_edited() const;
        void set_edited(const bool edited);
        bool is_empty() const;
        bool exists() const;
        bool can_open() const;
        Glib::ustring read() const;
        void write(const Glib::ustring& text) const;
        
        private:
        const std::string full_name;
        const std::string simple_name;
        encoding encode;
        bool edited = false;
    };
}

#endif
