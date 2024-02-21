CC = gcc
CFLAGS = -Wall -Wextra -std=c99
SOURCES = functions.c OF.c GA.c
EXECUTABLE = myprogram
LIBS = -lm

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(SOURCES) -o $(EXECUTABLE) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -f $(EXECUTABLE)