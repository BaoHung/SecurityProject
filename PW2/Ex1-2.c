#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include "gmp.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void encrypt_rsa(mpz_t *c, mpz_t m, mpz_t n, mpz_t e)
{
    printf("Encrypting message.\n");
    mpz_set_d(*c, 500);
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
        mpz_t *cypher;
        cypher = malloc(sizeof(mpz_t));
        mpz_t z_d, z_e, z_n, z_m;
        mpz_inits(*cypher, z_d, z_e, z_n, z_m, NULL);
        mpz_set_str(z_m, argv[1], 62);
        // char message[200];
        // mpz_get_str(message, 62, z_m);
        // printf("Message: %s\n", message);

        FILE *inputFile = fopen("key.txt", "rb");
        if (inputFile)
        {
            printf("Reading from key.txt ...\n");

            while (fscanf(inputFile, "%m[^=]=%ms\n", &key, &value) == 2)
            {
                // printf("Key: %s\n", key);
                // printf("Value: %s\n", value);

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
        gmp_printf("GMP cypher=%Zd\n", cypher);

        return 0;
    }
}