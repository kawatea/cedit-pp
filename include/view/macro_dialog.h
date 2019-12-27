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

#ifndef CEDIT_VIEW_MACRO_DIALOG_H_
#define CEDIT_VIEW_MACRO_DIALOG_H_

#include <set>
#include <glibmm/ustring.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/cellrenderer.h>
#include <gtkmm/cellrenderertext.h>
#include <gtkmm/dialog.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/liststore.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treeselection.h>
#include <gtkmm/treeview.h>
#include <gtkmm/window.h>

namespace view {
    class macro_dialog final : public Gtk::Dialog {
        public:
        macro_dialog(Gtk::Window& window);
        ~macro_dialog() = default;
        
        void update_macros();
        
        private:
        class macro_columns final : public Gtk::TreeModel::ColumnRecord {
            public:
            macro_columns();
            ~macro_columns() = default;
            
            Gtk::TreeModelColumn<Glib::ustring> before_expansion;
            Gtk::TreeModelColumn<Glib::ustring> after_expansion;
        };
        
        Gtk::Box box;
        Gtk::Box before_box;
        Gtk::Box after_box;
        Gtk::ScrolledWindow scrolled_window;
        Gtk::TreeView list;
        Glib::RefPtr<Gtk::TreeSelection> selection;
        Glib::RefPtr<Gtk::ListStore> model;
        macro_columns columns;
        Gtk::TreeView::Column before_column;
        Gtk::TreeView::Column after_column;
        Gtk::CellRendererText before_renderer;
        Gtk::CellRendererText after_renderer;
        Gtk::Label before_label;
        Gtk::Label after_label;
        Gtk::Entry before_entry;
        Gtk::Entry after_entry;
        std::set<Glib::ustring> keys;
        
        void on_entry_change(Gtk::Button* create_button, const bool before);
        void on_selection_change(Gtk::Button* remove_button);
        void on_before_edit(const Glib::ustring& path, const Glib::ustring& text);
        void on_after_edit(const Glib::ustring& path, const Glib::ustring& text);
        void on_before_render(Gtk::CellRenderer* renderer, const Gtk::TreeModel::iterator& iterator);
        void on_after_render(Gtk::CellRenderer* renderer, const Gtk::TreeModel::iterator& iterator);
        
        void append_row(const Glib::ustring& key, const Glib::ustring& value);
        void remove_row();
        void save_macros();
        bool get_response();
    };
}

#endif
