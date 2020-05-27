# Aether
My attempt at creating a Horizon-esque UI library for homebrew. While there's no examples just yet I recommend looking at [NX-Activity-Log](https://github.com/tallbl0nde/NX-Activity-Log) for an example of implementation as it relies on this library for it's UI.

Documentation can be found [here](https://tallbl0nde.github.io/Aether/)!

A massive thanks to [eXhumer](https://www,github.com/eXhumer/) for preparing the documentation :)

## Building
You'll need the following packages installed through (dkp-)pacman in order for Aether to compile:
```
devkitA64
libnx
switch-sdl2
switch-sdl2_gfx
switch-sdl2_image
switch-sdl2_ttf
```
Once these are installed, simply run `make` in the same directory as this README.

## Incorporating into your Project
### 1. Add as a submodule
I recommend adding Aether as a Git Submodule by running the following commands (note your project must have a git repository initialized):
```
git submodule add https://github.com/tallbl0nde/Aether.git
git submodule update --init
git commit -m "Add Aether library"
```
### 2. Edit Makefile
Change the following lines in **your** Makefile to:
```
LIBS    := -lAether -lnx <your libs here> `sdl2-config --libs` -lSDL2_ttf `freetype-config --libs` -lSDL2_gfx -lSDL2_image -lpng -ljpeg -lwebp

LIBDIRS := <your libdirs here> $(CURDIR)/Aether
```
### 3. Done!
**Note:** This will setup Aether to clone and compile in the root (top) directory in your project's repository.

To move the submodule to a different directory (for example, we will move Aether to `libs` in the root of project's directory), run the following command
```
git mv Aether/ libs/
```