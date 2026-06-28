#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes that can be read
#define MAX_BYTES 512
 
int main(int argc, char *argv[])
{
    // Accept a single command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    // Open the memory card
    FILE *mem_card = fopen(argv[1], "r");
    if (mem_card == NULL)
    {
        printf("Unable to open %s.\n", argv[1]);
    }

    // Create a buffer for a block of data
    uint8_t buffer[MAX_BYTES];
    FILE *output = NULL;

    // Intialize jpeg_count
    int jpeg_count = 0;

    // "000.jpg"
    char filename[8];

    // While there's still data left to read from the memory card
    while (fread(buffer, 1, MAX_BYTES, mem_card) == MAX_BYTES)
    {
        // Create JPEGs from the data
        // 1. Check for JPEG header
        bool is_jpeg = 
            buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0;

        // 2. If start of new JPEG
         if (is_jpeg)
        {
            // Close previous JPEG if it exitst
            if (output != NULL)
            {
                fclose(output);
            }

            // Create new JPEG filename
            sprintf(filename, "%03i.jpg", jpeg_count);
            output = fopen(filename, "w");
            jpeg_count++;
        }

         // 3. If currently writing a JPEG, write block
         if (output != NULL)
         {
             fwrite(buffer, sizeof(uint8_t), MAX_BYTES, output);
         }
    }

    // Close nay remaining files
    if (output != NULL)
    {
        fclose(mem_card);
    }
    fclose(mem_card);

    return 0;
}
