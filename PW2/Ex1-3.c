#include <stdio.h>
#include <stdlib.h>
#include "gmp.h"
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Please specify an input file and a key file.\n");
        return -1;
    }
    else
    {
        char *cmd, *key, *value;
        char md5[33]; // md5 has 32 characters + \0
        mpz_t z_d, z_n, z_s, z_h;
        mpz_inits(z_d, z_n, z_s, z_h, NULL);

        FILE *inputFile = fopen(argv[2], "rb");
        if (inputFile)
        {
            printf("Reading from %s ...\n", argv[2]);

            while (fscanf(inputFile, "%m[^=]=%ms\n", &key, &value) == 2)
            {
                switch (key[0])
                {
                case 'n':
                    mpz_set_str(z_n, value, 0);
                    break;
                case 'd':
                    mpz_set_str(z_d, value, 0);
                    break;
                }

                free(key);
                free(value);
            }

            fclose(inputFile);
        }

        cmd = malloc(sizeof("md5sum  | cut -d\" \" -f 1") + sizeof(argv[1]));
        sprintf(cmd, "md5sum %s | cut -d\" \" -f 1", argv[1]);

        FILE *stream;
        stream = popen(cmd, "r");
        if (stream)
        {
            fgets(md5, 33, stream);
            pclose(stream);
        }
        printf("MD5 sum: %s\n", md5);
        mpz_set_str(z_h, md5, 16);
        mpz_powm(z_s, z_h, z_d, z_n);
        gmp_printf("\nMD5 Signature: %Zx\n", z_s);

        return 0;
    }
}