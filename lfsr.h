#ifndef LFSR_H_
#define LFSR_H_

// linear-feedback shift register
static uint16_t lfsr_start = 0x01;
static uint16_t lfsr_curr = lfsr_start;
static uint16_t lfsr_bit;

inline static void next_lfsr() {
	// taps: 16 14 13 11, feedback: x^16 + x^14 + x^13 + x^11 + 1
	lfsr_bit = ((lfsr_curr >> 0) ^ (lfsr_curr >> 2) ^ (lfsr_curr >> 3)
			^ (lfsr_curr >> 5)) & 1;
	lfsr_curr = (lfsr_curr >> 1) | (lfsr_bit << 15);
}

#endif /* LFSR_H_ */
