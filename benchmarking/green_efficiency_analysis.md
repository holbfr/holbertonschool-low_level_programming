## Measurement Methodology
* The method that was used to  measure the performance of algorithms is by measuring the elapsed time between its start and end, in seconds.It's done by using the `clock()` function from time utilities in the C language, declared in `<time.h>` and standardized in C (C89/C90/C99/C11/C17).

* This method allows us to measure the CPU time, which means : **the amount of processor time** the program has actually used, not the real elapsed time.

* The `clock()` function returns a value of type `clock_t` which is an Arithmetic(until C11) and a Real(since C11) type capable of representing the processor time used by a process.

* Using this method is good choice to measure the performance of an algorithm or to compare CPU usage between different versions. However it's not a good for measuring real elapsed time.

## Observed Performance Differences
* Different code snippets were used to measure the efficiency of a few algorithms. Amongs them is the [comparison_algorithms.c](https://github.com/holbfr/holbertonschool-low_level_programming/blob/main/benchmarking/comparison_algorithms.c) one where we use one Naive algorithm (slow) and one Single-pass algorithm (more efficient) to observe the difference in performances.


* Based on the exemple of the `comparaison algorithm` we get the following result :

```
swe@SWE-VM:~/holberton-projects/holbertonschool-low_level_programming/benchmarking$ gcc -Wall -Werror -Wextra -pedantic -std=gnu89 comparison_algorithms.c 
swe@SWE-VM:~/holberton-projects/holbertonschool-low_level_programming/benchmarking$ ./a.out 
Naive algorithm result: 625025000
Naive algorithm time: 0.612185 seconds
Single-pass algorithm result: 25000
Single-pass algorithm time: 0.000025 seconds
swe@SWE-VM:~/holberton-projects/holbertonschool-low_level_programming/benchmarking$ ./a.out 
Naive algorithm result: 625025000
Naive algorithm time: 0.589252 seconds
Single-pass algorithm result: 25000
Single-pass algorithm time: 0.000024 seconds
swe@SWE-VM:~/holberton-projects/holbertonschool-low_level_programming/benchmarking$ ./a.out 
Naive algorithm result: 625025000
Naive algorithm time: 0.605635 seconds
Single-pass algorithm result: 25000
Single-pass algorithm time: 0.000024 seconds
swe@SWE-VM:~/holberton-projects/holbertonschool-low_level_programming/benchmarking$ 
```

##### Observations :

* **Average execution time**
    * Average execution time for the Naive algorithm : 0.602357 seconds.
    * Average execution time for the Single-pass algorithm : 0.000024 seconds.

* **Relative difference**
    * The Single-pass algorithm is about 25000 times faster than the Naive algorithm.
    * The Naive algorithm is about 25000 time slower than the Single-pass algorithm.
    * The Naive algorithm runs at only 0.00398% of the Single-pass algorithm's speed.

* From this exemple we can concluse that when an algorithm is optimized it can be so much more performant and faster compared to a naive one and that the implemented solution can play a major role in the effeciency and speed of our application.

## Relation Between Runtime and Energy Consumption

* A less performant algorithm is an algorithm that takes more time to execute. Taking more time means consuming more energy which also means more expenses. On a small scale i.e. in the contexte of small personal project or a application with a few users that doesn't ring a bell. However when our program starts getting more data to handle, it'll take much more time and what was a difference of a few milliseconds between a naive algorithm and an optimized one becomes a difference of seconds and even minutes. Getting to this stage, inefficient algorithms become a major issue for the environment because they'll consume more energy, which isn't eco-friendly.

## Limitations of the Experiment

The experiment was conducted on a single machine (personal computer) using a few simple algorithms, under one system. Even though the experiment is relevant and helps us observe the importante of algorithms' performance, it has its limitation :

* Hardware-dependent results :
    * Performance depends heavily on the specific CPU, cache size, RAM speed, and storage of the machine.
    * Results may not generalize to other systems.

* Compiler optimizations :
    * The compiler, its version and the flags used for the compilations can change execution time.

* Environmental factors
    * Power saving modes, CPU throttling and system state can affect speed during testing.

## Practical Engineering Takeaway

* Algorithmic efficiency is important and critical because when a program is slow and inefficient it's time-consuming and not eco-friendly.
* Writing efficient programs is an important skill that makes a good Software engineer differ from others because good programs save companies time and money and the faster a program is, the more clients and users are satified and vice versa.
* When measuring and testing the performance of a program it's essential to take into consideration different factors, among them :
    * Run the program many times and average the elapsed time of the executions.
    * Test in different environments (Hardware, OS, states and current usage).
    * The amount and size of data used for the test scenarios.
    * Take into consideration the tools used such us the compiler, the execution process and so on..
