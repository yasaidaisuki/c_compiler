# Variables
CXX = g++
CXXFLAGS = -std=c++20 -frtti -I/Users/yasai/llvm/include `llvm-config --cxxflags`
LDFLAGS = `llvm-config --ldflags --libs`
SOURCES = main.cc driver.cc scanner.cc parser.cc
OBJECTS = $(SOURCES:.cc=.o)
TARGET = my_compiler

# Build target
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(TARGET) $(OBJECTS)
