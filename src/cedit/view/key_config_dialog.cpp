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

#include <gtkmm/accelgroup.h>
#include "io/gettext.h"
#include "io/setting.h"
#include "view/key_config_dialog.h"

namespace view {
    key_config_dialog::key_config_columns::key_config_columns() {
        add(action_name);
        add(action_label);
        add(key1);
        add(key2);
    }
    
    key_config_dialog::key_config_dialog(Gtk::Window& window) : Gtk::Dialog(_("Key Config"), window) {
        model = Gtk::ListStore::create(columns);
        list.set_model(model);
        list.set_grid_lines(Gtk::TREE_VIEW_GRID_LINES_BOTH);
        list.set_search_column(-1);
        list.set_enable_search(false);
        
        list.append_column(_("Action"), columns.action_label);
        
        key1_column.set_title(_("Key1"));
        key1_column.pack_start(key1_renderer);
        key1_renderer.property_editable() = true;
        key1_renderer.signal_accel_edited().connect(sigc::mem_fun(*this, &key_config_dialog::on_key1_edit));
        key1_renderer.signal_accel_cleared().connect(sigc::mem_fun(*this, &key_config_dialog::on_key1_clear));
        key1_column.set_cell_data_func(key1_renderer, sigc::mem_fun(*this, &key_config_dialog::on_key1_render));
        list.append_column(key1_column);
        
        key2_column.set_title(_("Key2"));
        key2_column.pack_start(key2_renderer);
        key2_renderer.property_editable() = true;
        key2_renderer.signal_accel_edited().connect(sigc::mem_fun(*this, &key_config_dialog::on_key2_edit));
        key2_renderer.signal_accel_cleared().connect(sigc::mem_fun(*this, &key_config_dialog::on_key2_clear));
        key2_column.set_cell_data_func(key1_renderer, sigc::mem_fun(*this, &key_config_dialog::on_key2_render));
        list.append_column(key2_column);
        
        scrolled_window.add(list);
        scrolled_window.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
        scrolled_window.set_size_request(400, 400);
        
        get_content_area()->pack_start(scrolled_window);
        add_button(_("Save"), Gtk::RESPONSE_YES);
        add_button(_("Cancel"), Gtk::RESPONSE_CANCEL);
    }
    
    bool key_config_dialog::update_key_config() {
        load_accelerators();
        show_all_children();
        present();
        bool updated = false;
        switch (run()) {
            case Gtk::RESPONSE_YES: save_accelerators(); updated = true;
        }
        hide();
        return updated;
    }
    
    void key_config_dialog::on_key1_edit(const Glib::ustring& path, guint accel_key, Gdk::ModifierType accel_mods, guint hardware_keycode) {
        Gtk::TreeModel::Row row = *(model->get_iter(path));
        std::pair<guint, Gdk::ModifierType> old_key = row[columns.key1];
        std::pair<guint, Gdk::ModifierType> new_key = std::make_pair(accel_key, accel_mods);
        if (new_key == old_key || !keys.count(new_key)) {
            keys.erase(old_key);
            keys.insert(new_key);
            row[columns.key1] = new_key;
        }
    }
    
    void key_config_dialog::on_key1_clear(const Glib::ustring& path) {
        Gtk::TreeModel::Row row = *(model->get_iter(path));
        std::pair<guint, Gdk::ModifierType> old_key = row[columns.key1];
        std::pair<guint, Gdk::ModifierType> new_key = std::make_pair(0, Gdk::ModifierType(0));
        keys.erase(old_key);
        row[columns.key1] = new_key;
    }
    
    void key_config_dialog::on_key1_render(Gtk::CellRenderer* renderer, const Gtk::TreeModel::iterator& iterator) {
        std::pair<guint, Gdk::ModifierType> key = (*iterator)[columns.key1];
        key1_renderer.property_text() = Gtk::AccelGroup::get_label(key.first, key.second);
    }
    
    void key_config_dialog::on_key2_edit(const Glib::ustring& path, guint accel_key, Gdk::ModifierType accel_mods, guint hardware_keycode) {
        Gtk::TreeModel::Row row = *(model->get_iter(path));
        std::pair<guint, Gdk::ModifierType> old_key = row[columns.key2];
        std::pair<guint, Gdk::ModifierType> new_key = std::make_pair(accel_key, accel_mods);
        if (new_key == old_key || !keys.count(new_key)) {
            keys.erase(old_key);
            keys.insert(new_key);
            row[columns.key2] = new_key;
        }
    }
    
    void key_config_dialog::on_key2_clear(const Glib::ustring& path) {
        Gtk::TreeModel::Row row = *(model->get_iter(path));
        std::pair<guint, Gdk::ModifierType> old_key = row[columns.key2];
        std::pair<guint, Gdk::ModifierType> new_key = std::make_pair(0, Gdk::ModifierType(0));
        keys.erase(old_key);
        row[columns.key2] = new_key;
    }
    
    void key_config_dialog::on_key2_render(Gtk::CellRenderer* renderer, const Gtk::TreeModel::iterator& iterator) {
        std::pair<guint, Gdk::ModifierType> key = (*iterator)[columns.key2];
        key2_renderer.property_text() = Gtk::AccelGroup::get_label(key.first, key.second);
    }
    
    std::pair<guint, Gdk::ModifierType> parse_accelerator(const Glib::ustring& accelerator) {
        guint accel_key;
        Gdk::ModifierType accel_mods;
        Gtk::AccelGroup::parse(accelerator, accel_key, accel_mods);
        return std::make_pair(accel_key, accel_mods);
    }
    
    void key_config_dialog::load_accelerators() {
        model->clear();
        
        std::map<Glib::ustring, std::vector<Glib::ustring>> accelerators = io::setting::get().get_accelerators();
        for (const std::pair<Glib::ustring, Glib::ustring>& action : io::setting::list_acceleratable_actions()) {
            Gtk::TreeModel::Row row = *(model->append());
            row[columns.action_name] = action.first;
            row[columns.action_label] = action.second;
            std::vector<Glib::ustring>& accels = accelerators[action.first];
            if (accels.size() >= 1) {
                std::pair<guint, Gdk::ModifierType> key = parse_accelerator(accels[0]);
                keys.insert(key);
                row[columns.key1] = key;
            }
            if (accels.size() >= 2) {
                std::pair<guint, Gdk::ModifierType> key = parse_accelerator(accels[1]);
                keys.insert(key);
                row[columns.key2] = key;
            }
        }
    }
    
    void key_config_dialog::save_accelerators() {
        std::map<Glib::ustring, std::vector<Glib::ustring>> accelerators;
        for (const Gtk::TreeModel::Row row : model->children()) {
            std::pair<guint, Gdk::ModifierType> key1 = row[columns.key1];
            std::pair<guint, Gdk::ModifierType> key2 = row[columns.key2];
            std::vector<Glib::ustring> accels;
            if (key1.first != 0) accels.push_back(Gtk::AccelGroup::name(key1.first, key1.second));
            if (key2.first != 0) accels.push_back(Gtk::AccelGroup::name(key2.first, key2.second));
            if (!accels.empty()) accelerators[row[columns.action_name]] = accels;
        }
        io::setting::get().set_accelerators(accelerators);
    }
}
