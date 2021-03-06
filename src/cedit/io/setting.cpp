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
#include "io/gettext.h"
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
    const Glib::ustring setting::MACRO = "Macro";
    
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
    
    std::vector<std::pair<Glib::ustring, Glib::ustring>> setting::list_acceleratable_actions() {
        static std::vector<std::pair<Glib::ustring, Glib::ustring>> acceleratable_actions = {
            {"win.file_new", _("File/New")},
            {"win.file_open", _("File/Open")},
            {"win.file_reload", _("File/Reload")},
            {"win.file_save", _("File/Save")},
            {"win.file_save_as", _("File/Save As")},
            {"win.file_print", _("File/Print")},
            {"win.file_quit", _("File/Quit")},
            {"win.edit_undo", _("Edit/Undo")},
            {"win.edit_redo", _("Edit/Redo")},
            {"win.edit_cut", _("Edit/Cut")},
            {"win.edit_copy", _("Edit/Copy")},
            {"win.edit_paste", _("Edit/Paste")},
            {"win.edit_delete", _("Edit/Delete")},
            {"win.edit_kill_line", _("Edit/Kill Line")},
            {"win.edit_select_all", _("Edit/Select All")},
            {"win.search_find", _("Search/Find")},
            {"win.search_replace", _("Search/Replace")},
            {"win.move_line", _("Move/Move to Line")},
            {"win.move_top", _("Move/Move to Beginning of File")},
            {"win.move_bottom", _("Move/Move to End of File")},
            {"win.move_left", _("Move/Move to Beginning of Line")},
            {"win.move_right", _("Move/Move to End of Line")},
            {"win.library_call", _("Library/Call")},
            {"win.library_add", _("Library/Add")},
            {"win.library_remove", _("Library/Remove")},
            {"win.macro_setting", _("Macro/Setting")},
            {"win.macro_expand", _("Macro/Expand")},
            {"win.option_font", _("Option/Font")},
            {"win.option_key_config", _("Option/Key Config")},
            {"win.option_tab_width", _("Option/Tab Width")},
            {"win.option_flymake", _("Option/Flymake")},
            {"win.help_version_info", _("Help/Version Info")}
        };
        return acceleratable_actions;
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
        if (key_file.has_group(ACCELERATOR)) key_file.remove_group(ACCELERATOR);
        for (const std::pair<Glib::ustring, std::vector<Glib::ustring>>& accelerator : accelerators) key_file.set_string_list(ACCELERATOR, accelerator.first, accelerator.second);
        save();
    }
    
    std::vector<std::pair<Glib::ustring, Glib::ustring>> setting::get_macros() const {
        std::vector<std::pair<Glib::ustring, Glib::ustring>> macros;
        if (key_file.has_group(MACRO)) {
            for (const Glib::ustring& key : key_file.get_keys(MACRO)) macros.emplace_back(key, key_file.get_string(MACRO, key));
        }
        return macros;
    }
    
    void setting::set_macros(const std::vector<std::pair<Glib::ustring, Glib::ustring>>& macros) {
        if (key_file.has_group(MACRO)) key_file.remove_group(MACRO);
        for (const std::pair<Glib::ustring, Glib::ustring>& macro : macros) key_file.set_string(MACRO, macro.first, macro.second);
        save();
    }
}
