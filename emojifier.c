#include "auxillary.h"
#include "bmpreader.h"
#include "buffer.c"
#include <stdint.h>

#define defaultScale 3
#define printHelp()                                                                                                    \
    {                                                                                                                  \
        printf("emojifier [file name] {[buffer name]} {[emojis folder] [buffer resolution] [file count]}\n");          \
        return 1;                                                                                                      \
    }

int main(int argc, char *argv[])
{
    // 1 - file name, 2 - buffer name, 3 - buffer resolution, 4 - file count
    int32_t imageCount;
    pixel *emojiBuffer;
    pixel *averages;

    if (argc == 1)
        printHelp();

    if (argc == 2)
        readInternalBuffer(internalBuffer, &imageCount, &emojiBuffer, &averages);

    else if (argc == 4)
    {
        printf("Must specify bitmap scale\n\n");
        printHelp();
    }
    else if (argc == 5)
    {
        printf("Must specify number of images in directory\n\n");
        printHelp();
    }
    else if (argc > 6)
    {
        printHelp();
    }
    else
    {
        char *fName;

        if (argc == 6 && !(fName = generateChart(argv[2], argv[3], atoi(argv[4]), atoi(argv[5]))))
        {
            printf("Failed to generate buffer\n");
            return 1;
        }

        FILE *eF = fopen(fName, "rb");

        if (eF == NULL)
        {
            printf("Failed to read buffer %s\n", argv[2]);
            return 1;
        }
        else
            readBufferFile(eF, &imageCount, &emojiBuffer, &averages);
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

        int32_t emojiSize = height(emojiBuffer) * width(emojiBuffer); // get pixel # per image

        // enable transparency
        pixel *transparent = createPixelArray(width(emoji), height(emoji));
        for (int i = 0; i < width(emoji) * height(emoji); i++)
            (transparent + i)->alpha = 0;

        for (int y = 0; y < height(Image); y++)
        {
            for (int x = 0; x < width(Image); x++)
            {

                memcpy(emoji,
                       tempImage->alpha == 0 ? transparent
                                             : emojiBuffer + (findEmoji(tempImage, averages, imageCount) * emojiSize),
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

    return generated; // see if the program has actually failed to do anything at this point
}