#include <array>
#include <iostream>
#include <map>
#include <random>
#include <vector>
#include "nogo_chess.hpp"
#include "nogo_game.hpp"
#include "nogo_player.hpp"

class random_strategy_player: public nogo_player {
public:
    random_strategy_player(nogo_game &game): nogo_player(game) {}
public:
    virtual void play(const nogo_chess &chess) override {
        auto available_sequence = game_.available_sequence(chess);
        static std::random_device device;
        std::uniform_int_distribution<size_t> distribute(0, available_sequence.size() - 1);
        game_.set(available_sequence[distribute(device)], chess);
    }
};

class eye_strategy_player: public nogo_player {
public:
    eye_strategy_player(nogo_game &game): nogo_player(game) {}
public:
    virtual void play(const nogo_chess &chess) override {
        const auto self = chess;
        const auto opponent = (chess == nogo_chess::black) ? nogo_chess::white : nogo_chess::black;
        auto available_sequence = game_.available_sequence(chess);
        std::vector<float> weight(available_sequence.size(), 1);
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
            try { weight[i] *= game_.get(p.top()) == self ? eye_fill_factor : 1; } catch (...) { weight[i] *= eye_fill_factor; }
            try { weight[i] *= game_.get(p.bottom()) == self ? eye_fill_factor : 1; } catch (...) { weight[i] *= eye_fill_factor; }
            try { weight[i] *= game_.get(p.left()) == self ? eye_fill_factor : 1; } catch (...) { weight[i] *= eye_fill_factor; }
            try { weight[i] *= game_.get(p.right()) == self ? eye_fill_factor : 1; } catch (...) { weight[i] *= eye_fill_factor; }
            try { weight[i] *= game_.get(p.top()) == opponent ? eye_destruction_factor : 1; } catch (...) { weight[i] *= eye_destruction_factor; }
            try { weight[i] *= game_.get(p.bottom()) == opponent ? eye_destruction_factor : 1; } catch (...) { weight[i] *= eye_destruction_factor; }
            try { weight[i] *= game_.get(p.left()) == opponent ? eye_destruction_factor : 1; } catch (...) { weight[i] *= eye_destruction_factor; }
            try { weight[i] *= game_.get(p.right()) == opponent ? eye_destruction_factor : 1; } catch (...) { weight[i] *= eye_destruction_factor; }
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
    float parity_factor = 2.5;
    float eye_construction_factor = 1.5;
    float eye_fill_factor = 0.5;
    float eye_destruction_factor = 1.5;
};

class human_player: public nogo_player {
public:
    human_player(nogo_game &game): nogo_player(game) {}
public:
    virtual void play(const nogo_chess &chess) override {
        std::string input;
        while (true) {
            std::cout << chess << ": ";
            std::getline(std::cin, input);
            if (input.length() == 2) {
                size_t x = input[0] - 'A';
                size_t y = input[1] - '1';
                try {
                    game_.set({x, y}, chess);
                    break;
                } catch (const std::string &hint) {
                    std::cout << hint << std::endl;
                }
            }
        }
    }
};

int main(int argc, const char *argv[]) {
    nogo_game game;
    const size_t times = 1;
    std::map<nogo_chess, size_t> wins;
    wins[nogo_chess::black] = 0;
    wins[nogo_chess::white] = 0;
    for (size_t i = 0; i < times; ++i) {
        game.reset();
        game.set_player<eye_strategy_player>(nogo_chess::black);
        game.set_player<human_player>(nogo_chess::white);
        game.start(true);
        ++wins[game.winner()];
    }
    for (auto &w: wins) {
        std::cout << w.first << ": " << w.second << " wins (" << static_cast<double>(w.second) / static_cast<double>(times) * 100 << "%)" << std::endl;
    }
    std::cout << std::endl;
    return 0;
}
