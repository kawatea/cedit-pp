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

#ifndef CEDIT_IO_ENCODING_H_
#define CEDIT_IO_ENCODING_H_

#include <string>
#include <vector>

namespace io {
    class encoding final {
        public:
        static const encoding NONE;
        static const encoding JIS;
        static const encoding SJIS;
        static const encoding EUC;
        static const encoding UTF8;
        static const encoding UTF16;
        static const encoding UTF16BE;
        static const encoding UTF16LE;
        
        bool operator==(const encoding& e) const;
        bool operator!=(const encoding& e) const;
        
        static const std::vector<encoding>& all_encodings();
        static encoding from_name(const std::string& name);
        
        const std::string& get_name() const;
        const std::string& get_iconv_name() const;
        
        private:
        static const std::vector<encoding> encodings;
        
        std::vector<std::string> names;
        std::string iconv_name;
        
        encoding(const std::vector<std::string>& names, const std::string& iconv_name);
    };
    
    encoding detect_encoding(const std::string& file_name);
}

#endif
