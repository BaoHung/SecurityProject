#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char BYTE;
void aes_encrypt(BYTE *cipher, BYTE *message, BYTE *key);
void aes_decrypt(BYTE *message, BYTE *cipher, BYTE *key);

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Please specify input and output files.\n");
        return -1;
    }

    BYTE K[16] = {0x2B,
                  0x7E,
                  0x15,
                  0x16,
                  0x28,
                  0xAE,
                  0xD2,
                  0xA6,
                  0xAB,
                  0xF7,
                  0x15,
                  0x88,
                  0x09,
                  0xCF,
                  0x4F,
                  0x3C};
    BYTE *fileContent = 0;
    BYTE cbc[16] = {0}, tmp[16];
    long fileLength;
    char paddingLength;
    long finalLength;
    FILE *inputFile = fopen(argv[1], "rb");
    if (inputFile)
    {
        printf("Reading from %s ...\n", argv[1]);
        fseek(inputFile, 0, SEEK_END);
        fileLength = ftell(inputFile);
        paddingLength = (fileLength % 16) ? (16 - fileLength % 16) : 16;

        fseek(inputFile, 0, SEEK_SET);
        printf("Allocating memory for input...\n");
        fileContent = malloc(fileLength);
        if (fileContent)
        {
            fread(fileContent, 1, fileLength, inputFile);
        }
        fclose(inputFile);
    }

    if (fileContent)
    {
        int i, j, blocks;
        // Encrypt block by block
        printf("Decrypting....\n");

        blocks = fileLength / 16;
        for (i = 0; i < blocks; i++)
        {
            memcpy(tmp, fileContent, 16);
            aes_decrypt(fileContent, fileContent, K);
            for (j = 0; j < 16; j++)
                fileContent[j] ^= cbc[j];
            memcpy(cbc, tmp, 16);
            fileContent += 16;
        }
        fileContent -= fileLength;

        // for (i = 0; i < fileLength; i += 16)
        // {
        //     aes_decrypt(fullOutput + i, fileContent + i, K);
        // }
        printf("Decrypted\n");
        printf("Unpadding...\n");
        paddingLength = fileContent[fileLength - 1];
        finalLength = fileLength - paddingLength;
        printf("Padded length: %d ", fileContent[fileLength - 1]);
        printf("\nUnpadded\n");

        FILE *outputFile = fopen(argv[2], "wb");
        if (outputFile)
        {
            printf("Writing to %s ...\n", argv[2]);
            fwrite(fileContent, 1, finalLength, outputFile);
            fclose(outputFile);
        }
    }
    printf("\nExit\n");
    return 0;
}