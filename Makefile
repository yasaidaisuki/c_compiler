CXX = g++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS =

SRC = main.cpp Token.cc lexer.cc
OBJ = $(SRC:.cpp=.o)
TARGET = my_program

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)