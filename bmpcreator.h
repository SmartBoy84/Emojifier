#ifndef bmpcreator // prevent redefinitions
#define bmpcreator

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define fOffsetOffset 10
#define fWidthOffset 18

typedef struct
{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t alpha;
} pixel;

static inline int32_t height(pixel *pixels)
{
    return *((int32_t *)pixels - 1);
}
static inline int32_t width(pixel *pixels)
{
    return *((int32_t *)pixels - 2);
}
static inline int32_t arraySize(pixel *pixels)
{
    return height(pixels) * width(pixels);
}

#pragma pack(1)
struct
{                        // File header (14 bytes)
    char magicBytes[2];  // BM
    uint32_t fileSize; // in bytes
    uint32_t reserved; // ignore - 0
    uint32_t offset;   // offset to pixel array
} FileHeader = {
    .magicBytes = "BM",
    .fileSize = 0, // pixelarraysize + total headersize - placeholder
    .reserved = 0,
    .offset = 122 // 14 + 108
};

struct
{                          // BITMAPINFOHEADER 61
    uint32_t headerSize; // from this point onwards

    int32_t width; // in pixels
    int32_t height;

    uint16_t colorPlanes; // 1
    uint16_t bitDepth;    // 32

    uint32_t compression; // 3
    uint32_t bitmapSize;  // size of pixel array (bytes)

    int32_t horizontalResolution; // print resolution
    int32_t verticalResolution;

    uint32_t colorPaletteSize;    // ignore - 0
    uint32_t importantColorsSize; // ignore - 0

    uint32_t redBitMask; // set one bit to 256 [0xFF]
    uint32_t greenBitMask;
    uint32_t blueBitMask;
    uint32_t alphaBitMask;

    char colorSpace[4]; // little-endia sRGB (inversed - BGRs)

    uint8_t colorSpaceEndpoints[48]; // weird technical stuff - ignore
} InfoHeader = {.headerSize = 108,

                .width = 0,  // place holder
                .height = 0, // place holder
                .colorPlanes = 1,
                .bitDepth = 32,
                .compression = 3,

                .horizontalResolution = 2835, // Idk
                .verticalResolution = 2835,

                .colorPaletteSize = 0,
                .importantColorsSize = 0,

                // blue, green, red, alpha
                .blueBitMask = 0xFF,
                .greenBitMask = 0xFF << 8,
                .redBitMask = 0xFF << 16,
                .alphaBitMask = 0xFF << 24,

                .colorSpace = {'B', 'G', 'R', 's'},
                .colorSpaceEndpoints = {0}};

size_t headerSize = sizeof(FileHeader) + sizeof(InfoHeader);

char *getHeader(int32_t width, int32_t height)
{
    uint32_t pixelArraySize = width * height * 4; // 4 bytes per pixel

    FileHeader.fileSize = FileHeader.offset + pixelArraySize;

    InfoHeader.width = width;
    InfoHeader.height = height;
    InfoHeader.bitmapSize = pixelArraySize;

    char *buffer = (char *)malloc(headerSize);

    // this may seem really inefficient but this was bound to happen even if I created two header structs
    memcpy(buffer, (char *)&FileHeader, sizeof(FileHeader));
    memcpy(buffer + sizeof(FileHeader), (char *)&InfoHeader, sizeof(InfoHeader));

    return buffer;
}

pixel *createPixelArray(int32_t width, int32_t height)
{
    size_t dimension = (size_t)sizeof(int32_t) / sizeof(pixel); // make room for the dimensions at the start

    size_t arraySize = (2 * dimension) + (width * height);
    pixel *array = calloc(arraySize, sizeof(pixel));

    // tuck the width and height at the back somewhere
    memcpy(array, &width, sizeof(int32_t));
    array += dimension;

    memcpy(array, &height, sizeof(int32_t));
    array += dimension;

    return array;
}

void writeArray(pixel *pixels, char *fileName)
{

    FILE *wptr = fopen(fileName, "wb");

    fwrite(getHeader(width(pixels), height(pixels)), headerSize, 1, wptr);
    fwrite(pixels, arraySize(pixels), sizeof(pixel), wptr);

    fclose(wptr);
}
#endif /* !FILE_FOO_SEEN */