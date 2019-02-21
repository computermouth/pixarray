
#ifndef _FILES_H_
#define _FILES_H_

#include <stdint.h>

#include "states.h"

int verify_or_create_save(game_state_t *);
int msave(game_state_t *);
int mread();
int mload(game_state_t *);

#endif
