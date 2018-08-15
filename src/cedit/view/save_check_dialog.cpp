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

#include "io/gettext.h"
#include "view/save_check_dialog.h"

namespace view {
    save_check_dialog::save_check_dialog(Gtk::Window& window) : Gtk::MessageDialog(window, _("Do you want to save changes?"), false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_NONE) {
        add_button(_("Save"), Gtk::RESPONSE_YES);
        add_button(_("Don't Save"), Gtk::RESPONSE_NO);
        add_button(_("Cancel"), Gtk::RESPONSE_CANCEL);
    }
    
    int save_check_dialog::check_save() {
        show_all_children();
        present();
        const int response = run();
        hide();
        
        return response;
    }
}
