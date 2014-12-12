CPP_FILES = $(wildcard src/*.cpp)
OBJ_FILES = $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
LD_FLAGS = -Wall
CC_FLAGS = -Wall
CC = g++

all: main

main: $(OBJ_FILES)
	$(CC) $(LD_FLAGS) -o $@ $^

obj/%.o: src/%.cpp
	$(CC) $(CC_FLAGS) -c -o $@ $<

CC_FLAGS += -MMD
-include $(OBJFILES:.o=.d)

clean:
	rm -f $(OBJ_FILES)
