#ifndef HUMAN_PLAYER_HPP_INCLUDED
#define HUMAN_PLAYER_HPP_INCLUDED

#include <iostream>
#include <map>
#include <string>
#include "nogo_chess.hpp"
#include "nogo_game.hpp"
#include "nogo_player.hpp"

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

#endif
