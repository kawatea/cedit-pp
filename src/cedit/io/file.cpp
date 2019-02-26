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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <glib/gstdio.h>
#include <glibmm/iochannel.h>
#include <glibmm/refptr.h>
#include <glibmm/ustring.h>
#include "io/encoding.h"
#include "io/file.h"
#include "io/gettext.h"

namespace io {
    static std::string to_simple_name(const std::string& name) {
        return name.substr(name.find_last_of("/\\") + 1);
    }
    
    file::file(const std::string& name, const encoding& encode) : full_name(name), simple_name(to_simple_name(name)), encode(encode) {}
    
    const std::string& file::get_full_name() const {
        return full_name;
    }
    
    const std::string& file::get_simple_name() const {
        return simple_name;
    }
    
    std::string file::get_title_name() const {
        std::string name = (edited ? "* " : "");
        name += (is_empty() ? _("Untitled") : simple_name);
        return name;
    }
    
    const encoding& file::get_encoding() const {
        return encode;
    }
    
    void file::set_encoding(const encoding& encode) {
        this->encode = encode;
    }
    
    bool file::is_edited() const {
        return edited;
    }
    
    void file::set_edited(const bool edited) {
        this->edited = edited;
    }
    
    bool file::is_empty() const {
        return full_name.empty();
    }
    
    bool file::exists() const {
        return g_access(full_name.c_str(), F_OK) == 0;
    }
    
    bool file::can_open() const {
        GStatBuf st;
        if (g_stat(full_name.c_str(), &st) != 0) {
            return !simple_name.empty();
        } else {
            return S_ISREG(st.st_mode);
        }
    }
    
    Glib::ustring file::read() const {
        const Glib::RefPtr<Glib::IOChannel> channel = Glib::IOChannel::create_from_file(full_name, "r");
        channel->set_encoding(encode.get_iconv_name());
        Glib::ustring text;
        channel->read_to_end(text);
        return text;
    }
    
    void file::write(const Glib::ustring& text) const {
        const Glib::RefPtr<Glib::IOChannel> channel = Glib::IOChannel::create_from_file(full_name, "w");
        channel->set_encoding(encode.get_iconv_name());
        channel->write(text);
    }
}
