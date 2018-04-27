#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include "gmp.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define STR_BASE 62

void encrypt_rsa(mpz_t c, const mpz_t m, const mpz_t n, const mpz_t e)
{
    printf("Encrypting message...\n");
    mpz_powm(c, m, e, n);
}

void decrypt_rsa(mpz_t m, const mpz_t c, const mpz_t n, const mpz_t d)
{
    printf("Decrypting message...\n");
    mpz_powm(m, c, d, n);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Please specify a small text message.\n");
        return -1;
    }
    else
    {
        char *key, *value;
        mpz_t z_d, z_e, z_n, z_m, cypher, decrypted;
        mpz_inits(cypher, decrypted, z_d, z_e, z_n, z_m, NULL);
        mpz_set_str(z_m, argv[1], STR_BASE);

        FILE *inputFile = fopen("key.txt", "rb");
        if (inputFile)
        {
            printf("Reading from key.txt ...\n");

            while (fscanf(inputFile, "%m[^=]=%ms\n", &key, &value) == 2)
            {
                switch (key[0])
                {
                case 'e':
                    mpz_set_str(z_e, value, 0);
                    // gmp_printf("GMP e=%ZX\n", z_e);
                    break;
                case 'n':
                    mpz_set_str(z_n, value, 0);
                    // gmp_printf("GMP n=%ZX\n", z_n);
                    break;
                case 'd':
                    mpz_set_str(z_d, value, 0);
                    // gmp_printf("GMP d=%ZX\n", z_d);
                    break;
                }

                free(key);
                free(value);
            }

            fclose(inputFile);
        }

        encrypt_rsa(cypher, z_m, z_n, z_e);
        printf("Cypher: %s\n", mpz_get_str(NULL, STR_BASE, cypher));
        decrypt_rsa(decrypted, cypher, z_n, z_d);
        printf("Decrypted: %s\n", mpz_get_str(NULL, STR_BASE, decrypted));

        return 0;
    }
}