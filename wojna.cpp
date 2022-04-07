#define _XOPEN_SOURCE
#include <mpi.h>

#include "msg_handler.hpp"
#include "docks_manager.hpp"
#include "simulation.hpp"

#include <thread>

int main(int argc, char **argv)
{

    // setup
    srand(time(NULL));

    int size, rank;
    SpaceShip mySpaceShip;

    // tworzenie watku
    int provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    if (provided != MPI_THREAD_MULTIPLE)
    {
        perror("Brak wsparcia dla wątków");
        MPI_Finalize();
        exit(0);
    }
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    DockManager manager(size, rank);

    std::thread watek_msg_handler(msg_handler, std::ref(rank), std::ref(size), std::ref(manager));

    while (true)
    {
        // sprawdzamy czy statek ma pelne zycie i moze sie bic
        if (mySpaceShip.canGoFight())
        {
            Simulation::go_to_war(mySpaceShip, rank);
        }
        // czekamy az uda nam sie dostac do sekcji krytycznej dokow

        while (!(manager.startDocking()))
        {

        }
        // czekamy az uda nam sie dostac do mechanikow, i ich liczba bedzie odpowiadac naszym potrzeba
        while (!(manager.takeMechanick(mySpaceShip.howManyMechanicks())))
        {
            
        }
        // gdy udalo nam sie przejsc obie sekcje krytyczne
        Simulation::heal(mySpaceShip, rank);
        
        // czekamy az uda nam sie dostac do mechanikow, i zwracamy liczbe zaporzyczona na naprawy
        while (!(manager.releaseMechanic()))
        {
        }
        // wychodzimy z sekcji krytycznej K dokow
        manager.endDocking();
    }
    // zamkniecie watku msg_handler i koniec programu
    watek_msg_handler.join();
    printf("join\n");
    MPI_Finalize();
}
