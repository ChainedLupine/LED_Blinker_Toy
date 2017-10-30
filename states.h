#ifndef STATES_H_
#define STATES_H_

#include "shared.h"

/* patterns
 *
 * 1: lfsr shift left
 * 2: lfsr shift right
 * 3: cylon bar
 * 		just move 3 bits right to left, left to right
 * 4: volume bar
 * 		set to level n
 * 		slowly decay n to 0
 * 		once 0, repeat
 */

#define STATE_STANDBY 		0
#define STATE_LFSR_RIGHT	1
#define STATE_LFSR_LEFT		2
#define STATE_CYLON			3
#define STATE_VUMETER		4

#define NUM_STATES 5

extern uint8_t curr_state;

// cylon state info
extern uint8_t cylon_dir;

// prototypes
void prespool_lfsr();
void state_enter();
void state_leave();
void state_next();
void state_loop();


#endif /* STATES_H_ */
