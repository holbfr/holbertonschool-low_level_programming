# ALIASING EXAMPLE - MEMORY MAP

We will analyze the program **step by step**, tracking:

* Stack frames
* Heap allocations
* Pointer aliasing
* Object lifetimes
* Undefined behavior

---

# 🔎 Program Overview

There are two functions:

* `make_numbers(int n)`
* `main(void)`

The key focus of this program is:

* Pointer aliasing (`b = a`)
* Dangling pointer after `free`
* Use-after-free (undefined behavior)

---

# ✅ STEP 1 — Program Start

## Stack

```
STACK
-----
Frame: main
    int *a = NULL
    int *b = NULL
    int n = 5
```

## Heap

```
HEAP
-----
(empty)
```

### Lifetimes

* `a`, `b`, `n` live until `main` returns.
* No heap objects yet.

---

# ✅ STEP 2 — Call to make_numbers(n)

`a = make_numbers(n);`

---

## New Stack Frame Created

```
STACK
-----
Frame: make_numbers
    int n = 5
    int i = 0
    int *arr = NULL

Frame: main
    a = NULL
    b = NULL
    n = 5
```

---

# ✅ STEP 3 — malloc Execution

```
arr = malloc(5 * sizeof(int));
```

Assume:

* `sizeof(int) = 4`
* Allocation size = 20 bytes
* Heap returns address `0x1000`

---

## Memory State

```
STACK
-----
Frame: make_numbers
    n = 5
    i = 0
    arr = 0x1000

Frame: main
    a = NULL
    b = NULL
    n = 5


HEAP
-----
0x1000:
    [ ? ][ ? ][ ? ][ ? ][ ? ]
```

### Lifetime

* Heap block at `0x1000` becomes valid.
* Owned by `arr` (currently).

---

# ✅ STEP 4 — Initialization Loop

```
for (i = 0; i < 5; i++)
    arr[i] = i * 11;
```

Heap becomes:

```
HEAP
-----
0x1000:
    [ 0 ][ 11 ][ 22 ][ 33 ][ 44 ]
```

---

# ✅ STEP 5 — Return arr

`return arr;`

### Important:

* Stack frame `make_numbers` is destroyed.
* `arr` variable dies.
* The heap memory remains alive.
* Returned pointer value (0x1000) is copied into `a`.

---

## After Return

```
STACK
-----
Frame: main
    a = 0x1000
    b = NULL
    n = 5

HEAP
-----
0x1000:
    [ 0 ][ 11 ][ 22 ][ 33 ][ 44 ]
```

### Lifetime

* Heap block still valid.
* Now owned by `main` via `a`.

---

# ✅ STEP 6 — Pointer Aliasing

```
b = a;
```

Now:

```
STACK
-----
Frame: main
    a = 0x1000
    b = 0x1000
    n = 5
```

### 🔥 Aliasing

Both `a` and `b` point to the SAME heap object.

```
a ─┐
   ├──> 0x1000
b ─┘
```

There is only ONE allocation.

---

# ✅ STEP 7 — First printf (Safe Access)

```
a[2] = 22
b[2] = 22
```

Memory unchanged.

Still valid access.

---

# ✅ STEP 8 — free(a)

```
free(a);
```

### What happens:

* Heap block at `0x1000` is released.
* Memory becomes invalid.
* `a` and `b` STILL contain address 0x1000.
* Neither pointer is automatically set to NULL.

---

## Memory State After free

```
STACK
-----
Frame: main
    a = 0x1000   (DANGLING)
    b = 0x1000   (DANGLING)
    n = 5

HEAP
-----
0x1000:
    [ FREED MEMORY ]
```

### 🚨 Critical

Both `a` and `b` are now **dangling pointers**.

They point to memory that is no longer owned by the program.

---

# ❌ STEP 9 — Reading b[2]

```
printf("reading b[2]=%d\n", b[2]);
```

This is:

### 🔴 Use-After-Free

You are reading from freed memory.

This is **undefined behavior**.

Possible outcomes:

* It prints 22 (memory not yet reused)
* It prints garbage
* It crashes
* Valgrind reports invalid read

Memory state is unchanged but invalid.

---

# ❌ STEP 10 — Writing b[3] = 1234

```
b[3] = 1234;
```

### 🔴 Use-After-Free (Write)

Even worse than reading:

* You are writing into memory that may belong to allocator metadata.
* This can corrupt the heap.
* Future malloc/free may crash.

Still:

```
a = 0x1000 (dangling)
b = 0x1000 (dangling)
```

