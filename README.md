# Parallel Prime Counting with C and MPI

A parallel C program that counts the prime numbers in the inclusive
range from `2` to `N` using the Message Passing Interface (MPI). The
workload is distributed across multiple processes, and each process
counts the primes in its assigned portion of the range.

## Project Objectives

-   Implement `is_prime(long long n)` to determine whether a number is
    prime.
-   Handle special cases, including numbers less than `2`.
-   Optimize primality testing by checking divisors only up to the
    square root of the number.
-   Distribute the range `[2, N]` among MPI processes without duplicates
    or omissions.
-   Combine local counts using `MPI_Reduce`.
-   Validate correctness with different input values and process counts.
-   Compare sequential and parallel performance when timing is required.

## Algorithm Overview

### 1. Primality Testing: `is_prime`

A prime number is an integer greater than `1` whose only positive
divisors are `1` and itself.

The `is_prime` function follows these steps:

1.  Return `0` if `n < 2`, because such numbers are not prime.
2.  Return `1` if `n == 2`.
3.  Return `0` if `n` is even and greater than `2`.
4.  Test odd divisors starting at `3`, increasing by `2` each iteration.
5.  Stop when `i > sqrt(n)`. The implementation uses `i <= n / i`
    instead of `i * i <= n` to avoid potential integer overflow in the
    multiplication.
6.  If no divisor is found, return `1`.

The function returns `1` for prime numbers and `0` otherwise.

### 2. MPI Initialization and Process Information

The program uses these MPI functions:

-   `MPI_Init`: initializes the MPI environment.
-   `MPI_Comm_rank`: gets the current process rank.
-   `MPI_Comm_size`: gets the total number of processes.
-   `MPI_Bcast`: broadcasts `N` from process `0` to all processes.
-   `MPI_Reduce`: combines local counts into a total count on process
    `0`.
-   `MPI_Finalize`: shuts down the MPI environment.

### 3. Work Distribution

The program uses **cyclic distribution**. For each process, the first
assigned number is:

``` c
2 + rank
```

The process then advances by the total number of processes, `size`:

``` c
for (long long i = 2 + rank; i <= N; i += size)
```

For example, with four processes:

  Process     Numbers tested
  ----------- -------------------
  Process 0   2, 6, 10, 14, ...
  Process 1   3, 7, 11, 15, ...
  Process 2   4, 8, 12, 16, ...
  Process 3   5, 9, 13, 17, ...

This assignment covers every number in `[2, N]` exactly once. No number
is assigned to more than one process, and no number in the range is
skipped.

### 4. Combining the Results

Each process increments its `local_count` whenever it finds a prime
number. At the end of the local computation, `MPI_Reduce` with `MPI_SUM`
adds all local counts and stores the final total in `total_count` on
process `0`.

## Summary

This project demonstrates parallel computing by dividing primality tests
across MPI processes. Each process examines a unique subset of the input
range, counts its local prime numbers, and contributes that count to the
final result through `MPI_Reduce`. Correctness can be verified by
comparing results across different upper bounds and process counts.