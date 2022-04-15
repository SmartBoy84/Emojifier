#include "auxillary.h"
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

    FILE *emojiBuffer;
    if (argc >= 3)
    {
        generateChart(atoi(argv[2]));
    }

    if (!(emojiBuffer = fopen("emojis.bin", "rb")))
    {
        if (generateChart(defaultScale))
        {
            printf("Failed to find %s, exiting", "emojis.bin");
            return 1;
        }
        else
        {
            emojiBuffer = fopen("emojis.bin", "rb");
        }
    }

    __int32_t height;
    fread(&height, sizeof(__int32_t), 1, emojiBuffer);

    __int32_t width;
    fseek(emojiBuffer, sizeof(__int32_t), SEEK_SET);
    fread(&width, sizeof(__int32_t), 1, emojiBuffer);

    for (int i = 0; i < 3301; i++)
    {
        sleep(1);

        printf("%d\n", i);

        fseek(emojiBuffer, height * width * i * sizeof(pixel), SEEK_CUR);
        pixel *test = createPixelArray(width, height);
        fread(test, sizeof(pixel), height * width, emojiBuffer);

        writeArray(test, "test.bmp");
    }

    fclose(emojiBuffer);
    return 0;
}