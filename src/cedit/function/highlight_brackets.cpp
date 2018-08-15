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

#include <stack>
#include <gtkmm/textiter.h>
#include "function/highlight_brackets.h"

namespace function {
    static bool is_open_char(const gunichar c) {
        return (c == '(') || (c == '{') || (c == '[');
    }
    
    static bool is_close_char(const gunichar c) {
        return (c == ')') || (c == '}') || (c == ']');
    }
    
    static bool matches(const gunichar c1, const gunichar c2) {
        switch (c1) {
            case '(': return c2 == ')';
            case '{': return c2 == '}';
            case '[': return c2 == ']';
            default: return false;
        }
    }
    
    static bool is_escaped(const Gtk::TextIter& iterator) {
        if (iterator.is_start()) return false;
        Gtk::TextIter now = iterator;
        bool escape = false;
        for (now--; ; now--) {
            if (*now != '\\') return escape;
            escape = !escape;
            if (now.is_start()) return escape;
        }
    }
    
    highlight_brackets::highlight_brackets(const Glib::RefPtr<Gsv::Buffer>& buffer) {
        match_tag = buffer->create_tag("brackets_match");
        match_tag->property_background().set_value("#40E0D0");
        unmatch_tag = buffer->create_tag("brackets_unmatch");
        unmatch_tag->property_background().set_value("#A020F0");
    }
    
    void highlight_brackets::apply(const Glib::RefPtr<Gsv::Buffer>& buffer) {
        buffer->remove_tag(match_tag, buffer->begin(), buffer->end());
        buffer->remove_tag(unmatch_tag, buffer->begin(), buffer->end());
        if (!apply_backward(buffer)) apply_forward(buffer);
    }
    
    bool highlight_brackets::apply_backward(const Glib::RefPtr<Gsv::Buffer>& buffer) {
        Gtk::TextIter end = buffer->get_iter_at_mark(buffer->get_insert());
        if (end.is_start()) return false;
        end--;
        if (!is_close_char(*end) || is_escaped(end)) return false;
        
        Gtk::TextIter start = end;
        std::stack<gunichar> st;
        bool in_string = false, in_char = false;
        for (; ; start--) {
            const gunichar c = *start;
            if (is_escaped(start)) continue;
            if (in_string) {
                if (c == '"') in_string = false;
            } else if (in_char) {
                if (c == '\'') in_char = false;
            } else if (c == '"') {
                in_string = true;
            } else if (c == '\'') {
                in_char = true;
            } else if (is_open_char(c)) {
                if (!matches(c, st.top())) break;
                st.pop();
                if (st.empty()) {
                    apply_tag(buffer, start, true);
                    apply_tag(buffer, end, true);
                    return true;
                }
            } else if (is_close_char(c)) {
                st.push(c);
            }
            if (start.is_start()) break;
        }
        
        apply_tag(buffer, end, false);
        return true;
    }
    
    void highlight_brackets::apply_forward(const Glib::RefPtr<Gsv::Buffer>& buffer) {
        Gtk::TextIter start = buffer->get_iter_at_mark(buffer->get_insert());
        if (!is_open_char(*start) || is_escaped(start)) return;
        
        Gtk::TextIter end = start;
        std::stack<gunichar> st;
        bool escape = false, in_string = false, in_char = false;
        for (; !end.is_end(); end++) {
            const gunichar c = *end;
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
            } else if (is_open_char(c)) {
                st.push(c);
            } else if (is_close_char(c)) {
                if (!matches(st.top(), c)) break;
                st.pop();
                if (st.empty()) {
                    apply_tag(buffer, start, true);
                    apply_tag(buffer, end, true);
                    return;
                }
            }
        }
        
        apply_tag(buffer, start, false);
    }
    
    void highlight_brackets::apply_tag(const Glib::RefPtr<Gsv::Buffer>& buffer, const Gtk::TextIter& iterator, const bool match) {
        Gtk::TextIter end = iterator;
        end++;
        buffer->apply_tag(match ? match_tag : unmatch_tag, iterator, end);
    }
}
