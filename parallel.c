/*
 * parallel.c
 * MPI parallel prime-counting program.
 *
 * Usage:
 *   mpicc parallel.c -o parallel
 *   mpirun -np P ./parallel N
 * 
 * P -> number of processes
 * Counts prime numbers in the inclusive range [2, N].
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int is_prime(long long n) // Zeyad
{
    if (n < 2)
        return 0;

    if (n == 2)
        return 1;

    if (n % 2 == 0)
        return 0;

    for (long long i = 3; i <= n / i; i += 2)
        if (n % i == 0)
            return 0;

    return 1;
}

int main(int argc, char *argv[]) //Ali
{
    int rank, size;
    long long N, local_count = 0, total_count = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        if (argc != 2)
        {
            printf("Usage: %s N\n", argv[0]);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        N = atoll(argv[1]);
    }

    MPI_Bcast(&N, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    // print all prime numbers.
    /*
    if (rank == 0)
    {
        printf("Prime numbers: ");
        for (long long i = 2; i <= N; i++)
            if (is_prime(i))
                printf("%lld ", i);
        printf("\n");
    }
    */

    for (long long i = 2 + rank; i <= N; i += size)
        if (is_prime(i))
            local_count++;

    MPI_Reduce(&local_count, &total_count, 1, MPI_LONG_LONG,
               MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Range: [2, %lld]\n", N);
        printf("Number of prime numbers: %lld\n", total_count);
        printf("Processes: %d\n", size);
    }

    MPI_Finalize();
    return 0;
}