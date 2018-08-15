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
#include "view/file_chooser_dialog.h"

namespace view {
    file_chooser_dialog::file_chooser_dialog(Gtk::Window& window, const Glib::ustring& title, const Glib::ustring& button_label, bool save) : Gtk::FileChooserDialog(window, title, save ? Gtk::FILE_CHOOSER_ACTION_SAVE : Gtk::FILE_CHOOSER_ACTION_OPEN) {
        add_button(button_label, Gtk::RESPONSE_OK);
        add_button(_("Cancel"), Gtk::RESPONSE_CANCEL);
        if (save) set_do_overwrite_confirmation(true);
    }
    
    std::string file_chooser_dialog::choose_file() {
        switch (run()) {
            case Gtk::RESPONSE_OK: return get_filename();
            default: return "";
        }
    }
}
