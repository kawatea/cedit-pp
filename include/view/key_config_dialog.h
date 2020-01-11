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

#ifndef CEDIT_VIEW_KEY_CONFIG_DIALOG_H_
#define CEDIT_VIEW_KEY_CONFIG_DIALOG_H_

#include <set>
#include <glibmm/ustring.h>
#include <gtkmm/cellrenderer.h>
#include <gtkmm/cellrendereraccel.h>
#include <gtkmm/dialog.h>
#include <gtkmm/liststore.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treeview.h>
#include <gtkmm/window.h>

namespace view {
    class key_config_dialog final : public Gtk::Dialog {
        public:
        key_config_dialog(Gtk::Window& window);
        ~key_config_dialog() = default;
        
        bool update_key_config();
        
        private:
        class key_config_columns final : public Gtk::TreeModel::ColumnRecord {
            public:
            key_config_columns();
            ~key_config_columns() = default;
            
            Gtk::TreeModelColumn<Glib::ustring> action_name;
            Gtk::TreeModelColumn<Glib::ustring> action_label;
            Gtk::TreeModelColumn<std::pair<guint, Gdk::ModifierType>> key1;
            Gtk::TreeModelColumn<std::pair<guint, Gdk::ModifierType>> key2;
        };
        
        Gtk::ScrolledWindow scrolled_window;
        Gtk::TreeView list;
        Glib::RefPtr<Gtk::ListStore> model;
        key_config_columns columns;
        Gtk::TreeView::Column key1_column;
        Gtk::TreeView::Column key2_column;
        Gtk::CellRendererAccel key1_renderer;
        Gtk::CellRendererAccel key2_renderer;
        std::set<std::pair<guint, Gdk::ModifierType>> keys;
        
        void on_key1_edit(const Glib::ustring& path, guint accel_key, Gdk::ModifierType accel_mods, guint hardware_keycode);
        void on_key1_clear(const Glib::ustring& path);
        void on_key1_render(Gtk::CellRenderer* renderer, const Gtk::TreeModel::iterator& iterator);
        void on_key2_edit(const Glib::ustring& path, guint accel_key, Gdk::ModifierType accel_mods, guint hardware_keycode);
        void on_key2_clear(const Glib::ustring& path);
        void on_key2_render(Gtk::CellRenderer* renderer, const Gtk::TreeModel::iterator& iterator);
        
        void load_accelerators();
        void save_accelerators();
    };
}

#endif
