# Parser Lab Session 2 - (LA)LR programming language

## Task
Build LR or LALR parser for (reasonable subset of) some programming language
- Input is (unannotated) source code
- Output should be parse tree (make it an actual tree, but no other semantic actions needed)
- Error handling (or code generation) is not required, but appreciated

## Prerequisites
The tool used for generating the lexer is [Flex](https://github.com/westes/flex) and for the parser [GNU Bison](https://www.gnu.org/software/bison/) was used. To compile and run the software the following tools are needed
- Flex 2.6.4
- GNU Bison (at least version 3.2)
- [CMake](https://cmake.org/download/) (at least version 3.7)
- C++ compiler supporting C++17 (e.g. Apple Clang [used to develop this program], Clang, g++, MSVC)

## Build
To build the project run
```shell
cmake -B bld
cmake --build bld
```

## Usage
After compilation the binary is located in the `bld` directory. It only supports single file programs. Sample programs can be found in the subdirectory `samples`. The code file is passed as an argument to the binary and the parse tree gets printed to the next line and the program terminates.
```shell
./bld/spl_parser samples/division.spl
PROGRAM(COMP_STMT(COMP_STMT(ASSING_STMT(ID(q),CONST(0)),ASSING_STMT(ID(r),ID(m))),WHILE_STMT(B_EXPR(ID(r),>=,ID(n)),COMP_STMT(ASSING_STMT(ID(q),AR_EXPR(ID(q),+,CONST(1))),ASSING_STMT(ID(r),AR_EXPR(ID(r),-,ID(n)))))))
```

## Accepted programs
The progam accepts source code written in the sample programming langauage as described in semantics of programming languages.
```
<program> ::= <stmt>
<stmt> ::= <comp stmt> | <while stmt> | <if stmt> | <assign stmt> | <null stmt>
<comp stmt> ::= "begin" <stmt> ";" <stmt> "end"
<while stmt> ::= "while" <bexpr> "do" <stmt> "od"
<if stmt> ::= "if" <bexpr> "then" <stmt> "else" <stmt> "fi"
<assign stmt> ::= <id> ":=" <arexpr>
<null stmt> ::= "skip"
<bexpr> ::= <arexpr> <relopr> <arexpr>
<relopr> ::= "<" | "<=" | ">" | ">=" | "=" | "!="
<arexpr> ::= <opd> | <mopr> <opd> | <opd> <dopr> <opd>
<mopr> ::= "+" | "-"
<dopr> ::= "+" | "-" | "*"
<opd> ::= <id> | <const> | "(" <arexpr> ")"
<id> ::= <letter> | <id> <char>
<char> ::= <letter> | <digit> | "_"
<letter> ::= "a" | "b" | "c" | "d" | "e" | "f" | "g" | "h" | "i" | "j" | "k" | "l" | "m" | "n" | "o" | "p" | "q" | "r" | "s" | "t" | "u" | "v" | "w" | "x" | "y" | "z" | "A" | "B" | "C" | "D" | "E" | "F" | "G" | "H" | "I" | "J" | "K" | "L" | "M" | "N" | "O" | "P" | "Q" | "R" | "S" | "T" | "U" | "V" | "W" | "X" | "Y" | "Z"
<const> ::= <digit> | <non zero digit> <const>
<digit> ::= "0" | <non zero digit>
<non zero digit> ::= "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
```
Additionally line comments starting with `//` and block comments in the form of `/* ... */` are valid but will be ignored.

## Tools used for solution
The lexer is generated using Flex, used in version 2.6.4 during development. The parser is generated using GNU Bison, used in version 3.8.2 during development. The build system used for the project is CMake, during development used in version 3.29.5. the C++ compiler which was used for development and testing is AppleClang version 15.0.0.15000309.

## Statements about final CFG
