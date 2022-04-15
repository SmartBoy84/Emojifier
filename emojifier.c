#include "auxillary.h"
#include <time.h>

int main()
{
    generateChart(1);

    FILE *rptr = fopen("emojis.bin", "rb");

    __int32_t height;
    fread(&height, sizeof(__int32_t), 1, rptr);

    __int32_t width;
    fseek(rptr, sizeof(__int32_t), SEEK_SET);
    fread(&width, sizeof(__int32_t), 1, rptr);

    for (int i = 0; i < 3301; i++)
    {
        sleep(1);

        printf("%d\n", i);

        fseek(rptr, height * width * i * sizeof(pixel), SEEK_CUR);
        pixel *test = createPixelArray(width, height);
        fread(test, sizeof(pixel), height * width, rptr);

        writeArray(test, "test.bmp");
    }

    fclose(rptr);
    return 0;
}