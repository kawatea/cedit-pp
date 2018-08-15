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
#include "view/font_chooser_dialog.h"

namespace view {
    font_chooser_dialog::font_chooser_dialog(Gtk::Window& window, const Glib::ustring& font) : Gtk::FontChooserDialog(_("Font"), window) {
        set_font(font);
        set_preview_text("abcdefghij 0123456789 あいうえお");
    }
    
    Glib::ustring font_chooser_dialog::choose_font() {
        switch (run()) {
            case Gtk::RESPONSE_OK: return get_font();
            default: return "";
        }
    }
}
