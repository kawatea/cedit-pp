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

#include <glibmm/miscutils.h>
#include "io/io_util.h"

namespace io {
    static std::string get_directory() {
        std::string directory = Glib::getenv("CEDITPP_DIR");
        if (directory.empty()) directory = Glib::get_home_dir() + "/.cedit++";
        return directory;
    }
    
    std::string get_path(const std::string& filename) {
        static const std::string directory = get_directory();
        return directory + "/" + filename;
    }
}
