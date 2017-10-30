#include <Arduino.h>
#include "shared.h"
#include "states.h"

// the setup routine runs once when you press reset:
void setup() {
	initial_setup();	// set up the MCU

	// power-on flash
	flash_on_command();

	// pre-spool LFSR
	prespool_lfsr();

	// set our initial state
	curr_state = STATE_LFSR_RIGHT;
	//curr_state = STATE_CYLON;
	//curr_state = STATE_VUMETER;
	//curr_state = STATE_STANDBY;

	state_enter();
}


void loop() {

	state_loop();

	if (curr_state != STATE_STANDBY)
	{
		if (check_input_button())
		{
			flash_on_command();
			state_next();
		}
	}

}
