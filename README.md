# Emojifier
[Chek it out!](http://gabba.ga/files/img.png)
![Apple](https://user-images.githubusercontent.com/12468102/163734260-885d23b4-845f-4f97-b060-282c917034d2.jpg)

## Usage
`./emojifier [file name] {[buffer name]} {[emojis folder] [buffer resolution] [file count]}`
* argument wrapped in `{}` are optional and only need to be set if using/generating a custom buffer
Use convertimg.sh to convert to compatible 32bit bmp and compress afterwards if needed\

To convert image to a compatible format install imagemagick and run `mogrify -depth 32 -alpha set -format bmp [input img]`


## Dev instructions
1. Clone [this](https://github.com/SmartBoy84/EmojiScraper) and change line 27 of convert.js so that the BMP file names are only indexes.
2. After running convert, move all the images from whichever set you want into emojis and remove all other folders
3. Run `mogrify -format bmp * && rm *.png` to convert all files to 32 bit BMPs
4. Run `mak.sh` to compile
5. Either use the inbuilt emoji buffer, the transparent one linked in release or generate your own using above instructions

## Todo
- ~Elegantly downscale emoji files and input image~
- ~Handle user input flags~
- ~Add option to choose which emoji set to use~
- ~Embed emoji data into executable~
- Build in auto image conversion
