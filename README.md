# C Compiler

An end-to-end compiler for a subset of C, written in C++17 — from Flex/Bison front end through semantic analysis to LLVM IR and native object code.

## Example

**Input** (`examples/add.c`):

```c
int whatIsOnePlusOne() {
  int a = 1;
  int b = 1;
  return a + b;
}
```

**Generated LLVM IR** (`./compiler examples/add.c -emit-llvm`):

```llvm
; ⚠️ REPLACE ME: paste the actual output of your compiler here
define i32 @whatIsOnePlusOne() {
entry:
  %a = alloca i32
  store i32 1, i32* %a
  %b = alloca i32
  store i32 1, i32* %b
  %0 = load i32, i32* %a
  %1 = load i32, i32* %b
  %sum = add i32 %0, %1
  ret i32 %sum
}
```

Because the backend is LLVM, the emitted IR gets optimization passes and multi-target code generation for free.

## Architecture

![Compiler architecture](https://github.com/user-attachments/assets/98d9a004-44f2-42d1-bd0d-24a4d53f3aa9)

The compiler uses a **two-pass design**:

1. **Pass 1 — Symbol collection.** Walks the Bison-generated AST to build the symbol table (functions, variables, types) before any code is analyzed, so forward references resolve correctly.
2. **Pass 2 — Semantic analysis & code generation.** Type checking, type inference, and scope resolution against the symbol table, then LLVM IR emission node-by-node.

### Pipeline

| Stage | Tool | Output |
|---|---|---|
| Lexical analysis | Flex | Token stream |
| Parsing | Bison | Abstract Syntax Tree |
| Semantic analysis | C++17 visitor over AST | Typed, validated AST + symbol table |
| Code generation | LLVM C++ API | LLVM IR → object file |

## Key Design Decisions

**Polymorphic AST via `std::variant`.** The AST spans 15+ node types (`BinaryExp`, `FunctionDef`, `WhileStatement`, ...) stored in `std::variant` rather than a virtual-dispatch class hierarchy. This gives:

- **Type safety at compile time** — `std::visit` forces exhaustive handling of every node type, so adding a node type surfaces every site that must handle it as a compile error, not a runtime bug
- **Zero-overhead storage** — no vtable pointers or heap-allocated base-class indirection; nodes support move semantics for cheap tree construction

**Two passes instead of one.** Separating symbol collection from analysis keeps each pass simple and makes forward-declared functions work without backpatching.

**LLVM as the backend.** Emitting IR instead of assembly delegates register allocation, optimization, and target selection to LLVM — the compiler stays focused on the front end and semantics.

## Language Support

Currently implemented:

- Function definitions and calls
- Control flow: `if`/`else`, `while`
- Arithmetic and logical operators
- Integer types, local variables, type inference

Not (yet) supported: pointers, arrays, structs, floating point, preprocessor.

## Building & Running

### Prerequisites

```bash
sudo apt-get install flex bison llvm-dev g++ make
```

### Build

```bash
make clean && make
```

### Usage

```bash
./compiler input.c -o output.o     # Compile to object file
./compiler input.c -emit-llvm      # Emit LLVM IR to stdout
gcc output.o -o executable && ./executable
```

## Resources

- [LLVM Kaleidoscope Tutorial](https://llvm.org/docs/tutorial/)
- [Bison Manual](https://www.gnu.org/software/bison/manual/bison.html)
- *lex & yacc* — O'Reilly

## Author

**Dami Peng** — [LinkedIn](https://www.linkedin.com/in/dami-peng/) · [yasaidaisuki.github.io](https://yasaidaisuki.github.io/)
