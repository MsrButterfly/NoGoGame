//
//  main.cpp
//  NoGo
//
//  Created by Darren Liu on 14/6/19.
//  Copyright (c) 2014 MsrLab. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <sstream>
#include <queue>
#include <string>
#include <array>
#include <random>
#include <chrono>
#include <numeric>
#include <limits>
#include <thread>
#include <vector>
#include <list>
#include <algorithm>

//─━│┃┄┅┆┇┈┉┊┋┌┍┎┏┐┑┒┓└┕┖┗┘┙┚┛
//　　├┝┞┟┠┡┢┣┤┥┦┧┨┩┪┫┬┭┮┯┰┱┲┳┴┵┶┷┸┹┺┻
//　　┼┽┾┿╀╁╂╃╄╅╆╇╈╉╊╋
//　　═║╒╓╔╕╖╗╘╙╚╛╜╝╞╟╠╡╢╣╤╥╦╧╨╩╪╫╬╳
//　　╔ ╗╝╚ ╬ ═ ╓ ╩ ┠ ┨┯ ┷┏ ┓┗ ┛┳⊥﹃﹄┌╭╮╯╰

enum class chess {
    none,
    black,
    white
};

struct point {
    size_t x;
    size_t y;
};

bool operator==(const point &a, const point &b) {
    return a.x == b.x && a.y == b.y;
};

template <size_t M, size_t N>
class chessboard {
public:
    static const size_t size = M * N;
    static const size_t rows = M;
    static const size_t colomns = N;
    using chess_type = chess;
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

template <class Char>
std::basic_ostream<Char> &
operator<<(std::basic_ostream<Char> &os, const chess &c) {
    switch (c) {
        case chess::none:
            os << " ";
            break;
        case chess::black:
            os << "●";
            break;
        case chess::white:
            os << "○";
            break;
        default:
            break;
    }
    return os;
}

template <class Char, size_t M, size_t N>
std::basic_ostream<Char> &
operator<<(std::basic_ostream<Char> &os, const chessboard<M, N> &board) {
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
            if (c == chess::none) {
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

class nogo_chessboard: public chessboard<9, 9> {
public:
    bool available(const point &p, const chess_type &c) const {
        if (c_[p.x][p.y] != chess_type::none) {
            return false;
        } else if (c == chess_type::none) {
            return true;
        }
        auto r = chess_type::none;
        if (c == chess_type::black) {
            r = chess_type::white;
        } else {
            r = chess_type::black;
        }
        bool v[colomns][rows];
        auto reset_v = [&v, &p] {
            for (auto &a : v) {
                for (auto &b : a) {
                    b = false;
                }
            }
            v[p.x][p.y] = true;
        };
        reset_v();
        if (p.x > 0 && c_[p.x - 1][p.y] == r && !available({p.x - 1, p.y}, r, v)) {
            return false;
        }
        reset_v();
        if (p.x < colomns - 1 && c_[p.x + 1][p.y] == r && !available({p.x + 1, p.y}, r, v)) {
            return false;
        }
        reset_v();
        if (p.y > 0 && c_[p.x][p.y - 1] == r && !available({p.x, p.y - 1}, r, v)) {
            return false;
        }
        reset_v();
        if (p.y < rows - 1 && c_[p.x][p.y + 1] == r && !available({p.x, p.y + 1}, r, v)) {
            return false;
        }
        reset_v();
        return available(p, c, v);
    }
private:
    bool available(const point &p, const chess_type &c, bool (&v)[colomns][rows]) const {
        v[p.x][p.y] = true;
        bool left = false;
        bool right = false;
        bool top = false;
        bool bottom = false;
        if (p.x > 0 && !v[p.x - 1][p.y]) {
            auto _c = c_[p.x - 1][p.y];
            if (_c == c) {
                left = available({p.x - 1, p.y}, c, v);
            } else if (_c == chess_type::none) {
                return true;
            }
        }
        if (p.x < colomns - 1 && !v[p.x + 1][p.y]) {
            auto _c = c_[p.x + 1][p.y];
            if (_c == c) {
                right = available({p.x + 1, p.y}, c, v);
            } else if (_c == chess_type::none) {
                return true;
            }
        }
        if (p.y > 0 && !v[p.x][p.y - 1]) {
            auto _c = c_[p.x][p.y - 1];
            if (_c == c) {
                top = available({p.x, p.y - 1}, c, v);
            } else if (_c == chess_type::none) {
                return true;
            }
        }
        if (p.y < rows - 1 && !v[p.x][p.y + 1]) {
            auto _c = c_[p.x][p.y + 1];
            if (_c == c) {
                bottom = available({p.x, p.y + 1}, c, v);
            } else if (_c == chess_type::none) {
                return true;
            }
        }
        return left || right || top || bottom;
    }
};

chess nogo_test(const bool &show_process = false) {
    using namespace std;
    using namespace chrono;
    nogo_chessboard board;
    vector<point> ps;
    for (size_t i = 0; i < nogo_chessboard::colomns; ++i) {
        for (size_t j = 0; j < nogo_chessboard::rows; ++j) {
            ps.push_back({i, j});
        }
    }
    bool color = false;
    random_device device;
    while (ps.size() != 0) {
        auto c = static_cast<chess>(static_cast<size_t>(color) + 1);
        if (show_process) {
            cout << ps.size() << " points left." << endl;
        }
        auto ps_ = ps;
        bool found = false;
        while (ps_.size() != 0) {
            std::uniform_int_distribution<size_t> random(0, ps_.size() - 1);
            auto i = random(device);
            auto p = ps_[i];
            if (board.available(p, c)) {
                ps.erase(find(begin(ps), end(ps), p));
                found = true;
                board[p.x][p.y] = c;
                color = !color;
                if (show_process) {
                    cout << c << ' ' << static_cast<char>(p.x + 'A') << p.y + 1 << endl;
                    cout << board << endl;
                }
                break;
            } else {
                ps_.erase(begin(ps_) + i);
            }
        }
        if (!found) {
            if (show_process) {
                cout << c << " lost" << endl;
            }
            break;
        }
    }
    if (color) {
        return nogo_chessboard::chess_type::black;
    } else {
        return nogo_chessboard::chess_type::white;
    }
}

int main(int argc, const char *argv[]) {
    using namespace std;
    using namespace chrono;
//    size_t black_wins = 0;
//    size_t white_wins = 0;
//    auto total = [&black_wins, &white_wins] {
//        return black_wins + white_wins;
//    };
//    while (true) {
//        auto winner = nogo_test();
//        if (winner == nogo_chessboard::chess_type::black) {
//            ++black_wins;
//        } else {
//            ++white_wins;
//        }
//        cout << chess::black << ": " << static_cast<double>(black_wins) / total() << ' ';
//        cout << chess::white << ": " << static_cast<double>(white_wins) / total() << endl;
//    }
    nogo_test(true);
    return 0;
}
