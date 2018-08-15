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

#ifndef CEDIT_PARSER_PARSER_EXCEPTION_H_
#define CEDIT_PARSER_PARSER_EXCEPTION_H_

#include <exception>
#include <string>

namespace parser {
    class parser_exception final : public std::exception {
        public:
        parser_exception(const std::string& message) : message(message) {}
        ~parser_exception() = default;
        
        const char* what() const noexcept override {
            return message.c_str();
        }
        
        private:
        const std::string message;
    };
}

#endif
