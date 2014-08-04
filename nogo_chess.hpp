#ifndef NOGO_CHESS_HPP_INCLUDED
#define NOGO_CHESS_HPP_INCLUDED

#include <iostream>

enum class nogo_chess {
    none,
    black,
    white
};

template <class Char>
std::basic_ostream<Char> &
operator<<(std::basic_ostream<Char> &os, const nogo_chess &c) {
    switch (c) {
        case nogo_chess::none:
            os << " ";
            break;
        case nogo_chess::black:
            os << "●";
            break;
        case nogo_chess::white:
            os << "○";
            break;
        default:
            break;
    }
    return os;
}

#endif
