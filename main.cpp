#include <array>
#include <iostream>
#include <map>
#include <random>
#include "nogo_chess.hpp"
#include "nogo_game.hpp"
#include "nogo_player.hpp"

struct random_strategy_player: public nogo_player {
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

int main(int argc, const char *argv[]) {
    nogo_game game;
    const size_t sum = 100;
    std::map<nogo_chess, size_t> wins;
    wins[nogo_chess::black] = 0;
    wins[nogo_chess::white] = 0;
    for (size_t i = 0; i < sum; ++i) {
        game.reset();
        game.set_player<random_strategy_player>(nogo_chess::black);
        game.set_player<random_strategy_player>(nogo_chess::white);
        game.start();
        ++wins[game.winner()];
    }
    for (auto &w: wins) {
        std::cout << w.first << ": " << w.second << " wins (" << static_cast<double>(w.second) / static_cast<double>(sum) * 100 << "%)" << std::endl;
    }
    return 0;
}
