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

#include <dirent.h>
#include <sys/types.h>
#include <gtkmm/entrycompletion.h>
#include "io/gettext.h"
#include "io/io_util.h"
#include "view/library_chooser_dialog.h"

namespace view {
    library_chooser_dialog::library_chooser_dialog(Gtk::Window& window, const Glib::ustring& title) : Gtk::Dialog(title, window), box(Gtk::ORIENTATION_VERTICAL), list(1), label(_("Name: ")) {
        const Glib::RefPtr<Gtk::EntryCompletion> completion = Gtk::EntryCompletion::create();
        completion->set_model(list.get_model());
        completion->set_text_column(0);
        
        entry.set_activates_default(true);
        entry.set_completion(completion);
        
        list.set_column_title(0, _("Name"));
        list.set_activate_on_single_click(true);
        
        scrolled_window.add(list);
        scrolled_window.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
        scrolled_window.set_size_request(-1, 300);
        
        entry_box.pack_start(label);
        entry_box.pack_start(entry);
        
        box.pack_start(scrolled_window);
        box.pack_start(entry_box);
        
        get_content_area()->pack_start(box);
        Gtk::Button* button = add_button("OK", Gtk::RESPONSE_OK);
        set_default(*button);
        button->set_sensitive(false);
        entry.signal_changed().connect(sigc::bind<Gtk::Button*>(sigc::mem_fun(*this, &library_chooser_dialog::on_entry_change), button));
        list.signal_row_activated().connect(sigc::mem_fun(*this, &library_chooser_dialog::on_row_activated));
    }
    
    std::string library_chooser_dialog::choose_library() {
        list.clear_items();
        load_libraries();
        for (const std::string& library : libraries) list.append(library);
        entry.set_text("");
        
        show_all_children();
        present();
        entry.grab_focus();
        const std::string library = get_response();
        hide();
        
        return library;
    }
    
    void library_chooser_dialog::on_entry_change(Gtk::Button* button) {
        button->set_sensitive(libraries.count(entry.get_text()));
    }
    
    void library_chooser_dialog::on_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column) {
        entry.set_text(list.get_text(list.get_selected().front()));
        entry.grab_focus_without_selecting();
    }
    
    void library_chooser_dialog::load_libraries() {
        libraries.clear();
        DIR* directory = opendir(io::get_path("library").c_str());
        if (directory != NULL) {
            struct dirent* file;
            while ((file = readdir(directory)) != NULL) {
                if (file->d_name[0] != '.') libraries.emplace(file->d_name);
            }
            closedir(directory);
        }
    }
    
    std::string library_chooser_dialog::get_response() {
        switch (run()) {
            case Gtk::RESPONSE_OK: return io::get_path("library/" + entry.get_text());
            default: return "";
        }
    }
}
