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

#ifndef CEDIT_PARSER_DEFAULT_CONVERTER_H_
#define CEDIT_PARSER_DEFAULT_CONVERTER_H_

#include <functional>
#include <sstream>
#include <string>
#include "parser/parser_exception.h"
#include "parser/parser_util.h"

namespace parser {
    namespace converter {
        template <class T>
        T convert(const std::string& base) {
            std::stringstream ss(base);
            T value;
            ss >> value;
            if (ss.eof()) {
                return value;
            } else {
                throw parser_exception(quote(base) + " can't be converted into " + get_type_name<T>());
            }
        }
        
        template <>
        inline std::string convert(const std::string& base) {
            return base;
        }
    }
    
    template <class T>
    std::function<T(const std::string&)> default_converter = converter::convert<T>;
}

#endif
