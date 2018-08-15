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

#ifndef CEDIT_PARSER_POSITIONAL_ARGUMENT_H_
#define CEDIT_PARSER_POSITIONAL_ARGUMENT_H_

#include <functional>
#include <string>

namespace parser {
    template <class T>
    class positional_argument final : public argument {
        public:
        positional_argument(const std::string& name, const std::string& description, const bool required, const T& default_value, const std::function<T(const std::string&)>& converter) : argument(name, description, required), converter(converter), value(default_value) {}
        ~positional_argument() = default;
        
        bool is_boolean() const override {
            return false;
        }
        
        void set_value(const std::string& value) override {
            assigned = true;
            this->value = converter(value);
        }
        
        const T& get_value() const {
            return value;
        }
        
        private:
        const std::function<T(const std::string&)> converter;
        T value;
    };
}

#endif
