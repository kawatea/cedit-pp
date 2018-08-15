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

#ifndef CEDIT_VIEW_FONT_DIALOG_H_
#define CEDIT_VIEW_FONT_DIALOG_H_

#include <glibmm/ustring.h>
#include <gtkmm/fontchooserdialog.h>
#include <gtkmm/window.h>

namespace view {
    class font_chooser_dialog final : public Gtk::FontChooserDialog {
        public:
        font_chooser_dialog(Gtk::Window& window, const Glib::ustring& font);
        ~font_chooser_dialog() = default;
        
        Glib::ustring choose_font();
    };
}

#endif
