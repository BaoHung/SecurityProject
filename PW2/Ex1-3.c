#include <stdio.h>
#include <stdlib.h>
#include "gmp.h"
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        printf("Please specify a file, and private key pair (n and d).\n");
        return -1;
    }
    else
    {
        char *cmd;
        char md5[33]; // md5 has 32 characters + \0
        mpz_t z_d, z_n, z_s, z_h;
        mpz_inits(z_d, z_n, z_s, z_h, NULL);
        mpz_set_str(z_n, argv[2], 0);
        mpz_set_str(z_d, argv[2], 0);

        cmd = malloc(sizeof("md5sum  | cut -d"
                            " -f 1") +
                     sizeof(argv[1]));
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
        gmp_printf("MD5 Signature: 0x%Zx\n", z_s);

        return 0;
    }
}