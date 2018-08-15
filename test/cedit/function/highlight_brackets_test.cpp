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

#include <glibmm/ustring.h>
#include <gtkmm/textiter.h>
#include <gtkmm/texttag.h>
#include <gtkmm/texttagtable.h>
#include <gtksourceviewmm/buffer.h>
#include <gtksourceviewmm/init.h>
#include "gtest/gtest.h"
#include "function/highlight_brackets.h"

static Glib::RefPtr<Gsv::Buffer> highlight_brackets(const Glib::ustring& before, const Glib::ustring& after) {
    Gsv::init();
    const Glib::RefPtr<Gsv::Buffer> buffer = Gsv::Buffer::create();
    function::highlight_brackets highlight_brackets(buffer);
    buffer->set_text(before + after);
    buffer->place_cursor(buffer->get_iter_at_offset(before.size()));
    highlight_brackets.apply(buffer);
    return buffer;
}

static void highlight_brackets_no_match(const Glib::ustring& before, const Glib::ustring& after) {
    const Glib::RefPtr<Gsv::Buffer> buffer = highlight_brackets(before, after);
    const Glib::RefPtr<Gtk::TextTag> match = buffer->get_tag_table()->lookup("brackets_match");
    const Glib::RefPtr<Gtk::TextTag> unmatch = buffer->get_tag_table()->lookup("brackets_unmatch");
    Gtk::TextIter iterator = buffer->get_iter_at_mark(buffer->get_insert());
    
    EXPECT_FALSE(iterator.has_tag(match));
    EXPECT_FALSE(iterator.has_tag(unmatch));
    
    iterator--;
    
    EXPECT_FALSE(iterator.has_tag(match));
    EXPECT_FALSE(iterator.has_tag(unmatch));
}

static void highlight_brackets_unmatch(const bool forward, const Glib::ustring& before, const Glib::ustring& after) {
    const Glib::RefPtr<Gsv::Buffer> buffer = highlight_brackets(before, after);
    const Glib::RefPtr<Gtk::TextTag> match = buffer->get_tag_table()->lookup("brackets_match");
    const Glib::RefPtr<Gtk::TextTag> unmatch = buffer->get_tag_table()->lookup("brackets_unmatch");
    Gtk::TextIter iterator = buffer->get_iter_at_mark(buffer->get_insert());
    if (!forward) iterator--;
    
    EXPECT_FALSE(iterator.has_tag(match));
    EXPECT_TRUE(iterator.has_tag(unmatch));
}

static void highlight_brackets_match(const bool forward, const Glib::ustring& before, const Glib::ustring& middle, const Glib::ustring& after) {
    const Glib::RefPtr<Gsv::Buffer> buffer = (forward ? highlight_brackets(before, middle + after) : highlight_brackets(before + middle, after));
    const Glib::RefPtr<Gtk::TextTag> match = buffer->get_tag_table()->lookup("brackets_match");
    const Gtk::TextIter start = buffer->get_iter_at_offset(before.size());
    const Gtk::TextIter end = buffer->get_iter_at_offset(before.size() + middle.size() - 1);
    
    EXPECT_TRUE(start.has_tag(match));
    EXPECT_TRUE(end.has_tag(match));
}

TEST(highlight_brackets, test_no_match) {
    highlight_brackets_no_match("", "");
    highlight_brackets_no_match("", "test");
    highlight_brackets_no_match("test", "");
    highlight_brackets_no_match("\\)", "");
    highlight_brackets_no_match("\\}", "");
    highlight_brackets_no_match("\\]", "");
    highlight_brackets_no_match("\\\\\\)", "");
    highlight_brackets_no_match("\\\\\\}", "");
    highlight_brackets_no_match("\\\\\\]", "");
    highlight_brackets_no_match("\\)", "test");
    highlight_brackets_no_match("\\}", "test");
    highlight_brackets_no_match("\\]", "test");
    highlight_brackets_no_match("\\\\\\)", "test");
    highlight_brackets_no_match("\\\\\\}", "test");
    highlight_brackets_no_match("\\\\\\]", "test");
    highlight_brackets_no_match("test\\)", "");
    highlight_brackets_no_match("test\\}", "");
    highlight_brackets_no_match("test\\]", "");
    highlight_brackets_no_match("test\\\\\\)", "");
    highlight_brackets_no_match("test\\\\\\}", "");
    highlight_brackets_no_match("test\\\\\\]", "");
}

