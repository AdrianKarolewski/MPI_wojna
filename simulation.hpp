#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "spaceship.hpp"

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <unistd.h>

class Simulation
{
public:
    static void go_to_war(SpaceShip &ship, const int &rank)
    {
        double sleep_time = (static_cast<double>(rand()) / RAND_MAX);

        sleep_time += 1 - (rank / 4);
        sleep_time /= 2;
        ship.makeDemage(((rand() + rank) % MAX_LIVES) + 1);
        sleep(sleep_time);
        printf("Jesetm po bitwie proces nr %d potrzebuje %d mechanikow bilem sie %lf s\n", rank, ship.howManyMechanicks(), sleep_time);
        return;
    }
    static void heal(SpaceShip &ship, const int &rank)
    {
        printf("Zaczynam sie leczyc proces %d \n", rank);
        sleep(static_cast<double>(ship.howManyMechanicks() / 4));
        ship.healShip();
    }
};


#endif //! SIMULATION_HPP