---

# 📌 Final State Before Program Exit

```
STACK
-----
Frame: main
    a = 0x1000 (dangling)
    b = 0x1000 (dangling)
    n = 5

HEAP
-----
(no live allocations)
```

---

# 📊 Object Lifetime Summary

## Stack Objects

| Variable          | Lifetime           |
| ----------------- | ------------------ |
| make_numbers::i   | Function call only |
| make_numbers::arr | Function call only |
| main::a           | Entire main        |
| main::b           | Entire main        |
| main::n           | Entire main        |

All stack variables are automatically destroyed at scope exit.

---

## Heap Object

| Address | Allocated              | Freed           | Valid Period       |
| ------- | ---------------------- | --------------- | ------------------ |
| 0x1000  | malloc in make_numbers | free(a) in main | From malloc → free |

After `free(a)`:

* Object lifetime ends.
* Any access via `a` or `b` is undefined.

---

# 🔥 Pointer Aliasing Timeline

1. `arr` → 0x1000
2. returned into `a`
3. `b = a`
4. free(a)
5. `a` and `b` both dangling

Important insight:

Freeing through **one alias invalidates all aliases**.

---

# 🚨 Undefined Behaviors in This Program

1. Reading `b[2]` after free
2. Writing `b[3]` after free

Both are:

* Use-after-free
* Undefined behavior
* Correctly detected by Valgrind

---

# 💡 Key Memory Safety Lessons

* `free()` does NOT nullify pointers.
* Aliasing increases risk.
* After `free(ptr)`:

  * Set all aliases to NULL if possible.
* Ownership discipline is essential.

---

If you’d like, I can now:

* Draw this as a time-progression memory diagram,
* Or analyze a more complex example (double free, realloc, struct, etc.).

---
---

# CRASH EXAMPLE - MEMORY MAP

We analyze this program step-by-step, focusing on:

* Stack frames
* Heap allocations
* Pointer values
* Object lifetimes
* Exact crash point

This example demonstrates a **deterministic NULL dereference**.

---

# 🔎 PROGRAM OVERVIEW

Two functions:

* `allocate_numbers(int n)`
* `main(void)`

Key issue:

```c
nums = allocate_numbers(n);
nums[0] = 42;   // ← crash here
```

Since `n = 0`, `allocate_numbers` returns `NULL`.

---

# ✅ STEP 1 — Program Start

## Stack

```
STACK
-----
Frame: main
    int *nums = NULL
    int n = 0
```

## Heap

```
HEAP
-----
(empty)
```

### Lifetimes

* `nums`, `n` live for entire `main`.
* No heap objects yet.

---

# ✅ STEP 2 — Call allocate_numbers(n)

```
nums = allocate_numbers(n);
```

---

## New Stack Frame Created

```
STACK
-----
Frame: allocate_numbers
    int n = 0
    int *arr = NULL
    int i = 0

Frame: main
    nums = NULL
    n = 0
```

---

# ✅ STEP 3 — Early Return Path

Inside `allocate_numbers`:

```c
if (n <= 0)
    return NULL;
```

Since `n == 0`, the function returns immediately.

### Important observations:

* `malloc` is NEVER called.
* No heap allocation occurs.
* `arr` stays NULL.
* No loop executes.

---

## After Return to main

`allocate_numbers` stack frame is destroyed.

```
STACK
-----
Frame: main
    nums = NULL
    n = 0

HEAP
-----
(empty)
```

### Lifetime notes

* `allocate_numbers` local variables (`arr`, `i`) are destroyed.
* No heap object exists.
* `nums` now explicitly holds `NULL`.

---

# ❌ STEP 4 — NULL Dereference

```c
nums[0] = 42;
```

Equivalent to:

```c
*(nums + 0) = 42;
```

Since:

```
nums == NULL
```

This becomes:

```
*(NULL) = 42;
```

### 🔴 This is undefined behavior.

On almost all systems:

* NULL is address `0x0`
* Writing to address `0x0` causes a **segmentation fault**
* The crash is deterministic

---

## Memory State at Crash

```
STACK
-----
Frame: main
    nums = NULL
    n = 0

HEAP
-----
(empty)
```

No heap corruption.
No dangling pointers.
Just a NULL dereference.

---

# 🚨 Why This Always Crashes

Unlike use-after-free:

* Freed memory may still be mapped → unpredictable behavior.
* But NULL always points to an unmapped page.

Modern OS design:

* Page zero is protected.
* Accessing it triggers a hardware fault.

