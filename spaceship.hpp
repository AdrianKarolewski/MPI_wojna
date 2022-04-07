#ifndef SPACE_SHIPP_HPP
#define SPACE_SHIPP_HPP

#define MAX_LIVES 10

class Simulation;

class SpaceShip
{
    friend Simulation;
public:
    SpaceShip() : lives(MAX_LIVES) {}
    bool canGoFight();
    int howManyMechanicks();
private:
    void makeDemage(const int & d);
    void healShip();

    int lives;
};

#endif //!SPACE_SHIPP_HPP