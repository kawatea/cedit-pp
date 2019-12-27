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

#ifndef CEDIT_IO_SETTING_H_
#define CEDIT_IO_SETTING_H_

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <glibmm/keyfile.h>

namespace io {
    class setting final {
        public:
        static const Glib::ustring BORDER_WIDTH;
        static const Glib::ustring WIDTH;
        static const Glib::ustring HEIGHT;
        static const Glib::ustring POSITION_X;
        static const Glib::ustring POSITION_Y;
        static const Glib::ustring FONT;
        static const Glib::ustring SHOW_TOOLBAR;
        static const Glib::ustring SEARCH_CASE_SENSITIVE;
        static const Glib::ustring SEARCH_CONTROL_CHARACTERS;
        static const Glib::ustring SEARCH_INTERACTIVE;
        
        static const Glib::ustring TAB_WIDTH;
        static const Glib::ustring FLYMAKE_COMMAND;
        static const Glib::ustring WRAP_TEXT;
        static const Glib::ustring LINE_NUMBERS;
        static const Glib::ustring AUTO_INDENT;
        static const Glib::ustring HIGHLIGHT_LINE;
        static const Glib::ustring INSERT_SPACES;
        static const Glib::ustring HUNGRY_DELETE;
        
        ~setting() = default;
        setting(const setting&) = delete;
        setting& operator=(const setting&) = delete;
        setting(setting&&) = delete;
        setting& operator=(setting&&) = delete;
        
        static setting& get();
        // For test
        static void create(const std::string& filename);
        static void destroy();
        
        void load();
        void save();
        
        bool get_boolean(const Glib::ustring& key) const;
        bool get_boolean(const Glib::ustring& language, const Glib::ustring& key) const;
        void set_boolean(const Glib::ustring& key, const bool value);
        void set_boolean(const Glib::ustring& language, const Glib::ustring& key, const bool value);
        
        int get_integer(const Glib::ustring& key) const;
        int get_integer(const Glib::ustring& language, const Glib::ustring& key) const;
        void set_integer(const Glib::ustring& key, const int value);
        void set_integer(const Glib::ustring& language, const Glib::ustring& key, const int value);
        
        Glib::ustring get_string(const Glib::ustring& key) const;
        Glib::ustring get_string(const Glib::ustring& language, const Glib::ustring& key) const;
        void set_string(const Glib::ustring& key, const Glib::ustring& value);
        void set_string(const Glib::ustring& language, const Glib::ustring& key, const Glib::ustring& value);
        
        std::map<Glib::ustring, std::vector<Glib::ustring>> get_accelerators() const;
        void set_accelerators(const std::map<Glib::ustring, std::vector<Glib::ustring>>& accelerators);
        
        std::vector<std::pair<Glib::ustring, Glib::ustring>> get_macros() const;
        void set_macros(const std::vector<std::pair<Glib::ustring, Glib::ustring>>& macros);
        
        private:
        static const Glib::ustring GENERAL;
        static const Glib::ustring ACCELERATOR;
        static const Glib::ustring MACRO;
        
        static std::unique_ptr<setting> instance;
        
        const std::string filename;
        Glib::KeyFile key_file;
        
        setting(const std::string& filename);
    };
}

#endif
