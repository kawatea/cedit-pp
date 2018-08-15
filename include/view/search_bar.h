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

#ifndef CEDIT_VIEW_SEARCH_BAR_H_
#define CEDIT_VIEW_SEARCH_BAR_H_

#include <functional>
#include <glibmm/ustring.h>
#include <gtkmm/box.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/searchentry.h>
#include <gtkmm/textiter.h>
#include <gtkmm/texttag.h>
#include <gtksourceviewmm/buffer.h>
#include <gtksourceviewmm/view.h>

namespace view {
    class search_bar final : public Gtk::Box {
        public:
        search_bar(const Glib::RefPtr<Gsv::Buffer>& buffer, Gsv::View& view);
        ~search_bar() = default;
        
        void open_find();
        void open_replace();
        void remove_tags();
        
        protected:
        bool on_key_press_event(GdkEventKey* event) override;
        
        private:
        Glib::RefPtr<Gsv::Buffer> buffer;
        Glib::RefPtr<Gtk::TextTag> match_tag;
        Gtk::Box find_box;
        Gtk::Box replace_box;
        Gtk::SearchEntry find_entry;
        Gtk::SearchEntry replace_entry;
        Gtk::CheckButton case_button;
        Gtk::CheckButton control_button;
        Gtk::CheckButton interactive_button;
        std::function<void(Gtk::TextIter& iter)> on_scroll;
        std::function<void(void)> on_close;
        bool on_find;
        bool in_process = false;
        
        bool on_entry_key_press_event(GdkEventKey* event, const bool find);
        void on_find_text_change();
        void on_case_button_click();
        void on_control_button_click();
        void on_interactive_button_click();
        
        void open(const bool on_find);
        void switch_entry(const bool find);
        Glib::ustring convert_control_characters(const Glib::ustring& old_text);
        void search(const bool forward, const bool skip);
        bool find(const Glib::ustring& text, const bool forward, const bool wrap);
        bool replace(const Glib::ustring& old_text, const Glib::ustring& new_text);
        bool replace_interactive(const Glib::ustring& old_text, const Glib::ustring& new_text, const bool skip);
    };
}

#endif
