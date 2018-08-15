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

#include <locale.h>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <gtkmm/application.h>
#include <gtksourceviewmm/init.h>
#include "io/encoding.h"
#include "io/file.h"
#include "io/gettext.h"
#include "io/io_util.h"
#include "parser/command_line_parser.h"
#include "view/window.h"

int main(int argc, char** argv) {
    setlocale(LC_ALL, "");
    bindtextdomain("cedit++", io::get_path("share/locale").c_str());
    bind_textdomain_codeset("cedit++", "UTF-8");
    textdomain("cedit++");
    
    parser::command_line_parser parser("cedit", "cedit++ " VERSION "\nCopyright (C) 2018 kawatea");
    parser.add_positional_argument<std::string>("file", "file name to open");
    parser.add_property<io::encoding>('e', "encoding", "encoding of opening file, guessed when unspecified", false, io::encoding::NONE, io::encoding::from_name);
    
    parser.parse(argc, argv);
    
    std::unique_ptr<io::file> file = std::make_unique<io::file>(parser.get_positional_argument<std::string>(), parser.get<io::encoding>('e'));
    if (!file->is_empty() && !file->can_open()) {
        std::cerr << "can't open a file" << std::endl;
        return EXIT_FAILURE;
    }
    
    Gsv::init();
    const Glib::RefPtr<Gtk::Application> application = Gtk::Application::create("kawatea.cedit", Gio::APPLICATION_NON_UNIQUE);
    view::window window(std::move(file));
    
    return application->run(window);
}
