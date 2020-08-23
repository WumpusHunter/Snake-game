// Snake game's graphics

//------------------------------------------------------------------------------

#include <cassert>
#include "Game_graph.h"
#include "RandomNumber/Generator.h"

//------------------------------------------------------------------------------

namespace Graph_lib {

    //------------------------------------------------------------------------------

    // Indexes of snake's body
    constexpr int head_ind = 0;

    // Constructs snake with top left-angle of its head at xy, of sz
    // cells, and with size of each cell equal to cell_w * cell_h
    Snake::Snake(Point xy, int cell_w, int cell_h, int sz)
        : body{}, head{ Direction::right }
    {
        if (sz <= 0)        // Error handling
            throw invalid_argument("Bad Snake: non-positive length");
        // Fill of body
        for (int i = 0; i < sz; ++i)        // Horizontal line
            body.push_back(new Rectangle{ Point{ xy.x - i * cell_w, xy.y }, cell_w, cell_h });
        add(xy);            // Top-left angle of snake's head
    }

    // Draws snake and fills it with color, if required
    void Snake::draw_lines() const
    {
        // Draw each cell of body
        for (int i = 0; i < body.size(); ++i)
            body[i].draw();
    }

    // Moves snake by dx at x-coordinate and dy at y-coordinate 
    void Snake::move(int dx, int dy)
    {
        Shape::move(dx, dy);
        // Move each cell of body
        for (int i = 0; i < body.size(); ++i)
            body[i].move(dx, dy);
    }

    // Moves snake forward, that is, moves each cell from tail to head
    // to its next neighbour, and moves head one cell in its direction
    void Snake::move_forward()
    {
        // Move each cell from tail to head to its next neighbour
        for (int i = body.size() - 1; i > 0; --i) {
            body[i].move(-body[i].point(0).x, -body[i].point(0).y);         // Move to initial point
            body[i].move(body[i - 1].point(0).x, body[i - 1].point(0).y);   // Move to neigbhour's point
        }
        // Move head one cell in its direction
        switch (head) {
        case Direction::left:       // Left-side
            body[head_ind].move(-body[head_ind].width(), 0);
            break;
        case Direction::up:         // Up-side
            body[head_ind].move(0, -body[head_ind].height());
            break;
        case Direction::right:      // Right-side
            body[head_ind].move(body[head_ind].width(), 0);
            break;
        case Direction::down:       // Down-side
            body[head_ind].move(0, body[head_ind].height());
            break;
        }
        set_point(0, body[head_ind].point(0));         // Update location of snake's head
    }

    // Grows snake in length, that is, adds one cell to its tail
    void Snake::grow_length()
    {
        const Point tail = body[body.size() - 1].point(0);      // Tail's coordinate
        move_forward();
        // Add new cell into body at previous tail's location
        body.push_back(new Rectangle{ tail, body[head_ind].width(), body[head_ind].height() });
        // Set same parameters for new tail as for all body
        body[body.size() - 1].set_color(color());
        body[body.size() - 1].set_fill_color(fill_color());
        body[body.size() - 1].set_style(style());
    }

    // Shrinks snake in length, that is, removes num cells from its body, starting with tail
    void Snake::shrink_length(int num)
    {
        if (num >= body.size())     // Error handling
            throw invalid_argument("Bad Snake: can't shrink to non-positive length");
        constexpr bool own = true;  // Cells are owned by body
        // Remove num cells from snake's body
        for (int i = 0; i < num; ++i)
            body.pop_back(own);
    }

    // Sets c as color of snake's lines
    void Snake::set_color(Color c)
    {
        Shape::set_color(c);
        // Set c as color of lines to each cell of body
        for (int i = 0; i < body.size(); ++i)
            body[i].set_color(c);
    }

    // Sets c as fill color of snake's body
    void Snake::set_fill_color(Color c)
    {
        Shape::set_fill_color(c);
        // Set c as fill color to each cell of body
        for (int i = 0; i < body.size(); ++i)
            body[i].set_fill_color(c);
    }

    // Sets c as fill color of snake's head
    void Snake::head_set_fill_color(Color c)
    {
        assert(body.begin() != body.end()       // Error handling
            && "Bad Snake: can't set fill color to head of empty snake");
        body[head_ind].set_fill_color(c);
    }

    // Sets ls as line style of snake's body
    void Snake::set_style(Line_style ls)
    {
        Shape::set_style(ls);
        // Set ls as line style to each cell of body
        for (int i = 0; i < body.size(); ++i)
            body[i].set_style(ls);
    }

    // Sets d as direction of snake's head
    void Snake::set_direction(Direction d)
    {
        constexpr int opposite_diff = 2;     // Module of opposite direction's difference
        // Difference of directions
        const int diff = abs(static_cast<int>(head) - static_cast<int>(d));
        if (diff != opposite_diff)           // Set direction if it's not opposite
            head = d;
    }

    // Gets snake's head
    const Rectangle& Snake::body_head() const
    {
        assert(body.cbegin() != body.cend()     // Error handling
            && "Bad Snake: can't get head of empty snake");
        return body[head_ind];
    }

    // Determines either cell is one of snake's body's cells
    bool Snake::is_body(const Rectangle& cell) const
    {
        // Search for cell in snake's body, located same as cell, and compare parameters
        return find_if(body.cbegin(), body.cend(), [&cell](const Rectangle* rect)
            { return rect->point(0) == cell.point(0); }) != body.cend()
            && body[0].width() == cell.width() && body[0].height() == cell.height();
    }

    // Determines either cell is one of snake's body's cells, except its head
    bool Snake::is_body_except_head(const Rectangle& cell) const
    {
        // Search for cell in snake's body, located same as cell, except snake's head, and compare parameters
        return body.cbegin() != body.cend() ? find_if(next(body.cbegin()), body.cend(),
            [&cell](const Rectangle* rect) { return rect->point(0) == cell.point(0); }) != body.cend()
            && body[0].width() == cell.width() && body[0].height() == cell.height() : false;
    }

    //------------------------------------------------------------------------------

    // Moves rect randomly in range [xy.x; xy.x + w] for x-coordinate and [xy.y; xy.y + h] for
    // y-coordinate, with xy as original point, w as width of range and h as height of range
    void random_move(Rectangle& rect, Point xy, int w, int h)
    {
        if (w < 0 || h < 0)     // Error handling
            throw invalid_argument("Bad random_move: invalid range for coordinates");
        // Move to original location, that is, xy
        rect.move(-(rect.point(0).x - xy.x), -(rect.point(0).y - xy.y));
        rect.move(rect.width() * randint(0, w / rect.width()),      // Random x-coordinate
            rect.height() * randint(0, h / rect.height()));         // Random y-coordinate
    }

    //------------------------------------------------------------------------------

}	// End of Graph_lib namespace

//------------------------------------------------------------------------------