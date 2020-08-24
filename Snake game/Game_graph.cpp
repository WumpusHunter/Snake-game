// Snake game's graphics

//------------------------------------------------------------------------------

#include <cassert>
#include "Game_graph.h"
#include "RandomNumber/Generator.h"

//------------------------------------------------------------------------------

namespace Graph_lib {

    //------------------------------------------------------------------------------

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
        for (auto pos = body.cbegin(); pos != body.cend(); ++pos)
            (*pos)->draw();
    }

    // Moves snake by dx at x-coordinate and dy at y-coordinate 
    void Snake::move(int dx, int dy)
    {
        Shape::move(dx, dy);
        // Move each cell of body
        for (auto pos = body.begin(); pos != body.end(); ++pos)
            (*pos)->move(dx, dy);
    }

    // Moves snake forward, that is, adds new head and removes old tail
    void Snake::move_forward()
    {
        grow_length();      // Add new head
        shrink_length();    // Remove old tail
    }

    // Grows snake in length, that is, adds one cell into its body as its new head
    void Snake::grow_length()
    {
        Rectangle& prev_head = body_head();     // Previous head
        // Add new cell into snake's body as its new head
        body.push_front(new Rectangle{ prev_head.point(0), prev_head.width(), prev_head.height() });
        // Move head one cell forward in its direction
        switch (head) {
        case Direction::left:       // Left-side
            body_head().move(-body_head().width(), 0);
            break;
        case Direction::up:         // Up-side
            body_head().move(0, -body_head().height());
            break;
        case Direction::right:      // Right-side
            body_head().move(body_head().width(), 0);
            break;
        case Direction::down:       // Down-side
            body_head().move(0, body_head().height());
            break;
        }
        set_point(0, body_head().point(0));         // Update location of snake's head
        // Set same parameters for new head as for old head
        body_head().set_color(prev_head.color());
        body_head().set_fill_color(prev_head.fill_color());
        body_head().set_style(prev_head.style());
        // Set same parameters for old head as for all body
        prev_head.set_color(color());
        prev_head.set_fill_color(fill_color());
        prev_head.set_style(style());
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
        for (auto pos = body.begin(); pos != body.end(); ++pos)
            (*pos)->set_color(c);
    }

    // Sets c as fill color of snake's body
    void Snake::set_fill_color(Color c)
    {
        Shape::set_fill_color(c);
        // Set c as fill color to each cell of body
        for (auto pos = body.begin(); pos != body.end(); ++pos)
            (*pos)->set_fill_color(c);
    }

    // Sets c as fill color of snake's head
    void Snake::head_set_fill_color(Color c)
    {
        body_head().set_fill_color(c);
    }

    // Sets ls as line style of snake's body
    void Snake::set_style(Line_style ls)
    {
        Shape::set_style(ls);
        // Set ls as line style to each cell of body
        for (auto pos = body.begin(); pos != body.end(); ++pos)
            (*pos)->set_style(ls);
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
        return body.front();
    }

    // Gets snake's tail
    const Rectangle& Snake::body_tail() const
    {
        assert(body.cbegin() != body.cend()     // Error handling
            && "Bad Snake: can't get tail of empty snake");
        return body.back();
    }

    // Gets snake's head
    Rectangle& Snake::body_head()
    {
        const Snake& self = static_cast<const Snake&>(*this);       // Self-reference
        return const_cast<Rectangle&>(self.body_head());            // Cast constness from const-version
    }

    // Gets snake's tail
    Rectangle& Snake::body_tail()
    {
        const Snake& self = static_cast<const Snake&>(*this);       // Self-reference
        return const_cast<Rectangle&>(self.body_tail());            // Cast constness from const-version
    }

    // Determines either cell is one of snake's body's cells
    bool Snake::is_body(const Rectangle& cell) const
    {
        // Search for cell in snake's body, located same as cell, and compare parameters
        return find_if(body.cbegin(), body.cend(), [&cell](const Rectangle* rect)
            { return rect->point(0) == cell.point(0); }) != body.cend()
            && body_head().width() == cell.width() && body_head().height() == cell.height();
    }

    // Determines either cell is one of snake's body's cells, except its head
    bool Snake::is_body_except_head(const Rectangle& cell) const
    {
        // Search for cell in snake's body, located same as cell, except snake's head, and compare parameters
        constexpr int head_num = 1;     // Order number of head
        return body.size() > head_num ? find_if(next(body.cbegin()), body.cend(),
            [&cell](const Rectangle* rect) { return rect->point(0) == cell.point(0); }) != body.cend()
            && body_head().width() == cell.width() && body_head().height() == cell.height() : false;
    }

    //------------------------------------------------------------------------------

    // Moves s to xy; requires s to have only one point
    void move_to(Shape* s, Point xy)
    {
        if (s == nullptr)     // Error handling
            throw invalid_argument("Bad move_to: can't move unexisted Shape");
        s->move(xy.x - s->point(0).x, xy.y - s->point(0).y);
    }

    // Moves rect randomly in range [xy.x; xy.x + w] for x-coordinate and [xy.y; xy.y + h] for
    // y-coordinate, with xy as original point, w as width of range and h as height of range
    void random_move(Rectangle& rect, Point xy, int w, int h)
    {
        if (w < 0 || h < 0)     // Error handling
            throw invalid_argument("Bad random_move: invalid range for coordinates");
        // Random x and y coordinates
        const int rand_x = xy.x + rect.width() * randint(0, w / rect.width()),
            rand_y = xy.y + rect.height() * randint(0, h / rect.height());
        move_to(&rect, Point{ rand_x, rand_y });     // Move to random point
    }

    //------------------------------------------------------------------------------

}	// End of Graph_lib namespace

//------------------------------------------------------------------------------