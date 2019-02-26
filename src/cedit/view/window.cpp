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

#include <cstdio>
#include <utility>
#include <vector>
#include <gdkmm/pixbuf.h>
#include <giomm/menu.h>
#include <glibmm/convert.h>
#include <glibmm/ustring.h>
#include <gtkmm/application.h>
#include <gtkmm/builder.h>
#include <gtkmm/menubar.h>
#include <gtkmm/textiter.h>
#include <gtkmm/toolbar.h>
#include <gtksourceviewmm/language.h>
#include <pangomm/fontdescription.h>
#include "function/hungry_delete.h"
#include "function/indent.h"
#include "io/encoding.h"
#include "io/file.h"
#include "io/gettext.h"
#include "io/io_util.h"
#include "io/setting.h"
#include "view/about_dialog.h"
#include "view/file_chooser_dialog.h"
#include "view/flymake_dialog.h"
#include "view/font_chooser_dialog.h"
#include "view/library_chooser_dialog.h"
#include "view/library_save_dialog.h"
#include "view/number_chooser_dialog.h"
#include "view/print_dialog.h"
#include "view/save_check_dialog.h"
#include "view/window.h"

namespace view {
    window::window(std::unique_ptr<io::file>&& file) : box(Gtk::ORIENTATION_VERTICAL), editor_box(Gtk::ORIENTATION_VERTICAL), buffer(Gsv::Buffer::create()), view(buffer), language_manager(Gsv::LanguageManager::create()), style_scheme_manager(Gsv::StyleSchemeManager::create()), file(std::move(file)), search_bar(buffer, view), highlight_brackets(buffer) {
        language_manager->set_search_path({io::get_path("src/resource/language")});
        style_scheme_manager->set_search_path({io::get_path("src/resource/style")});
        
        buffer->set_highlight_matching_brackets(false);
        buffer->set_highlight_syntax(true);
        buffer->set_max_undo_levels(-1);
        buffer->set_style_scheme(style_scheme_manager->get_scheme("mystyle"));
        
        view.set_can_focus(true);
        view.set_has_tooltip(true);
        
        scrolled_window.add(view);
        scrolled_window.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_ALWAYS);
        
        editor_box.pack_start(search_bar, Gtk::PACK_SHRINK);
        editor_box.pack_start(scrolled_window);
        
        notebook.append_page(editor_box);
        
        load_menubar();
        load_toolbar();
        load_popup();
        box.pack_start(notebook);
        
        add(box);
        set_title("cedit++");
        set_icon(Gdk::Pixbuf::create_from_resource("/resource/icon.png"));
        apply_general_settings();
        add_actions();
        
        change_file();
        load_file();
        
        show_all_children();
        view.grab_focus();
        
