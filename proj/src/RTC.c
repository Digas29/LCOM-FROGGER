#include "RTC.h"
#include "colors.h"
#include "strings.h"
#include <stdio.h>
#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>

unsigned int hookID;
void enable(){
	asm("sti");
}
void disable(){
	asm("cli");
}

unsigned long bcdToBin(unsigned long bcd){
	return ((bcd >>4 )*10)+(bcd & 0x0F);
}
unsigned long binToBCD(unsigned long bin){
	unsigned long u,d;
	u = bin % 10;
	d = bin / 10;
	d = d << 4;
	return d + u;
}

int subscribe_RTC(void) {
	hookID = 8;
	unsigned long bitmask = BIT(hookID);

	if (sys_irqsetpolicy(IRQ_RTC, IRQ_REENABLE | IRQ_EXCLUSIVE, &hookID) != OK || sys_irqenable(&hookID) != OK) {
		return -1;
	}
	return bitmask;
}

int unsubscribe_RTC() {
	return sys_irqdisable(&hookID) != OK || sys_irqrmpolicy(&hookID) != OK;
}
void RTC_IH(){
	unsigned long regC;

	sys_outb(RTC_ADDR_REG, REG_C);
	sys_inb(RTC_DATA_REG, &regC);

	if(regC & RTC_UF)
		drawString("update", 640,300,green);
	if(regC & RTC_AF)
		drawString("alarm", 640,400,green);
	if(regC & RTC_PF)
		drawString("period", 640,500,green);

}

void setupRTCInteruptions(){

	unsigned long regB = 0;

	disable();
	sys_outb(RTC_ADDR_REG, REG_B);
	sys_inb(RTC_DATA_REG, &regB);
	if(!(regB & AIE)){
		regB |= AIE;
		sys_outb(RTC_ADDR_REG, REG_B);
		sys_outb(RTC_DATA_REG, regB);
	}
	enable();
}

void wait_valid_rtc(void) {
	unsigned long regA = 0;
	do {
		disable();
		sys_outb(RTC_ADDR_REG, REG_A);
		sys_inb(RTC_DATA_REG, &regA);
		enable();
	} while ( regA & RTC_UIP);
}

void getHour(unsigned long *seconds, unsigned long *minutes, unsigned long *hour){
	wait_valid_rtc();
	disable();
	sys_outb(RTC_ADDR_REG, SECONDS);
	sys_inb(RTC_DATA_REG, seconds);
	sys_outb(RTC_ADDR_REG, MINUTES);
	sys_inb(RTC_DATA_REG, minutes);
	sys_outb(RTC_ADDR_REG, HOURS);
	sys_inb(RTC_DATA_REG, hour);
	*seconds = bcdToBin(*seconds);
	*minutes = bcdToBin(*minutes);
	*hour = bcdToBin(*hour);
	enable();
}
void getDate(unsigned long *day, unsigned long *month, unsigned long *year){
	wait_valid_rtc();
	disable();
	sys_outb(RTC_ADDR_REG, MONTH_DAY);
	sys_inb(RTC_DATA_REG, day);
	sys_outb(RTC_ADDR_REG, MONTH);
	sys_inb(RTC_DATA_REG, month);
	sys_outb(RTC_ADDR_REG, YEAR);
	sys_inb(RTC_DATA_REG, year);
	*day = bcdToBin(*day);
	*month = bcdToBin(*month);
	*year = bcdToBin(*year);
	enable();
}
void programDeltaAlarm(int hours, int minutes, int seconds){
	unsigned long s, m, h;
	getHour(&s,&m,&h);
	s +=seconds;
	if(s >= 60){
		m++;
		s -= 60;
	}
	m+=minutes;
	if(minutes >= 60){
 		h++;
		m -= 60;
	}
	if(hours>=60){
		hours-=60;
	}
	s = binToBCD(s);
	m = binToBCD(m);
	h = binToBCD(h);
	disable();
	sys_outb(RTC_ADDR_REG, SECONDS_A);
	sys_outb(RTC_DATA_REG, s);
	sys_outb(RTC_ADDR_REG, MINUTES_A);
	sys_outb(RTC_DATA_REG, m);
	sys_outb(RTC_ADDR_REG, HOURS_A);
	sys_outb(RTC_DATA_REG, h);
	enable();
}
