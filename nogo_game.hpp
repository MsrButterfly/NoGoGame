#ifndef NOGO_GAME_HPP_INCLUDED
#define NOGO_GAME_HPP_INCLUDED

#include <array>
#include <map>
#include <memory>
#include <stack>
#include <string>
#include <vector>
#include "nogo_chessboard.hpp"
#include "nogo_player.hpp"
#include "point.hpp"

class nogo_game {
public:
    using available_matrix_type = std::array<std::array<bool, nogo_chessboard::rows>, nogo_chessboard::colomns>;
public:
    nogo_game() { reset(); }
    nogo_game(const nogo_game &game)
    : winner_(game.winner_), chessboard_(game.chessboard_), available_(game.available_), remaining_(game.remaining_), player_(game.player_), step_(game.step_) {}
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
            if (remaining_[next] == 0) {
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
        remaining_[nogo_chess::black] = nogo_chessboard::size;
        remaining_[nogo_chess::white] = nogo_chessboard::size;
        player_[nogo_chess::black] = nullptr;
        player_[nogo_chess::white] = nullptr;
        chessboard_ = nogo_chessboard();
        winner_ = nogo_chess::none;
        step_ = std::stack<point>();
    }
    const nogo_chess &winner() const {
        return winner_;
    }
    template <class Player>
    void set_player(const nogo_chess &chess) {
        player_[chess] = std::make_shared<Player>(*this);
    }
    template <class Player>
    std::shared_ptr<Player> get_player(const nogo_chess &chess) {
        auto player = std::dynamic_pointer_cast<Player>(player_[chess]);
        return player;
    }
    void set(const point &p, const nogo_chess &chess, const bool &show = false) {
        if (p.x >= nogo_chessboard::colomns || p.y >= nogo_chessboard::rows) {
            throw std::string("Specific point does not exist.");
        }
        auto &available = available_[chess];
        if (available[p.x][p.y]) {
            chessboard_[p.x][p.y] = chess;
            step_.push(p);
            if (show) {
                std::cout << chess << ' ' << static_cast<char>(p.x + 'A') << static_cast<char>(p.y + '1') << std::endl;
            }
        } else {
            throw std::string("Point not available.");
        }
        update(nogo_chess::black);
        update(nogo_chess::white);
    }
    const nogo_chess &get(const point &p) const {
        if (p.x >= nogo_chessboard::colomns || p.y >= nogo_chessboard::rows) {
            throw std::string("Specific point does not exist.");
        }
        return chessboard_[p.x][p.y];
    }
    void undo(const bool show = false) {
        if (step_.size() == 0) {
            throw std::string("Already at first step.");
        }
        auto p = step_.top();
        step_.pop();
        winner_ = nogo_chess::none;
        chessboard_[p.x][p.y] = nogo_chess::none;
        update(nogo_chess::black);
        update(nogo_chess::white);
        if (show) {
            std::cout << chessboard_ << std::endl;
        }
    }
    const size_t &remaining(const nogo_chess &chess) const {
        return remaining_.at(chess);
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
    size_t step_count() const {
        return step_.size();
    }
private:
    void update(const nogo_chess &chess) {
        auto &available = available_[chess];
        for (size_t i = 0; i < nogo_chessboard::colomns; ++i) {
            for (size_t j = 0; j < nogo_chessboard::rows; ++j) {
                auto before = available[i][j];
                available[i][j] = chessboard_.available({i, j}, chess);
                auto after = available[i][j];
                if (before != after) {
                    remaining_[chess] += (before == false) ? 1 : -1;
                }
            }
        }
    }
private:
    nogo_chess winner_;
    nogo_chessboard chessboard_;
    std::map<nogo_chess, available_matrix_type> available_;
    std::map<nogo_chess, size_t> remaining_;
    std::map<nogo_chess, std::shared_ptr<nogo_player>> player_;
    std::stack<point> step_;
};

#endif
