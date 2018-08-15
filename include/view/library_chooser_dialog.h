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

#ifndef CEDIT_VIEW_LIBRARY_CHOOSER_DIALOG_H_
#define CEDIT_VIEW_LIBRARY_CHOOSER_DIALOG_H_

#include <set>
#include <string>
#include <glibmm/ustring.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/dialog.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/listviewtext.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treeviewcolumn.h>
#include <gtkmm/window.h>

namespace view {
    class library_chooser_dialog final : public Gtk::Dialog {
        public:
        library_chooser_dialog(Gtk::Window& window, const Glib::ustring& title);
        ~library_chooser_dialog() = default;
        
        std::string choose_library();
        
        private:
        Gtk::Box box;
        Gtk::Box entry_box;
        Gtk::ScrolledWindow scrolled_window;
        Gtk::ListViewText list;
        Gtk::Label label;
        Gtk::Entry entry;
        std::set<std::string> libraries;
        
        void on_entry_change(Gtk::Button* button);
        void on_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
        
        void load_libraries();
        std::string get_response();
    };
}

#endif
