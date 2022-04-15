# Emojifier

1. Clone [this](https://github.com/SmartBoy84/EmojiScraper) and change line 27 of convert.js so that the BMP file names are only indexes.
2. After running convert, move all the images from emojis/Apple into emojis and remove all other folders
3. Run `mogrify -format bmp * && rm *.png` to convert all files to 32 bit BMPs
4. Compile and run `emojifier.c`
5. `emoji.bin` contains raw pixel data of all the images and `emojichart.bmp` image of every emoji
