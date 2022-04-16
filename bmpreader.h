#ifndef bmpreader // prevent redefinitions
#define bmpreader

#include "bmpcreator.h"

// double for buffers because we want access to the pointer value not the pointer to
void *readBuffer(FILE *file, __uint32_t *iC, pixel **eBuffer, pixel **aBuffer)
{

    fread(iC, sizeof(__int32_t), 1, file);

    // get the width and height of the images from file
    __int32_t eWidth;
    fread(&eWidth, sizeof(__int32_t), 1, file);

    __int32_t eHeight;
    fread(&eHeight, sizeof(__int32_t), 1, file);

    printf("%d %d", eWidth, eHeight);

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

    if (!(info = fopen(name, "rb")))
    {
        printf("No file in emojis folder?");
        return 1;
    }

    fseek(info, fOffsetOffset, SEEK_SET);
    fread(offset, sizeof(__uint32_t), 1, info);

    // read height and width from file
    fseek(info, fWidthOffset, SEEK_SET);
    fread(width, sizeof(__int32_t), 1, info);

    fseek(info, fWidthOffset + sizeof(__int32_t), SEEK_SET);
    fread(height, sizeof(__int32_t), 1, info);

    fclose(info);
    return 0;
}
#endif /* !FILE_FOO_SEEN */