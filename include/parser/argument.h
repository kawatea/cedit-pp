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

#ifndef CEDIT_PARSER_ARGUMENT_H_
#define CEDIT_PARSER_ARGUMENT_H_

#include <string>

namespace parser {
    class argument {
        public:
        virtual ~argument() = default;
        
        virtual bool is_boolean() const = 0;
        virtual void set_value(const std::string& value) = 0;
        
        const std::string& get_name() const {
            return name;
        }
        
        const std::string& get_description() const {
            return description;
        }
        
        bool is_required() const {
            return required;
        }
        
        bool is_assigned() const {
            return assigned;
        }
        
        protected:
        bool assigned = false;
        
        argument(const std::string& name, const std::string& description, const bool required) : name(name), description(description), required(required) {}
        
        private:
        const std::string name;
        const std::string description;
        const bool required;
    };
}

#endif
