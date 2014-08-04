#ifndef POINT_HPP_INCLUDED
#define POINT_HPP_INCLUDED

#include <iostream>

struct point {
    using self_type = point;
    size_t x;
    size_t y;
    self_type left() {
        return {x - 1, y};
    }
    self_type right() {
        return {x + 1, y};
    }
    self_type top() {
        return {x, y - 1};
    }
    self_type bottom() {
        return {x, y + 1};
    }
    self_type top_left() {
        return {x - 1, y - 1};
    }
    self_type top_right() {
        return {x + 1, y - 1};
    }
    self_type bottom_left() {
        return {x - 1, y + 1};
    }
    self_type bottom_right() {
        return {x + 1, y + 1};
    }
};

#endif
