#ifndef SHARED_H_
#define SHARED_H_

#include <Arduino.h>

#define NUM_BOARDS 3	// number of SN74HC595 shift register chips attached

//#define CPU_16MHZ			// set if we want to run the internal 16MHZ clock

// digital pins
#define PIN_DATA_OUT 	0
#define PIN_CLOCK		1
#define PIN_LATCH		2
#define PIN_INPUT		4

// analog pins
#define PIN_POT			3

// pattern to convert 8-bit value to LEDs
static const uint8_t uv_pattern[] PROGMEM = {
		0b00000000,  // 0
		0b10000000,  // 1
		0b11000000,  // 2
		0b11100000,  // 3
		0b11110000,  // 4
		0b11111000,  // 5
		0b11111100,  // 6
		0b11111110,  // 7
		0b11111111   // 8
} ;

// externs
extern unsigned long last_delay_MS;
extern unsigned long last_input_MS;

extern uint16_t pot_val;
extern uint8_t board_buff[NUM_BOARDS];

// prototypes
void initial_setup();
uint8_t check_pot_timer();
uint8_t check_input_button();
void clock_out_data (uint8_t data);
void send_to_leds();
void shift_leds_right();
void shift_leds_left();
void clear_leds();
void flash_on_command();
uint8_t get_uv_pattern(uint8_t val);
void set_uv(uint8_t val);
void prespool_lfsr();



#endif /* SHARED_H_ */
