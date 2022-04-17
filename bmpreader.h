#ifndef bmpreader // prevent redefinitions
#define bmpreader

#include "bmpcreator.h"
#include "math.h"

// double for buffers because we want access to the pointer value not the pointer to
void readBuffer(FILE *file, __uint32_t *iC, pixel **eBuffer, pixel **aBuffer)
{

    fread(iC, sizeof(__int32_t), 1, file);

    // get the width and height of the images from file
    __int32_t eWidth;
    fread(&eWidth, sizeof(__int32_t), 1, file);

    __int32_t eHeight;
    fread(&eHeight, sizeof(__int32_t), 1, file);

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
    *((__int32_t *)*eBuffer - 1) = (__int32_t)eHeight; // change height var of array to height of each image
    *((__int32_t *)*eBuffer - 2) = (__int32_t)eWidth;  // change height var of array to height of each image
}

int getInfo(char *name, __uint32_t *offset, __int32_t *width, __int32_t *height)
{
    FILE *info;
    char magicBytes[2] = {0};

    if (info = fopen(name, "rb"))
    {
        fread(&magicBytes, sizeof(char), 2, info);
        if (magicBytes[0] == 'B' && magicBytes[1] == 'M')
        {

            fseek(info, fOffsetOffset, SEEK_SET);
            fread(offset, sizeof(__uint32_t), 1, info);

            // read height and width from file
            fseek(info, fWidthOffset, SEEK_SET);
            fread(width, sizeof(__int32_t), 1, info);

            fread(height, sizeof(__int32_t), 1, info);

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
        __int32_t fWidth;
        __int32_t fHeight;
        __uint32_t fOffset;

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

int findEmoji(pixel *colour, pixel *averages, __int32_t eCount)
{
    pixel closest = {0};
    __uint32_t difference = 0; // set to minimum value

    int index = 0;
    __uint32_t sum = 0;

    for (int i = 0; i < eCount; i++)
    {
        if (averages->alpha > 0) // if not transparent
        {
            sum = (__uint32_t)(255 - abs(averages->blue - colour->blue)) *
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