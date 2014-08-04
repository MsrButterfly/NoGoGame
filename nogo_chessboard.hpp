#ifndef NOGO_CHESSBOARD_HPP_INCLUDED
#define NOGO_CHESSBOARD_HPP_INCLUDED

#include "chessboard.hpp"
#include "nogo_chess.hpp"
#include "point.hpp"

class nogo_chessboard: public chessboard<nogo_chess, 9, 9> {
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

#endif
