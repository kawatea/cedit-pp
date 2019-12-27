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

#include <gtkmm/cssprovider.h>
#include <gtkmm/stylecontext.h>
#include <gtkmm/treepath.h>
#include "io/gettext.h"
#include "io/setting.h"
#include "view/macro_dialog.h"

namespace view {
    macro_dialog::macro_columns::macro_columns() {
        add(before_expansion);
        add(after_expansion);
    }
    
    macro_dialog::macro_dialog(Gtk::Window& window) : Gtk::Dialog(_("Macro"), window), box(Gtk::ORIENTATION_VERTICAL), before_label(_("Before expansion: ")), after_label(_("After expansion: ")) {
        model = Gtk::ListStore::create(columns);
        list.set_model(model);
        list.set_grid_lines(Gtk::TREE_VIEW_GRID_LINES_BOTH);
        list.set_activate_on_single_click(true);
        list.set_reorderable(true);
        selection = list.get_selection();
        
        before_column.set_title(_("Before expansion"));
        before_column.pack_start(before_renderer);
        before_renderer.property_editable() = true;
        before_renderer.signal_edited().connect(sigc::mem_fun(*this, &macro_dialog::on_before_edit));
        before_column.set_cell_data_func(before_renderer, sigc::mem_fun(*this, &macro_dialog::on_before_render));
        list.append_column(before_column);
        
        after_column.set_title(_("After expansion"));
        after_column.pack_start(after_renderer);
        after_renderer.property_editable() = true;
        after_renderer.signal_edited().connect(sigc::mem_fun(*this, &macro_dialog::on_after_edit));
        after_column.set_cell_data_func(after_renderer, sigc::mem_fun(*this, &macro_dialog::on_after_render));
        list.append_column(after_column);
        
        scrolled_window.add(list);
        scrolled_window.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
        scrolled_window.set_size_request(400, 400);
        
        const Glib::RefPtr<Gtk::CssProvider> provider = Gtk::CssProvider::create();
        provider->load_from_data(".duplicate { background-color: #FFB6C1; }");
        before_entry.get_style_context()->add_provider(provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        
        before_box.pack_start(before_label);
        before_box.pack_start(before_entry);
        
        after_box.pack_start(after_label);
        after_box.pack_start(after_entry);
        
        box.pack_start(scrolled_window);
        box.pack_start(before_box);
        box.pack_start(after_box);
        
        get_content_area()->pack_start(box);
        Gtk::Button* create_button = add_button(_("Create"), Gtk::RESPONSE_ACCEPT);
        Gtk::Button* remove_button = add_button(_("Remove"), Gtk::RESPONSE_REJECT);
        add_button(_("Save"), Gtk::RESPONSE_YES);
        add_button(_("Cancel"), Gtk::RESPONSE_CANCEL);
        create_button->set_sensitive(false);
        remove_button->set_sensitive(false);
        before_entry.signal_changed().connect(sigc::bind<Gtk::Button*, bool>(sigc::mem_fun(*this, &macro_dialog::on_entry_change), create_button, true));
        after_entry.signal_changed().connect(sigc::bind<Gtk::Button*, bool>(sigc::mem_fun(*this, &macro_dialog::on_entry_change), create_button, false));
        selection->signal_changed().connect(sigc::bind<Gtk::Button*>(sigc::mem_fun(*this, &macro_dialog::on_selection_change), remove_button));
    }
    
    void macro_dialog::update_macros() {
        keys.clear();
        model->clear();
        before_entry.set_text("");
        after_entry.set_text("");
        for (const std::pair<Glib::ustring, Glib::ustring>& macro : io::setting::get().get_macros()) {
            append_row(macro.first, macro.second);
        }
        
        show_all_children();
        present();
        while (get_response()) ;
        hide();
    }
    
    void macro_dialog::on_entry_change(Gtk::Button* create_button, const bool before) {
        if (before) before_entry.get_style_context()->remove_class("duplicate");
        create_button->set_sensitive(before_entry.get_text_length() > 0 && after_entry.get_text_length() > 0);
    }
    
    void macro_dialog::on_selection_change(Gtk::Button* remove_button) {
        remove_button->set_sensitive(selection->count_selected_rows() > 0);
    }
    
    void macro_dialog::on_before_edit(const Glib::ustring& path, const Glib::ustring& text) {
        Gtk::TreeModel::Row row = *(model->get_iter(path));
        const Glib::ustring& old_text = row[columns.before_expansion];
        if (!text.empty() && (text == old_text || !keys.count(text))) {
            keys.erase(old_text);
            keys.insert(text);
            row[columns.before_expansion] = text;
        }
    }
    
    void macro_dialog::on_before_render(Gtk::CellRenderer* renderer, const Gtk::TreeModel::iterator& iterator) {
        before_renderer.property_text() = (*iterator)[columns.before_expansion];
    }
    
    void macro_dialog::on_after_edit(const Glib::ustring& path, const Glib::ustring& text) {
        if (!text.empty()) (*(model->get_iter(path)))[columns.after_expansion] = text;
    }
    
    void macro_dialog::on_after_render(Gtk::CellRenderer* renderer, const Gtk::TreeModel::iterator& iterator) {
        after_renderer.property_text() = (*iterator)[columns.after_expansion];
    }
    
    void macro_dialog::append_row(const Glib::ustring& key, const Glib::ustring& value) {
        if (keys.count(key)) {
            before_entry.get_style_context()->add_class("duplicate");
        } else {
            keys.insert(key);
            Gtk::TreeModel::Row row = *(model->append());
            row[columns.before_expansion] = key;
            row[columns.after_expansion] = value;
            before_entry.set_text("");
            after_entry.set_text("");
        }
    }
    
    void macro_dialog::remove_row() {
        const Gtk::TreeModel::iterator& iterator = selection->get_selected();
        keys.erase((*iterator)[columns.before_expansion]);
        model->erase(iterator);
    }
    
    void macro_dialog::save_macros() {
        std::vector<std::pair<Glib::ustring, Glib::ustring>> macros;
        for (const Gtk::TreeModel::Row row : model->children()) {
            macros.emplace_back(row[columns.before_expansion], row[columns.after_expansion]);
        }
        io::setting::get().set_macros(macros);
    }
    
    bool macro_dialog::get_response() {
        switch (run()) {
            case Gtk::RESPONSE_ACCEPT: append_row(before_entry.get_text(), after_entry.get_text()); return true;
            case Gtk::RESPONSE_REJECT: remove_row(); return true;
            case Gtk::RESPONSE_YES: save_macros(); return false;
            default: return false;
        }
    }
}
