## Secure Input & Memory Lab (Student Starter)

#### Introduction

* This repository contains intentionally vulnerable C code for analysis and patching.

* You will find the source files under `src/`.

* In this laboratory project, you will work with intentionally vulnerable C programs that misuse memory and input handling. Your task is to detect, analyze, and patch these issues using a combination of:
	* Manual code inspection
	* Runtime analysis with Valgrind
	* Structured reasoning about ownership, lifetimes, and bounds
	* AI-assisted explanations to articulate why a bug exists and how it was fixed


#### Compilation command

```
-std=gnu89 -Wall -Wextra -Werror -pedantic -g
```

* Use `-fsanitize=address` as optional exploration.

