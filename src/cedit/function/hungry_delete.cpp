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

#include <gtkmm/textiter.h>
#include "function/hungry_delete.h"

namespace function {
    static gunichar get_backward_char(const Gtk::TextIter& iterator) {
        if (iterator.is_start()) return 0xFFFC;
        Gtk::TextIter now = iterator;
        now--;
        return *now;
    }
    
    static bool is_space_char(const gunichar c) {
        return (c == ' ') || (c == '\t');
    }
    
    bool hungry_delete_forward(const Glib::RefPtr<Gsv::Buffer>& buffer) {
        Gtk::TextIter start = buffer->get_iter_at_mark(buffer->get_insert());
        if (!is_space_char(*start)) return false;
        Gtk::TextIter end = start;
        for (end++; !end.is_end(); end++) {
            if (!is_space_char(*end)) break;
        }
        buffer->erase(start, end);
        return true;
    }
    
    bool hungry_delete_backward(const Glib::RefPtr<Gsv::Buffer>& buffer) {
        Gtk::TextIter end = buffer->get_iter_at_mark(buffer->get_insert());
        if (!is_space_char(get_backward_char(end))) return false;
        Gtk::TextIter start = end;
        for (start--; !start.is_start(); start--) {
            if (!is_space_char(get_backward_char(start))) break;
        }
        buffer->erase(start, end);
        return true;
    }
}