        if (!io::setting::get().get_boolean(io::setting::SHOW_TOOLBAR)) on_option_show_toolbar();
        search_bar.hide();
    }
    
    void window::load_menubar() {
        const Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_resource("/resource/menubar.xml");
        const Glib::RefPtr<Gio::Menu> menubar = Glib::RefPtr<Gio::Menu>::cast_dynamic(builder->get_object("menubar"));
        box.pack_start(*Gtk::manage(new Gtk::MenuBar(menubar)), Gtk::PACK_SHRINK);
    }
    
    void window::load_toolbar() {
        const Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_resource("/resource/toolbar.xml");
        Gtk::Toolbar* toolbar = nullptr;
        builder->get_widget("toolbar", toolbar);
        box.pack_start(*toolbar, Gtk::PACK_SHRINK);
    }
    
    void window::load_popup() {
        const Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_resource("/resource/popup.xml");
        popup = Gtk::Menu(Glib::RefPtr<Gio::Menu>::cast_dynamic(builder->get_object("popup")));
        popup.attach_to_widget(*this);
    }
    
    void window::apply_general_settings() {
        const io::setting& setting = io::setting::get();
        
        const int border_width = setting.get_integer(io::setting::BORDER_WIDTH);
        if (border_width > 0) set_border_width(border_width);
        
        const int width = setting.get_integer(io::setting::WIDTH);
        const int height = setting.get_integer(io::setting::HEIGHT);
        if (width > 0 && height > 0) set_default_size(width, height);
        
        const int x = setting.get_integer(io::setting::POSITION_X);
        const int y = setting.get_integer(io::setting::POSITION_Y);
        if (x > 0 && y > 0) move(x, y);
        
        const Glib::ustring font = setting.get_string(io::setting::FONT);
        if (font != "") view.override_font(Pango::FontDescription(font));
    }
    
    void window::apply_language_settings() {
        view.set_show_line_numbers(false);
        view.set_auto_indent(false);
        view.set_insert_spaces_instead_of_tabs(false);
        view.set_highlight_current_line(false);
        view.set_indent_on_tab(false);
        
        action_wrap_text = add_action_bool("option_wrap_text", sigc::mem_fun(*this, &window::on_option_wrap_text), false);
        action_line_numbers = add_action_bool("option_line_numbers", sigc::mem_fun(*this, &window::on_option_line_numbers), false);
        action_auto_indent = add_action_bool("option_auto_indent", sigc::mem_fun(*this, &window::on_option_auto_indent), false);
        action_highlight_line = add_action_bool("option_highlight_line", sigc::mem_fun(*this, &window::on_option_highlight_line), false);
        action_insert_spaces = add_action_bool("option_insert_spaces", sigc::mem_fun(*this, &window::on_option_insert_spaces), false);
        action_hungry_delete = add_action_bool("option_hungry_delete", sigc::mem_fun(*this, &window::on_option_hungry_delete), false);
        
        const io::setting& setting = io::setting::get();
        
        const int tab_width = setting.get_integer(language, io::setting::TAB_WIDTH);
        if (tab_width > 0) view.set_tab_width(tab_width);
        
        const bool wrap_text = setting.get_boolean(language, io::setting::WRAP_TEXT);
        if (wrap_text) on_option_wrap_text();
        
        const bool line_numbers = setting.get_boolean(language, io::setting::LINE_NUMBERS);
        if (line_numbers) on_option_line_numbers();
        
        const bool auto_indent = setting.get_boolean(language, io::setting::AUTO_INDENT);
        if (auto_indent) on_option_auto_indent();
        
        const bool highlight_line = setting.get_boolean(language, io::setting::HIGHLIGHT_LINE);
        if (highlight_line) on_option_highlight_line();
        
        const bool insert_spaces = setting.get_boolean(language, io::setting::INSERT_SPACES);
        if (insert_spaces) on_option_insert_spaces();
        
        const bool hungry_delete = setting.get_boolean(language, io::setting::HUNGRY_DELETE);
        if (hungry_delete) on_option_hungry_delete();
    }
    
    void window::add_actions() {
        view.signal_key_press_event().connect(sigc::mem_fun(*this, &window::on_key_press_release_event), false);
        view.signal_key_release_event().connect(sigc::mem_fun(*this, &window::on_key_press_release_event), false);
        view.signal_button_press_event().connect(sigc::mem_fun(*this, &window::on_button_press), false);
        view.signal_query_tooltip().connect(sigc::mem_fun(*this, &window::on_query_tooltip));
        buffer->property_cursor_position().signal_changed().connect(sigc::mem_fun(*this, &window::on_cursor_change));
        connection_text_change = buffer->signal_changed().connect(sigc::mem_fun(*this, &window::on_text_change));
        buffer->property_has_selection().signal_changed().connect(sigc::mem_fun(*this, &window::on_selection_change));
        buffer->property_can_undo().signal_changed().connect(sigc::mem_fun(*this, &window::on_undo_change));
        buffer->property_can_redo().signal_changed().connect(sigc::mem_fun(*this, &window::on_redo_change));
        
        add_action("file_open", sigc::mem_fun(*this, &window::on_file_open));
        add_action("file_save_as", sigc::mem_fun(*this, &window::on_file_save_as));
        add_action("file_print", sigc::mem_fun(*this, &window::on_file_print));
        add_action("file_quit", sigc::mem_fun(*this, &window::on_file_quit));
        
        add_action("edit_paste", sigc::mem_fun(*this, &window::on_edit_paste));
        add_action("edit_kill_line", sigc::mem_fun(*this, &window::on_edit_kill_line));
        add_action("edit_select_all", sigc::mem_fun(*this, &window::on_edit_select_all));
        
        add_action("search_find", sigc::mem_fun(*this, &window::on_search_find));
        add_action("search_replace", sigc::mem_fun(*this, &window::on_search_replace));
        
        add_action("move_line", sigc::mem_fun(*this, &window::on_move_line));
        add_action("move_top", sigc::mem_fun(*this, &window::on_move_top));
        add_action("move_bottom", sigc::mem_fun(*this, &window::on_move_bottom));
        add_action("move_left", sigc::mem_fun(*this, &window::on_move_left));
        add_action("move_right", sigc::mem_fun(*this, &window::on_move_right));
        
        add_action("library_call", sigc::mem_fun(*this, &window::on_library_call));
        add_action("library_add", sigc::mem_fun(*this, &window::on_library_add));
        add_action("library_remove", sigc::mem_fun(*this, &window::on_library_remove));
        
        action_encoding = add_action_radio_string("option_encoding", sigc::mem_fun(*this, &window::on_option_encoding), "UTF-8");
        add_action("option_font", sigc::mem_fun(*this, &window::on_option_font));
        action_show_toolbar = add_action_bool("option_show_toolbar", sigc::mem_fun(*this, &window::on_option_show_toolbar), true);
        add_action("option_tab_width", sigc::mem_fun(*this, &window::on_option_tab_width));
        add_action("option_flymake", sigc::mem_fun(*this, &window::on_option_flymake));
        
        add_action("help_version_info", sigc::mem_fun(*this, &window::on_help_version_info));
    }
    
    void window::change_file() {
        Glib::RefPtr<Gsv::Language> language;
        if (!file->is_empty()) language = language_manager->guess_language(file->get_simple_name(), "");
        buffer->set_language(language);
        this->language = language ? language->get_name() : "";
        apply_language_settings();
        
        if (file->is_empty()) {
            remove_action("file_new");
            remove_action("file_reload");
        } else {
            add_action("file_new", sigc::mem_fun(*this, &window::on_file_new));
            add_action("file_reload", sigc::mem_fun(*this, &window::on_file_reload));
        }
    }
    
    void window::load_file() {
        buffer->begin_not_undoable_action();
        
        if (!file->exists()) {
            if (file->get_encoding() == io::encoding::NONE) file->set_encoding(io::encoding::UTF8);
            buffer->set_text("");
        } else {
            if (file->get_encoding() == io::encoding::NONE) file->set_encoding(io::detect_encoding(Glib::locale_from_utf8(file->get_full_name())));
            buffer->set_text(file->read());
        }
        action_encoding->change_state(Glib::ustring(file->get_encoding().get_name()));
        
        update_edited(false);
        on_move_top();
        
        buffer->end_not_undoable_action();
    }
    
    void window::on_show() {
        Gtk::ApplicationWindow::on_show();
        const Glib::RefPtr<Gtk::Application> application = get_application();
        for (const std::pair<Glib::ustring, std::vector<Glib::ustring>>& accelerator : io::setting::get().get_accelerators()) application->set_accels_for_action(accelerator.first, accelerator.second);
    }
    
    bool window::on_delete_event(GdkEventAny* event) {
        if (can_continue()) {
            io::setting& setting = io::setting::get();
            
            int width, height;
            get_size(width, height);
            setting.set_integer(io::setting::WIDTH, width);
            setting.set_integer(io::setting::HEIGHT, height);
            
            int x, y;
            get_position(x, y);
            setting.set_integer(io::setting::POSITION_X, x);
            setting.set_integer(io::setting::POSITION_Y, y);
            
            setting.save();
            
            return false;
        } else {
            return true;
        }
    }
    
    bool window::on_key_press_release_event(GdkEventKey* event) {
        static bool brace_pressed = false;
        const io::setting& setting = io::setting::get();
        if (event->type == GDK_KEY_PRESS) {
            switch (event->keyval) {
                case GDK_KEY_BackSpace: return (setting.get_boolean(language, io::setting::HUNGRY_DELETE) && !buffer->get_has_selection()) ? function::hungry_delete_backward(buffer) : false;
                case GDK_KEY_Delete: return (setting.get_boolean(language, io::setting::HUNGRY_DELETE) && !buffer->get_has_selection()) ? function::hungry_delete_forward(buffer) : false;
                case GDK_KEY_Tab: return setting.get_boolean(language, io::setting::AUTO_INDENT) ? (function::indent(buffer, setting.get_boolean(language, io::setting::INSERT_SPACES), view.get_tab_width()), true) : false;
                case GDK_KEY_braceright: brace_pressed = true;
                default: return false;
            }
        } else {
            switch (event->keyval) {
                case GDK_KEY_bracketright: if (!brace_pressed) return false;
                case GDK_KEY_braceright: brace_pressed = false;
                case GDK_KEY_Return: if (setting.get_boolean(language, io::setting::AUTO_INDENT)) function::indent(buffer, setting.get_boolean(language, io::setting::INSERT_SPACES), view.get_tab_width());
                default: return false;
            }
        }
    }
    
    bool window::on_button_press(GdkEventButton* event) {
        if (event->type == GDK_BUTTON_PRESS && event->button == 3) {
            popup.popup(event->button, event->time);
            return true;
        } else {
            return false;
        }
    }
    
    bool window::on_query_tooltip(int x, int y, bool keyboard_tooltip, const Glib::RefPtr<Gtk::Tooltip>& tooltip) {
        if (keyboard_tooltip) return false;
        
        int mouse_x, mouse_y, trailing;
        Gtk::TextIter iter;
        view.window_to_buffer_coords(Gtk::TEXT_WINDOW_TEXT, x, y, mouse_x, mouse_y);
        view.get_iter_at_position(iter, trailing, mouse_x, mouse_y);
        return flymake.show_tooltip(iter, tooltip);
    }
    
    void window::on_cursor_change() {
        search_bar.remove_tags();
        highlight_brackets.apply(buffer);
        kill_line.clear();
    }
    
    void window::on_text_change() {
        update_edited(true);
    }
    
    void window::on_selection_change() {
        if (buffer->get_has_selection()) {
            add_action("edit_cut", sigc::mem_fun(*this, &window::on_edit_cut));
            add_action("edit_copy", sigc::mem_fun(*this, &window::on_edit_copy));
            add_action("edit_delete", sigc::mem_fun(*this, &window::on_edit_delete));
        } else {
            remove_action("edit_cut");
            remove_action("edit_coyp");
            remove_action("edit_delete");
        }
    }
    
    void window::on_undo_change() {
        if (buffer->can_undo()) {
            add_action("edit_undo", sigc::mem_fun(*this, &window::on_edit_undo));
        } else {
            remove_action("edit_undo");
        }
    }
    
    void window::on_redo_change() {
        if (buffer->can_redo()) {
            add_action("edit_redo", sigc::mem_fun(*this, &window::on_edit_redo));
        } else {
            remove_action("edit_redo");
        }
    }
    
    void window::on_file_new() {
        if (can_continue()) {
            file.reset(new io::file());
            change_file();
            load_file();
        }
    }
    
    void window::on_file_open() {
        if (!can_continue()) return;
        file_chooser_dialog dialog(*this, _("Open"), _("Open"), false);
        const std::string filename = dialog.choose_file();
        if (!filename.empty()) {
            file.reset(new io::file(filename));
            change_file();
            load_file();
        }
    }
    
    void window::on_file_reload() {
        if (can_continue()) {
            io::setting::get().load();
            apply_language_settings();
            load_file();
        }
    }
    
    void window::on_file_save() {
        if (file->is_empty()) {
            on_file_save_as();
        } else {
            file->write(buffer->get_text());
            update_edited(false);
            flymake.apply(buffer, Glib::locale_from_utf8(file->get_full_name()), language);
        }
    }
    
    void window::on_file_save_as() {
        file_chooser_dialog dialog(*this, _("Save As"), _("Save"), true);
        const std::string filename = dialog.choose_file();
        if (!filename.empty()) {
            file.reset(new io::file(filename, file->get_encoding()));
            change_file();
            on_file_save();
        }
    }
    
    void window::on_file_print() {
        print_dialog dialog;
        dialog.print(*this, view);
    }
    
    void window::on_file_quit() {
        if (can_continue()) hide();
    }
    
    void window::on_edit_undo() {
        buffer->undo();
    }
    
    void window::on_edit_redo() {
        buffer->redo();
    }
    
    void window::on_edit_cut() {
        buffer->cut_clipboard(Gtk::Clipboard::get());
    }
    
    void window::on_edit_copy() {
        buffer->copy_clipboard(Gtk::Clipboard::get());
    }
    
    void window::on_edit_paste() {
        buffer->paste_clipboard(Gtk::Clipboard::get());
    }
    
    void window::on_edit_delete() {
        buffer->erase_selection();
    }
    
    void window::on_edit_kill_line() {
        kill_line.apply(buffer);
    }
    
    void window::on_edit_select_all() {
        buffer->select_range(buffer->begin(), buffer->end());
    }
    
    void window::on_search_find() {
        search_bar.open_find();
    }
    
    void window::on_search_replace() {
        search_bar.open_replace();
    }
    
    void window::on_move_line() {
        static number_chooser_dialog dialog(*this, _("Move to Line"), _("Line Number: "));
        const int line_number = dialog.choose_number(1, buffer->get_line_count()) - 1;
        if (line_number >= 0) {
            Gtk::TextIter iterator = buffer->get_iter_at_line(line_number);
            buffer->place_cursor(iterator);
            view.scroll_to(iterator);
        }
    }
    
    void window::on_move_top() {
        buffer->place_cursor(buffer->begin());
        const Glib::RefPtr<Gtk::Adjustment> adjustment = view.get_vadjustment();
        adjustment->set_value(adjustment->get_lower());
    }
    
    void window::on_move_bottom() {
        buffer->place_cursor(buffer->end());
        const Glib::RefPtr<Gtk::Adjustment> adjustment = view.get_vadjustment();
        adjustment->set_value(adjustment->get_upper());
    }
    
    void window::on_move_left() {
        Gtk::TextIter iterator = buffer->get_iter_at_mark(buffer->get_insert());
        iterator.backward_chars(iterator.get_line_offset());
        buffer->place_cursor(iterator);
        view.scroll_to(iterator);
    }
    
    void window::on_move_right() {
        Gtk::TextIter iterator = buffer->get_iter_at_mark(buffer->get_insert());
        if (!iterator.ends_line()) iterator.forward_to_line_end();
        buffer->place_cursor(iterator);
        view.scroll_to(iterator);
    }
    
    void window::on_library_call() {
        static library_chooser_dialog dialog(*this, _("Library Call"));
        const std::string library = dialog.choose_library();
        if (!library.empty()) {
            const io::file file(library, io::encoding::UTF8);
            buffer->insert_at_cursor(file.read());
        }
    }
    
    void window::on_library_add() {
        static library_save_dialog dialog(*this);
        const std::string library = dialog.choose_library();
        if (!library.empty()) {
            const io::file file(library, io::encoding::UTF8);
            file.write(buffer->get_text());
        }
    }
    
    void window::on_library_remove() {
        static library_chooser_dialog dialog(*this, _("Library Remove"));
        const std::string library = dialog.choose_library();
        if (!library.empty()) {
            std::remove(library.c_str());
        }
    }
    
    void window::on_option_encoding(const Glib::ustring& encoding) {
        if (encoding == file->get_encoding().get_name()) return;
        action_encoding->change_state(encoding);
        file->set_encoding(io::encoding::from_name(encoding));
        if (!file->is_edited()) update_edited(true);
    }
    
    void window::on_option_font() {
        font_chooser_dialog dialog(*this, io::setting::get().get_string(io::setting::FONT));
        const Glib::ustring font = dialog.choose_font();
        if (font != "") {
            io::setting::get().set_string(io::setting::FONT, font);
            view.override_font(Pango::FontDescription(font));
        }
    }
    
    void window::on_option_show_toolbar() {
        const bool active = update_state(action_show_toolbar);
        io::setting::get().set_boolean(io::setting::SHOW_TOOLBAR, active);
        Gtk::Widget* toolbar = box.get_children()[1];
        active ? toolbar->show() : toolbar->hide();
    }
    
    void window::on_option_tab_width() {
        static number_chooser_dialog dialog(*this, _("Tab Width"), _("New Tab Width: "));
        const int tab_width = dialog.choose_number(view.get_tab_width(), 31);
        
        if (tab_width > 0 && tab_width != view.get_tab_width()) {
            io::setting::get().set_integer(language, io::setting::TAB_WIDTH, tab_width);
            view.set_tab_width(tab_width);
        }
    }
    
    void window::on_option_flymake() {
        static flymake_dialog dialog(*this);
        dialog.update_command(language);
    }
    
    void window::on_option_wrap_text() {
        const bool active = update_state(action_wrap_text);
        io::setting::get().set_boolean(language, io::setting::WRAP_TEXT, active);
        view.set_wrap_mode(active ? Gtk::WrapMode::WRAP_CHAR : Gtk::WrapMode::WRAP_NONE);
    }
    
    void window::on_option_line_numbers() {
        const bool active = update_state(action_line_numbers);
        io::setting::get().set_boolean(language, io::setting::LINE_NUMBERS, active);
        view.set_show_line_numbers(active);
    }
    
    void window::on_option_auto_indent() {
        const bool active = update_state(action_auto_indent);
        io::setting::get().set_boolean(language, io::setting::AUTO_INDENT, active);
    }
    
    void window::on_option_highlight_line() {
        const bool active = update_state(action_highlight_line);
        io::setting::get().set_boolean(language, io::setting::HIGHLIGHT_LINE, active);
        view.set_highlight_current_line(active);
    }
    
    void window::on_option_insert_spaces() {
        const bool active = update_state(action_insert_spaces);
        io::setting::get().set_boolean(language, io::setting::INSERT_SPACES, active);
        view.set_insert_spaces_instead_of_tabs(active);
    }
    
    void window::on_option_hungry_delete() {
        const bool active = update_state(action_hungry_delete);
        io::setting::get().set_boolean(language, io::setting::HUNGRY_DELETE, active);
    }
    
    void window::on_help_version_info() {
        about_dialog dialog(*this);
        dialog.run();
    }
    
    bool window::can_continue() {
        if (!file->is_edited()) return true;
        static save_check_dialog dialog(*this);
        switch (dialog.check_save()) {
            case Gtk::RESPONSE_YES: on_file_save(); return !file->is_edited();
            case Gtk::RESPONSE_NO: return true;
            default: return false;
        }
    }
    
    void window::update_edited(const bool edited) {
        file->set_edited(edited);
        notebook.set_tab_label_text(editor_box, file->get_title_name());
        connection_text_change.block(edited);
        if (edited) {
            add_action("file_save", sigc::mem_fun(*this, &window::on_file_save));
        } else {
            remove_action("file_save");
        }
    }
    
    bool window::update_state(const Glib::RefPtr<Gio::SimpleAction>& action) {
        bool active;
        action->get_state(active);
        active = !active;
        action->change_state(active);
        return active;
    }
}
