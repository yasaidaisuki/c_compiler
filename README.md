# About
A subet compiler for C language that lexes, parses, performs semantic analysis, and generates LLVM IR.
Currently supports variable assignments, if statements, and returns.

![image](https://github.com/user-attachments/assets/98d9a004-44f2-42d1-bd0d-24a4d53f3aa9)
# Technologies:
- C++
- C
- Flex & Bison (Scanning and Lexical Analysis)
- LLVM (IR and Object Code emission)

# Dependencies:
  - GCC/G++
  - Flex & Bison
  - LLVM
  - Makefile

# Build Instructions:
```
cmake
./compiler <input file> -o <output file>
```

# Resources:
  - https://llvm.org/docs/tutorial/
  - https://www.gnu.org/software/bison/manual/bison.html
  - https://stackoverflow.com/questions/56552396/using-variant-in-bison-and-flex
  - https://stackoverflow.com/questions/76509844/how-do-you-interface-c-flex-with-c-bison
  - Lex & Yacc - O'Reilly
