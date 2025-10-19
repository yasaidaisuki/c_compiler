# C Compiler

A full compiler implementation for a C-like language, featuring lexical analysis, parsing, semantic analysis, and LLVM IR code generation.

## Features

- **Lexical Analysis** – Flex-based tokenization of source code
- **Parsing** – Bison parser generating Abstract Syntax Tree (AST)
- **Semantic Analysis** – Type checking, symbol table management, type inference
- **Code Generation** – LLVM IR emission for target-independent optimization
- **Supported Language Features** – Function definitions, control flow (if/while), arithmetic/logic operations, integer types

## Architecture
![image](https://github.com/user-attachments/assets/98d9a004-44f2-42d1-bd0d-24a4d53f3aa9)

The AST is represented using C++17 `std::variant` for type-safe, zero-overhead abstraction across 15+ node types (BinaryExp, FunctionDef, WhileStatement, etc.).

## Technologies

- **Languages:** C++17, C
- **Lexer/Parser:** Flex & Bison
- **Backend:** LLVM (IR and object code emission)
- **Build:** Makefile, GCC/G++

## Building & Running

### Prerequisites
```bash
sudo apt-get install flex bison llvm-dev g++ make
```

### Build
```bash
make clean
make
```

### Usage
```bash
./compiler input.c -o output.o          # Compile to object file
./compiler input.c -emit-llvm           # Emit LLVM IR
gcc output.o -o executable && ./executable
```

### Example Input
```c
int whatIsOnePlusOne {
  int a = 1;
  int b = 1;
  return a + b;
}
```
## Key Design Decisions

1. **Polymorphic AST with std::variant** – Enables type-safe node storage without runtime overhead
2. **Two-pass compilation** – First pass for symbol collection, second for semantic analysis and code generation
3. **LLVM backend** – Allows optimization passes and multi-target code generation

## Resources

- [LLVM Tutorial](https://llvm.org/docs/tutorial/)
- [Bison Manual](https://www.gnu.org/software/bison/manual/bison.html)
- Lex & Yacc - O'Reilly

## Author

Dami Peng
