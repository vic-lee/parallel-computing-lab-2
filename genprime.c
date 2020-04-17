#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <omp.h>

#define N_MAX 100000000
#define T_MAX 100

int read_in(int argc, char *argv[], int *N, int *t)
{
    if (argc != 3)
        return 1;

    *N = atoi(argv[1]);
    *t = atoi(argv[2]);

    return 0;
}

int to_file(int arr_sz, char *is_prime, char *fname)
{
    FILE *fp;
    fp = fopen(fname, "w");

    fprintf(fp, "2\n");
    for (size_t i = 3; i < arr_sz; i++)
    {
        if (i % 2 != 0 && is_prime[i])
            fprintf(fp, "%zu\n", i);
    }

    fclose(fp);

    return 0;
}

/**
 * Inputs:
 *   N:             2 <= N <= N_MAX
 *   thread_cnt:    1 <= thread_cnt <= T_MAX
 */
void sieve(int N, int thread_cnt, char *is_prime, int prime_sz)
{
    int sqrt_N = (int)ceil(sqrt(N));
    printf("sqrt n: %d\n", sqrt_N);

    // initialize base is_prime values
    is_prime[0] = 0;
    is_prime[1] = 0;

    for (size_t i = 3; i < sqrt_N; i += 2)
    {
        if (is_prime[i])
        {
            for (size_t j = i * i, delta = i * 2; j < prime_sz; j += delta)
            {
                is_prime[j] = 0;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    int N, t;
    if (read_in(argc, argv, &N, &t) != 0)
    {
        printf("Must supply exactly 2 arguments: `N` and `t`.\n");
        return 1;
    }

    char *is_prime;
    int prime_sz = N + 1;
    is_prime = (char *)calloc(prime_sz, sizeof(char));
    memset(is_prime, 1, prime_sz * sizeof(char));

    sieve(N, t, is_prime, prime_sz);

    char fname[20];
    sprintf(fname, "%d.txt", N);
    to_file(prime_sz, is_prime, fname);

    free(is_prime);

    return 0;
}
