#ifndef RANDOM_STRATEGY_PLAYER_HPP_INCLUDED
#define RANDOM_STRATEGY_PLAYER_HPP_INCLUDED

#include <random>
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

#endif
