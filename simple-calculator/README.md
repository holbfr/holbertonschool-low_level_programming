## Simple calculator

#### Introduction
* This project is a terminal-based interactive calculator using the C programming language.

#### Project Expectations
* You are given requirements and observable behaviors, not step-by-step instructions.
* You are expected to research how input works in C and apply that knowledge.
* You must implement the program incrementally, keeping it working at every step.
* This project is intentionally open in implementation. Different correct solutions are possible.

#### Compilation command
```
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 calculator.c -o calculator
```

#### Usage
* Execute the program
```
$ ./calculator
```
* Outcome
```
Simple Calculator
1) Add
2) Subtract
3) Multiply
4) Divide
0) Quit
Choice: 0
Bye!
```

#### Supported Operations
* Addition with the + operator
* Substraction with the - operator
* Multiplication with the * operator
* Division with the / operator

#### Known limitations
* For now the program handles only numeric values.