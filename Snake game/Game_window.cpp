// Snake game's window

//------------------------------------------------------------------------------

#include "Game_window.h"

//------------------------------------------------------------------------------

namespace Graph_lib {

	//------------------------------------------------------------------------------

    // Min possible size of window
    constexpr int min_w = 400;      // Window's min width
    constexpr int min_h = 300;      // Window's min height
    // Size of cells
    constexpr int cell_w = 50;      // Cell's width
    constexpr int cell_h = 50;      // Cell's height
    // Default parameters of snake
    constexpr int snake_sz = 3;     // Snake's length
    // Default location of graphics
    Point snake_xy = { 0, 0 };      // Snake's location
    Point fruit_xy = { 0, 0 };      // Fruit's location
    // Size of widgets
    constexpr int widget_h = 25;    // Widgets' height
    constexpr int out_box_w = 30;   // Output boxes' width
    constexpr int button_w = 100;   // Buttons' width
    // Indexes of game menu's buttons
    constexpr int new_game_ind = 0; // New game button's index
    constexpr int pause_ind = 1;    // Pause button's index
    constexpr int quit_ind = 2;     // Quit button's index
    // Default parameters for widgets
    const string help_text = R"( SNAKE GAME
 Snake is a video game concept where the player maneuvers a line
that grows in length, with the line itself being a primary obstacle.
The concept originated in the 1976 arcade game Blockade.
 GAMEPLAY
The player controls an object on a bordered plane. As it moves for-
ward, it leaves a trail behind, resembling a moving snake. The snake
has a specific length, so there is a moving tail a fixed number of units
away from the head. The player loses when the snake runs into the
screen border or itself.
 A sole player attempts to eat items by running into them with the he-
ad of the snake. Each item eaten makes the snake longer, so con-
trolling is progressively more difficult.
 CONTROL
 The snake moves forward automatically, everything you need to do
is to choose the direction of moving. To choose the direction of mov-
ing use arrow-buttons, that is,
1) Left-arrow - to move in the left direction;
2) Up-arrow - to move in the up direction;
3) Right-arrow - to move in the right direction;
4) Down-arrow - to move in the down direction.
Remember: you can't rotate the snake's head to the opposite direc-
tion, for instance, from the left to the right, or from the up to the
down.
 ADDITIONAL NOTES
 Good luck on the game, try to eat as much as you can!)";

    // Constructs window with top-left angle at xy, of size w * h (if
    // it's not less than min, which is 400 * 300), labeled with lab
    Snake_window::Snake_window(Point xy, int w, int h, const string& lab)
        : Window{ xy, w >= min_w ? w - w % cell_w : min_w, h >= min_h ? h - h % cell_h : min_h, lab },
        field{ Point{ 0, cell_h }, cell_w, cell_h, x_max() / cell_w, (y_max() - cell_h) / cell_h },
        snake{ Point{ snake_sz * cell_w, y_max() / 2 }, cell_w, cell_h, snake_sz },
        fruit{ Point{ x_max() - cell_w * 2, y_max() / 2 }, cell_w, cell_h },
        game_menu{ Point{ 0, 0 }, button_w, widget_h, Menu::Kind::horizontal, "Game" },
        game_button{ Point{ 0, 0 }, button_w, widget_h, "&Game", cb_game },
        new_game_button{ Point{ 0, 0 }, 0, 0, "&New game", cb_new_game },
        pause_button{ Point{ 0, 0 }, 0, 0, "&Pause", cb_pause },
        quit_button{ Point{ 0, 0 }, 0, 0, "&Quit", cb_quit },
        help_button{ Point{ button_w, 0 }, button_w, widget_h, "&Help", cb_help },
        help_box{ Point{ 0, cell_h }, x_max(), y_max() - cell_h, "" },
        score_box{ Point{ cell_w * 2, widget_h }, out_box_w, widget_h, "Current score: " },
        max_score_box{ Point{ cell_w * 4 + out_box_w, widget_h }, out_box_w, widget_h, "Max score: " }
    {
        if (w <= 0 || h <= 0)       // Error handling
            throw invalid_argument("Bad Snake_window: non-positive size");
        // Keep default location of graphics
        snake_xy = snake.point(0);
        fruit_xy = fruit.point(0);
        // Attach graphics to window
        attach(field);
        attach(snake);
        attach(fruit);
        // Attach widgets to window
        game_menu.attach(new_game_button);
        game_menu.attach(pause_button);
        game_menu.attach(quit_button);
        attach(game_menu);
        attach(game_button);
        attach(help_button);
        attach(help_box);
        attach(score_box);
        attach(max_score_box);
        // Default value for graphics
        show_graphics();
        put_on_top(snake);
        // Default value for widgets
        game_menu.hide();
        help_box.put(help_text);
        help_box.hide();
        score_box.put(0);
        max_score_box.put(0);
    }

    // Handles passed to window event, for instance, pressed key
    int Snake_window::handle(int event)
    {
        switch (event) {
        case FL_FOCUS: case FL_UNFOCUS:     // Focuses are skipped (required by system)
            return 1;
        case FL_KEYBOARD: {                 // Keys, pressed using keyboard
            switch (Fl::event_key()) {
                // Arrow-keys used to change snake's direction
            case FL_Left:                   // Left-arrow
                snake.set_direction(Snake::Direction::left);
                cout << "Changed direction to the left (" << static_cast<int>(snake.direction()) << ")\n";
                return 1;
            case FL_Up:                     // Up-arrow
                snake.set_direction(Snake::Direction::up);
                cout << "Changed direction to the up (" << static_cast<int>(snake.direction()) << ")\n";
                return 1;
            case FL_Right:                  // Right-arrow
                snake.set_direction(Snake::Direction::right);
                cout << "Changed direction to the right (" << static_cast<int>(snake.direction()) << ")\n";
                return 1;
            case FL_Down:                   // Down-arrow
                snake.set_direction(Snake::Direction::down);
                cout << "Changed direction to the down (" << static_cast<int>(snake.direction()) << ")\n";
                return 1;
            }
        }
        }
        return Window::handle(event);       // Everything else is handled by base window
    }

    // Callback function for game_loop
    void Snake_window::cb_game_loop(Address pw)
    {
        constexpr double delay = 0.25;                          // Delay of game's loop
        Snake_window& self = reference_to<Snake_window>(pw);    // Self-reference
        self.game_loop();                                       // Call of action function
        Fl::repeat_timeout(delay, cb_game_loop, pw);            // Execute delay of game's loop
    }

    // Callback function for pause
    void Snake_window::cb_pause(Address, Address pw)
    {
        Snake_window& self = reference_to<Snake_window>(pw);    // Self-reference
        self.pause();
        self.game();
    }

    // Callback function for new game
    void Snake_window::cb_new_game(Address, Address pw)
    {
        Snake_window& self = reference_to<Snake_window>(pw);    // Self-reference
        self.new_game();
        self.game();
    }

    // Callback function for quit
    void Snake_window::cb_quit(Address, Address pw)
    {
        Snake_window& self = reference_to<Snake_window>(pw);    // Self-reference
        self.quit();
        self.game();
    }

    // Callback function for game
    void Snake_window::cb_game(Address, Address pw)
    {
        Snake_window& self = reference_to<Snake_window>(pw);    // Self-reference
        self.game();
    }

    // Callback function for help
    void Snake_window::cb_help(Address, Address pw)
    {
        Snake_window& self = reference_to<Snake_window>(pw);    // Self-reference
        self.help();
    }

    // Starts game's loop
    void Snake_window::start()
    {
        constexpr double delay = 1.0;                   // Delay before first timeout
        Fl::add_timeout(delay, cb_game_loop, this);     // Start game's loop and delay proccess
        cout << "Started the game\n";
    }

    // Starts all proccesses of game's loop
    void Snake_window::game_loop()
    {
        // Snake's bumping (obstacle is snake's body or field's borders)
        const Snake& const_snake = snake;
        if (snake.is_body_except_head(const_snake.body_head())) {   // Snake's body as obstacle
            cout << "Bumped into the snake's body\n";
            // Pause after losed game
            return Fl::add_timeout(0.0, [](Address pw) { cb_pause(nullptr, pw); }, this);;
        }
        if (!is_grid(field, const_snake.body_head())) {             // Grid's border as obstacle
            cout << "Bumped into the grid's border\n";
            // Pause after losed game
            return Fl::add_timeout(0.0, [](Address pw) { cb_pause(nullptr, pw); }, this);
        }
        // Snake's eating
        if (snake.point(0) == fruit.point(0)) {
            snake.grow_length();
            put_score();                // Update score after eating
            cout << "Ate the fruit; the length becomes equal to " << snake.length() << '\n';
            // Randomly change location of fruit to everywhere, except snake's body
            while (snake.is_body(fruit))
                random_move(fruit, field.point(0), field.width() - fruit.width(), field.height() - fruit.height());
        }
        else snake.move_forward();      // Snake's moving
        cout << "Moved to (" << snake.point(0).x << ", " << snake.point(0).y << ")\n";
        redraw();                       // Redraw window after made changes
    }

    // Determines either game is paused or not
    bool Snake_window::is_pause()
    {
        return Fl::has_timeout(cb_game_loop, this) ? false : true;
    }

    // Pauses game if it's plays, or starts if it's already
    // paused, that is, pause prevents snake's moves
    void Snake_window::pause()
    {
        if (!is_pause()) {
            Fl::remove_timeout(cb_game_loop, this);     // Stop timeout
            cout << "Paused the game\n";
        }
        else start();                                   // Start timeout
    }

    // Starts new game, that is, returns everything to initial state
    void Snake_window::new_game()
    {
        if (!is_pause()) pause();                   // Pause game
        snake.shrink_length(current_score());       // Shrink length to default length
        // Return graphics to default location
        snake.set_direction(Snake::Direction::up);
        snake.set_direction(Snake::Direction::right);
        for (int i = 0; i < snake_sz; ++i)
            snake.move_forward();
        snake.move(-snake.point(0).x, -snake.point(0).y);       // Top-left angle of window
        snake.move(snake_xy.x, snake_xy.y);
        fruit.move(-fruit.point(0).x, -fruit.point(0).y);       // Top-left angle of window
        fruit.move(fruit_xy.x, fruit_xy.y);
        cout << "Started the new game; shrank the length to " << snake.length() << '\n';
        put_score();                                // Update score after shrinking
        redraw();                                   // Redraw window after made changes
    }

    // Quits game, that is, closes window
    void Snake_window::quit()
    {
        Window::hide();         // Hide window to close it
        cout << "Quited the game\n";
    }

    // Hides game button and shows game menu, if game button is pressed,
    // or shows game button and hides game menu, if game menu is pressed
    void Snake_window::game()
    {
        // Hide game button and show game menu
        if (game_button.visible()) {        // Game button is pressed
            game_button.hide();
            game_menu.show();
            help_button.move(game_menu.selection.size() * game_menu.width - help_button.width, 0);
            cout << "Hid the game button and showed the game menu\n";
        }
        // Hide game menu and show game button
        else {                              // Game menu is pressed
            game_menu.hide();
            game_button.show();
            help_button.move(help_button.width - game_menu.selection.size() * game_menu.width, 0);
            cout << "Hid the game menu and showed the game button\n";
        }
    }

    // Shows help box if it's invisible, or hides it if it's visible
    void Snake_window::help()
    {
        // Show help box
        if (!help_box.visible()) {      // Help box is invisible
            if (!is_pause()) pause();   // Pause game
            game_menu.selection[pause_ind].deactivate();
            hide_graphics();
            help_box.show();
            cout << "Showed the help box\n";
        }
        // Hide help box
        else {                          // Help box is visible
            game_menu.selection[pause_ind].activate();
            help_box.hide();
            show_graphics();
            cout << "Hid the help box\n";
        }
    }

    // Determines current score
    int Snake_window::current_score()
    {
        return snake.length() - snake_sz;
    }

    // Writes current score and max score into score boxes, if required
    void Snake_window::put_score()
    {
        int score = current_score();
        score_box.put(score);                   // Write current score
        if (score > max_score_box.get_int()) {  // New record
            max_score_box.put(score);           // Write max score
            cout << "Updated the max score to " << score << '\n';
        }
        cout << "Updated the current score to " << score << '\n';
    }

    // Shows game's graphics, that is, makes field, snake, and fruit visible
    void Snake_window::show_graphics()
    {
        // Modify color parameters of graphics
        field.set_color(Color::black);
        field.set_fill_color(Color::dark_green);
        snake.set_color(Color::black);
        snake.set_fill_color(Color::dark_yellow);
        snake.head_set_fill_color(Color::yellow);
        fruit.set_color(Color::black);
        fruit.set_fill_color(Color::red);
        cout << "Showed the graphics\n";
    }

    // Hides game's graphics, that is, makes field, snake, and fruit invisible
    void Snake_window::hide_graphics()
    {
        // Modify color parameters of graphics
        field.set_color(Color::invisible);
        field.set_fill_color(Color::invisible);
        snake.set_color(Color::invisible);
        snake.set_fill_color(Color::invisible);
        snake.head_set_fill_color(Color::invisible);
        fruit.set_color(Color::invisible);
        fruit.set_fill_color(Color::invisible);
        cout << "Hid the graphics\n";
    }

	//------------------------------------------------------------------------------

}	// End of Graph_lib namespace

//------------------------------------------------------------------------------