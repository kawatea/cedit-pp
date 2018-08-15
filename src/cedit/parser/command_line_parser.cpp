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

#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include "parser/argument.h"
#include "parser/command_line_parser.h"
#include "parser/parser_exception.h"
#include "parser/parser_util.h"

namespace parser {
    command_line_parser::command_line_parser(const std::string& program_name, const std::string& version_info) : program_name(program_name), version_info(version_info) {
        add_property<bool>('h', "help", "display this help and exit");
        add_property<bool>('v', "version", "output version information and exit");
    }
    
    void command_line_parser::parse(const int argc, const char* const* argv) {
        try {
            if (argc < 1) throw parser_exception("argument number must be bigger than  0");
            
            for (int i = 1; i < argc; i++) {
                std::string arg = argv[i];
                if (arg.find('-') == 0) {
                    const int pos = arg.find('=');
                    std::string value = "";
                    if (pos > 0) {
                        value = arg.substr(pos + 1);
                        arg = arg.substr(0, pos);
                        if (value == "") throw parser_exception(quote(arg) + " needs value");
                    }
                    if (!properties.count(arg)) throw parser_exception(quote(arg) + " is undefined");
                    
                    std::shared_ptr<argument> prop = properties.at(arg);
                    if (prop->is_assigned()) throw parser_exception(quote(prop->get_name()) + " is already parsed");
                    if (prop->is_boolean()) {
                        if (value != "") throw parser_exception(quote(prop->get_name()) + " doesn't need value");
                        if (arg == "-h" || arg == "--help") show_usage();
                        if (arg == "-v" || arg == "--version") show_version();
                        prop->set_value("1");
                    } else {
                        if (value == "") {
                            i++;
                            if (i >= argc) throw parser_exception(quote(prop->get_name()) + " needs value");
                            value = argv[i];
                        }
                        prop->set_value(value);
                    }
                } else {
                    if (!positional_arg) throw parser_exception("positional argument is undefined");
                    if (positional_arg->is_assigned()) throw parser_exception(quote(positional_arg->get_name()) + " is already parsed");
                    positional_arg->set_value(arg);
                }
            }
            
            for (const std::shared_ptr<argument>& prop : all_properties) {
                if (prop->is_required() && !prop->is_assigned()) throw parser_exception(quote(prop->get_name()) + " is required");
            }
        } catch (const std::exception& e) {
            show_usage(e.what());
        }
    }
    
    void command_line_parser::show_usage(const std::string& error_message) const {
        if (!error_message.empty()) std::cerr << error_message << std::endl;
        
        std::cerr << "Usage: " << program_name;
        if (positional_arg) {
            std::cerr << " " << (positional_arg->is_required() ? positional_arg->get_name() : ("[" + positional_arg->get_name() + "]"));
        }
        std::cerr << " [options]..." << std::endl;
        
        std::cerr << "Options: (* is required)" << std::endl;
        size_t max_length = 0;
        for (const std::shared_ptr<argument>& prop : all_properties) max_length = std::max(max_length, prop->get_name().length());
        for (const std::shared_ptr<argument>& prop : all_properties) std::cerr << (prop->is_required() ? "* " : "  ") << prop->get_name() << std::string(max_length - prop->get_name().length(), ' ') << "  " << prop->get_description() << std::endl;
        
        std::exit(error_message.empty() ? EXIT_SUCCESS : EXIT_FAILURE);
    }
    
    void command_line_parser::show_version() const {
        std::cerr << version_info << std::endl;
        std::exit(EXIT_SUCCESS);
    }
}
