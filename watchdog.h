#ifndef WATCHDOG_H_
#define WATCHDOG_H_

void watchdog_disable();
void watchdog_enable(const uint8_t timeout_amount);
void watchdog_sleep(const uint8_t timeout_amount);

#endif /* WATCHDOG_H_ */
