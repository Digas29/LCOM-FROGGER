#pragma once


#define IRQ_RTC 8
#define BIT(n) (0x01<<(n))
#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71
#define SECONDS 0
#define SECONDS_A 1
#define MINUTES 2
#define MINUTES_A 3
#define HOURS 4
#define HOURS_A 5
#define WEEK_DAY 6
#define MONTH_DAY 7
#define MONTH 8
#define YEAR 9
#define REG_A 10
#define REG_B 11
#define REG_C 12
#define REG_D 13
#define RTC_UIP BIT(7)
#define RTC_UF BIT(4)
#define RTC_AF BIT(5)
#define RTC_PF BIT(6)
#define PIE BIT(6)
#define AIE BIT(5)
#define UIE BIT(4)


int subscribe_RTC(void);
int unsubscribe_RTC();
void wait_valid_rtc(void);
void RTC_IH();
void setupRTCInteruptions();
void stopRTCInteruptions();
void programDeltaAlarm(int hours, int minutes, int seconds);
void getDate(unsigned long *day, unsigned long *month, unsigned long *year);
void getHour(unsigned long *seconds, unsigned long *minutes, unsigned long *hour);
