#include "auxillary.h"
#include "bmpreader.h"
#include <time.h>

#define defaultScale 3

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
    {
        generateChart(atoi(argv[2]));
    }

    if (!(eF = fopen("emojis.bin", "rb")))
    {
        if (generateChart(defaultScale))
        {
            printf("Failed to find %s, exiting", "emojis.bin");
            return 1;
        }
        else
        {
            eF = fopen("emojis.bin", "rb");
        }
    }

    __int32_t imageCount;
    pixel *emojiBuffer = readBuffer(eF, &imageCount);
    __int32_t imageSize = height(emojiBuffer) * width(emojiBuffer); // get pixel # per image

    printf("\nHeight: %d\nWidth: %d\nEmoji count: %d\nEmoji pixel: %d\n", height(emojiBuffer), width(emojiBuffer),
           imageCount, imageSize);

    pixel *test = createPixelArray(width(emojiBuffer), height(emojiBuffer));

    for (int i = 0; i < imageCount; i++)
    {
        sleep(1);
        printf("%d\n", i);

        memcpy(test, emojiBuffer + (imageSize * i), imageSize * sizeof(pixel));
        writeArray(test, "test.bmp");
    }

    return 0;
}