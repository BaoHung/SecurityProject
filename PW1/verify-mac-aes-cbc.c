#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef unsigned char BYTE;
void aes_encrypt(BYTE *cipher, BYTE *message, BYTE *key);
void aes_decrypt(BYTE *message, BYTE *cipher, BYTE *key);

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Please specify input file and MAC value.\n");
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
    BYTE cbc[16] = {0};
    BYTE *fileContent = 0;
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
        int i, j, blocks;
        for (i = 0; i < paddingLength; i++)
        {
            fileContent[fileLength + i] = paddingLength;
        }

        // Encrypt block by block
        printf("Encrypting....\n");

        blocks = totalLength / 16;
        for (i = 0; i < blocks; i++)
        {
            for (j = 0; j < 16; j++)
            {
                cbc[j] ^= fileContent[j];
            }
            aes_encrypt(cbc, cbc, K);
            memcpy(fileContent, cbc, 16);
            fileContent += 16;
        }
        printf("Encrypted\n");

        char MAC[2];
        char isMACgood = 1;
        if (strlen(argv[2]) == 16 * 2)
        {
            for (i = 0; i < 16; i++)
            {
                sprintf(MAC, "%02x", *(fileContent - 16 + i));
                if (tolower(MAC[0]) != tolower(argv[2][i * 2]) || tolower(MAC[1]) != tolower(argv[2][i * 2 + 1]))
                {
                    isMACgood = 0;
                    break;
                }
            }
        }
        else
        {
            isMACgood = 0;
        }

        printf("====================\n");
        if (isMACgood)
        {
            printf("The MAC value is correct.\n");
        }
        else
        {
            printf("The MAC value is NOT correct.\n");
        }
    }
    printf("\nExit\n");
    return 0;
}