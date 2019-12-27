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

#include <algorithm>
#include <vector>
#include <gtkmm/textiter.h>
#include "function/macro.h"
#include "io/setting.h"

namespace function {
    static Glib::ustring get_backward_text(const Gtk::TextIter& end, const int num) {
        Gtk::TextIter start = end;
        start.backward_chars(num);
        return start.get_slice(end);
    }
    
    bool apply_macro(const Glib::RefPtr<Gsv::Buffer>& buffer) {
        const std::vector<std::pair<Glib::ustring, Glib::ustring>> macros = io::setting::get().get_macros();
        std::string::size_type max_num = 0;
        for (const std::pair<Glib::ustring, Glib::ustring>& macro : macros) max_num = std::max(max_num, macro.first.size());
        
        const Gtk::TextIter end = buffer->get_iter_at_mark(buffer->get_insert());
        const Glib::ustring text = get_backward_text(end, max_num);
        for (const std::pair<Glib::ustring, Glib::ustring>& macro : macros) {
            const Glib::ustring& key = macro.first;
            if (text.size() >= key.size() && text.substr(text.size() - key.size()) == key) {
                Gtk::TextIter start = end;
                start.backward_chars(key.size());
                const Gtk::TextIter now = buffer->erase(start, end);
                buffer->insert(now, macro.second);
                return true;
            }
        }
        return false;
    }
}
