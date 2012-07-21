CC 	    = gcc
CFLAGS  = -c -Wall -pipe -Wshadow
LDFLAGS = -Wl,--gc-sections
LIBS    =

SOURCES = main.c checkerboard.c heuristics.c moves.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = aimesbot

ifeq ($(DEBUG),y)
	CFLAGS += -g
	STRIP =
else
	CFLAGS += -O3 -march=native -fomit-frame-pointer
	STRIP = strip --strip-all $(EXECUTABLE)
endif

all: $(SOURCES) $(EXECUTABLE)
	$(STRIP)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(LIBS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o aimesbot
