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

#include <memory>
#include <sstream>
#include <stdexcept>
#include <glibmm/error.h>
#include <glibmm/spawn.h>
#include <gtkmm/textiter.h>
#include "function/flymake.h"
#include "io/setting.h"

#ifdef _WIN32
#include <windows.h>
#endif

namespace function {
    void flymake::apply(const Glib::RefPtr<Gsv::Buffer>& buffer, const std::string& filename, const Glib::ustring& language) {
        for (const std::pair<Glib::RefPtr<Gtk::TextTag>, std::string>& tag : tags) buffer->remove_tag(tag.first, buffer->begin(), buffer->end());
        tags.clear();
        
        std::istringstream stream(read_output(filename, language));
        std::string line;
        while (std::getline(stream, line)) {
            const std::string::size_type pos = line.find(':');
            if (pos == std::string::npos || line.substr(0, pos) != filename) continue;
            line = line.substr(pos + 1);
            try {
                const int line_num = std::stoi(line) - 1;
                line = line.substr(line.find(' ') + 1);
                const std::string::size_type pos = line.find(':');
                const std::string type = line.substr(0, pos);
                const std::string message = line.substr(pos + 2);
                add_tag(buffer, line_num, type, message);
            } catch (const std::invalid_argument& e) {
                continue;
            }
        }
    }
    
    bool flymake::show_tooltip(const Gtk::TextIter& iter, const Glib::RefPtr<Gtk::Tooltip>& tooltip) {
        std::string message = "";
        for (const std::pair<Glib::RefPtr<Gtk::TextTag>, std::string>& tag : tags) {
            if (iter.has_tag(tag.first)) {
                if (!message.empty()) message += "\n";
                message += tag.second;
            }
        }
        if (message.empty()) {
            return false;
        } else {
            tooltip->set_text(message);
            return true;
        }
    }
    
    std::string flymake::read_output(const std::string& filename, const Glib::ustring& language) {
        const Glib::ustring command = io::setting::get().get_string(language, io::setting::FLYMAKE_COMMAND);
        if (command.empty()) return "";
        
        const std::string actual_command = command + " " + filename;
        std::string output = "";
        
        #ifdef _WIN32
        HANDLE read, write;
        SECURITY_ATTRIBUTES security_attributes = {};
        security_attributes.nLength = sizeof(SECURITY_ATTRIBUTES);
        security_attributes.lpSecurityDescriptor = NULL;
        security_attributes.bInheritHandle = true;
        if (!CreatePipe(&read, &write, &security_attributes, 0)) return "";
        
        do {
            STARTUPINFO startup_info = {};
            PROCESS_INFORMATION process_info = {};
            startup_info.cb = sizeof(STARTUPINFO);
            startup_info.dwFlags = STARTF_USESTDHANDLES;
            startup_info.wShowWindow = SW_HIDE;
            startup_info.hStdError = write;
            if (!CreateProcess(NULL, const_cast<char*>(actual_command.c_str()), NULL, NULL, true, CREATE_NO_WINDOW, NULL, NULL, &startup_info, &process_info)) break;
            
            WaitForInputIdle(process_info.hProcess, INFINITE);
            WaitForSingleObject(process_info.hProcess, INFINITE);
            
            CloseHandle(process_info.hThread);
            CloseHandle(process_info.hProcess);
            
            DWORD bytes;
            if (!PeekNamedPipe(read, NULL, 0, NULL, &bytes, NULL)) break;
            if (bytes > 0) {
                std::unique_ptr<char[]> buffer = std::make_unique<char[]>(bytes + 1);
                ReadFile(read, buffer.get(), bytes, &bytes, NULL);
                output = std::string(buffer.get(), bytes);
            }
        } while (false);
        
        CloseHandle(read);
        CloseHandle(write);
        #else
        try {
            Glib::spawn_command_line_sync(actual_command, nullptr, &output, nullptr);
        } catch (const Glib::Error& e) {
            return "";
        }
        #endif
        
        return output;
    }
    
    void flymake::add_tag(const Glib::RefPtr<Gsv::Buffer>& buffer, const int line, const std::string& type, const std::string& message) {
        Gtk::TextIter start, end;
        start = end = buffer->get_iter_at_line(line);
        while (*start == ' ' || *start == '\t') start++;
        end.forward_line();
        
        const Glib::RefPtr<Gtk::TextTag> tag = buffer->create_tag();
        const Glib::ustring color = ((type == "fatal error" || type == "error") ? "#FFBBBB" : "#B0E0E6");
        tag->property_background().set_value(color);
        tags.emplace_back(tag, message);
        buffer->apply_tag(tag, start, end);
    }
}
