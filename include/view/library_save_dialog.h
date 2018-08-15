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

#ifndef CEDIT_VIEW_LIBRARY_SAVE_DIALOG_H_
#define CEDIT_VIEW_LIBRARY_SAVE_DIALOG_H_

#include <string>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/dialog.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/window.h>

namespace view {
    class library_save_dialog final : public Gtk::Dialog {
        public:
        library_save_dialog(Gtk::Window& window);
        ~library_save_dialog() = default;
        
        std::string choose_library();
        
        private:
        Gtk::Box box;
        Gtk::Label label;
        Gtk::Entry entry;
        
        void on_entry_change(Gtk::Button* button);
        
        std::string get_response();
        bool can_continue();
    };
}

#endif
