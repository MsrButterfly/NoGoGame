#include <iostream>
#include <map>
#include "nogo_chess.hpp"
#include "nogo_game.hpp"
#include "random_strategy_player.hpp"
#include "eye_strategy_player.hpp"
#include "human_player.hpp"

int main(int argc, const char *argv[]) {
    nogo_game game;
    const size_t times = 1000;
    std::map<nogo_chess, size_t> wins;
    wins[nogo_chess::black] = 0;
    wins[nogo_chess::white] = 0;
    for (size_t i = 0; i < times; ++i) {
        game.reset();
        game.set_player<eye_strategy_player>(nogo_chess::black);
        game.set_player<random_strategy_player>(nogo_chess::white);
        game.start();
        ++wins[game.winner()];
        std::cout << i + 1 << ". " << game.winner() << std::endl;
    }
    for (auto &w: wins) {
        std::cout << w.first << ": " << w.second << " wins (" << static_cast<double>(w.second) / static_cast<double>(times) * 100 << "%)" << std::endl;
    }
    std::cout << std::endl;
    return 0;
}