TEST(highlight_brackets, test_forward) {
    highlight_brackets_unmatch(true, "", "(");
    highlight_brackets_unmatch(true, "", "{");
    highlight_brackets_unmatch(true, "", "[");
    highlight_brackets_unmatch(true, "", "(()");
    highlight_brackets_unmatch(true, "", "{{}");
    highlight_brackets_unmatch(true, "", "[[]");
    highlight_brackets_unmatch(true, "", "(\\)");
    highlight_brackets_unmatch(true, "", "{\\}");
    highlight_brackets_unmatch(true, "", "[\\]");
    highlight_brackets_unmatch(true, "", "(\")");
    highlight_brackets_unmatch(true, "", "{\"}");
    highlight_brackets_unmatch(true, "", "[\"]");
    highlight_brackets_unmatch(true, "", "(')");
    highlight_brackets_unmatch(true, "", "{'}");
    highlight_brackets_unmatch(true, "", "[']");
    highlight_brackets_unmatch(true, "", "(}");
    highlight_brackets_unmatch(true, "", "{]");
    highlight_brackets_unmatch(true, "", "[)");
    highlight_brackets_unmatch(true, "test", "(");
    highlight_brackets_unmatch(true, "test", "{");
    highlight_brackets_unmatch(true, "test", "[");
    highlight_brackets_unmatch(true, "test", "(()");
    highlight_brackets_unmatch(true, "test", "{{}");
    highlight_brackets_unmatch(true, "test", "[[]");
    highlight_brackets_unmatch(true, "test", "(\\)");
    highlight_brackets_unmatch(true, "test", "{\\}");
    highlight_brackets_unmatch(true, "test", "[\\]");
    highlight_brackets_unmatch(true, "test", "(\")");
    highlight_brackets_unmatch(true, "test", "{\"}");
    highlight_brackets_unmatch(true, "test", "[\"]");
    highlight_brackets_unmatch(true, "test", "(')");
    highlight_brackets_unmatch(true, "test", "{'}");
    highlight_brackets_unmatch(true, "test", "[']");
    highlight_brackets_unmatch(true, "test", "(}");
    highlight_brackets_unmatch(true, "test", "{]");
    highlight_brackets_unmatch(true, "test", "[)");
    highlight_brackets_unmatch(true, "\\)", "(");
    highlight_brackets_unmatch(true, "\\)", "{");
    highlight_brackets_unmatch(true, "\\)", "[");
    highlight_brackets_unmatch(true, "\\)", "(()");
    highlight_brackets_unmatch(true, "\\)", "{{}");
    highlight_brackets_unmatch(true, "\\)", "[[]");
    highlight_brackets_unmatch(true, "\\)", "(\\)");
    highlight_brackets_unmatch(true, "\\)", "{\\}");
    highlight_brackets_unmatch(true, "\\)", "[\\]");
    highlight_brackets_unmatch(true, "\\)", "(\")");
    highlight_brackets_unmatch(true, "\\)", "{\"}");
    highlight_brackets_unmatch(true, "\\)", "[\"]");
    highlight_brackets_unmatch(true, "\\)", "(')");
    highlight_brackets_unmatch(true, "\\)", "{'}");
    highlight_brackets_unmatch(true, "\\)", "[']");
    highlight_brackets_unmatch(true, "\\)", "(}");
    highlight_brackets_unmatch(true, "\\)", "{]");
    highlight_brackets_unmatch(true, "\\)", "[)");
    highlight_brackets_match(true, "", "()", "");
    highlight_brackets_match(true, "", "{}", "");
    highlight_brackets_match(true, "", "[]", "");
    highlight_brackets_match(true, "", "(test)", "");
    highlight_brackets_match(true, "", "{test}", "");
    highlight_brackets_match(true, "", "[test]", "");
    highlight_brackets_match(true, "", "(())", "");
    highlight_brackets_match(true, "", "{{}}", "");
    highlight_brackets_match(true, "", "[[]]", "");
    highlight_brackets_match(true, "", "({})", "");
    highlight_brackets_match(true, "", "{[]}", "");
    highlight_brackets_match(true, "", "[()]", "");
    highlight_brackets_match(true, "", "(\\()", "");
    highlight_brackets_match(true, "", "{\\{}", "");
    highlight_brackets_match(true, "", "[\\[]", "");
    highlight_brackets_match(true, "", "(\"(\")", "");
    highlight_brackets_match(true, "", "{\"{\"}", "");
    highlight_brackets_match(true, "", "[\"[\"]", "");
    highlight_brackets_match(true, "", "('(')", "");
    highlight_brackets_match(true, "", "{'{'}", "");
    highlight_brackets_match(true, "", "['[']", "");
    highlight_brackets_match(true, "", "(\"'\")", "");
    highlight_brackets_match(true, "", "{\"'\"}", "");
    highlight_brackets_match(true, "", "[\"'\"]", "");
    highlight_brackets_match(true, "", "('\"')", "");
    highlight_brackets_match(true, "", "{'\"'}", "");
    highlight_brackets_match(true, "", "['\"']", "");
    highlight_brackets_match(true, "test", "()", "");
    highlight_brackets_match(true, "test", "{}", "");
    highlight_brackets_match(true, "test", "[]", "");
    highlight_brackets_match(true, "test", "(test)", "");
    highlight_brackets_match(true, "test", "{test}", "");
    highlight_brackets_match(true, "test", "[test]", "");
    highlight_brackets_match(true, "test", "(())", "");
    highlight_brackets_match(true, "test", "{{}}", "");
    highlight_brackets_match(true, "test", "[[]]", "");
    highlight_brackets_match(true, "test", "({})", "");
    highlight_brackets_match(true, "test", "{[]}", "");
    highlight_brackets_match(true, "test", "[()]", "");
    highlight_brackets_match(true, "test", "(\\()", "");
    highlight_brackets_match(true, "test", "{\\{}", "");
    highlight_brackets_match(true, "test", "[\\[]", "");
    highlight_brackets_match(true, "test", "(\"(\")", "");
    highlight_brackets_match(true, "test", "{\"{\"}", "");
    highlight_brackets_match(true, "test", "[\"[\"]", "");
    highlight_brackets_match(true, "test", "('(')", "");
    highlight_brackets_match(true, "test", "{'{'}", "");
    highlight_brackets_match(true, "test", "['[']", "");
    highlight_brackets_match(true, "test", "(\"'\")", "");
    highlight_brackets_match(true, "test", "{\"'\"}", "");
    highlight_brackets_match(true, "test", "[\"'\"]", "");
    highlight_brackets_match(true, "test", "('\"')", "");
    highlight_brackets_match(true, "test", "{'\"'}", "");
    highlight_brackets_match(true, "test", "['\"']", "");
    highlight_brackets_match(true, "", "()", "test");
    highlight_brackets_match(true, "", "{}", "test");
    highlight_brackets_match(true, "", "[]", "test");
    highlight_brackets_match(true, "", "(test)", "test");
    highlight_brackets_match(true, "", "{test}", "test");
    highlight_brackets_match(true, "", "[test]", "test");
    highlight_brackets_match(true, "", "(())", "test");
    highlight_brackets_match(true, "", "{{}}", "test");
    highlight_brackets_match(true, "", "[[]]", "test");
    highlight_brackets_match(true, "", "({})", "test");
    highlight_brackets_match(true, "", "{[]}", "test");
    highlight_brackets_match(true, "", "[()]", "test");
    highlight_brackets_match(true, "", "(\\()", "test");
    highlight_brackets_match(true, "", "{\\{}", "test");
    highlight_brackets_match(true, "", "[\\[]", "test");
    highlight_brackets_match(true, "", "(\"(\")", "test");
    highlight_brackets_match(true, "", "{\"{\"}", "test");
    highlight_brackets_match(true, "", "[\"[\"]", "test");
    highlight_brackets_match(true, "", "('(')", "test");
    highlight_brackets_match(true, "", "{'{'}", "test");
    highlight_brackets_match(true, "", "['[']", "test");
    highlight_brackets_match(true, "", "(\"'\")", "test");
    highlight_brackets_match(true, "", "{\"'\"}", "test");
    highlight_brackets_match(true, "", "[\"'\"]", "test");
    highlight_brackets_match(true, "", "('\"')", "test");
    highlight_brackets_match(true, "", "{'\"'}", "test");
    highlight_brackets_match(true, "", "['\"']", "test");
    highlight_brackets_match(true, "test", "()", "test");
    highlight_brackets_match(true, "test", "{}", "test");
    highlight_brackets_match(true, "test", "[]", "test");
    highlight_brackets_match(true, "test", "(test)", "test");
    highlight_brackets_match(true, "test", "{test}", "test");
    highlight_brackets_match(true, "test", "[test]", "test");
    highlight_brackets_match(true, "test", "(())", "test");
    highlight_brackets_match(true, "test", "{{}}", "test");
    highlight_brackets_match(true, "test", "[[]]", "test");
    highlight_brackets_match(true, "test", "({})", "test");
    highlight_brackets_match(true, "test", "{[]}", "test");
    highlight_brackets_match(true, "test", "[()]", "test");
    highlight_brackets_match(true, "test", "(\\()", "test");
    highlight_brackets_match(true, "test", "{\\{}", "test");
    highlight_brackets_match(true, "test", "[\\[]", "test");
    highlight_brackets_match(true, "test", "(\"(\")", "test");
    highlight_brackets_match(true, "test", "{\"{\"}", "test");
    highlight_brackets_match(true, "test", "[\"[\"]", "test");
    highlight_brackets_match(true, "test", "('(')", "test");
    highlight_brackets_match(true, "test", "{'{'}", "test");
    highlight_brackets_match(true, "test", "['[']", "test");
    highlight_brackets_match(true, "test", "(\"'\")", "test");
    highlight_brackets_match(true, "test", "{\"'\"}", "test");
    highlight_brackets_match(true, "test", "[\"'\"]", "test");
    highlight_brackets_match(true, "test", "('\"')", "test");
    highlight_brackets_match(true, "test", "{'\"'}", "test");
    highlight_brackets_match(true, "test", "['\"']", "test");
    highlight_brackets_match(true, "\\)", "()", "");
    highlight_brackets_match(true, "\\)", "{}", "");
    highlight_brackets_match(true, "\\)", "[]", "");
    highlight_brackets_match(true, "\\)", "(test)", "");
    highlight_brackets_match(true, "\\)", "{test}", "");
    highlight_brackets_match(true, "\\)", "[test]", "");
    highlight_brackets_match(true, "\\)", "(())", "");
    highlight_brackets_match(true, "\\)", "{{}}", "");
    highlight_brackets_match(true, "\\)", "[[]]", "");
    highlight_brackets_match(true, "\\)", "({})", "");
    highlight_brackets_match(true, "\\)", "{[]}", "");
    highlight_brackets_match(true, "\\)", "[()]", "");
    highlight_brackets_match(true, "\\)", "(\\()", "");
    highlight_brackets_match(true, "\\)", "{\\{}", "");
    highlight_brackets_match(true, "\\)", "[\\[]", "");
    highlight_brackets_match(true, "\\)", "(\"(\")", "");
    highlight_brackets_match(true, "\\)", "{\"{\"}", "");
    highlight_brackets_match(true, "\\)", "[\"[\"]", "");
    highlight_brackets_match(true, "\\)", "('(')", "");
    highlight_brackets_match(true, "\\)", "{'{'}", "");
    highlight_brackets_match(true, "\\)", "['[']", "");
    highlight_brackets_match(true, "\\)", "(\"'\")", "");
    highlight_brackets_match(true, "\\)", "{\"'\"}", "");
    highlight_brackets_match(true, "\\)", "[\"'\"]", "");
    highlight_brackets_match(true, "\\)", "('\"')", "");
    highlight_brackets_match(true, "\\)", "{'\"'}", "");
    highlight_brackets_match(true, "\\)", "['\"']", "");
}

