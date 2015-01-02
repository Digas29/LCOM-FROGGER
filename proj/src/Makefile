COMPILER_TYPE = gnu

CC = gcc

PROG = proj
SRCS = main.c sprite.c vbe.c graphics.c colors.c bitmap.c frogger.c keyboard.c mouse.c timer.c
SRCS +=  MainMenu.c rectangle.c game.c car.c log.c turtles.c strings.c RTC.c settings.c path.c
SRCS += highscores.c newRecord.c
CCFLAGS= -Wall -O3

DPADD += ${LIBDRIVER} ${LIBSYS}
LDADD += -llm -ldriver -lsys

LDFLAGS += -L.

MAN=
BINDIR? = /usr/sbin

.include <bsd.prog.mk>
.include <bsd.gcc.mk>