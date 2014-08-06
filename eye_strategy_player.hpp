#ifndef EYE_STRATEGY_PLAYER_HPP
#define EYE_STRATEGY_PLAYER_HPP

#include <algorithm>
#include <random>
#include <vector>
#include "nogo_chess.hpp"
#include "nogo_game.hpp"
#include "nogo_player.hpp"

class eye_strategy_player: public nogo_player {
public:
    eye_strategy_player(nogo_game &game): nogo_player(game) {}
public:
    virtual void play(const nogo_chess &chess) override {
        const auto self = chess;
        const auto opponent = (chess == nogo_chess::black) ? nogo_chess::white : nogo_chess::black;
        auto available_sequence = game_.available_sequence(chess);
        static std::random_device device;
        std::mt19937 generator(device());
        std::shuffle(begin(available_sequence), end(available_sequence), generator);
        std::vector<float> weight(available_sequence.size(), 1);
        size_t liberty = 0;
        for (size_t i = 0; i < available_sequence.size(); ++i) {
            game_.set(available_sequence[i], chess);
            weight[i] = game_.remaining(opponent);
            game_.undo();
            liberty += weight[i];
        }
        float average_liberty = float(liberty) / available_sequence.size();
        for (size_t i = 0; i < available_sequence.size(); ++i) {
            weight[i] = (average_liberty - weight[i] + 1) * liberty_factor;
        }
        for (size_t i = 0; i < weight.size(); ++i) {
            auto &p = available_sequence[i];
            try { weight[i] *= game_.get(p.top().left()) == self ? eye_construction_factor : 1; } catch (...) { weight[i] *= eye_construction_factor; }
            try { weight[i] *= game_.get(p.top().right()) == self ? eye_construction_factor : 1; } catch (...) { weight[i] *= eye_construction_factor; }
            try { weight[i] *= game_.get(p.bottom().left()) == self ? eye_construction_factor : 1; } catch (...) { weight[i] *= eye_construction_factor; }
            try { weight[i] *= game_.get(p.bottom().right()) == self ? eye_construction_factor : 1; } catch (...) { weight[i] *= eye_construction_factor; }
            try { weight[i] *= game_.get(p.top().top()) == self ? eye_construction_factor : 1; } catch (...) { weight[i] *= eye_construction_factor; }
            try { weight[i] *= game_.get(p.bottom().bottom()) == self ? eye_construction_factor : 1; } catch (...) { weight[i] *= eye_construction_factor; }
            try { weight[i] *= game_.get(p.left().left()) == self ? eye_construction_factor : 1; } catch (...) { weight[i] *= eye_construction_factor; }
            try { weight[i] *= game_.get(p.right().right()) == self ? eye_construction_factor : 1; } catch (...) { weight[i] *= eye_construction_factor; }
            bool top = true, bottom = true, left = true, right = true;
            try { top = (game_.get(p.top()) == self); } catch (...) {}
            try { bottom = (game_.get(p.bottom()) == self); } catch (...) {}
            try { left = (game_.get(p.left()) == self); } catch (...) {}
            try { right = (game_.get(p.right()) == self); } catch (...) {}
            if (top && bottom && left && right) {
                weight[i] *= eye_fill_factor;
            }
            try { weight[i] *= game_.get(p.top()) == self ? adjacency_factor : 1; } catch (...) { weight[i] *= adjacency_factor; }
            try { weight[i] *= game_.get(p.bottom()) == self ? adjacency_factor : 1; } catch (...) { weight[i] *= adjacency_factor; }
            try { weight[i] *= game_.get(p.left()) == self ? adjacency_factor : 1; } catch (...) { weight[i] *= adjacency_factor; }
            try { weight[i] *= game_.get(p.right()) == self ? adjacency_factor : 1; } catch (...) { weight[i] *= adjacency_factor; }
            try { weight[i] *= game_.get(p.top()) == opponent ? eye_destruction_factor : 1; } catch (...) { weight[i] *= eye_destruction_factor; }
            try { weight[i] *= game_.get(p.bottom()) == opponent ? eye_destruction_factor : 1; } catch (...) { weight[i] *= eye_destruction_factor; }
            try { weight[i] *= game_.get(p.left()) == opponent ? eye_destruction_factor : 1; } catch (...) { weight[i] *= eye_destruction_factor; }
            try { weight[i] *= game_.get(p.right()) == opponent ? eye_destruction_factor : 1; } catch (...) { weight[i] *= eye_destruction_factor; }
            top = bottom = left = right = true;
            try { top = (game_.get(p.top()) == opponent); } catch (...) {}
            try { bottom = (game_.get(p.bottom()) == opponent); } catch (...) {}
            try { left = (game_.get(p.left()) == opponent); } catch (...) {}
            try { right = (game_.get(p.right()) == opponent); } catch (...) {}
            if (size_t(top) + size_t(bottom) + size_t(left) + size_t(right) == 3) {
                weight[i] *= 1 / eye_fill_factor;
            }
            weight[i] *= ((p.x + p.y) % 2 == 1) ? parity_factor : 1;
        }
        float max = 1;
        point p = available_sequence[0];
        for (size_t i = 0; i < weight.size(); ++i) {
            if (weight[i] > max) {
                max = weight[i];
                p = available_sequence[i];
            }
        }
        game_.set(p, chess);
    }
public:
    float parity_factor = 2;
    float eye_construction_factor = 2;
    float eye_fill_factor = 0.01;
    float eye_destruction_factor = 2;
    float adjacency_factor = 0.3;
    float liberty_factor = 1.5;
    float random_factor = 0.5;
};

#endif
