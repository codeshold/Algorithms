CC = g++
CFLAGS = -std=c++11 -Wall -O2
TARGET = antSearch

SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %cpp, %o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf *.o $(TARGET)
