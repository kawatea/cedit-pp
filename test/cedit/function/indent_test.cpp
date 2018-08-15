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
#include <gtksourceviewmm/buffer.h>
#include <gtksourceviewmm/init.h>
#include "gtest/gtest.h"
#include "function/indent.h"

static void indent(const Glib::ustring& before, const Glib::ustring& middle, const Glib::ustring& after, const bool use_spaces, const int tab_width, const Glib::ustring& expected) {
    Gsv::init();
    const Glib::RefPtr<Gsv::Buffer> buffer = Gsv::Buffer::create();
    buffer->set_text(before + middle + after);
    const Gtk::TextIter start = buffer->get_iter_at_offset(before.size());
    const Gtk::TextIter end = buffer->get_iter_at_offset(before.size() + middle.size());
    buffer->select_range(start, end);
    function::indent(buffer, use_spaces, tab_width);
    
    EXPECT_EQ(expected, buffer->get_text());
}

TEST(indent, test_no_selection) {
    indent("", "", "", true, 4, "");
    indent("test", "", "", true, 4, "test");
    indent("{", "", "", true, 4, "{");
    indent("}", "", "", true, 4, "}");
    indent("{\ntest", "", "", true, 4, "{\n    test");
    indent("{{\ntest", "", "", true, 4, "{{\n        test");
    indent("    test\n    }", "", "", true, 4, "    test\n}");
    indent("    test\n    }}", "", "", true, 4, "    test\n}}");
    indent("    test\n    {}", "", "", true, 4, "    test\n    {}");
    indent("    {}\n    test", "", "", true, 4, "    {}\n    test");
    indent("    test\n    }{", "", "", true, 4, "    test\n}{");
    indent("    }{\n    test", "", "", true, 4, "    }{\n        test");
    indent("\\{\ntest", "", "", true, 4, "\\{\ntest");
    indent("    test\n    \\}", "", "", true, 4, "    test\n    \\}");
    indent("\"{\"\ntest", "", "", true, 4, "\"{\"\ntest");
    indent("    test\n    \"}\"", "", "", true, 4, "    test\n    \"}\"");
    indent("'{'\ntest", "", "", true, 4, "'{'\ntest");
    indent("    test\n    '}'", "", "", true, 4, "    test\n    '}'");
    indent("\\{{\ntest", "", "", true, 4, "\\{{\n    test");
    indent("    test\n    \\}}", "", "", true, 4, "    test\n\\}}");
    indent("\"{\"{\ntest", "", "", true, 4, "\"{\"{\n    test");
    indent("    test\n    \"}\"}", "", "", true, 4, "    test\n\"}\"}");
    indent("'{'{\ntest", "", "", true, 4, "'{'{\n    test");
    indent("    test\n    '}'}", "", "", true, 4, "    test\n'}'}");
    indent("    test\n\ntest", "", "", true, 4, "    test\n\n    test");
    indent("\ttest\n    }", "", "", true, 4, "\ttest\n}");
    indent("\ttest\n    }}", "", "", true, 4, "\ttest\n}}");
    indent("\ttest\n    {}", "", "", true, 4, "\ttest\n    {}");
    indent("\t{}\n    test", "", "", true, 4, "\t{}\n    test");
    indent("\ttest\n    }{", "", "", true, 4, "\ttest\n}{");
    indent("\t}{\n    test", "", "", true, 4, "\t}{\n        test");
    indent("\ttest\n    \\}", "", "", true, 4, "\ttest\n    \\}");
    indent("\ttest\n    \"}\"", "", "", true, 4, "\ttest\n    \"}\"");
    indent("\ttest\n    '}'", "", "", true, 4, "\ttest\n    '}'");
    indent("\ttest\n    \\}}", "", "", true, 4, "\ttest\n\\}}");
    indent("\ttest\n    \"}\"}", "", "", true, 4, "\ttest\n\"}\"}");
    indent("\ttest\n    '}'}", "", "", true, 4, "\ttest\n'}'}");
    indent("\ttest\n\ntest", "", "", true, 4, "\ttest\n\n    test");
    indent("", "", "test", true, 4, "test");
    indent("test", "", "test", true, 4, "testtest");
    indent("{", "", "test", true, 4, "{test");
    indent("}", "", "test", true, 4, "}test");
    indent("{\ntest", "", "test", true, 4, "{\n    testtest");
    indent("{{\ntest", "", "test", true, 4, "{{\n        testtest");
    indent("    test\n    }", "", "test", true, 4, "    test\n}test");
    indent("    test\n    }}", "", "test", true, 4, "    test\n}}test");
    indent("    test\n    {}", "", "test", true, 4, "    test\n    {}test");
    indent("    {}\n    test", "", "test", true, 4, "    {}\n    testtest");
    indent("    test\n    }{", "", "test", true, 4, "    test\n}{test");
    indent("    }{\n    test", "", "test", true, 4, "    }{\n        testtest");
    indent("\\{\ntest", "", "test", true, 4, "\\{\ntesttest");
    indent("    test\n    \\}", "", "test", true, 4, "    test\n    \\}test");
    indent("\"{\"\ntest", "", "test", true, 4, "\"{\"\ntesttest");
    indent("    test\n    \"}\"", "", "test", true, 4, "    test\n    \"}\"test");
    indent("'{'\ntest", "", "test", true, 4, "'{'\ntesttest");
    indent("    test\n    '}'", "", "test", true, 4, "    test\n    '}'test");
    indent("\\{{\ntest", "", "test", true, 4, "\\{{\n    testtest");
    indent("    test\n    \\}}", "", "test", true, 4, "    test\n\\}}test");
    indent("\"{\"{\ntest", "", "test", true, 4, "\"{\"{\n    testtest");
    indent("    test\n    \"}\"}", "", "test", true, 4, "    test\n\"}\"}test");
    indent("'{'{\ntest", "", "test", true, 4, "'{'{\n    testtest");
    indent("    test\n    '}'}", "", "test", true, 4, "    test\n'}'}test");
    indent("    test\n\ntest", "", "test", true, 4, "    test\n\n    testtest");
    indent("\ttest\n    }", "", "test", true, 4, "\ttest\n}test");
    indent("\ttest\n    }}", "", "test", true, 4, "\ttest\n}}test");
    indent("\ttest\n    {}", "", "test", true, 4, "\ttest\n    {}test");
    indent("\t{}\n    test", "", "test", true, 4, "\t{}\n    testtest");
    indent("\ttest\n    }{", "", "test", true, 4, "\ttest\n}{test");
    indent("\t}{\n    test", "", "test", true, 4, "\t}{\n        testtest");
    indent("\ttest\n    \\}", "", "test", true, 4, "\ttest\n    \\}test");
    indent("\ttest\n    \"}\"", "", "test", true, 4, "\ttest\n    \"}\"test");
    indent("\ttest\n    '}'", "", "test", true, 4, "\ttest\n    '}'test");
    indent("\ttest\n    \\}}", "", "test", true, 4, "\ttest\n\\}}test");
    indent("\ttest\n    \"}\"}", "", "test", true, 4, "\ttest\n\"}\"}test");
    indent("\ttest\n    '}'}", "", "test", true, 4, "\ttest\n'}'}test");
    indent("\ttest\n\ntest", "", "test", true, 4, "\ttest\n\n    testtest");
    indent("", "", "", false, 4, "");
    indent("test", "", "", false, 4, "test");
    indent("{", "", "", false, 4, "{");
    indent("}", "", "", false, 4, "}");
    indent("{\ntest", "", "", false, 4, "{\n\ttest");
    indent("{{\ntest", "", "", false, 4, "{{\n\t\ttest");
    indent("    test\n    }", "", "", false, 4, "    test\n}");
    indent("    test\n    }}", "", "", false, 4, "    test\n}}");
    indent("    test\n    {}", "", "", false, 4, "    test\n\t{}");
    indent("    {}\n    test", "", "", false, 4, "    {}\n\ttest");
    indent("    test\n    }{", "", "", false, 4, "    test\n}{");
    indent("    }{\n    test", "", "", false, 4, "    }{\n\t\ttest");
    indent("\\{\ntest", "", "", false, 4, "\\{\ntest");
    indent("    test\n    \\}", "", "", false, 4, "    test\n\t\\}");
    indent("\"{\"\ntest", "", "", false, 4, "\"{\"\ntest");
    indent("    test\n    \"}\"", "", "", false, 4, "    test\n\t\"}\"");
    indent("'{'\ntest", "", "", false, 4, "'{'\ntest");
    indent("    test\n    '}'", "", "", false, 4, "    test\n\t'}'");
    indent("\\{{\ntest", "", "", false, 4, "\\{{\n\ttest");
    indent("    test\n    \\}}", "", "", false, 4, "    test\n\\}}");
    indent("\"{\"{\ntest", "", "", false, 4, "\"{\"{\n\ttest");
    indent("    test\n    \"}\"}", "", "", false, 4, "    test\n\"}\"}");
    indent("'{'{\ntest", "", "", false, 4, "'{'{\n\ttest");
    indent("    test\n    '}'}", "", "", false, 4, "    test\n'}'}");
    indent("    test\n\ntest", "", "", false, 4, "    test\n\n\ttest");
    indent("\ttest\n    }", "", "", false, 4, "\ttest\n}");
    indent("\ttest\n    }}", "", "", false, 4, "\ttest\n}}");
    indent("\ttest\n    {}", "", "", false, 4, "\ttest\n\t{}");
    indent("\t{}\n    test", "", "", false, 4, "\t{}\n\ttest");
    indent("\ttest\n    }{", "", "", false, 4, "\ttest\n}{");
    indent("\t}{\n    test", "", "", false, 4, "\t}{\n\t\ttest");
    indent("\ttest\n    \\}", "", "", false, 4, "\ttest\n\t\\}");
    indent("\ttest\n    \"}\"", "", "", false, 4, "\ttest\n\t\"}\"");
    indent("\ttest\n    '}'", "", "", false, 4, "\ttest\n\t'}'");
    indent("\ttest\n    \\}}", "", "", false, 4, "\ttest\n\\}}");
    indent("\ttest\n    \"}\"}", "", "", false, 4, "\ttest\n\"}\"}");
    indent("\ttest\n    '}'}", "", "", false, 4, "\ttest\n'}'}");
    indent("\ttest\n\ntest", "", "", false, 4, "\ttest\n\n\ttest");
    indent("", "", "test", false, 4, "test");
    indent("test", "", "test", false, 4, "testtest");
    indent("{", "", "test", false, 4, "{test");
    indent("}", "", "test", false, 4, "}test");
    indent("{\ntest", "", "test", false, 4, "{\n\ttesttest");
    indent("{{\ntest", "", "test", false, 4, "{{\n\t\ttesttest");
    indent("    test\n    }", "", "test", false, 4, "    test\n}test");
    indent("    test\n    }}", "", "test", false, 4, "    test\n}}test");
    indent("    test\n    {}", "", "test", false, 4, "    test\n\t{}test");
    indent("    {}\n    test", "", "test", false, 4, "    {}\n\ttesttest");
    indent("    test\n    }{", "", "test", false, 4, "    test\n}{test");
    indent("    }{\n    test", "", "test", false, 4, "    }{\n\t\ttesttest");
    indent("\\{\ntest", "", "test", false, 4, "\\{\ntesttest");
    indent("    test\n    \\}", "", "test", false, 4, "    test\n\t\\}test");
    indent("\"{\"\ntest", "", "test", false, 4, "\"{\"\ntesttest");
    indent("    test\n    \"}\"", "", "test", false, 4, "    test\n\t\"}\"test");
    indent("'{'\ntest", "", "test", false, 4, "'{'\ntesttest");
    indent("    test\n    '}'", "", "test", false, 4, "    test\n\t'}'test");
    indent("\\{{\ntest", "", "test", false, 4, "\\{{\n\ttesttest");
    indent("    test\n    \\}}", "", "test", false, 4, "    test\n\\}}test");
    indent("\"{\"{\ntest", "", "test", false, 4, "\"{\"{\n\ttesttest");
    indent("    test\n    \"}\"}", "", "test", false, 4, "    test\n\"}\"}test");
    indent("'{'{\ntest", "", "test", false, 4, "'{'{\n\ttesttest");
    indent("    test\n    '}'}", "", "test", false, 4, "    test\n'}'}test");
    indent("    test\n\ntest", "", "test", false, 4, "    test\n\n\ttesttest");
    indent("\ttest\n    }", "", "test", false, 4, "\ttest\n}test");
    indent("\ttest\n    }}", "", "test", false, 4, "\ttest\n}}test");
    indent("\ttest\n    {}", "", "test", false, 4, "\ttest\n\t{}test");
    indent("\t{}\n    test", "", "test", false, 4, "\t{}\n\ttesttest");
    indent("\ttest\n    }{", "", "test", false, 4, "\ttest\n}{test");
    indent("\t}{\n    test", "", "test", false, 4, "\t}{\n\t\ttesttest");
    indent("\ttest\n    \\}", "", "test", false, 4, "\ttest\n\t\\}test");
    indent("\ttest\n    \"}\"", "", "test", false, 4, "\ttest\n\t\"}\"test");
    indent("\ttest\n    '}'", "", "test", false, 4, "\ttest\n\t'}'test");
    indent("\ttest\n    \\}}", "", "test", false, 4, "\ttest\n\\}}test");
    indent("\ttest\n    \"}\"}", "", "test", false, 4, "\ttest\n\"}\"}test");
    indent("\ttest\n    '}'}", "", "test", false, 4, "\ttest\n'}'}test");
    indent("\ttest\n\ntest", "", "test", false, 4, "\ttest\n\n\ttesttest");
}

