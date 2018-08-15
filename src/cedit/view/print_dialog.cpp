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

#include "view/print_dialog.h"
#include <iostream>

namespace view {
    void print_dialog::print(Gtk::Window& window, const Gsv::View& view) {
        const Glib::RefPtr<Gsv::PrintCompositor> compositor = Gsv::PrintCompositor::create(view);
        
        signal_begin_print().connect(sigc::bind<const Glib::RefPtr<Gsv::PrintCompositor>&>(sigc::mem_fun(*this, &print_dialog::on_begin_print), compositor));
        signal_draw_page().connect(sigc::bind<const Glib::RefPtr<Gsv::PrintCompositor>&>(sigc::mem_fun(*this, &print_dialog::on_draw_page), compositor));
        
        run(Gtk::PrintOperationAction::PRINT_OPERATION_ACTION_PRINT_DIALOG, window);
    }
    
    void print_dialog::on_begin_print(const Glib::RefPtr<Gtk::PrintContext>& context, const Glib::RefPtr<Gsv::PrintCompositor>& compositor) {
        while (!compositor->paginate(context)) ;
        set_n_pages(compositor->get_n_pages());
    }
    
    void print_dialog::on_draw_page(const Glib::RefPtr<Gtk::PrintContext>& context, int page_nr, const Glib::RefPtr<Gsv::PrintCompositor>& compositor) {
        compositor->draw_page(context, page_nr);
    }
}
