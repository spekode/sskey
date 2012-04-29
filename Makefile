CC=gcc
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=sskey.c base32.c mt19937ar.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=sskey

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS) $(EXECUTABLE)
