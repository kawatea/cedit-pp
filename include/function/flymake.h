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

#ifndef CEDIT_FUNCTION_FLYMAKE_H_
#define CEDIT_FUNCTION_FLYMAKE_H_

#include <string>
#include <utility>
#include <vector>
#include <glibmm/ustring.h>
#include <gtkmm/textiter.h>
#include <gtkmm/texttag.h>
#include <gtkmm/tooltip.h>
#include <gtksourceviewmm/buffer.h>

namespace function {
    class flymake final {
        public:
        flymake() = default;
        ~flymake() = default;
        
        void apply(const Glib::RefPtr<Gsv::Buffer>& buffer, const std::string& filename, const Glib::ustring& language);
        bool show_tooltip(const Gtk::TextIter& iter, const Glib::RefPtr<Gtk::Tooltip>& tooltip);
        
        private:
        std::vector<std::pair<Glib::RefPtr<Gtk::TextTag>, std::string>> tags;
        
        std::string read_output(const std::string& filename, const Glib::ustring& language);
        void add_tag(const Glib::RefPtr<Gsv::Buffer>& buffer, const int line, const std::string& type, const std::string& message);
    };
}

#endif
