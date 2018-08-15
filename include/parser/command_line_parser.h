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

#ifndef CEDIT_PARSER_COMMAND_LINE_PARSER_H_
#define CEDIT_PARSER_COMMAND_LINE_PARSER_H_

#include <functional>
#include <map>
#include <memory>
#include <new>
#include <string>
#include <vector>
#include "parser/argument.h"
#include "parser/default_converter.h"
#include "parser/parser_exception.h"
#include "parser/parser_util.h"
#include "parser/positional_argument.h"
#include "parser/property.h"

namespace parser {
    class command_line_parser final {
        public:
        command_line_parser(const std::string& program_name = "", const std::string& version_info = "");
        ~command_line_parser() = default;
        
        template <class T>
        void add_property(const char short_name = '\0', const std::string& long_name = "", const std::string& description = "", const bool required = false, const T& default_value = T(), const std::function<T(const std::string&)>& converter = default_converter<T>);
        template <class T>
        void add_positional_argument(const std::string& name, const std::string& description = "", const bool required = false, const T& default_value = T(), const std::function<T(const std::string&)>& converter = default_converter<T>);
        
        void parse(const int argc, const char* const* argv);
        
        template <class T>
        const T& get(const char short_name) const;
        template <class T>
        const T& get(const std::string& long_name) const;
        template <class T>
        const T& get_positional_argument() const;
        
        private:
        const std::string program_name;
        const std::string version_info;
        std::shared_ptr<argument> positional_arg;
        std::map<std::string, std::shared_ptr<argument>> properties;
        std::vector<std::shared_ptr<argument>> all_properties;
        
        void show_usage(const std::string& error_message = "") const;
        void show_version() const;
        template <class T>
        const T& get_value(const std::string& name) const;
    };
    
    template <class T>
    void command_line_parser::add_property(const char short_name, const std::string& long_name, const std::string& description, const bool required, const T& default_value, const std::function<T(const std::string&)>& converter) {
        if (short_name == '\0' && long_name == "") throw parser_exception("both short name and long name are empty");
        
        const std::string short_option = to_short_option(short_name);
        const std::string long_option = to_long_option(long_name);
        std::string name = "";
        if (short_name != '\0') name += short_option;
        if (long_name != "") name += ((name == "") ? "" : ", ") + long_option;
        std::shared_ptr<argument> prop = std::make_shared<property<T>>(name, description, required, default_value, converter);
        
        if (short_name != '\0') {
            if (properties.count(short_option)) throw parser_exception(quote(short_option) + " is already defined");
            properties[short_option] = prop;
        }
        if (long_name != "") {
            if (properties.count(long_option)) throw parser_exception(quote(long_option) + " is already defined");
            properties[long_option] = prop;
        }
        all_properties.push_back(prop);
    }
    
    template <class T>
    void command_line_parser::add_positional_argument(const std::string& name, const std::string& description, const bool required, const T& default_value, const std::function<T(const std::string&)>& converter) {
        if (name == "") throw parser_exception("name is empty");
        if (positional_arg) throw parser_exception("another positional argument " + quote(positional_arg->get_name()) + " is already defined");
        positional_arg.reset(new positional_argument<T>(name, description, required, default_value, converter));
        all_properties.insert(all_properties.begin(), positional_arg);
    }
    
    template <class T>
    const T& command_line_parser::get(const char short_name) const {
        return get_value<T>(to_short_option(short_name));
    }
    
    template <class T>
    const T& command_line_parser::get(const std::string& long_name) const {
        return get_value<T>(to_long_option(long_name));
    }
    
    template <class T>
    const T& command_line_parser::get_positional_argument() const {
        if (!positional_arg) throw parser_exception("positional argument is undefined");
        std::shared_ptr<positional_argument<T>> arg = std::dynamic_pointer_cast<positional_argument<T>>(positional_arg);
        if (!arg) throw parser_exception(quote(positional_arg->get_name()) + " doesn't match " + get_type_name<T>());
        return arg->get_value();
    }
    
    template <class T>
    const T& command_line_parser::get_value(const std::string& name) const {
        if (properties.count(name) == 0) throw parser_exception(quote(name) + " is undefined");
        std::shared_ptr<property<T>> prop = std::dynamic_pointer_cast<property<T>>(properties.at(name));
        if (!prop) throw parser_exception(quote(name) + " doesn't match " + get_type_name<T>());
        return prop->get_value();
    }
}

#endif
