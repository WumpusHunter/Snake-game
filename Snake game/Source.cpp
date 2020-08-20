/*
    Snake game

    Revision history:
        Written by Oleg Kovalevskiy in August 2020
*/

//------------------------------------------------------------------------------------

#include "Game_window.h"
using namespace Graph_lib;

//------------------------------------------------------------------------------------

int main()
try {
    // Window with top-left angle at (100, 100), of size 600 * 400, labeled "Snake game"
    Snake_window win{ Point{ 100, 100 }, 600, 400, "Snake game" };
    
    return gui_main();
}
catch (const exception& e) {
    cerr << "Error message: " << e.what() << '\n';
    return 1;
}
catch (...) {
    cerr << "Unknown error\n";
    return 1;
}

//------------------------------------------------------------------------------------