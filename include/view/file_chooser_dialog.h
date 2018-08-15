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

#ifndef CEDIT_VIEW_FILE_CHOOSER_DIALOG_H_
#define CEDIT_VIEW_FILE_CHOOSER_DIALOG_H_

#include <string>
#include <glibmm/ustring.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/window.h>

namespace view {
    class file_chooser_dialog final : public Gtk::FileChooserDialog {
        public:
        file_chooser_dialog(Gtk::Window& window, const Glib::ustring& title, const Glib::ustring& button_label, bool save);
        ~file_chooser_dialog() = default;
        
        std::string choose_file();
    };
}

#endif
