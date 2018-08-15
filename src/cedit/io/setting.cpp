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
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <glibmm/miscutils.h>
#include "io/io_util.h"
#include "io/setting.h"

namespace io {
    const Glib::ustring setting::BORDER_WIDTH = "Border Width";
    const Glib::ustring setting::WIDTH = "Width";
    const Glib::ustring setting::HEIGHT = "Height";
    const Glib::ustring setting::POSITION_X = "Position x";
    const Glib::ustring setting::POSITION_Y = "Position y";
    const Glib::ustring setting::FONT = "Font";
    const Glib::ustring setting::SHOW_TOOLBAR = "Show Toolbar";
    const Glib::ustring setting::SEARCH_CASE_SENSITIVE = "Search Case Sensitive";
    const Glib::ustring setting::SEARCH_CONTROL_CHARACTERS = "Search Control Characters";
    const Glib::ustring setting::SEARCH_INTERACTIVE = "Search Interactive";
    
    const Glib::ustring setting::TAB_WIDTH = "Tab Width";
    const Glib::ustring setting::FLYMAKE_COMMAND = "Flymake Command";
    const Glib::ustring setting::WRAP_TEXT = "Wrap Text";
    const Glib::ustring setting::LINE_NUMBERS = "Show Line Numbers";
    const Glib::ustring setting::AUTO_INDENT = "Auto Indent";
    const Glib::ustring setting::HIGHLIGHT_LINE = "Highlight Current Line";
    const Glib::ustring setting::INSERT_SPACES = "Spaces instead of Tabs";
    const Glib::ustring setting::HUNGRY_DELETE = "Hungry Delete";
    
    const Glib::ustring setting::GENERAL = "General";
    const Glib::ustring setting::ACCELERATOR = "Accelerator";
    
    std::unique_ptr<setting> setting::instance;
    
    setting::setting(const std::string& filename) : filename(filename) {
        load();
    }
    
    setting& setting::get() {
        if (!instance) create(get_path("src/resource/setting/setting"));
        return *instance;
    }
    
    void setting::create(const std::string& filename) {
        if (instance && instance->filename != filename) {
            std::cerr << "can't create multiple settings with different names" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        if (!instance) instance.reset(new setting(filename));
    }
    
    void setting::destroy() {
        instance.reset();
    }
    
    void setting::load() {
        key_file.load_from_file(filename, Glib::KEY_FILE_KEEP_COMMENTS);
    }
    
    void setting::save() {
        key_file.save_to_file(filename);
    }
    
    bool setting::get_boolean(const Glib::ustring& key) const {
        return get_boolean(GENERAL, key);
    }
    
    bool setting::get_boolean(const Glib::ustring& language, const Glib::ustring& key) const {
        if (key_file.has_group(language) && key_file.has_key(language, key)) return key_file.get_boolean(language, key);
        if (key_file.has_group(GENERAL) && key_file.has_key(GENERAL, key)) return key_file.get_boolean(GENERAL, key);
        return false;
    }
    
    void setting::set_boolean(const Glib::ustring& key, const bool value) {
        set_boolean(GENERAL, key, value);
    }
    
    void setting::set_boolean(const Glib::ustring& language, const Glib::ustring& key, const bool value) {
        if (get_boolean(language, key) == value) return;
        key_file.set_boolean((language == "") ? GENERAL : language, key, value);
        save();
    }
    
    int setting::get_integer(const Glib::ustring& key) const {
        return get_integer(GENERAL, key);
    }
    
    int setting::get_integer(const Glib::ustring& language, const Glib::ustring& key) const {
        if (key_file.has_group(language) && key_file.has_key(language, key)) return key_file.get_integer(language, key);
        if (key_file.has_group(GENERAL) && key_file.has_key(GENERAL, key)) return key_file.get_integer(GENERAL, key);
        return 0;
    }
    
    void setting::set_integer(const Glib::ustring& key, const int value) {
        set_integer(GENERAL, key, value);
    }
    
    void setting::set_integer(const Glib::ustring& language, const Glib::ustring& key, const int value) {
        if (get_integer(language, key) == value) return;
        key_file.set_integer((language == "") ? GENERAL : language, key, value);
        save();
    }
    
    Glib::ustring setting::get_string(const Glib::ustring& key) const {
        return get_string(GENERAL, key);
    }
    
    Glib::ustring setting::get_string(const Glib::ustring& language, const Glib::ustring& key) const {
        if (key_file.has_group(language) && key_file.has_key(language, key)) return key_file.get_string(language, key);
        if (key_file.has_group(GENERAL) && key_file.has_key(GENERAL, key)) return key_file.get_string(GENERAL, key);
        return "";
    }
    
    void setting::set_string(const Glib::ustring& key, const Glib::ustring& value) {
        set_string(GENERAL, key, value);
    }
    
    void setting::set_string(const Glib::ustring& language, const Glib::ustring& key, const Glib::ustring& value) {
        if (get_string(language, key) == value) return;
        key_file.set_string((language == "") ? GENERAL : language, key, value);
        save();
    }
    
    std::map<Glib::ustring, std::vector<Glib::ustring>> setting::get_accelerators() const {
        std::map<Glib::ustring, std::vector<Glib::ustring>> accelerators;
        if (key_file.has_group(ACCELERATOR)) {
            for (const Glib::ustring& key : key_file.get_keys(ACCELERATOR)) accelerators[key] = key_file.get_string_list(ACCELERATOR, key);
        }
        return accelerators;
    }
    
    void setting::set_accelerators(const std::map<Glib::ustring, std::vector<Glib::ustring>>& accelerators) {
        for (const std::pair<Glib::ustring, std::vector<Glib::ustring>>& accelerator : accelerators) key_file.set_string_list(ACCELERATOR, accelerator.first, accelerator.second);
        save();
    }
}
