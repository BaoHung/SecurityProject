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
    BYTE *fullOutput;
    long fileLength;
    char paddingLength;
    long totalLength;
    FILE *inputFile = fopen(argv[1], "rb");
    if (inputFile)
    {
        printf("Reading from %s ...\n", argv[1]);
        fseek(inputFile, 0, SEEK_END);
        fileLength = ftell(inputFile);
        paddingLength = (fileLength % 16) ? (16 - fileLength % 16) : 16;
        totalLength = fileLength + paddingLength;

        fseek(inputFile, 0, SEEK_SET);
        printf("Allocating memory for input...\n");
        fileContent = malloc(totalLength);
        if (fileContent)
        {
            fread(fileContent, 1, fileLength, inputFile);
        }
        fclose(inputFile);
    }

    if (fileContent)
    {
        // Add padding
        printf("File size: %li\n", fileLength);
        printf("Padding Size: %d\n", paddingLength);
        int i;
        for (i = 0; i < paddingLength; i++)
        {
            fileContent[fileLength + i] = paddingLength;
        }

        // Allocate output
        printf("Allocating memory for output...\n");
        fullOutput = malloc(totalLength);
        // Encrypt block by block
        printf("Encrypting....\n");
        for (i = 0; i < totalLength; i += 16)
        {
            aes_encrypt(fullOutput + i, fileContent + i, K);
        }
        printf("Encrypted\n");
        FILE *outputFile = fopen(argv[2], "wb");
        if (outputFile)
        {
            printf("Writing to %s ...\n", argv[2]);
            fwrite(fullOutput, 1, totalLength, outputFile);
            fclose(outputFile);
        }
    }
    printf("\nExit\n");
    return 0;
}