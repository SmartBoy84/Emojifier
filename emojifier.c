#include "auxillary.h"
#include "bmpreader.h"
#include <time.h>

#define defaultScale 3

// fread() MOVES THE DAMN POINTER

int main(int argc, char *argv[])
{
    FILE *Image;
    if (argc < 2 || !(Image = fopen(argv[0], "rb")))
    {
        printf("File doesn't exist, exiting");
        return 1;
    }

    FILE *eF;
    if (argc >= 3)
        generateChart(atoi(argv[2]));

    if (!(eF = fopen("emojis.bin", "rb")))
    {
        if (generateChart(defaultScale))
        {
            printf("Failed to find %s, exiting", "emojis.bin");
            return 1;
        }
        else
            eF = fopen("emojis.bin", "rb");
    }

    __int32_t imageCount;
    pixel *emojiBuffer;
    pixel *averages;

    readBuffer(eF, &imageCount, &emojiBuffer, &averages);

    __int32_t imageSize = height(emojiBuffer) * width(emojiBuffer); // get pixel # per image

    printf("\nHeight: %d\nWidth: %d\nEmoji count: %d\nEmoji pixel: %d\n", height(emojiBuffer), width(emojiBuffer),
           imageCount, imageSize);

    // read test image
    pixel *test = createPixelArray(width(emojiBuffer), height(emojiBuffer));
    // int index = 1968;

    for (int index = 0; index < imageCount; index++)
    {
        usleep(1000000 / 5);

        printf("%d - rgba(%d, %d, %d, %d)\n", index, *((__uint8_t *)(averages + index) + 2),
               *((__uint8_t *)(averages + index) + 1), *((__uint8_t *)(averages + index)),
               *((__uint8_t *)(averages + index) + 3));

        pixel *averageStrip = createPixelArray(width(emojiBuffer), 3);
        for (int z = 0; z < width(averageStrip) * height(averageStrip); z++)
            *(averageStrip + z) = *(averages + index);

        memcpy(test, emojiBuffer + (imageSize * index), imageSize * sizeof(pixel));
        memcpy(test, averageStrip, width(averageStrip) * height(averageStrip) * sizeof(pixel));

        writeArray(test, "test.bmp");
    }

    return 0;
}