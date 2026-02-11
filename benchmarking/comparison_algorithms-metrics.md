## Compilation and Execution Three times

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

## Average execution time
* Average execution time for the Naive algorithm : 0.602357 seconds.
* Average execution time for the Single-pass algorithm : 0.000024 seconds.

## Relative difference
* The Single-pass algorithm is about 25000 times faster than the Naive algorithm.
* The Naive algorithm is about 25000 time slower than the Single-pass algorithm.
* The Naive algorithm runs at only 0.00398% of the Single-pass algorithm's speed.



