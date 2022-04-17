#ifndef bmpreader // prevent redefinitions
#define bmpreader

#include <math.h>
#include <stdint.h>
#include "bmpcreator.h"


void readInternalBuffer(unsigned char *buffer, uint32_t *iC, pixel **eBuffer, pixel **aBuffer)
{
    *iC = *((uint32_t *)buffer);
    int32_t eWidth = *((int32_t *)buffer + 1);
    int32_t eHeight = *((int32_t *)buffer + 2);

    buffer += (sizeof(uint32_t) * 3) / sizeof(unsigned char);

    size_t rowSize = eWidth * eHeight;

    *eBuffer = createPixelArray(eWidth * eHeight, *iC);
    *aBuffer = createPixelArray(*iC, 1);

    for (int i = 0; i < *iC; i++)
    {
        memcpy(*eBuffer + (rowSize * i), buffer, sizeof(pixel) * rowSize);
        buffer += (sizeof(pixel) * rowSize) / sizeof(unsigned char);

        memcpy(*aBuffer + i, buffer, sizeof(pixel));
        buffer += (sizeof(pixel)) / sizeof(unsigned char);
    }

    // set dimensions in the buffer for easy access later
    *((int32_t *)*eBuffer - 1) = (int32_t)eHeight; // change height var of array to height of each image
    *((int32_t *)*eBuffer - 2) = (int32_t)eWidth;  // change height var of array to height of each image
}

// double for buffers because we want access to the pointer value not the pointer to
void readBufferFile(FILE *file, uint32_t *iC, pixel **eBuffer, pixel **aBuffer)
{

    fread(iC, sizeof(int32_t), 1, file);

    // get the width and height of the images from file
    int32_t eWidth;
    fread(&eWidth, sizeof(int32_t), 1, file);

    int32_t eHeight;
    fread(&eHeight, sizeof(int32_t), 1, file);

    // extract image buffers and averages
    size_t rowSize = eWidth * eHeight;

    *eBuffer = createPixelArray(eWidth * eHeight, *iC);
    *aBuffer = createPixelArray(*iC, 1);

    for (int i = 0; i < *iC; i++)
    {
        fread(*eBuffer + (rowSize * i), sizeof(pixel), rowSize, file);
        fread(*aBuffer + i, sizeof(pixel), 1, file);
    }

    fclose(file);

    // set dimensions in the buffer for easy access later
    *((int32_t *)*eBuffer - 1) = (int32_t)eHeight; // change height var of array to height of each image
    *((int32_t *)*eBuffer - 2) = (int32_t)eWidth;  // change height var of array to height of each image
}

int getInfo(char *name, uint32_t *offset, int32_t *width, int32_t *height)
{
    FILE *info;
    char magicBytes[2] = {0};

    if (info = fopen(name, "rb"))
    {
        fread(&magicBytes, sizeof(char), 2, info);
        if (magicBytes[0] == 'B' && magicBytes[1] == 'M')
        {

            fseek(info, fOffsetOffset, SEEK_SET);
            fread(offset, sizeof(uint32_t), 1, info);

            // read height and width from file
            fseek(info, fWidthOffset, SEEK_SET);
            fread(width, sizeof(int32_t), 1, info);

            fread(height, sizeof(int32_t), 1, info);

            fclose(info);

            return 0;
        }
    }

    return 1;
}

pixel *readFile(char *fileName)
{
    FILE *rptr; // read pointer

    if (rptr = fopen(fileName, "rb"))
    {
        int32_t fWidth;
        int32_t fHeight;
        uint32_t fOffset;

        if (!getInfo(fileName, &fOffset, &fWidth, &fHeight))
        { // check to see if it is actually a bmp

            pixel *fileBuffer = createPixelArray(fWidth, fHeight);

            fseek(rptr, fOffset, SEEK_SET);
            fread(fileBuffer, sizeof(pixel), fWidth * fHeight, rptr);
            fclose(rptr);

            return fileBuffer;
        }
        else
            printf("Malformed BMP: %s\n", fileName);
    }
    else
        printf("File doesn't exist: %s\n", fileName);

    return NULL;
}

int findEmoji(pixel *colour, pixel *averages, int32_t eCount)
{
    pixel closest = {0};
    uint32_t difference = 0; // set to minimum value

    int index = 0;
    uint32_t sum = 0;

    for (int i = 0; i < eCount; i++)
    {
        if (averages->alpha > 0) // if not transparent
        {
            sum = (uint32_t)(255 - abs(averages->blue - colour->blue)) *
                  (255 - abs(averages->green - colour->green)) *
                  (255 - abs(averages->red -
                             colour->red)); // basically returns the same as finding different in euclidiean space

            if (sum > difference)
            {
                index = i;
                difference = sum;
            }
        }

        averages++;
    }

    return index;
}
#endif /* !FILE_FOO_SEEN */