CC	:= gcc
CFLAGS	:= -Os -std=c99 -Wall -Wextra $(shell pkg-config --cflags --libs dbus-1)
GFX	= $(shell pkg-config --cflags --libs gtk+-3.0)
TARGET	= spm
RM	= rm -fr
CP	= cp -fr
MKDIR	= mkdir -p
PREFIX	= /usr/local/bin
SHARE	= /usr/share/spm
RES	= resources
SRC	= spm.c format.c logger.c power_options.c
GFXSRC	= gui.c
OBJ	= ${SRC:.c=.o}
GFXOBJ	= ${GFXSRC:.c=.o}

all : ${OBJ}
	${CC} ${CFLAGS} ${GFX} ${SRC} ${GFXSRC} -D_ENABLE_GUI -o ${TARGET}

nogui : ${OBJ}
	${CC} ${CFLAGS} ${SRC} -o ${TARGET}

clean :
	${RM} ${OBJ} ${GFXOBJ}

install :
	${MKDIR} ${SHARE}
	${CP} ${RES} ${SHARE}
	${CP} ${TARGET} ${PREFIX}

uninstall :
	${RM} ${PREFIX}/${TARGET}
	${RM} ${SHARE}

.PHONY : clean install uninstall