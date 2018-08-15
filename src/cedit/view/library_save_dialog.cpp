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

#include <gtkmm/messagedialog.h>
#include "io/file.h"
#include "io/gettext.h"
#include "io/io_util.h"
#include "view/library_save_dialog.h"

namespace view {
    library_save_dialog::library_save_dialog(Gtk::Window& window) : Gtk::Dialog(_("Library Add"), window), label(_("Name: ")) {
        entry.set_activates_default(true);
        
        box.pack_start(label);
        box.pack_start(entry);
        
        get_content_area()->pack_start(box);
        Gtk::Button* button = add_button("OK", Gtk::RESPONSE_OK);
        set_default(*button);
        button->set_sensitive(false);
        entry.signal_changed().connect(sigc::bind<Gtk::Button*>(sigc::mem_fun(*this, &library_save_dialog::on_entry_change), button));
    }
    
    std::string library_save_dialog::choose_library() {
        entry.set_text("");
        show_all_children();
        present();
        
        std::string library;
        while (true) {
            library = get_response();
            if (library.empty()) break;
            
            const io::file file(library);
            if (!file.exists() || can_continue()) break;
        }
        
        hide();
        
        return library;
    }
    
    void library_save_dialog::on_entry_change(Gtk::Button* button) {
        button->set_sensitive(!entry.get_text().empty());
    }
    
    std::string library_save_dialog::get_response() {
        switch (run()) {
            case Gtk::RESPONSE_OK: return io::get_path("library/" + entry.get_text());
            default: return "";
        }
    }
    
    bool library_save_dialog::can_continue() {
        Gtk::MessageDialog dialog(*get_transient_for(), _("Do you overwrite an existing library?"), false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO);
        return dialog.run() == Gtk::RESPONSE_YES;
    }
}
