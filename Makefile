CC 	    = gcc
CFLAGS  = -c -Wall
LDFLAGS = -Wl,--gc-sections
LIBS    =

SOURCES = main.c checkerboard.c heuristics.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = aimesbot

ifeq ($(DEBUG),y)
	CFLAGS += -g
	STRIP =
else
	CFLAGS += -O3 -march=native
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
