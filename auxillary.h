#include "bmpcreator.h"
#include <dirent.h>

// file specs
#define fOffset 138
#define fSize 20874
#define fWidth 72
#define fHeight 72
#define fN 3301

// array dimensions
#define aWidth 56
#define aHeight 59

__uint8_t generateChart()
{
    DIR *dir = opendir("emojis");

    if (dir)
    {
        __uint8_t *pixelChart = createPixelArray(fWidth * aWidth, fHeight * aHeight); // emoji chart
        __uint8_t *pixelBuffer = createPixelArray(fWidth * fHeight, fN);              // raw rgb buffer

        // read files
        __uint8_t *pixelArray = pixelChart;

        for (int i = 1; i <= fN; i++)
        {
            char name[100];
            char fileName[100] = "emojis/";
            sprintf(name, "%d", i);
            strcat(fileName, name);
            strcat(fileName, ".bmp");

            FILE *rptr; // read pointer

            if (rptr = fopen(fileName, "rb"))
            {
                __uint8_t *fileBuffer = calloc(fWidth * fHeight * 4, sizeof(__uint8_t));

                fseek(rptr, fOffset, SEEK_SET);
                size_t success = fread(fileBuffer, sizeof(__uint8_t), fSize - fOffset, rptr);
                fclose(rptr);

                // make raw rbg buffer
                memcpy(pixelBuffer + ((i - 1) * width(pixelBuffer) * 4), fileBuffer, width(pixelBuffer) * 4);

                // write emoji chart
                for (int y = 0; y < fHeight; y++)
                {
                    memcpy(pixelArray + (width(pixelChart) * y * 4), fileBuffer + (fWidth * y * 4), fWidth * 4);
                }

                pixelArray += i % aWidth == 0 ? width(pixelChart) * (fHeight - 1) * 4 : fWidth * 4;
            }
            else
            {
                printf("Missing %s", fileName);
                return 1;
            }
        }

        // write chart
        writeArray(pixelChart, "emojichart.bmp");

        // populate raw rgb file
        FILE *wptr = fopen("emojis.bin", "wb");
        fwrite(pixelBuffer, arraySize(pixelBuffer), 1, wptr);
        fclose(wptr);

        return 0;
    }
    else
    {
        printf("Emojis directory doesn't exist");
        return 1;
    }
}