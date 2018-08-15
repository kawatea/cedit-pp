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

#include <gtkmm/adjustment.h>
#include "io/gettext.h"
#include "view/number_chooser_dialog.h"

namespace view {
    number_chooser_dialog::number_chooser_dialog(Gtk::Window& window, const Glib::ustring& title, const Glib::ustring& label_text) : Gtk::Dialog(title, window), label(label_text) {
        spin_button.set_activates_default(true);
        
        box.pack_start(label);
        box.pack_start(spin_button);
        
        get_content_area()->pack_start(box);
        set_default(*add_button("OK", Gtk::RESPONSE_OK));
    }
    
    int number_chooser_dialog::choose_number(const int start_number, const int maximum_number) {
        spin_button.set_adjustment(Gtk::Adjustment::create(start_number, 1, maximum_number));
        
        show_all_children();
        present();
        const int number = get_response();
        hide();
        
        return number;
    }
    
    int number_chooser_dialog::get_response() {
        switch (run()) {
            case Gtk::RESPONSE_OK: return spin_button.get_value_as_int();
            default: return 0;
        }
    }
}
