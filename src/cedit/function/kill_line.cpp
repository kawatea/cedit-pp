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

#include <gtkmm/clipboard.h>
#include <gtkmm/textiter.h>
#include "function/kill_line.h"

namespace function {
    void kill_line::clear() {
        if (!in_process) kill_buffer = "";
    }
    
    void kill_line::apply(const Glib::RefPtr<Gsv::Buffer>& buffer) {
        in_process = true;
        const Gtk::TextIter start = buffer->get_iter_at_mark(buffer->get_insert());
        Gtk::TextIter end = start;
        if (start.starts_line() || start.ends_line() || !kill_buffer.empty()) {
            end.forward_line();
        } else {
            end.forward_to_line_end();
        }
        kill_buffer += buffer->get_text(start, end);
        buffer->erase(start, end);
        Gtk::Clipboard::get()->set_text(kill_buffer);
        in_process = false;
    }
}
