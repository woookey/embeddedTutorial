#ifndef TUT4_COMMS_H
#define TUT4_COMMS_H

#include "comms_cmd.h"

/// \brief initialise communication stack
void comms_init(void);

/// \brief handles communication transmission and received commands
///        to be called at specific frequency to maintain synchronicity
void comms_handle(void);

/// \brief callback to process the recent command
/// \param current_cmd last command received
/// \note implement in the application
extern void comms_process_cmd(comms_cmd_t* current_cmd);

#endif