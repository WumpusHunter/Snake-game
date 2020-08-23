// Snake game's window

//------------------------------------------------------------------------------

#pragma once
#include "GraphicsLib/Window.h"
#include "GraphicsLib/GUI.h"
#include "GraphicsLib/Graph.h"
#include "Game_graph.h"

//------------------------------------------------------------------------------

namespace Graph_lib {

    //------------------------------------------------------------------------------

    // Invariant: w > 0, h > 0
    class Snake_window : public Window {        // Game window
    public:
        // Construction
        Snake_window(Point xy, int w, int h, const string& lab);

    private:
        // Callback functions
        int handle(int event) override;
        static void cb_game_loop(Address pw);
        static void cb_pause(Address, Address pw);
        static void cb_new_game(Address, Address pw);
        static void cb_quit(Address, Address pw);
        static void cb_game(Address, Address pw);
        static void cb_help(Address, Address pw);

        // Action functions
        void start();
        void game_loop();
        bool is_pause();
        void pause();
        void new_game();
        void quit();
        void game();
        void help();
        int current_score();
        void put_score();
        void show_graphics();
        void hide_graphics();

    private:
        // Graphics
        Grid field;
        Snake snake;
        Rectangle fruit;
        // GUI
        Menu game_menu;
        Button game_button;
        Button new_game_button;
        Button pause_button;
        Button quit_button;
        Button help_button;
        Text_box help_box;
        Out_box score_box;
        Out_box max_score_box;
    };

    //------------------------------------------------------------------------------

}	// End of Graph_lib namespace

//------------------------------------------------------------------------------