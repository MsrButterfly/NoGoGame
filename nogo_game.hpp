#ifndef NOGO_GAME_HPP_INCLUDED
#define NOGO_GAME_HPP_INCLUDED

#include <array>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "nogo_chessboard.hpp"
#include "nogo_player.hpp"
#include "point.hpp"

class nogo_game {
public:
    using available_matrix_type = std::array<std::array<bool, nogo_chessboard::rows>, nogo_chessboard::colomns>;
public:
    nogo_game() {
        reset();
    }
public:
    void start(const bool &show_progress = false, nogo_chess current = nogo_chess::black) {
        if (!player_[nogo_chess::black] || !player_[nogo_chess::white]) {
            throw std::string("Players not yet ready.");
        }
        while (true) {
            player_[current]->play(current);
            if (show_progress) {
                std::cout << chessboard_ << std::endl;
            }
            auto next = (current == nogo_chess::black) ? nogo_chess::white : nogo_chess::black;
            if (remain_[next] == 0) {
                winner_ = current;
                break;
            }
            current = next;
        }
    }
    void reset() {
        auto clear = [](available_matrix_type &matrix) {
            for (auto &r: matrix) {
                for (auto &p: r) {
                    p = true;
                }
            }
        };
        clear(available_[nogo_chess::black]);
        clear(available_[nogo_chess::white]);
        remain_[nogo_chess::black] = nogo_chessboard::size;
        remain_[nogo_chess::white] = nogo_chessboard::size;
        player_[nogo_chess::black] = nullptr;
        player_[nogo_chess::white] = nullptr;
        chessboard_ = nogo_chessboard();
        winner_ = nogo_chess::none;
    }
    const nogo_chess &winner() const {
        return winner_;
    }
    template <class Player>
    void set_player(const nogo_chess &chess) {
        player_[chess] = std::make_shared<Player>(*this);
    }
    void set(const point &p, const nogo_chess &chess, const bool &show = false) {
        if (p.x >= nogo_chessboard::colomns || p.y >= nogo_chessboard::rows) {
            throw std::string("Specific point does not exist.");
        }
        auto &available = available_[chess];
        if (available[p.x][p.y]) {
            chessboard_[p.x][p.y] = chess;
            if (show) {
                std::cout << chess << ' ' << static_cast<char>(p.x + 'A') << static_cast<char>(p.y + '1') << std::endl;
            }
        } else {
            throw std::string("Point not available.");
        }
        auto update = [&](const nogo_chess &c) {
            auto &available = available_[c];
            for (size_t i = 0; i < nogo_chessboard::colomns; ++i) {
                for (size_t j = 0; j < nogo_chessboard::rows; ++j) {
                    if (available[i][j]) {
                        available[i][j] = chessboard_.available({i, j}, c);
                        if (!available[i][j]) {
                            --remain_[c];
                        }
                    }
                }
            }
        };
        update(nogo_chess::black);
        update(nogo_chess::white);
    }
    const nogo_chess &get(const point &p) const {
        if (p.x >= nogo_chessboard::colomns || p.y >= nogo_chessboard::rows) {
            throw std::string("Specific point does not exist.");
        }
        return chessboard_[p.x][p.y];
    }
    const size_t &remain(const nogo_chess &chess) const {
        return remain_.at(chess);
    }
    const bool &available(const point &p, const nogo_chess &chess) const {
        return available_.at(chess)[p.x][p.y];
    }
    const std::vector<point> available_sequence(const nogo_chess &chess) const {
        std::vector<point> sequence;
        auto &available = available_.at(chess);
        for (size_t i = 0; i < nogo_chessboard::colomns; ++i) {
            for (size_t j = 0; j < nogo_chessboard::rows; ++j) {
                if (available[i][j]) {
                    sequence.push_back({i, j});
                }
            }
        }
        return sequence;
    }
private:
    nogo_chess winner_;
    nogo_chessboard chessboard_;
    std::map<nogo_chess, available_matrix_type> available_;
    std::map<nogo_chess, size_t> remain_;
    std::map<nogo_chess, std::shared_ptr<nogo_player>> player_;
};

#endif
