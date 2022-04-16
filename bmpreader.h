#include "bmpcreator.h"

pixel *readBuffer(FILE *file, __uint32_t *iC)
{

    fread(iC, sizeof(__int32_t), 1, file);

    __int32_t eWidth;
    fseek(file, sizeof(__int32_t), SEEK_SET);
    fread(&eWidth, sizeof(__int32_t), 1, file);

    __int32_t eHeight;
    fseek(file, sizeof(__int32_t) * 2, SEEK_SET);
    fread(&eHeight, sizeof(__int32_t), 1, file);

    printf("%d %d", eWidth, eHeight);

    pixel *pixelArray = createPixelArray(eWidth * eHeight, *iC);
    fseek(file, sizeof(__int32_t) * 3, SEEK_SET);
    fread(pixelArray, sizeof(pixel), eHeight * eWidth * *iC, file);

    fclose(file);

    *((__int32_t *)pixelArray - 1) = (__int32_t)eHeight; // change height var of array to height of each image
    *((__int32_t *)pixelArray - 2) = (__int32_t)eWidth;  // change height var of array to height of each image

    return pixelArray;
}