---

# ⚠️ What About free(nums)?

This line:

```c
free(nums);
```

Would have been safe because:

```c
free(NULL);  // defined behavior, does nothing
```

But the program never reaches it due to the crash.

---

# 📊 Object Lifetime Summary

## Stack Objects

| Variable              | Lifetime             |
| --------------------- | -------------------- |
| allocate_numbers::arr | During function call |
| allocate_numbers::i   | During function call |
| main::nums            | Entire main          |
| main::n               | Entire main          |

All destroyed at scope exit (or at crash).

---

## Heap Objects

None were ever allocated.

---

# 🔬 Pointer State Timeline

1. `nums = NULL`
2. `allocate_numbers(0)` returns NULL
3. `nums` remains NULL
4. `nums[0]` → dereference NULL → crash

No aliasing.
No heap activity.
No dangling pointer.

---

# 💥 Exact Failure Type

* Type: NULL pointer dereference
* Category: Undefined behavior
* Result: Segmentation fault
* Determinism: High (almost guaranteed crash)

---

# 🧠 Key Safety Lesson

Always validate returned pointers before dereferencing:

```c
nums = allocate_numbers(n);
if (!nums) {
    printf("allocation failed or invalid size\n");
    return 1;
}
```

Or validate `n` before calling.

---

If you'd like, the next program could explore:

* Double free
* Returning pointer to stack memory
* realloc pitfalls
* Structs with embedded pointers
* Memory leaks
* Function ownership contracts

Send the next one when ready.


---
---

# HEAP EXAMPLE - MEMORY MAP

This program demonstrates:

* Multiple heap allocations per object
* Nested ownership (`Person` owns `name`)
* Correct free for one object (`bob`)
* Deliberate memory leak for another (`alice->name`)

We will track everything precisely.

---

# 🔎 STRUCTURE LAYOUT

```c
typedef struct Person
{
    char *name;   // pointer (8 bytes on 64-bit)
    int age;      // 4 bytes
} Person;
```

On a typical 64-bit system:

* `char *name` → 8 bytes
* `int age` → 4 bytes
* 4 bytes padding for alignment

So:

```
sizeof(Person) = 16 bytes
```

---

# ✅ STEP 1 — Program Start

## Stack

```
STACK
-----
Frame: main
    Person *alice = NULL
    Person *bob   = NULL
```

## Heap

```
HEAP
-----
(empty)
```

---

# ✅ STEP 2 — alice = person_new("Alice", 30)

---

## 2.1 — Enter person_new

```
STACK
-----
Frame: person_new
    name = pointer to string literal "Alice"
    age = 30
    size_t i = 0
    size_t len = 0
    Person *p = uninitialized

Frame: main
    alice = NULL
    bob = NULL
```

### Important

The string `"Alice"` is a **string literal**:

* Stored in read-only static memory
* Lifetime: entire program
* Not heap allocated

---

## 2.2 — Allocate Person struct

```
p = malloc(sizeof(Person));
```

Assume heap returns: `0x1000`

```
HEAP
-----
0x1000 (16 bytes) → Person
    name = ?
    age  = ?
```

`p = 0x1000`

---

## 2.3 — Compute length of "Alice"

`len = 5`

---

## 2.4 — Allocate name buffer

```
p->name = malloc(len + 1);  // 6 bytes
```

Assume heap returns: `0x2000`

```
HEAP
-----
0x1000 → Person
    name = 0x2000
    age  = ?

0x2000 → 6 bytes
    [ ? ? ? ? ? ? ]
```

---

## 2.5 — Copy string

After copy:

```
0x2000:
    [ 'A' 'l' 'i' 'c' 'e' '\0' ]
```

Set age:

```
0x1000:
    name = 0x2000
    age  = 30
```

---

## 2.6 — Return p

Stack frame `person_new` destroyed.

---

## After Return to main

```
STACK
-----
Frame: main
    alice = 0x1000
    bob   = NULL
```

```
HEAP
-----
0x1000 → Person
    name = 0x2000
    age  = 30

0x2000 → "Alice"
```

Ownership:

* `alice` owns struct at 0x1000
* That struct owns name at 0x2000

---

# ✅ STEP 3 — bob = person_new("Bob", 41)

Repeat process.

Assume allocations:

* Person at `0x3000`
* Name at `0x4000`

After creation:

```
STACK
-----
Frame: main
    alice = 0x1000
    bob   = 0x3000
```

