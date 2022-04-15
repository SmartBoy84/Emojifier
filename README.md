# Emojifier
## Instructions
1. Clone [this](https://github.com/SmartBoy84/EmojiScraper) and change line 27 of convert.js so that the BMP file names are only indexes.
2. After running convert, move all the images from emojis/Apple into emojis and remove all other folders
3. Run `mogrify -format bmp * && rm *.png` to convert all files to 32 bit BMPs
4. Compile and run `emojifier.c`
5. `emoji.bin` contains raw pixel data of all the images and `emojichart.bmp` image of every emoji

## Idea
- Get average colour for each emoji, generate post downscale
- For each pixel of an image, loop through averages and find the closest color
> Add total deviation from red, blue and green
- Using index of image, get from raw buffer and write to main
> 1000 x 1000 image and set of 3300 emojis would mean 3,300,000,000 recurrences - takes ~1.5 minute\
> Add an option for live progress and set interval
> Scale factor of 3 means emoji size is 24 x 24 which would mean for a 1000 x 1000 image total size is 1000^2 * 24^2 = 576,000,000 pixels = 24,000 x 24,000
## Todo
- ~Elegantly downscale emoji files and input image~
- Add option to choose which emoji set to use
- Embed emoji data into executable
- Handle user input flags
