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
#include "io/setting.h"
#include "view/flymake_dialog.h"

namespace view {
    flymake_dialog::flymake_dialog(Gtk::Window& window) : Gtk::Dialog("Flymake", window), label(_("Command: ")) {
        entry.set_activates_default(true);
        
        box.pack_start(label);
        box.pack_start(entry);
        
        get_content_area()->pack_start(box);
        set_default(*add_button("OK", Gtk::RESPONSE_OK));
    }
    
    void flymake_dialog::update_command(const std::string& language) {
        io::setting& setting = io::setting::get();
        entry.set_text(setting.get_string(language, io::setting::FLYMAKE_COMMAND));
        
        show_all_children();
        present();
        
        switch (run()) {
            case Gtk::RESPONSE_OK: setting.set_string(language, io::setting::FLYMAKE_COMMAND, entry.get_text());
        }
        
        hide();
    }
}
