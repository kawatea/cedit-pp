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

#include <gdkmm/pixbuf.h>
#include "io/gettext.h"
#include "view/about_dialog.h"

namespace view {
    about_dialog::about_dialog(Gtk::Window& window) {
        set_transient_for(window);
        set_program_name("cedit++");
        set_version(VERSION);
        set_copyright("Copyright (C) 2018 kawatea");
        set_comments(_("User-friendly source code editor"));
        set_license_type(Gtk::License::LICENSE_GPL_3_0);
        set_website("http://github.com/kawatea/cedit++");
        set_website_label("github");
        set_authors({"kawatea"});
        set_logo(Gdk::Pixbuf::create_from_resource("/resource/icon.png"));
    }
}
