Compiler for sub-set of C. 
- Notes:
  - Up to while-loops
  - some LLVM header problems
 Technologies:
  - C++
  - C
  - Flex & Bison (Scanning and Lexical Analysis)
  - LLVM (IR and Object Code emission)
 ![image](https://github.com/user-attachments/assets/98d9a004-44f2-42d1-bd0d-24a4d53f3aa9)

Dependencies:
  - GCC/G++
  - Flex & Bison
  - LLVM
  - Makefile

Resources I used:
https://llvm.org/docs/tutorial/
https://www.gnu.org/software/bison/manual/bison.html 
https://stackoverflow.com/questions/56552396/using-variant-in-bison-and-flex
https://stackoverflow.com/questions/76509844/how-do-you-interface-c-flex-with-c-bison
Lex & Yacc - O'Reilly

Side Note:
- Flex and Bison interface with C++ causes parser to become re-entrant (seek bison 3.8.1 manual for more info)
