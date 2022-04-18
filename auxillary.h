#include "bmpcreator.h"
#include "bmpreader.h"
#include <dirent.h>
#include <math.h>
#include <stdint.h>

char fileName[100];
int generated = 1;

char *createName(int index, char *folder)
{
    // come onnnn c jesus
    char *name = calloc(100, sizeof(char));
    memset(fileName, 0, 100 * sizeof(char));

    strcat(fileName, folder);
    strcat(fileName, "/");
    sprintf(name, "%d", index);
    strcat(fileName, name);
    strcat(fileName, ".bmp");

    return fileName;
}

char *generateChart(char *bufferName, char *emojisFolder, uint8_t scale, int fN)
{
    printf("Generating auxillary buffers %s[.bin & .bmp]\nScale: %d, file count: %d\n", bufferName, scale, fN);

    DIR *dir = opendir("emojis");

    if (dir)
    {
        int32_t fWidth;
        int32_t fHeight;
        uint32_t fOffset;

        if (getInfo(createName(1, emojisFolder), &fOffset, &fWidth, &fHeight))
        {
            printf("No file in emojis folder?\n");
            return NULL;
        }

        if (scale > 1)
        {
            if (fWidth % scale != 0 || fHeight % scale != 0)
            {
                printf("%dx%d not scalable by factor %d\n", fWidth, fHeight, scale);
                return NULL;
            }
            else if (fWidth < 24 || fHeight < 24)
            {
                printf("Woah there! That is one TINY file (%dx%d) you're trying to scale there, we don't do work on "
                       "children here",
                       fWidth, fHeight);
                return NULL;
            }
        }

        int32_t aWidth = sqrt(fN) + 1;
        int32_t aHeight = (fN / aWidth) + 1;

        int32_t fileWidth = (uint8_t)fWidth / scale;
        int32_t fileHeight = (uint8_t)fHeight / scale;

        pixel *pixelChart = createPixelArray(fileWidth * aWidth, fileHeight * aHeight); // emoji chart
        pixel *pixelBuffer =
            createPixelArray((fileWidth * fileHeight) + 1, fN); // raw rgb buffer + 1 pixel per image for average

        // read files
        pixel *tempPixelChart = pixelChart;
        pixel *tempPixelBuffer = pixelBuffer;

        for (int i = 1; i <= fN; i++)
        {
            // try to read file
            pixel *fileBuffer = calloc(fWidth * fHeight, sizeof(pixel));
            pixel *tempBuffer;

            if (tempBuffer = readFile(createName(i, emojisFolder)))
            {
                memcpy(fileBuffer, tempBuffer, fWidth * fHeight * sizeof(pixel));

                uint32_t averages[3] = {0};
                uint32_t averageCount = 0;
                uint8_t *colorBuffer;

                char color = 0;

                for (int y = 0; y < fileHeight; y++)
                {
                    for (int x = 0; x < fileWidth; x++)
                    {
                        if (scale > 1)
                            fileBuffer[(y * fileWidth) + x] = fileBuffer[(y * scale * fWidth) + (x * scale)];

                        // get average
                        colorBuffer = (uint8_t *)(fileBuffer + (y * scale * fWidth) + (x * scale));

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
                    *((uint8_t *)&average + i) = (uint8_t)(averages[i] / averageCount);

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
                return NULL;
        }

        // write chart
        memset(fileName, 0, 100 * sizeof(char));
        strcat(fileName, bufferName);
        strcat(fileName, ".bmp");

        writeArray(pixelChart, fileName);

        // populate raw rgb file
        memset(fileName, 0, 100 * sizeof(char));
        strcat(fileName, bufferName);
        strcat(fileName, ".bin");

        FILE *wptr = fopen(fileName, "wb");

        int32_t dimensions[] = {fN, fileWidth, fileHeight};
        fwrite(&dimensions, 3, sizeof(int32_t), wptr); // so I can read it later on

        fwrite(pixelBuffer, arraySize(pixelBuffer), sizeof(pixel), wptr);

        fclose(wptr);

        generated = 0;
        return fileName;
    }
    else
    {
        printf("%s doesn't exist", emojisFolder);
        return NULL;
    }
}