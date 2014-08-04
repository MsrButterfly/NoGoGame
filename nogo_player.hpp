#ifndef NOGO_PLAYER_HPP_INCLUDED
#define NOGO_PLAYER_HPP_INCLUDED

class nogo_game;
enum class nogo_chess;

class nogo_player {
public:
    nogo_player(nogo_game &game): game_(game) {}
public:
    virtual void play(const nogo_chess &chess) = 0;
protected:
    nogo_game &game_;
};

#endif