```
HEAP
-----
0x1000 → Person (Alice)
    name = 0x2000
    age  = 30

0x2000 → "Alice"

0x3000 → Person (Bob)
    name = 0x4000
    age  = 41

0x4000 → "Bob"
```

---

# ✅ STEP 4 — Print Phase

Safe reads.

No memory changes.

---

# ✅ STEP 5 — free(bob->name)

```
free(0x4000);
```

Now:

```
HEAP
-----
0x1000 → Person (Alice)
    name = 0x2000
    age  = 30

0x2000 → "Alice"

0x3000 → Person (Bob)
    name = 0x4000 (DANGLING)
    age  = 41

0x4000 → FREED
```

Important:

* `bob->name` becomes dangling
* But we do not use it again

---

# ✅ STEP 6 — free(bob)

```
free(0x3000);
```

Now:

```
STACK
-----
alice = 0x1000
bob   = 0x3000 (dangling)
```

```
HEAP
-----
0x1000 → Person (Alice)
    name = 0x2000
    age  = 30

0x2000 → "Alice"
```

Bob is fully cleaned up.

---

# ❌ STEP 7 — person_free_partial(alice)

Let's inspect:

```c
static void person_free_partial(Person *p)
{
    if (!p)
        return;

    free(p);
}
```

It only frees the struct — NOT the name.

So:

```
free(0x1000);
```

Now:

```
STACK
-----
alice = 0x1000 (dangling)
bob   = 0x3000 (dangling)
```

```
HEAP
-----
0x2000 → "Alice"   ← MEMORY LEAK
```

---

# 🚨 FINAL MEMORY STATE

One remaining allocation:

```
0x2000 → "Alice"
```

No pointer references it anymore.

This is a **definite memory leak**.

Valgrind would report:

```
definitely lost: 6 bytes
```

---

# 📊 Object Lifetime Table

## Alice

| Object                 | Allocated  | Freed               | Status |
| ---------------------- | ---------- | ------------------- | ------ |
| Person struct (0x1000) | person_new | person_free_partial | freed  |
| name buffer (0x2000)   | person_new | ❌ never freed       | LEAKED |

---

## Bob

| Object                 | Allocated  | Freed           | Status |
| ---------------------- | ---------- | --------------- | ------ |
| Person struct (0x3000) | person_new | free(bob)       | freed  |
| name buffer (0x4000)   | person_new | free(bob->name) | freed  |

---

# 🔥 Ownership Analysis

Correct ownership model should be:

```
Person owns:
    -> name
```

Therefore destructor must:

```
free(p->name);
free(p);
```

But `person_free_partial` violates ownership contract.

---

# 🧠 Memory Errors Present

* ❌ Memory leak: `alice->name`
* No use-after-free
* No double free
* No NULL dereference
* No aliasing issue

---

# 💡 Correct Destructor

```c
static void person_free(Person *p)
{
    if (!p)
        return;

    free(p->name);
    free(p);
}
```

---

# 🧩 Conceptual Diagram of Final Leak

Before exit:

```
            (no pointer)
                ↓
        0x2000 → "Alice"
```

Unreachable heap block → leaked.

---

If you'd like, next we can analyze:

* Double-free variant
* Shallow copy bug
* Struct containing struct
* Returning stack-allocated struct
* realloc misuse
* Ownership transfer example

Send the next program.


---
---

# STACK EXAMPLE - MEMORY MAP

Short answer:

✅ **No memory leak**
✅ **No undefined behavior**
✅ **No invalid frees**
✅ **No dangling pointers escaping scope**
✅ **No buffer overflow**

This program is **memory-safe and correct**.

Now let’s prove it rigorously with a full memory analysis.

---

# 🔎 High-Level Behavior

This program demonstrates:

* Recursive stack growth
* Independent stack frames
* Local variable lifetimes
* Stack memory reuse on unwind

There is:

* No heap allocation
* No global mutable state
* No pointer escaping its lifetime

---

# MEMORY REGIONS INVOLVED

Only:

```
STACK
STATIC (string literals)
```

No HEAP usage at all.

---

# STEP-BY-STEP EXECUTION

Call:

```c
walk_stack(0, 3);
```

This creates recursive frames until depth == 3.

---

# 🔹 DEPTH 0

## Stack after entering walk_stack(0,3)

```
Frame: walk_stack(depth=0)
    int depth = 0
    int max_depth = 3
    int marker = 0

Frame: main
```

Then:

```c
dump_frame("enter", 0);
```

---

## dump_frame at depth 0

New frame:

