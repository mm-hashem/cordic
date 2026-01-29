CC = gcc
CFLAGS = -Wall -g
SOURCES = cordic.c cordic_test.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = cordic.exe

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@