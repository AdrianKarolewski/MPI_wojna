#include "spaceship.hpp"

void SpaceShip::makeDemage(const int & d)
{
    if(d > 0 && d < 11)
    {
        lives -= d;
    }
}
void SpaceShip::healShip()
{
    lives = MAX_LIVES;
}
int SpaceShip::howManyMechanicks()
{
    return MAX_LIVES - lives;
}
bool SpaceShip::canGoFight()
{
    return MAX_LIVES == lives;
}