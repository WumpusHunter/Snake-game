//------------------------------------------------------------------------------

#include "GUI.h"
#include "std_lib_facilities.h"
#include <sstream>

//using namespace Graph_lib;

//------------------------------------------------------------------------------

namespace Graph_lib {

	//------------------------------------------------------------------------------

	void Button::attach(Graph_lib::Window& win)
	{
		pw = new Fl_Button(loc.x, loc.y, width, height, label.c_str());
		pw->callback(reinterpret_cast<Fl_Callback*>(do_it), &win); // pass the window
		own = &win;
	}

	//------------------------------------------------------------------------------

	int In_box::get_int()
	{
		Fl_Input& pi = reference_to<Fl_Input>(pw);
		//	return atoi(pi.value());
		const char* p = pi.value();
		if (!isdigit(p[0])) return -999999;
		return atoi(p);
	}

	string In_box::get_string()
	{
		Fl_Input& pi = reference_to<Fl_Input>(pw);
		return string(pi.value());
	}

	void In_box::attach(Graph_lib::Window& win)
	{
		pw = new Fl_Input(loc.x, loc.y, width, height, label.c_str());
		own = &win;
	}

	//------------------------------------------------------------------------------

	void Out_box::put(int i)
	{
		Fl_Output& po = reference_to<Fl_Output>(pw);
		std::stringstream ss;
		ss << i;
		po.value(ss.str().c_str());
	}

	void Out_box::put(const string& s)
	{
		reference_to<Fl_Output>(pw).value(s.c_str());
	}

	int Out_box::get_int() const
	{
		constexpr int fail = -999999;		// Key for fail
		auto val = reference_to<Fl_Output>(pw).value();
		return isdigit(val[0]) ? atoi(val) : fail;
	}

	string Out_box::get_string() const
	{
		return string{ reference_to<Fl_Output>(pw).value() };
	}

	void Out_box::attach(Graph_lib::Window& win)
	{
		pw = new Fl_Output(loc.x, loc.y, width, height, label.c_str());
		own = &win;
	}

	//------------------------------------------------------------------------------

	/*
	Menu::Menu(Point xy, int w, int h, Kind kk, const string& s)
	:Widget(xy,w,h,s,0), k(kk), offset(0)
	{
	}
	*/

	int Menu::attach(Button& b)
	{
		b.width = width;
		b.height = height;

		switch (k) {
		case horizontal:
			b.loc = Point(loc.x + offset, loc.y);
			offset += b.width;
			break;
		case vertical:
			b.loc = Point(loc.x, loc.y + offset);
			offset += b.height;
			break;
		}
		selection.push_back(&b);
		return int(selection.size() - 1);
	}

	int Menu::attach(Button* p)
	{
		//	owned.push_back(p);
		return attach(*p);
	}

	//------------------------------------------------------------------------------

	// Constructs text box with top-left angle at xy, of size w * h, and labeled with lab
	Text_box::Text_box(Point xy, int w, int h, const string& lab)
		: Widget{ xy, w, h, lab, nullptr }
	{
		if (w <= 0 || h <= 0)		// Error handling
			throw invalid_argument("Bad Text_box: non-positive size");
	}

	// Puts text into text box's buffer; requires text box to be attached to window
	void Text_box::put(const string& str)
	{
		if (pw == nullptr)		// Error handling
			throw invalid_argument("Bad Text_box: can't put text into unattached box");
		Fl_Text_Display& self = reference_to<Fl_Text_Display>(pw);		// Self-reference
		self.buffer()->text(str.c_str());			// Put Text into text box's buffer
	}

	// Gets text from text box's buffer; requires text box to be attached to window
	string Text_box::get() const
	{
		if (pw == nullptr)		// Error handling
			throw invalid_argument("Bad Text_box: can't put text into unattached box");
		Fl_Text_Display& self = reference_to<Fl_Text_Display>(pw);		// Self-reference
		return string{ self.buffer()->text() };		// Get text from text box's buffer
	}

	// Attaches text box to window
	void Text_box::attach(Graph_lib::Window& win)
	{
		// Create text box with top-left angle at loc, of size width * height, labeled label
		pw = new Fl_Text_Display(loc.x, loc.y, width, height, label.c_str());
		Fl_Text_Display& self = reference_to<Fl_Text_Display>(pw);		// Self-reference
		own = &win;
		self.buffer(new Fl_Text_Buffer());			// Create buffer for text box
	}

	//------------------------------------------------------------------------------

}	// of namespace Graph_lib

//------------------------------------------------------------------------------