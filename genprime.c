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

    // initialize base is_prime values
    is_prime[0] = 0;
    is_prime[1] = 0;

#pragma omp parallel num_threads(thread_cnt) default(none) \
    firstprivate(N, prime_sz) shared(is_prime)
    {
        int sqrt_N = (int)ceil(sqrt(N));
        for (size_t i = 3; i < sqrt_N; i += 2)
        {
            if (is_prime[i])
            {
                size_t j, delta = i * 2;
#pragma omp for
                for (j = i * i; j < prime_sz; j += delta)
                {
                    is_prime[j] = 0;
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{

    /*********** Read input ***********/

    double t_start = 0.0, t_taken;

    int N, t;
    if (read_in(argc, argv, &N, &t) != 0)
    {
        printf("Must supply exactly 2 arguments: `N` and `t`.\n");
        return 1;
    }

    /*********** Find primes ***********/

    t_start = omp_get_wtime();

    char *is_prime;
    int prime_sz = N + 1;
    is_prime = (char *)calloc(prime_sz, sizeof(char));
    memset(is_prime, 1, prime_sz * sizeof(char));

    sieve(N, t, is_prime, prime_sz);

    t_taken = omp_get_wtime() - t_start;
    printf("Time take for the main part: %f\n", t_taken);

    /*********** Write to file ***********/

    char fname[20];
    sprintf(fname, "%d.txt", N);
    to_file(prime_sz, is_prime, fname);

    free(is_prime);

    return 0;
}
