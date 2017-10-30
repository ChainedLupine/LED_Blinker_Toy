#include <Arduino.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

/*
 * Mostly the same as avr/wdt.h, but puts WDT into interrupt mode instead of system reset mode
 */

void watchdog_disable()
{
    wdt_reset();
    cli();							// time critical
    MCUSR = 0x00;					// clear all MCU status flags
    // to change the WDT, we have to write WDCR/WDE to WDTCR and any prescalar changes within 4 cycles
    WDTCR |= _BV(WDCE) | _BV(WDE);	// write control register with change-enable and system-reset enable
    WDTCR = 0x00;					// turn off WDT entirely
    sei();							// done, re-enable all interrupts
}

void watchdog_enable(const uint8_t timeout_amount)
{
    wdt_reset();
    cli();											// time critical
    MCUSR = 0x00;									// clear all MCU status flags
    WDTCR |= _BV(WDCE) | _BV(WDE);					// see above on how to change WDT control register
    WDTCR = _BV(WDIE) | (timeout_amount & 0x07);	// set interrupt enable and prescalar
    sei();											// done, re-enable all interrupts
}

void watchdog_sleep(const uint8_t timeout_amount)
{
	watchdog_enable(timeout_amount);

	wdt_reset();

	set_sleep_mode (SLEEP_MODE_PWR_DOWN);	// set to completely power down during sleep
	sleep_mode();							// power down

	// at this point, we're asleep.  The MCU won't start running code until we've timed out
	// the watchdog, and the WDT interrupt fires.  At this point, code runs executing at next
	// statement.
	sleep_disable();
}

// interrupt vector for WDT, which just disables the WDT
ISR(WDT_vect)
{
	watchdog_disable();
}
