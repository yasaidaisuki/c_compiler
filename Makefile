# Makefile for compiling the C++ compiler project

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall

# Source files
LEX_SRC = lex.l
LEX_OUTPUT = lex.yy.cc
PARSER_SRC = parser.y
PARSER_OUTPUT_CC = parser.tab.cc
PARSER_OUTPUT_HH = parser.tab.hh
MAIN_SRC = main.cc
AST_SRC = ast.h

# Output executable
TARGET = compiler

# Targets and rules
all: $(TARGET)

# Link the final executable
$(TARGET): $(MAIN_SRC) $(LEX_OUTPUT) $(PARSER_OUTPUT_CC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(MAIN_SRC) $(LEX_OUTPUT) $(PARSER_OUTPUT_CC) -lfl

# Generate the C++ source file from the Lex file
$(LEX_OUTPUT): $(LEX_SRC)
	flex -+ -o $(LEX_OUTPUT) $(LEX_SRC)

# Generate the C++ source and header files from the Bison file
$(PARSER_OUTPUT_CC) $(PARSER_OUTPUT_HH): $(PARSER_SRC)
	bison -d -o $(PARSER_OUTPUT_CC) $(PARSER_SRC)

# Clean up generated files
clean:
	rm -f $(TARGET) $(LEX_OUTPUT) $(PARSER_OUTPUT_CC) $(PARSER_OUTPUT_HH)

.PHONY: all clean
