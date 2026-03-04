# Secure Data Handling Lab

#### Learning Objectives
* By completing this project, you will be able to:
* Reason about ownership and lifetime of heap-allocated structures.
* Identify and fix security-relevant memory bugs such as:
* memory leaks,
* use-after-free,
* double free,
* lost pointers.
* Apply dynamic analysis to validate memory safety.
* Implement safe creation, update, and destruction of data structures.
* Understand why incorrect data handling is a security problem, not just a correctness issue.


#### Tools to use
* GCC for compiling
* GDB for debugging
* Valgrind for memory leaks


#### General Rules
* You must not change the public API (function names, signatures).
* You must not change the CLI behavior.
* Your fixes must be done in:
* session.c
* store.c
* You may update headers only if strictly necessary for correctness.


#### Compilation command
```
gcc -std=gnu89 -Wall -Wextra -Werror -pedantic
```


