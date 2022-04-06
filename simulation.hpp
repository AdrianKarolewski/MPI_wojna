#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "spaceship.hpp"

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <unistd.h>

#define M_MECHANICS 100
#define K_DOCKS 10

class Simulation
{
public:
    static void go_to_war(SpaceShip &ship, const int &rank)
    {
        double sleep_time = (static_cast<double>(rand()) / RAND_MAX);

        sleep_time += rank + 1;
        sleep_time /= rank + 1;
        ship.makeDemage(((rand() + rank) % MAX_LIVES) + 1);
        sleep(sleep_time);

        printf("Jesetm po bitwie proces nr %d potrzebuje %d mechanikow bilem sie %lf s\n", rank, ship.howManyMechanicks(), sleep_time);
    }
    static void heal(SpaceShip &ship, const int &rank)
    {
        sleep(ship.howManyMechanicks() / 2);
        ship.healShip();
    }
};


#endif //! SIMULATION_HPP