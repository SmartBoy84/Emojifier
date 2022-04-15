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

__uint8_t generateChart(__uint8_t scale)
{

    if (fWidth % scale != 0 || fHeight % scale != 0)
    {
        printf("Unsupported scale factor - should be 2, 3, 4, 6, 8, 9, 12, 18, 26, 36");
        return 1;
    }

    DIR *dir = opendir("emojis");

    if (dir)
    {
        __int32_t fileWidth = scale == 1 ? fWidth : (__uint8_t)fWidth / scale;
        __int32_t fileHeight = scale == 1 ? fHeight : (__uint8_t)fHeight / scale;

        pixel *pixelChart = createPixelArray(fileWidth * aWidth, fileHeight * aHeight); // emoji chart
        pixel *pixelBuffer = createPixelArray(fileWidth * fileHeight, fN);              // raw rgb buffer

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

                if (scale > 1)
                {
                    for (int y = 0; y < fileHeight; y++)
                    {
                        for (int x = 0; x < fileWidth; x++)
                        {
                            fileBuffer[(y * fileWidth) + x] = fileBuffer[(y * scale * fWidth) + (x * scale)];
                        }
                    }

                    fileBuffer = realloc(fileBuffer, (fileWidth * fileHeight) * sizeof(pixel));
                }

                // make raw rbg buffer
                memcpy(pixelBuffer + ((i - 1) * width(pixelBuffer)), fileBuffer, width(pixelBuffer) * sizeof(pixel));

                // write emoji chart
                for (int y = 0; y < fileHeight; y++)
                {
                    memcpy(pixelArray + (width(pixelChart) * y), fileBuffer + (fileWidth * y),
                           fileWidth * sizeof(pixel));
                }

                pixelArray += i % aWidth == 0 ? width(pixelChart) * (fileHeight - 1) : fileWidth;
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

        __int32_t dimensions[] = {fileWidth, fileHeight};
        fwrite(&dimensions, 2, sizeof(__int32_t), wptr); // so I can read it later on

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