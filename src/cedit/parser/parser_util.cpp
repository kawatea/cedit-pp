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

#include <string>
#include "parser/parser_util.h"

namespace parser {
    std::string quote(const std::string& value) {
        return '"' + value + '"';
    }
    
    std::string to_short_option(const char short_name) {
        return std::string("-") + short_name;
    }
    
    std::string to_long_option(const std::string& long_name) {
        return "--" + long_name;
    }
}
