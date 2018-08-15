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

#ifndef CEDIT_VIEW_WINDOW_H_
#define CEDIT_VIEW_WINDOW_H_

#include <memory>
#include <giomm/simpleaction.h>
#include <glibmm/ustring.h>
#include <gtkmm/applicationwindow.h>
#include <gtkmm/box.h>
#include <gtkmm/menu.h>
#include <gtkmm/notebook.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/tooltip.h>
#include <gtksourceviewmm/buffer.h>
#include <gtksourceviewmm/languagemanager.h>
#include <gtksourceviewmm/styleschememanager.h>
#include <gtksourceviewmm/view.h>
#include <sigc++/connection.h>
#include "function/flymake.h"
#include "function/highlight_brackets.h"
#include "function/kill_line.h"
#include "io/file.h"
#include "view/search_bar.h"

namespace view {
    class window final : public Gtk::ApplicationWindow {
        public:
        window(std::unique_ptr<io::file>&& file);
        ~window() = default;
        
        protected:
        void on_show() override;
        bool on_delete_event(GdkEventAny* event) override;
        bool on_key_press_release_event(GdkEventKey* event);
        bool on_button_press(GdkEventButton* event);
        bool on_query_tooltip(int x, int y, bool keyboard_tooltip, const Glib::RefPtr<Gtk::Tooltip>& tooltip);
        void on_cursor_change();
        void on_text_change();
        void on_selection_change();
        void on_undo_change();
        void on_redo_change();
        
        void on_file_new();
        void on_file_open();
        void on_file_reload();
        void on_file_save();
        void on_file_save_as();
        void on_file_print();
        void on_file_quit();
        
        void on_edit_undo();
        void on_edit_redo();
        void on_edit_cut();
        void on_edit_copy();
        void on_edit_paste();
        void on_edit_delete();
        void on_edit_kill_line();
        void on_edit_select_all();
        
        void on_search_find();
        void on_search_replace();
        
        void on_move_line();
        void on_move_top();
        void on_move_bottom();
        void on_move_left();
        void on_move_right();
        
        void on_library_call();
        void on_library_add();
        void on_library_remove();
        
        void on_option_encoding(const Glib::ustring& encoding);
        void on_option_font();
        void on_option_show_toolbar();
        void on_option_tab_width();
        void on_option_flymake();
        void on_option_wrap_text();
        void on_option_line_numbers();
        void on_option_auto_indent();
        void on_option_highlight_line();
        void on_option_insert_spaces();
        void on_option_hungry_delete();
        
        void on_help_version_info();
        
        private:
        Gtk::Box box;
        Gtk::Notebook notebook;
        Gtk::Box editor_box;
        Gtk::ScrolledWindow scrolled_window;
        Glib::RefPtr<Gsv::Buffer> buffer;
        Gsv::View view;
        Glib::RefPtr<Gsv::LanguageManager> language_manager;
        Glib::RefPtr<Gsv::StyleSchemeManager> style_scheme_manager;
        Gtk::Menu popup;
        std::unique_ptr<io::file> file;
        Glib::ustring language;
        sigc::connection connection_text_change;
        Glib::RefPtr<Gio::SimpleAction> action_encoding;
        Glib::RefPtr<Gio::SimpleAction> action_show_toolbar;
        Glib::RefPtr<Gio::SimpleAction> action_wrap_text;
        Glib::RefPtr<Gio::SimpleAction> action_line_numbers;
        Glib::RefPtr<Gio::SimpleAction> action_auto_indent;
        Glib::RefPtr<Gio::SimpleAction> action_highlight_line;
        Glib::RefPtr<Gio::SimpleAction> action_insert_spaces;
        Glib::RefPtr<Gio::SimpleAction> action_hungry_delete;
        view::search_bar search_bar;
        function::flymake flymake;
        function::highlight_brackets highlight_brackets;
        function::kill_line kill_line;
        
        void load_menubar();
        void load_toolbar();
        void load_popup();
        void apply_general_settings();
        void apply_language_settings();
        void add_actions();
        void change_file();
        void load_file();
        
        bool can_continue();
        void update_edited(const bool edited);
        bool update_state(const Glib::RefPtr<Gio::SimpleAction>& action);
    };
}

#endif
