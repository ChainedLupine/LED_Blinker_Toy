#include <avr/power.h>
#include <avr/wdt.h>
#include "states.h"
#include "watchdog.h"
#include "lfsr.h"

 uint8_t curr_state = 0;

// cylon state info
uint8_t cylon_dir ;

// lfsr returns better values if we let it run for a bit first
void prespool_lfsr()
{
	for(uint8_t i = 0; i < 50; i++)
		next_lfsr();
}

// set up a state
void state_enter()
{
	switch(curr_state)
	{
		case STATE_STANDBY:
			clear_leds();
			power_all_disable();
			power_timer0_enable(); // just leave timer0 running, so loop() will still work

			break;

		case STATE_LFSR_LEFT:
		case STATE_LFSR_RIGHT:

			/*
			 * shift board_buff to left
			 * insert bit from LFSR to b[0]
			 *
			 * reverse for right
			 */

			break;

		case STATE_CYLON:

			/*
			 * turn on first 3 LEDs
			 * set dir=1
			 * if dir=1 cycle LEDs to right
			 *   if b[2]=0x7, then reverse
			 * if dir=0 cycle LEDs to left
			 *   if b[0]=0xE0, then reverse
			 */

			board_buff[0] = 0b11100000;
			board_buff[1] = 0b00000000;
			board_buff[2] = 0b00000000;

			send_to_leds();

			cylon_dir = 1;

			break;

		case STATE_VUMETER:

			/*
			 * pick random number between 8x(NUM_BOARDS)
			 * turn on bits up to number
			 * shift LEDs left until b[0] = 0
			 */

			set_uv(8 * NUM_BOARDS);

			break;

	}
}

// do any exit transition for states
void state_leave()
{
	switch(curr_state)
	{
		case STATE_STANDBY:
			power_all_enable();

			break ;
	}
}

void state_next()
{
	state_leave();
	curr_state++;
	if(curr_state > NUM_STATES - 1)
		curr_state = 0;
	state_enter();
}


void state_loop()
{
	switch(curr_state)
	{
		case STATE_STANDBY:
			// sleep for 1 second
			watchdog_sleep(WDTO_1S);
			// give us some slight time to settle
			delay(10);
			// check input, see if it's on, if so power up
			if (digitalRead(PIN_INPUT))
			{
				flash_on_command();
				delay(250);
				state_next();
			}

			break;

		case STATE_LFSR_LEFT:
		case STATE_LFSR_RIGHT:
			if (check_pot_timer())
			{
				send_to_leds();

				if (curr_state == STATE_LFSR_RIGHT)
					shift_leds_right();
				else
					shift_leds_left();

				next_lfsr();

				if (curr_state == STATE_LFSR_RIGHT)
				{
					// mask off the end
					board_buff[0] = board_buff[0] & 0x7F ;
					// insert a pseudo-random bit
					board_buff[0] |= ((lfsr_curr & 0x1) << 7) ;
				} else {
					// mask off the end
					board_buff[NUM_BOARDS-1] = board_buff[NUM_BOARDS-1] & 0xFE;
					// insert a pseudo-random bit
					board_buff[NUM_BOARDS-1] |= (lfsr_curr & 0x1);
				}
			}

			break;

		case STATE_CYLON:
			if (check_pot_timer())
			{
				send_to_leds();

				if (cylon_dir) // moving L to R
				{
					shift_leds_right();
					// if last 3 LEDs on last board is lit, time to reverse
					if (board_buff[2] == 0x07)
						cylon_dir = 0;
				}
				else // movingR to L
				{
					shift_leds_left();
					// if first 3 LEDs on first board is lit, time to reverse
					if (board_buff[0] == 0xE0)
						cylon_dir = 1;
				}
			}

			break ;

		case STATE_VUMETER:

			if (check_pot_timer())
			{
				// if we've run out of LEDs...
				if (board_buff[0] == 0)
				{
					// set UV level to some pseudo-random level
					set_uv(lfsr_curr % (NUM_BOARDS * 8));
					next_lfsr();
					// delay a tiny random bit, so it feels more natural
					delay(lfsr_curr % 100);
					next_lfsr();
				}

				shift_leds_left();
				send_to_leds();

			}

			break;
	}

}


