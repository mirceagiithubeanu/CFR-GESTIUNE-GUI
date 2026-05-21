.PHONY: all clean

CC := g++
TARGET := cfr

SRC := $(wildcard *.cpp)
HEADS := $(wildcard *.h)
OBJS := $(SRC:%.cpp=%.o)

all: $(TARGET)

%.o: %.cpp $(HEADS)
	$(CC) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

clean:
	@echo 'deleting .o files'
	@rm -rf $(OBJS) $(TARGET)


