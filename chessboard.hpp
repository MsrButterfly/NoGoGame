#ifndef CHESSBOARD_HPP_INCLUDED
#define CHESSBOARD_HPP_INCLUDED

#include <array>
#include <iomanip>
#include <iostream>
#include <sstream>

template <class Chess, size_t M, size_t N>
class chessboard {
public:
    static const size_t size = M * N;
    static const size_t rows = M;
    static const size_t colomns = N;
    using chess_type = Chess;
    using row_type = std::array<chess_type, colomns>;
protected:
    std::array<row_type, rows> c_;
public:
    chessboard(): c_({chess_type::none}) {}
public:
    typename decltype(c_)::value_type &
    operator[](const typename decltype(c_)::size_type &size) {
        return c_[size];
    }
    const typename decltype(c_)::value_type &
    operator[](const typename decltype(c_)::size_type &size) const {
        return c_[size];
    }
    typename decltype(c_)::iterator begin() {
        return std::begin(c_);
    }
    typename decltype(c_)::iterator end() {
        return std::end(c_);
    }
    typename decltype(c_)::const_iterator begin() const {
        return std::begin(c_);
    }
    typename decltype(c_)::const_iterator end() const {
        return std::end(c_);
    }
};

template <class Char, class Chess, size_t M, size_t N>
std::basic_ostream<Char> &
operator<<(std::basic_ostream<Char> &os, const chessboard<Chess, M, N> &board) {
    os << std::right;
    std::stringstream ss;
    ss << N;
    auto l = ss.str().length();
    os << std::string(l, ' ');
    for (size_t i = 0; i < M; ++i) {
        os << std::setw(2) << static_cast<char>('A' + i);
    }
    os << std::endl;
    for (size_t i = 0; i < N; ++i) {
        auto line = "─";
        if (i == 0 || i == N - 1) {
            line = "━";
        }
        auto left = "┠";
        auto right = "┨";
        auto cross = "┼";
        if (i == 0) {
            left = "╔";
            right = "╗";
            cross = "┯";
        } else if (i == N - 1) {
            left = "╚";
            right = "╝";
            cross = "┷";
        }
        os << std::setw(static_cast<int>(l)) << i + 1 << " ";
        for (size_t j = 0; j < M; ++j) {
            const auto &c = board[j][i];
            if (c == Chess::none) {
                if (j == 0) {
                    os << left;
                } else if (j == M - 1) {
                    os << right;
                } else {
                    os << cross;
                }
            } else {
                os << c;
            }
            if (j != M - 1) {
                os << line;
            }
        }
        os << std::endl;
    }
    return os;
}

#endif
