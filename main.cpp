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

class eye_creating_strategy_player: public nogo_player {
public:
    eye_creating_strategy_player(nogo_game &game): nogo_player(game) {}
public:
    virtual void play(const nogo_chess &chess) override {
        auto available_sequence = game_.available_sequence(chess);
        std::vector<float> weight(available_sequence.size(), 1);
        for (size_t i = 0; i < weight.size(); ++i) {
            try { weight[i] *= game_.get(available_sequence[i].top_left()) == chess ? 1.5 : 1; } catch (...) {}
            try { weight[i] *= game_.get(available_sequence[i].top_right()) == chess ? 1.5 : 1; } catch (...) {}
            try { weight[i] *= game_.get(available_sequence[i].bottom_left()) == chess ? 1.5 : 1; } catch (...) {}
            try { weight[i] *= game_.get(available_sequence[i].bottom_right()) == chess ? 1.5 : 1; } catch (...) {}
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
};

int main(int argc, const char *argv[]) {
    nogo_game game;
    const size_t sum = 1000;
    std::map<nogo_chess, size_t> wins;
    wins[nogo_chess::black] = 0;
    wins[nogo_chess::white] = 0;
    for (size_t i = 0; i < sum; ++i) {
        game.reset();
        game.set_player<eye_creating_strategy_player>(nogo_chess::black);
        game.set_player<random_strategy_player>(nogo_chess::white);
        game.start();
        ++wins[game.winner()];
    }
    for (auto &w: wins) {
        std::cout << w.first << ": " << w.second << " wins (" << static_cast<double>(w.second) / static_cast<double>(sum) * 100 << "%)" << std::endl;
    }
    return 0;
}
