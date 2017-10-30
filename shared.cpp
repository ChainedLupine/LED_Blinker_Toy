#include <Arduino.h>
#include "shared.h"
#include "watchdog.h"

unsigned long last_delay_MS;
unsigned long last_input_MS;

uint16_t pot_val = 0;
uint8_t board_buff[NUM_BOARDS];

void initial_setup()
{
	watchdog_disable();

	#if CPU_16MHZ
	if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
	#endif

	// initialize digital pins
	pinMode(PIN_CLOCK, OUTPUT);
	pinMode(PIN_DATA_OUT, OUTPUT);
	pinMode(PIN_LATCH, OUTPUT);
	pinMode(PIN_INPUT, INPUT);

	last_delay_MS = last_input_MS = 0;
}

uint8_t check_pot_timer()
{
	pot_val = analogRead(PIN_POT);
	pot_val = map(pot_val, 0, 1023, 0, 255);
	if (pot_val < 10)
		pot_val = 10;

	if (millis() - last_delay_MS > pot_val) {
		last_delay_MS = millis();
		return true ;
	} else
		return false;
}

uint8_t check_input_button()
{
	if (digitalRead(PIN_INPUT))
	{
		if (last_input_MS == 0)
			last_input_MS = millis();

		if (millis() - last_input_MS > 100) {
			return true ;
		} else
			return false;
	} else {
		last_input_MS = 0 ;
		return false;
	}
}

void clock_out_data (uint8_t data)
{
	shiftOut(PIN_DATA_OUT, PIN_CLOCK, MSBFIRST, data);
}

void send_to_leds()
{
	digitalWrite(PIN_LATCH, LOW);

	for (int idx = NUM_BOARDS - 1; idx >= 0; idx--) {
		clock_out_data(board_buff[idx]);
	}

	digitalWrite(PIN_LATCH, HIGH);
}

void shift_leds_right()
{
	// 0000 0001 0001 1000
	// 0000 0001 0000 1100  [n]>>1
	// 0000 0001            v = [n-1]&1 = 1
	//           1000 1100  [n]=[n]|v>>7
	for (int i = NUM_BOARDS - 1; i >= 0; i--) {
		if (i > 0)
			board_buff[i] = (board_buff[i] >> 1) | ((board_buff[i-1] & 0x1) << 7);
		else
			board_buff[i] = (board_buff[i] >> 1);
	}
}

void shift_leds_left()
{
	// 0000 0001 1001 1000
	// 0000 0010 1001 1000  [0]<<1
	//           1001 1000  v = ([1]&80)>>7 = 1
	// 0000 0011            [0]=[0]|v
	for (int i = 0; i < NUM_BOARDS; i++) {
		if (i <= NUM_BOARDS - 1)
			board_buff[i] = (board_buff[i] << 1) | ((board_buff[i+1] & 0x80) >> 7);
		else
			board_buff[i] = (board_buff[i] << 1);
	}

}

void clear_leds()
{
	memset(&board_buff[0], 0, sizeof(board_buff));
	send_to_leds();
}

void flash_on_command()
{
	board_buff[0] = 0b00110011 ;
	board_buff[1] = 0b11001100 ;
	board_buff[2] = 0b00110011 ;

	send_to_leds();

	delay(250);

	board_buff[0] = 0b11001100 ;
	board_buff[1] = 0b00110011 ;
	board_buff[2] = 0b11001100 ;

	send_to_leds();

	delay(250);

	clear_leds();
}

uint8_t get_uv_pattern(uint8_t val)
{
	if (val > 8)
		val = 8 ;

	return pgm_read_byte(&(uv_pattern[val]));
}

void set_uv(uint8_t val)
{
	if (val > 16)
		board_buff[2] = get_uv_pattern(val - 16) ;
	else
		board_buff[2] = 0;

	if (val > 8)
		board_buff[1] = get_uv_pattern(val - 8) ;
	else
		board_buff[1] = 0 ;

	board_buff[0] = get_uv_pattern(val) ;

	send_to_leds();
}



