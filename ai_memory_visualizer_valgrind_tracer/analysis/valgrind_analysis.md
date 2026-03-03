Your valgrind_analysis.md must:

* Correctly classify each Valgrind warning.
* Map each warning to a specific memory misuse.
* Use precise terminology (e.g., “use-after-free”, “leak due to lost ownership”).
* Include at least one instance where:
* An AI explanation was partially or fully incorrect.
* You explain why it is incorrect.

Copying Valgrind output without explanation is insufficient.

# Aliasing Example:
* The program doesn't compile so there's no possibility to check memory leaks.

---

# Crash Example:
* Issue : Invalid write of size 4
* Type of error : Invalid write - writing operation using a NULL pointer.
* Memory Object: The pointer nums (or similar), expected to point to a valid integer or array, was NULL at the time of access.
* Violation: Writing to a NULL pointer is undefined behavior and results in a segmentation fault.
* Severity: Critical (causes segmentation fault)
* Location: main function, line 32 in crash_example.c
* The AI gave a correct explanation for this issue.

---

# Stack Example :
* This program doesn't crash and it doesn't have any memory issues nor bugs.

---

# Heap example :
* Issue : Memory Leak
* Type of error : In the heap, 5 allocs took place with 4 frees instead of 5, which emplies a memory leak issue.
* Memory Object: the **alice** variable, which is a pointer to a Person (structure).
* Severity : Critical (Unfreed memory after it's been malloced).
* Location: Allocated in person_new (line 21, `heap_example.c`), called from main (line 51, `heap_example.c`).
* Violation: The pointer to this memory was lost, making it impossible to free. Because the **alice** is a pointer to struct and this struct contains 2 members one string (pointer to char) and one int. The function that frees the memory space allocated for the **alice** object doesn't free its member which is a pointer to character.
* The AI gave a correct explanation for this issue.
