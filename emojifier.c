#include "auxillary.h"
#include "bmpreader.h"
#include "buffer.c"
#include <time.h>

#define defaultScale 3

int main(int argc, char *argv[])
{
    // 1 - file name, 2 - buffer name, 3 - buffer resolution, 4 - file count
    __int32_t imageCount;
    pixel *emojiBuffer;
    pixel *averages;

    if (argc == 1)
    {
        printf("emojifier [file name] {[buffer name] [buffer resolution] [file count]}\n");
        return 0;
    }

    if (argc == 2)
        readInternalBuffer(internalBuffer, &imageCount, &emojiBuffer, &averages);

    else if (argc == 4)
    {
        printf("Must specify number of images in directory\n");
        return 1;
    }
    else
    {
        FILE *eF;

        if (argc >= 5 && generateChart(argv[2], atoi(argv[3]), atoi(argv[4])))
        {
            printf("Failed to generate buffer\n");
            return 1;
        }

        if (eF = fopen(argv[2], "rb"))
            readBufferFile(eF, &imageCount, &emojiBuffer, &averages);
        else
        {
            printf("Failed to read buffer %s\n", argv[2]);
            return 1;
        }
    }

    pixel *Image;

    if (Image = readFile(argv[1]))
    {

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

    return 1;
}