#include "auxillary.h"
#include "bmpreader.h"
#include <time.h>

#define defaultScale 3

int main(int argc, char *argv[])
{
    pixel *Image;

    if (argc < 2 || !(Image = readFile(argv[1])))
        return 1;

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

    printf("Height: %d\nWidth: %d\nEmoji count: %d\n", height(emojiBuffer), width(emojiBuffer), imageCount);

    // CREATE THE DAMNED IMAGE NOW
    pixel *finalCanvas =
        createPixelArray(width(Image) * width(emojiBuffer),
                         height(Image) * height(emojiBuffer)); // Remember height(emojiBuffer) return that of one

    printf("\nImage size: %dx%d\nCanvas size: %dx%d\n", width(Image), height(Image), width(finalCanvas),
           height(finalCanvas));

    pixel *emoji = createPixelArray(height(emojiBuffer), width(emojiBuffer)); // emoji matching average colour

    // create reference pointers
    pixel *tempImage = Image;
    pixel *tempCanvas = finalCanvas; // temp buffer for canvas
    pixel *tempEmoji;

    pixel *tempInternalCanvas = finalCanvas;

    __int32_t emojiSize = height(emojiBuffer) * width(emojiBuffer); // get pixel # per image

    for (int y = 0; y < height(Image); y++)
    {
        for (int x = 0; x < width(Image); x++)
        {
            memcpy(emoji, emojiBuffer + (findEmoji(tempImage, averages, imageCount) * emojiSize),
                   emojiSize * sizeof(pixel));

            tempEmoji = emoji;
            tempInternalCanvas = tempCanvas;

            for (int i = 0; i < height(emoji); i++)
            {
                memcpy(tempInternalCanvas, tempEmoji, width(emoji) * sizeof(pixel));

                tempEmoji += width(emoji);
                tempInternalCanvas += width(finalCanvas);
            }

            tempImage++;
            tempCanvas += width(emoji);
        }

        tempCanvas += (width(finalCanvas) * (height(emoji) - 1));
    }

    writeArray(finalCanvas, "output.bmp");

    return 0;
}