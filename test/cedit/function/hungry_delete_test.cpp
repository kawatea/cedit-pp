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
#include <gtksourceviewmm/buffer.h>
#include <gtksourceviewmm/init.h>
#include "gtest/gtest.h"
#include "function/hungry_delete.h"

static void hungry_delete(const bool forward, const Glib::ustring& before, const Glib::ustring& after, const bool deleted, const Glib::ustring& expected) {
    Gsv::init();
    const Glib::RefPtr<Gsv::Buffer> buffer = Gsv::Buffer::create();
    buffer->set_text(before + after);
    buffer->place_cursor(buffer->get_iter_at_offset(before.size()));
    
    EXPECT_EQ(deleted, forward ? function::hungry_delete_forward(buffer) : function::hungry_delete_backward(buffer));
    EXPECT_EQ(expected, buffer->get_text());
}

TEST(hungry_delete, test_forward) {
    hungry_delete(true, "", "", false, "");
    hungry_delete(true, "", "test", false, "test");
    hungry_delete(true, "", "\ntest", false, "\ntest");
    hungry_delete(true, "", " ", true, "");
    hungry_delete(true, "", "  ", true, "");
    hungry_delete(true, "", " test", true, "test");
    hungry_delete(true, "", "  test", true, "test");
    hungry_delete(true, "", "\ttest", true, "test");
    hungry_delete(true, "", "\t\ttest", true, "test");
    hungry_delete(true, "", " \ttest", true, "test");
    hungry_delete(true, "", "\t test", true, "test");
    hungry_delete(true, "", " \n test", true, "\n test");
    hungry_delete(true, "test", "", false, "test");
    hungry_delete(true, "test", "test", false, "testtest");
    hungry_delete(true, "test", "\ntest", false, "test\ntest");
    hungry_delete(true, "test", " ", true, "test");
    hungry_delete(true, "test", "  ", true, "test");
    hungry_delete(true, "test", " test", true, "testtest");
    hungry_delete(true, "test", "  test", true, "testtest");
    hungry_delete(true, "test", "\ttest", true, "testtest");
    hungry_delete(true, "test", "\t\ttest", true, "testtest");
    hungry_delete(true, "test", " \ttest", true, "testtest");
    hungry_delete(true, "test", "\t test", true, "testtest");
    hungry_delete(true, "test", " \n test", true, "test\n test");
}

TEST(hungry_delete, test_backward) {
    hungry_delete(false, "", "", false, "");
    hungry_delete(false, "test", "", false, "test");
    hungry_delete(false, "test\n", "", false, "test\n");
    hungry_delete(false, " ", "", true, "");
    hungry_delete(false, "  ", "", true, "");
    hungry_delete(false, "test ", "", true, "test");
    hungry_delete(false, "test  ", "", true, "test");
    hungry_delete(false, "test\t", "", true, "test");
    hungry_delete(false, "test\t\t", "", true, "test");
    hungry_delete(false, "test\t ", "", true, "test");
    hungry_delete(false, "test \t", "", true, "test");
    hungry_delete(false, "test \n ", "", true, "test \n");
    hungry_delete(false, "", "test", false, "test");
    hungry_delete(false, "test", "test", false, "testtest");
    hungry_delete(false, "test\n", "test", false, "test\ntest");
    hungry_delete(false, " ", "test", true, "test");
    hungry_delete(false, "  ", "test", true, "test");
    hungry_delete(false, "test ", "test", true, "testtest");
    hungry_delete(false, "test  ", "test", true, "testtest");
    hungry_delete(false, "test\t", "test", true, "testtest");
    hungry_delete(false, "test\t\t", "test", true, "testtest");
    hungry_delete(false, "test\t ", "test", true, "testtest");
    hungry_delete(false, "test \t", "test", true, "testtest");
    hungry_delete(false, "test \n ", "test", true, "test \ntest");
}
