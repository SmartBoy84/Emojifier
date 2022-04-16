#include "bmpcreator.h"
#include "bmpreader.h"
#include <dirent.h>
#include <math.h>

#define fN 3301

int generateChart(__uint8_t scale)
{
    printf("Generating auxillary buffers...");

    DIR *dir = opendir("emojis");

    if (dir)
    {
        __int32_t fWidth;
        __int32_t fHeight;
        __uint32_t fOffset;

        if (getInfo("emojis/1.bmp", &fOffset, &fWidth, &fHeight))
        {
            printf("No file in emojis folder?");
            return 1;
        }

        if (scale > 1 && (fWidth % scale != 0 || fHeight % scale != 0))
        {
            printf("Unsupported scale factor - should be 2, 3, 4, 6, 8, 9, 12, 18, 26, 36");
            return 1;
        }

        __int32_t aWidth = sqrt(fN) + 1;
        __int32_t aHeight = (fN / aWidth) + 1;

        __int32_t fileWidth = (__uint8_t)fWidth / scale;
        __int32_t fileHeight = (__uint8_t)fHeight / scale;

        pixel *pixelChart = createPixelArray(fileWidth * aWidth, fileHeight * aHeight); // emoji chart
        pixel *pixelBuffer =
            createPixelArray((fileWidth * fileHeight) + 1, fN); // raw rgb buffer + 1 pixel per image for average

        // read files
        pixel *tempPixelChart = pixelChart;
        pixel *tempPixelBuffer = pixelBuffer;

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

                __uint32_t averages[3] = {0};
                __uint32_t averageCount = 0;
                __uint8_t *colorBuffer;

                for (int y = 0; y < fileHeight; y++)
                {
                    for (int x = 0; x < fileWidth; x++)
                    {
                        if (scale > 1)
                            fileBuffer[(y * fileWidth) + x] = fileBuffer[(y * scale * fWidth) + (x * scale)];

                        // get average
                        colorBuffer = (__uint8_t *)(fileBuffer + (y * scale * fWidth) + (x * scale));

                        if (*(colorBuffer + 3) > 0)
                        {
                            averageCount++;

                            for (int i = 0; i < 3; i++)
                                averages[i] += *(colorBuffer + i);
                        }
                    }
                }

                pixel average = {0};

                for (int i = 0; i < 3; i++)
                    *((__uint8_t *)&average + i) = (__uint8_t)(averages[i] / averageCount);

                // handle case where entire image is transparent
                if (average.blue > 0 || average.green > 0 || average.red > 0)
                    average.alpha = 255;

                fileBuffer = realloc(fileBuffer, ((fileWidth * fileHeight) + 1) * sizeof(pixel));
                fileBuffer[fileWidth * fileHeight] = average;

                // make raw rbg buffer
                memcpy(tempPixelBuffer, fileBuffer, width(pixelBuffer) * sizeof(pixel)); // copy image

                // write emoji chart
                for (int y = 0; y < fileHeight; y++)
                {
                    memcpy(tempPixelChart + (width(pixelChart) * y), fileBuffer + (fileWidth * y),
                           fileWidth * sizeof(pixel));
                }

                tempPixelBuffer += width(pixelBuffer);
                tempPixelChart += i % aWidth == 0 ? width(pixelChart) * (fileHeight - 1) : fileWidth;
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

        __int32_t dimensions[] = {fN, fileWidth, fileHeight};
        fwrite(&dimensions, 3, sizeof(__int32_t), wptr); // so I can read it later on

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