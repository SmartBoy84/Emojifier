#include "auxillary.h"
#include "bmpreader.h"
#include <time.h>

#define defaultScale 3

// fread() MOVES THE DAMN POINTER

int main(int argc, char *argv[])
{
    pixel *Image;
    if (argc < 2 || !(Image = readFile(argv[1])))
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

    pixel *imageBuffer;

    readBuffer(eF, &imageCount, &emojiBuffer, &averages);

    __int32_t imageSize = height(emojiBuffer) * width(emojiBuffer); // get pixel # per image

    printf("Height: %d\nWidth: %d\nEmoji count: %d\nEmoji pixel: %d\n", height(emojiBuffer), width(emojiBuffer),
           imageCount, imageSize);

    return 0;
}