```
Frame: dump_frame
    label → "enter" (string literal, static storage)
    depth = 0
    int local_int = 100
    char local_buf[16]
    int *p_local = &local_int
```

### Important details

* `local_int` lives only inside this frame.
* `p_local` points to `local_int`.
* That pointer NEVER escapes the function.
* `local_buf` is 16 bytes.
* We write only:

  * index 0
  * index 1
    → fully safe.

Then function returns → entire frame destroyed.

No dangling pointers remain.

---

# 🔹 DEPTH 1

`walk_stack(1,3)` is called.

New stack frame is pushed above previous one:

```
Frame: walk_stack(depth=1)
    marker = 10
```

Then another `dump_frame`.

Each recursion level has:

* Its own `marker`
* Its own `local_int`
* Its own `local_buf`
* Its own `p_local`

They do NOT alias.

---

# 🔹 DEPTH 2 and 3

Same pattern.

Maximum stack depth:

```
main
  walk_stack(0)
    walk_stack(1)
      walk_stack(2)
        walk_stack(3)
```

At depth 3 recursion stops.

Then unwinding begins.

---

# 🔎 STACK LAYOUT (Conceptual)

Stack grows downward (typical architecture).

At maximum depth:

```
-------------------------
dump_frame (exit, depth=3)
walk_stack(depth=3)
dump_frame (enter, depth=3)

walk_stack(depth=2)
dump_frame (enter, depth=2)

walk_stack(depth=1)
dump_frame (enter, depth=1)

walk_stack(depth=0)
dump_frame (enter, depth=0)

main
-------------------------
```

Each frame has its own:

* `marker`
* `local_int`
* `local_buf`
* `p_local`

No overlap between active frames.

---

# 🔎 Pointer Safety Analysis

## 1️⃣ p_local

```c
int *p_local = &local_int;
```

✔ Valid pointer
✔ Points to stack memory
✔ Used only while frame is alive
✔ Never returned
✔ Never stored globally

Safe.

---

## 2️⃣ local_buf

```c
char local_buf[16];
```

Writes:

```c
local_buf[0] = 'A' + depth;
local_buf[1] = '\0';
```

Maximum depth = 3 → `'A' + 3` = 'D'

Indexes used: 0 and 1

✔ No overflow
✔ Fully inside bounds

Safe.

---

## 3️⃣ marker

Each recursive frame has its own `marker`.

When recursion unwinds:

* Inner frame destroyed
* Outer frame still valid

No pointer references `marker` outside its lifetime.

Safe.

---

# 🔎 Lifetime Table

| Variable  | Storage                   | Lifetime        | Escapes? |
| --------- | ------------------------- | --------------- | -------- |
| local_int | stack                     | dump_frame call | ❌        |
| p_local   | stack                     | dump_frame call | ❌        |
| local_buf | stack                     | dump_frame call | ❌        |
| marker    | stack                     | walk_stack call | ❌        |
| label     | pointer to static literal | entire program  | ❌        |

No variable outlives its frame.

---

# 🔎 Any Hidden Undefined Behavior?

Let’s check subtle points.

### Printing addresses

```c
printf("%p", (void *)&local_int);
```

✔ Correct: cast to `(void*)`

### Recursion depth

Depth limited to 3 → trivial stack usage.

If max_depth were huge → possible stack overflow.
But here → safe.

### Signed char arithmetic

```c
'A' + (char)depth
```

Depth ≤ 3 → safe.
No overflow risk.

---

# 🚫 What This Program Does NOT Do

* No malloc
* No free
* No double free
* No memory leak
* No use-after-free
* No returning address of local variable
* No buffer overflow
* No invalid pointer arithmetic

---

# 🧠 What This Program Demonstrates

* Stack frame isolation
* Different addresses for same-named locals
* Stack growth with recursion
* Frame destruction on unwind

---

# ⚠️ The Only Theoretical Risk

If you changed:

```c
walk_stack(0, 1000000);
```

You would eventually get:

❌ Stack overflow (segmentation fault)

But with max_depth = 3:

✔ Completely safe.

---

# ✅ FINAL VERDICT

The program is:

✔ Correct
✔ Memory safe
✔ Leak-free
✔ Undefined-behavior free
✔ Proper stack usage

It is a clean demonstration of stack frames and recursion.

---

If you’d like, I can now:

* Draw a precise stack memory evolution diagram step-by-step
* Modify it to intentionally introduce a stack bug
* Show how returning `&local_int` would break it
* Or analyze a harder case (function pointers, setjmp/longjmp, VLAs, etc.)
