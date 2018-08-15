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

#ifndef CEDIT_PARSER_PARSER_UTIL_H_
#define CEDIT_PARSER_PARSER_UTIL_H_

#include <string>
#include <typeinfo>

namespace parser {
    template <class T>
    std::string get_type_name();
    
    std::string quote(const std::string& value);
    
    std::string to_short_option(const char short_name);
    
    std::string to_long_option(const std::string& long_name);
}

#ifdef __GNUG__
#include <cxxabi.h>
#include <cstdlib>
#include <memory>

namespace parser {
    template <class T>
    std::string get_type_name() {
        int status = 0;
        const char* name = typeid(T).name();
        std::unique_ptr<char, decltype(std::free)*> demangled(abi::__cxa_demangle(name, nullptr, nullptr, &status), std::free);
        return (status == 0) ? demangled.get() : name;
    }
}

#else

namespace parser {
    template <class T>
    std::string get_type_name() {
        return typeid(T).name();
    }
}

#endif

namespace parser {
    template <>
    inline std::string get_type_name<std::string>() {
        return "string";
    }
}

#endif
