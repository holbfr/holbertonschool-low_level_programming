# Crash Report

#### 1. Description of the Crash
* Error: Segmentation fault (SIGSEGV) — "core dumped"
* Location: `main` function, at the line `nums[0] = 42;`
* Behavior: The program crashes when attempting to write to `nums[0]`.


#### 2. Root Cause Analysis

* Function Call: `nums = allocate_numbers(n);` where `n = 0`
* Allocation Logic: In `allocate_numbers`, if `n <= 0`, the function returns `NULL`.
* Result: `nums` is assigned `NULL` because `n = 0`.
* Invalid Access: The program then attempts to write to nums[0], which is equivalent to dereferencing a NULL pointer.


#### 3. Why the Memory Access is Invalid
* NULL Pointer Dereference: nums is NULL, so nums[0] is an attempt to write to address 0x0.
* Memory Protection: The operating system does not allow any process to access memory at address 0x0 (NULL).
* Undefined Behavior: Dereferencing a NULL pointer is undefined behavior in C, and typically results in a segmentation fault.


#### Suggested Fix
##### Check for NULL before dereferencing
```
nums = allocate_numbers(n);
if (nums != NULL) {
    nums[0] = 42;
    printf("  nums[0]=%d\n", nums[0]);
    free(nums);
} else {
    printf("  Error: allocation failed (n=%d)\n", n);
}
```