TEST(indent, test_has_selection) {
    indent("", "test", "", true, 4, "test");
    indent("", "{", "", true, 4, "{");
    indent("", "}", "", true, 4, "}");
    indent("", "{\ntest\n}", "", true, 4, "{\n    test\n}");
    indent("", "{\ntest\n}{\ntest\n}", "", true, 4, "{\n    test\n}{\n    test\n}");
    indent("    test\n", "test", "", true, 4, "    test\n    test");
    indent("    test\n", "{", "", true, 4, "    test\n    {");
    indent("    test\n", "}", "", true, 4, "    test\n}");
    indent("    test\n", "{\ntest\n}", "", true, 4, "    test\n    {\n        test\n    }");
    indent("    test\n", "{\ntest\n}{\ntest\n}", "", true, 4, "    test\n    {\n        test\n    }{\n        test\n    }");
    indent("", "test", "\ntest", true, 4, "test\ntest");
    indent("", "{", "\ntest", true, 4, "{\ntest");
    indent("", "}", "\ntest", true, 4, "}\ntest");
    indent("", "{\ntest\n}", "\ntest", true, 4, "{\n    test\n}\ntest");
    indent("", "{\ntest\n}{\ntest\n}", "\ntest", true, 4, "{\n    test\n}{\n    test\n}\ntest");
    indent("", "test", "", false, 4, "test");
    indent("", "{", "", false, 4, "{");
    indent("", "}", "", false, 4, "}");
    indent("", "{\ntest\n}", "", false, 4, "{\n\ttest\n}");
    indent("", "{\ntest\n}{\ntest\n}", "", false, 4, "{\n\ttest\n}{\n\ttest\n}");
    indent("    test\n", "test", "", false, 4, "    test\n\ttest");
    indent("    test\n", "{", "", false, 4, "    test\n\t{");
    indent("    test\n", "}", "", false, 4, "    test\n}");
    indent("    test\n", "{\ntest\n}", "", false, 4, "    test\n\t{\n\t\ttest\n\t}");
    indent("    test\n", "{\ntest\n}{\ntest\n}", "", false, 4, "    test\n\t{\n\t\ttest\n\t}{\n\t\ttest\n\t}");
    indent("", "test", "\ntest", false, 4, "test\ntest");
    indent("", "{", "\ntest", false, 4, "{\ntest");
    indent("", "}", "\ntest", false, 4, "}\ntest");
    indent("", "{\ntest\n}", "\ntest", false, 4, "{\n\ttest\n}\ntest");
    indent("", "{\ntest\n}{\ntest\n}", "\ntest", false, 4, "{\n\ttest\n}{\n\ttest\n}\ntest");
}
