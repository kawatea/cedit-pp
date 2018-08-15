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

#include <utility>
#include <glibmm/ustring.h>
#include <gtkmm/textiter.h>
#include "function/indent.h"

namespace function {
    static bool is_space_char(const gunichar c) {
        return (c == ' ') || (c == '\t');
    }
    
    static std::pair<int, int> count_brackets(const Gtk::TextIter& iterator) {
        Gtk::TextIter now = iterator;
        int open = 0, close = 0;
        bool escape = false, in_string = false, in_char = false;
        for (; !now.ends_line(); now++) {
            const gunichar c = *now;
            if (escape) {
                escape = false;
            } else if (c == '\\') {
                escape = true;
            } else if (in_string) {
                if (c == '"') in_string = false;
            } else if (in_char) {
                if (c == '\'') in_char = false;
            } else if (c == '"') {
                in_string = true;
            } else if (c == '\'') {
                in_char = true;
            } else if (c == '{') {
                open++;
            } else if (c == '}') {
                (open > 0) ? open-- : close++;
            }
        }
        return std::make_pair(open, close);
    }
    
    static int calc_depth(const Glib::RefPtr<Gsv::Buffer>& buffer, const int tab_width, int line) {
        for (; line >= 0; line--) {
            Gtk::TextIter iterator = buffer->get_iter_at_line(line);
            if (iterator.ends_line()) continue;
            int depth = 0;
            for (; is_space_char(*iterator); iterator++) {
                (*iterator == ' ') ? depth++ : depth += tab_width;
            }
            return depth + count_brackets(iterator).first * tab_width;
        }
        return 0;
    }
    
    static Gtk::TextIter erase_leading_blanks(const Glib::RefPtr<Gsv::Buffer>& buffer, const int line) {
        const Gtk::TextIter start = buffer->get_iter_at_line(line);
        Gtk::TextIter end = start;
        for (; is_space_char(*end); end++) ;
        return buffer->erase(start, end);
    }
    
    static int indent_line(const Glib::RefPtr<Gsv::Buffer>& buffer, const bool use_spaces, const int tab_width, const int line, int depth) {
        const Gtk::TextIter iterator = erase_leading_blanks(buffer, line);
        const std::pair<int, int> count = count_brackets(iterator);
        depth -= count.second * tab_width;
        if (depth < 0) depth = 0;
        buffer->insert(iterator, use_spaces ? Glib::ustring(depth, ' ') : Glib::ustring(depth / tab_width, '\t'));
        return depth + count.first * tab_width;
    }
    
    void indent(const Glib::RefPtr<Gsv::Buffer>& buffer, const bool use_spaces, const int tab_width) {
        Gtk::TextIter start, end;
        buffer->get_selection_bounds(start, end);
        const int line_start = start.get_line();
        const int line_end = end.get_line();
        int depth = calc_depth(buffer, tab_width, line_start - 1);
        for (int line = line_start; line <= line_end; line++) depth = indent_line(buffer, use_spaces, tab_width, line, depth);
    }
}
