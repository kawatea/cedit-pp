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

#include <functional>
#include <gtkmm/cssprovider.h>
#include <gtkmm/stylecontext.h>
#include <gtkmm/textiter.h>
#include <gtkmm/textmark.h>
#include "io/gettext.h"
#include "io/setting.h"
#include "view/search_bar.h"

namespace view {
    search_bar::search_bar(const Glib::RefPtr<Gsv::Buffer>& buffer, Gsv::View& view) : Gtk::Box(Gtk::ORIENTATION_VERTICAL), buffer(buffer), case_button(_("Case Sensitive")), control_button(_("Control Characters")), interactive_button(_("Interactive")) {
        on_scroll = [&view](Gtk::TextIter& iter) { view.scroll_to(iter); };
        on_close = [this, &view] { hide(); view.grab_focus(); };
        
        match_tag = buffer->create_tag("search_match");
        match_tag->property_background().set_value("#FFFF00");
        
        const io::setting& setting = io::setting::get();
        case_button.set_active(setting.get_boolean(io::setting::SEARCH_CASE_SENSITIVE));
        control_button.set_active(setting.get_boolean(io::setting::SEARCH_CONTROL_CHARACTERS));
        interactive_button.set_active(setting.get_boolean(io::setting::SEARCH_INTERACTIVE));
        
        const Glib::RefPtr<Gtk::CssProvider> provider = Gtk::CssProvider::create();
        provider->load_from_data(".unmatch { background-color: #FFB6C1; }");
        find_entry.get_style_context()->add_provider(provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        
        find_entry.set_icon_from_icon_name("edit-find");
        replace_entry.set_icon_from_icon_name("edit-find-replace");
        
        find_box.pack_start(find_entry);
        find_box.pack_start(case_button, Gtk::PACK_SHRINK);
        find_box.pack_start(control_button, Gtk::PACK_SHRINK);
        
        replace_box.pack_start(replace_entry);
        replace_box.pack_start(interactive_button, Gtk::PACK_SHRINK);
        
        pack_start(find_box, Gtk::PACK_SHRINK);
        pack_start(replace_box, Gtk::PACK_SHRINK);
        
        find_entry.signal_key_press_event().connect(sigc::bind<const bool>(sigc::mem_fun(*this, &search_bar::on_entry_key_press_event), true), false);
        replace_entry.signal_key_press_event().connect(sigc::bind<const bool>(sigc::mem_fun(*this, &search_bar::on_entry_key_press_event), false), false);
        find_entry.signal_changed().connect(sigc::mem_fun(*this, &search_bar::on_find_text_change));
        case_button.signal_clicked().connect(sigc::mem_fun(*this, &search_bar::on_case_button_click));
        control_button.signal_clicked().connect(sigc::mem_fun(*this, &search_bar::on_control_button_click));
        interactive_button.signal_clicked().connect(sigc::mem_fun(*this, &search_bar::on_interactive_button_click));
    }
    
    void search_bar::open_find() {
        open(true);
    }
    
    void search_bar::open_replace() {
        open(false);
    }
    
    void search_bar::open(const bool on_find) {
        this->on_find = on_find;
        show_all();
        if (on_find) replace_box.hide();
        on_find_text_change();
        find_entry.grab_focus();
    }
    
    void search_bar::remove_tags() {
        if (!in_process) buffer->remove_tag(match_tag, buffer->begin(), buffer->end());
    }
    
    bool search_bar::has_focus() const {
        return find_entry.has_focus() || replace_entry.has_focus();
    }
    
    bool search_bar::on_key_press_event(GdkEventKey* event) {
        switch (event->keyval) {
            case GDK_KEY_Escape: on_close(); return true;
            default: return false;
        }
    }
    
    bool search_bar::on_entry_key_press_event(GdkEventKey* event, const bool find) {
        switch (event->keyval) {
            case GDK_KEY_Return: search(event->state != GDK_SHIFT_MASK, false); return true;
            case GDK_KEY_Up: switch_entry(false); return true;
            case GDK_KEY_Down: (!on_find && !find && interactive_button.get_active()) ? search(true, true) : switch_entry(true); return true;
            case GDK_KEY_Tab:
            case GDK_KEY_ISO_Left_Tab: switch_entry(find); return true;
            case GDK_KEY_Escape: on_close(); return true;
            default: return false;
        }
    }
    
    void search_bar::on_edit_paste() {
        if (find_entry.has_focus()) {
            find_entry.paste_clipboard();
        } else {
            replace_entry.paste_clipboard();
        }
    }
    
    void search_bar::on_edit_select_all() {
        if (find_entry.has_focus()) {
            find_entry.select_region(0, -1);
        } else {
            replace_entry.select_region(0, -1);
        }
    }
    
    void search_bar::on_find_text_change() {
        remove_tags();
        find_entry.get_style_context()->remove_class("unmatch");
    }
    
    void search_bar::on_case_button_click() {
        const bool active = case_button.get_active();
        io::setting::get().set_boolean(io::setting::SEARCH_CASE_SENSITIVE, active);
        remove_tags();
    }
    
    void search_bar::on_control_button_click() {
        const bool active = control_button.get_active();
        io::setting::get().set_boolean(io::setting::SEARCH_CONTROL_CHARACTERS, active);
        remove_tags();
    }
    
    void search_bar::on_interactive_button_click() {
        const bool active = interactive_button.get_active();
        io::setting::get().set_boolean(io::setting::SEARCH_INTERACTIVE, active);
        remove_tags();
    }
    
    void search_bar::switch_entry(const bool find) {
        if (on_find) return;
        (find ? replace_entry : find_entry).grab_focus();
    }
    
    Glib::ustring search_bar::convert_control_characters(const Glib::ustring& old_text) {
        if (!control_button.get_active()) return old_text;
        Glib::ustring new_text;
        for (int i = 0; i < old_text.size(); i++) {
            if (old_text[i] != '\\' || i + 1 == old_text.size()) {
                new_text += old_text[i];
            } else {
                switch (old_text[++i]) {
                    case 'a': new_text += '\a'; break;
                    case 'b': new_text += '\b'; break;
                    case 't': new_text += '\t'; break;
                    case 'n': new_text += '\n'; break;
                    case 'v': new_text += '\v'; break;
                    case 'f': new_text += '\f'; break;
                    case 'r': new_text += '\r'; break;
                    case 'e': new_text += '\e'; break;
                    case '0': new_text += '\0'; break;
                    case '\\': new_text += '\\'; break;
                    default: new_text += '\\'; i--;
                }
            }
        }
        return new_text;
    }
    
    void search_bar::search(const bool forward, const bool skip) {
        if (find_entry.get_text_length() == 0 || (!forward && !on_find)) return;
        
        bool match;
        in_process = true;
        if (on_find) {
            const Glib::ustring text = convert_control_characters(find_entry.get_text());
            match = find(text, forward, true);
        } else {
            const Glib::ustring old_text = convert_control_characters(find_entry.get_text());
            const Glib::ustring new_text = convert_control_characters(replace_entry.get_text());
            if (interactive_button.get_active()) {
                match = replace_interactive(old_text, new_text, skip);
            } else {
                match = replace(old_text, new_text);
            }
        }
        in_process = false;
        
        if (match) {
            find_entry.get_style_context()->remove_class("unmatch");
        } else {
            find_entry.get_style_context()->add_class("unmatch");
        }
    }
    
    bool search_bar::find(const Glib::ustring& text, const bool forward, const bool wrap) {
        const Gtk::TextIter now = buffer->get_iter_at_mark(buffer->get_insert());
        const Gtk::TextSearchFlags flags = (case_button.get_active() ? Gtk::TextSearchFlags() : Gtk::TEXT_SEARCH_CASE_INSENSITIVE);
        Gtk::TextIter start, end;
        bool match;
        
        if (forward) {
            match = now.forward_search(text, flags, start, end);
        } else {
            match = now.backward_search(text, flags, start, end);
        }
        
        if (!match && wrap) {
            if (forward) {
                match = buffer->begin().forward_search(text, flags, start, end);
            } else {
                match = buffer->end().backward_search(text, flags, start, end);
            }
        }
        
        buffer->remove_tag(match_tag, buffer->begin(), buffer->end());
        if (match) {
            buffer->apply_tag(match_tag, start, end);
            buffer->place_cursor(forward ? end : start);
            on_scroll(start);
        }
        return match;
    }
    
    bool search_bar::replace(const Glib::ustring& old_text, const Glib::ustring& new_text) {
        Gtk::TextIter now = buffer->get_iter_at_mark(buffer->get_insert());
        const Gtk::TextSearchFlags flags = (case_button.get_active() ? Gtk::TextSearchFlags() : Gtk::TEXT_SEARCH_CASE_INSENSITIVE);
        bool match = false;
        
        buffer->remove_tag(match_tag, buffer->begin(), buffer->end());
        buffer->begin_user_action();
        while (true) {
            Gtk::TextIter start, end;
            if (!now.forward_search(old_text, flags, start, end)) break;
            now = buffer->erase(start, end);
            start = end = now = buffer->insert(now, new_text);
            start.backward_chars(new_text.size());
            buffer->apply_tag(match_tag, start, end);
            match = true;
        }
        buffer->end_user_action();
        return match;
    }
    
    bool search_bar::replace_interactive(const Glib::ustring& old_text, const Glib::ustring& new_text, const bool skip) {
        Gtk::TextIter now = buffer->get_iter_at_mark(buffer->get_insert());
        if (now.ends_tag(match_tag) && !skip) {
            Gtk::TextIter start, end;
            start = end = now;
            start.backward_chars(old_text.size());
            buffer->begin_user_action();
            now = buffer->erase(start, end);
            buffer->insert(now, new_text);
            buffer->end_user_action();
        }
        return find(old_text, true, false);
    }
}
