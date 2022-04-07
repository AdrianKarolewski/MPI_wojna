#ifndef MSG_HANDLER_HPP
#define MSG_HANDLER_HPP

#include "docks_manager.hpp"
#include "mechanics_manager.hpp"

void msg_handler(const int & _rank, const int & _size, DockManager & dockManager, MechanicsManager & mechanicsManager);

#endif //!MSG_HANDLER_HPP