TEST(highlight_brackets, test_backward) {
    highlight_brackets_unmatch(false, ")", "");
    highlight_brackets_unmatch(false, "}", "");
    highlight_brackets_unmatch(false, "]", "");
    highlight_brackets_unmatch(false, "())", "");
    highlight_brackets_unmatch(false, "())", "");
    highlight_brackets_unmatch(false, "[]]", "");
    highlight_brackets_unmatch(false, "\\()", "");
    highlight_brackets_unmatch(false, "\\{}", "");
    highlight_brackets_unmatch(false, "\\[]", "");
    highlight_brackets_unmatch(false, "(\")", "");
    highlight_brackets_unmatch(false, "{\"}", "");
    highlight_brackets_unmatch(false, "[\"]", "");
    highlight_brackets_unmatch(false, "(')", "");
    highlight_brackets_unmatch(false, "{'}", "");
    highlight_brackets_unmatch(false, "[']", "");
    highlight_brackets_unmatch(false, "{)", "");
    highlight_brackets_unmatch(false, "[}", "");
    highlight_brackets_unmatch(false, "(]", "");
    highlight_brackets_unmatch(false, ")", "test");
    highlight_brackets_unmatch(false, "}", "test");
    highlight_brackets_unmatch(false, "]", "test");
    highlight_brackets_unmatch(false, "())", "test");
    highlight_brackets_unmatch(false, "())", "test");
    highlight_brackets_unmatch(false, "[]]", "test");
    highlight_brackets_unmatch(false, "\\()", "test");
    highlight_brackets_unmatch(false, "\\{}", "test");
    highlight_brackets_unmatch(false, "\\[]", "test");
    highlight_brackets_unmatch(false, "(\")", "test");
    highlight_brackets_unmatch(false, "{\"}", "test");
    highlight_brackets_unmatch(false, "[\"]", "test");
    highlight_brackets_unmatch(false, "(')", "test");
    highlight_brackets_unmatch(false, "{'}", "test");
    highlight_brackets_unmatch(false, "[']", "test");
    highlight_brackets_unmatch(false, "{)", "test");
    highlight_brackets_unmatch(false, "[}", "test");
    highlight_brackets_unmatch(false, "(]", "test");
    highlight_brackets_unmatch(false, ")", "(");
    highlight_brackets_unmatch(false, "}", "(");
    highlight_brackets_unmatch(false, "]", "(");
    highlight_brackets_unmatch(false, "())", "(");
    highlight_brackets_unmatch(false, "())", "(");
    highlight_brackets_unmatch(false, "[]]", "(");
    highlight_brackets_unmatch(false, "\\()", "(");
    highlight_brackets_unmatch(false, "\\{}", "(");
    highlight_brackets_unmatch(false, "\\[]", "(");
    highlight_brackets_unmatch(false, "(\")", "(");
    highlight_brackets_unmatch(false, "{\"}", "(");
    highlight_brackets_unmatch(false, "[\"]", "(");
    highlight_brackets_unmatch(false, "(')", "(");
    highlight_brackets_unmatch(false, "{'}", "(");
    highlight_brackets_unmatch(false, "[']", "(");
    highlight_brackets_unmatch(false, "{)", "(");
    highlight_brackets_unmatch(false, "[}", "(");
    highlight_brackets_unmatch(false, "(]", "(");
    highlight_brackets_match(false, "", "()", "");
    highlight_brackets_match(false, "", "{}", "");
    highlight_brackets_match(false, "", "[]", "");
    highlight_brackets_match(false, "", "(test)", "");
    highlight_brackets_match(false, "", "{test}", "");
    highlight_brackets_match(false, "", "[test]", "");
    highlight_brackets_match(false, "", "(())", "");
    highlight_brackets_match(false, "", "{{}}", "");
    highlight_brackets_match(false, "", "[[]]", "");
    highlight_brackets_match(false, "", "({})", "");
    highlight_brackets_match(false, "", "{[]}", "");
    highlight_brackets_match(false, "", "[()]", "");
    highlight_brackets_match(false, "", "(\\()", "");
    highlight_brackets_match(false, "", "{\\{}", "");
    highlight_brackets_match(false, "", "[\\[]", "");
    highlight_brackets_match(false, "", "(\"(\")", "");
    highlight_brackets_match(false, "", "{\"{\"}", "");
    highlight_brackets_match(false, "", "[\"[\"]", "");
    highlight_brackets_match(false, "", "('(')", "");
    highlight_brackets_match(false, "", "{'{'}", "");
    highlight_brackets_match(false, "", "['[']", "");
    highlight_brackets_match(false, "", "(\"'\")", "");
    highlight_brackets_match(false, "", "{\"'\"}", "");
    highlight_brackets_match(false, "", "[\"'\"]", "");
    highlight_brackets_match(false, "", "('\"')", "");
    highlight_brackets_match(false, "", "{'\"'}", "");
    highlight_brackets_match(false, "", "['\"']", "");
    highlight_brackets_match(false, "test", "()", "");
    highlight_brackets_match(false, "test", "{}", "");
    highlight_brackets_match(false, "test", "[]", "");
    highlight_brackets_match(false, "test", "(test)", "");
    highlight_brackets_match(false, "test", "{test}", "");
    highlight_brackets_match(false, "test", "[test]", "");
    highlight_brackets_match(false, "test", "(())", "");
    highlight_brackets_match(false, "test", "{{}}", "");
    highlight_brackets_match(false, "test", "[[]]", "");
    highlight_brackets_match(false, "test", "({})", "");
    highlight_brackets_match(false, "test", "{[]}", "");
    highlight_brackets_match(false, "test", "[()]", "");
    highlight_brackets_match(false, "test", "(\\()", "");
    highlight_brackets_match(false, "test", "{\\{}", "");
    highlight_brackets_match(false, "test", "[\\[]", "");
    highlight_brackets_match(false, "test", "(\"(\")", "");
    highlight_brackets_match(false, "test", "{\"{\"}", "");
    highlight_brackets_match(false, "test", "[\"[\"]", "");
    highlight_brackets_match(false, "test", "('(')", "");
    highlight_brackets_match(false, "test", "{'{'}", "");
    highlight_brackets_match(false, "test", "['[']", "");
    highlight_brackets_match(false, "test", "(\"'\")", "");
    highlight_brackets_match(false, "test", "{\"'\"}", "");
    highlight_brackets_match(false, "test", "[\"'\"]", "");
    highlight_brackets_match(false, "test", "('\"')", "");
    highlight_brackets_match(false, "test", "{'\"'}", "");
    highlight_brackets_match(false, "test", "['\"']", "");
    highlight_brackets_match(false, "", "()", "test");
    highlight_brackets_match(false, "", "{}", "test");
    highlight_brackets_match(false, "", "[]", "test");
    highlight_brackets_match(false, "", "(test)", "test");
    highlight_brackets_match(false, "", "{test}", "test");
    highlight_brackets_match(false, "", "[test]", "test");
    highlight_brackets_match(false, "", "(())", "test");
    highlight_brackets_match(false, "", "{{}}", "test");
    highlight_brackets_match(false, "", "[[]]", "test");
    highlight_brackets_match(false, "", "({})", "test");
    highlight_brackets_match(false, "", "{[]}", "test");
    highlight_brackets_match(false, "", "[()]", "test");
    highlight_brackets_match(false, "", "(\\()", "test");
    highlight_brackets_match(false, "", "{\\{}", "test");
    highlight_brackets_match(false, "", "[\\[]", "test");
    highlight_brackets_match(false, "", "(\"(\")", "test");
    highlight_brackets_match(false, "", "{\"{\"}", "test");
    highlight_brackets_match(false, "", "[\"[\"]", "test");
    highlight_brackets_match(false, "", "('(')", "test");
    highlight_brackets_match(false, "", "{'{'}", "test");
    highlight_brackets_match(false, "", "['[']", "test");
    highlight_brackets_match(false, "", "(\"'\")", "test");
    highlight_brackets_match(false, "", "{\"'\"}", "test");
    highlight_brackets_match(false, "", "[\"'\"]", "test");
    highlight_brackets_match(false, "", "('\"')", "test");
    highlight_brackets_match(false, "", "{'\"'}", "test");
    highlight_brackets_match(false, "", "['\"']", "test");
    highlight_brackets_match(false, "test", "()", "test");
    highlight_brackets_match(false, "test", "{}", "test");
    highlight_brackets_match(false, "test", "[]", "test");
    highlight_brackets_match(false, "test", "(test)", "test");
    highlight_brackets_match(false, "test", "{test}", "test");
    highlight_brackets_match(false, "test", "[test]", "test");
    highlight_brackets_match(false, "test", "(())", "test");
    highlight_brackets_match(false, "test", "{{}}", "test");
    highlight_brackets_match(false, "test", "[[]]", "test");
    highlight_brackets_match(false, "test", "({})", "test");
    highlight_brackets_match(false, "test", "{[]}", "test");
    highlight_brackets_match(false, "test", "[()]", "test");
    highlight_brackets_match(false, "test", "(\\()", "test");
    highlight_brackets_match(false, "test", "{\\{}", "test");
    highlight_brackets_match(false, "test", "[\\[]", "test");
    highlight_brackets_match(false, "test", "(\"(\")", "test");
    highlight_brackets_match(false, "test", "{\"{\"}", "test");
    highlight_brackets_match(false, "test", "[\"[\"]", "test");
    highlight_brackets_match(false, "test", "('(')", "test");
    highlight_brackets_match(false, "test", "{'{'}", "test");
    highlight_brackets_match(false, "test", "['[']", "test");
    highlight_brackets_match(false, "test", "(\"'\")", "test");
    highlight_brackets_match(false, "test", "{\"'\"}", "test");
    highlight_brackets_match(false, "test", "[\"'\"]", "test");
    highlight_brackets_match(false, "test", "('\"')", "test");
    highlight_brackets_match(false, "test", "{'\"'}", "test");
    highlight_brackets_match(false, "test", "['\"']", "test");
    highlight_brackets_match(false, "", "()", "(");
    highlight_brackets_match(false, "", "{}", "(");
    highlight_brackets_match(false, "", "[]", "(");
    highlight_brackets_match(false, "", "(test)", "(");
    highlight_brackets_match(false, "", "{test}", "(");
    highlight_brackets_match(false, "", "[test]", "(");
    highlight_brackets_match(false, "", "(())", "(");
    highlight_brackets_match(false, "", "{{}}", "(");
    highlight_brackets_match(false, "", "[[]]", "(");
    highlight_brackets_match(false, "", "({})", "(");
    highlight_brackets_match(false, "", "{[]}", "(");
    highlight_brackets_match(false, "", "[()]", "(");
    highlight_brackets_match(false, "", "(\\()", "(");
    highlight_brackets_match(false, "", "{\\{}", "(");
    highlight_brackets_match(false, "", "[\\[]", "(");
    highlight_brackets_match(false, "", "(\"(\")", "(");
    highlight_brackets_match(false, "", "{\"{\"}", "(");
    highlight_brackets_match(false, "", "[\"[\"]", "(");
    highlight_brackets_match(false, "", "('(')", "(");
    highlight_brackets_match(false, "", "{'{'}", "(");
    highlight_brackets_match(false, "", "['[']", "(");
    highlight_brackets_match(false, "", "(\"'\")", "(");
    highlight_brackets_match(false, "", "{\"'\"}", "(");
    highlight_brackets_match(false, "", "[\"'\"]", "(");
    highlight_brackets_match(false, "", "('\"')", "(");
    highlight_brackets_match(false, "", "{'\"'}", "(");
    highlight_brackets_match(false, "", "['\"']", "(");
}
