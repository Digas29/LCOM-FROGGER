#ifndef __TIMER_H
#define __TIMER_H

/** @defgroup timer timer
 * @{
 *
 * Functions for using the i8254 timers
 */

#define TIMER0_IRQ 0

#define BIT(n) (0x01<<(n))

typedef struct{
	int counter;
	int ticked;
}Timer;

/**
 * @brief Subscribes and enables Timer 0 interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */

Timer * newTimer();
void timerHandler(Timer * timer);
void resetTimerFlag(Timer* timer);
void deleteTimer(Timer * timer);

int subscribe_timer(void );

/**
 * @brief Unsubscribes Timer 0 interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int unsubscribe_timer();

#endif /* __TIMER_H */
