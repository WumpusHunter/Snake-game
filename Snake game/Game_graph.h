// Snake game's graphics

//------------------------------------------------------------------------------

#pragma once
#include "GraphicsLib/Graph.h"

//------------------------------------------------------------------------------

namespace Graph_lib {

    //------------------------------------------------------------------------------

    // Invariant: cell_w > 0, cell_h > 0, sz > 0
    class Snake : public Shape {
    public:
        enum class Direction {      // Possible directions of head
            left, up, right, down
        };

        // Construction
        Snake(Point xy, int cell_w, int cell_h, int sz);

        // Drawing
        void draw_lines() const override;
        void move(int dx, int dy) override;
        void move_forward();
        void grow_length();
        void shrink_length(int num);

        // Modification of parameters
        void set_color(Color c);
        void set_fill_color(Color c);
        void set_style(Line_style ls);
        void set_direction(Direction d);
        void head_set_fill_color(Color c);

        // Access to parameters
        const Rectangle& body_head() const;
        const Rectangle& body_tail() const;
        Direction direction() const { return head; }
        int length() const { return body.size(); }
        bool is_body(const Rectangle& cell) const;
        bool is_body_except_head(const Rectangle& cell) const;

    protected:
        // Access to parameters
        Rectangle& body_head();
        Rectangle& body_tail();

    private:
        Vector_ref<Rectangle> body;
        Direction head;                 // Direction of head
    };

    //------------------------------------------------------------------------------

    // Helper function
    void random_move(Rectangle& rect, Point xy, int w, int h);

    //------------------------------------------------------------------------------

}	// End of Graph_lib namespace

//------------------------------------------------------------------------------