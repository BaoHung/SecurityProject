#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include "gmp.h"
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Please specify k-bit and public exponent.\n");
        return -1;
    }
    else
    {
        int outputFile = open("output.txt", O_RDWR | O_CREAT | O_APPEND, 0600);
        if (outputFile)
        {
            dup2(outputFile, 1);
        }

        int k = atoi(argv[1]), e = atoi(argv[2]);
        // printf("k = %d\ne = %d\n", k,e);

        gmp_randstate_t prng;
        mpz_t z_p, z_q, z_powerK, z_phi, z_d, z_e, z_n;

        gmp_randinit_default(prng);
        gmp_randseed_ui(prng, time(NULL));
        mpz_inits(z_p, z_q, z_powerK, z_phi, z_d, z_e, z_n, NULL);
        mpz_set_ui(z_e, e);
        mpz_ui_pow_ui(z_powerK, 2, k - 1);

        int isPrime = 0, isInvertible = 0;
        while (!isPrime)
        {
            mpz_rrandomb(z_p, prng, k / 2 - 1);
            mpz_add(z_p, z_p, z_powerK);
            isPrime = mpz_probab_prime_p(z_p, 5);
        }
        // gmp_printf("z_p = %Zu\n", z_p);
        // printf("Test prime: %d\n", isPrime);

        isPrime = 0;
        while (!isPrime || mpz_cmp(z_p, z_q) == 0 || !isInvertible)
        { // Check if e mod phi(n) exist
            mpz_rrandomb(z_q, prng, k / 2 - 1);
            mpz_add(z_q, z_q, z_powerK);
            isPrime = mpz_probab_prime_p(z_q, 5);

            mpz_sub_ui(z_p, z_p, 1);
            mpz_sub_ui(z_q, z_q, 1);
            mpz_mul(z_phi, z_p, z_q);
            isInvertible = mpz_invert(z_d, z_e, z_phi);
        }
        mpz_mul(z_n, z_p, z_q);
        // gmp_printf("z_q = %Zu\n", z_q);
        // printf("Test prime: %d\n", isPrime);

        // gmp_printf("z_phi = %Zu\n", z_phi);

        gmp_printf("e = 0x%ZX\n", z_e);
        gmp_printf("n = 0x%ZX\n", z_n);
        gmp_printf("d = 0x%ZX\n", z_d);

        mpz_clears(z_p, z_q, z_powerK, z_phi, z_d, z_e, z_n, NULL);
        gmp_randclear(prng);

        if (outputFile)
        {
            close(outputFile);
        }
        return 0;
    }
}