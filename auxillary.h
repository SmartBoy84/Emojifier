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
        pixel *pixelChart = createPixelArray(fWidth * aWidth, fHeight * aHeight); // emoji chart
        pixel *pixelBuffer = createPixelArray(fWidth * fHeight, fN);              // raw rgb buffer

        // read files
        pixel *pixelArray = pixelChart;

        for (int i = 1; i <= fN; i++)
        {
            // set name
            char name[100];
            char fileName[100] = "emojis/";
            sprintf(name, "%d", i);
            strcat(fileName, name);
            strcat(fileName, ".bmp");

            // try to read file
            FILE *rptr; // read pointer

            if (rptr = fopen(fileName, "rb"))
            {
                pixel *fileBuffer = calloc(fWidth * fHeight, sizeof(pixel));

                fseek(rptr, fOffset, SEEK_SET);
                size_t success = fread(fileBuffer, sizeof(pixel), fWidth * fHeight, rptr);
                fclose(rptr);

                // make raw rbg buffer
                memcpy(pixelBuffer + ((i - 1) * width(pixelBuffer)), fileBuffer, width(pixelBuffer) * sizeof(pixel));

                // write emoji chart
                for (int y = 0; y < fHeight; y++)
                {
                    memcpy(pixelArray + (width(pixelChart) * y), fileBuffer + (fWidth * y), fWidth * sizeof(pixel));
                }

                pixelArray += i % aWidth == 0 ? width(pixelChart) * (fHeight - 1) : fWidth;
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
        fwrite(pixelBuffer, arraySize(pixelBuffer), sizeof(pixel), wptr);
        fclose(wptr);

        return 0;
    }
    else
    {
        printf("Emojis directory doesn't exist");
        return 1;
    }
}