# Aether ![Build Status](https://github.com/tallbl0nde/Aether/workflows/Build/badge.svg)

My attempt at creating a Horizon-esque UI library for homebrew. While there's no examples just yet, I recommend looking at [NX-Activity-Log](https://github.com/tallbl0nde/NX-Activity-Log) for an example of implementation as it relies on this library for it's UI.

Documentation can be found [here](https://tallbl0nde.github.io/Aether/)!

## Building
You'll need the following packages installed through (dkp-)pacman in order for Aether to compile:
```
devkit-env
devkitA64
libnx
switch-freetype
switch-sdl2
switch-sdl2_gfx
switch-sdl2_image
switch-sdl2_ttf
```

After installing the packages, run the following command.

#### For Arch Linux based distro / Windows users
```
source ${DEVKITPRO}/switchvars.sh
```

#### For all others UNIX based distro users
```
PATH=${DEVKITPRO}/portlibs/switch/bin/:${PATH}
```

**NOTE**: If you just installed `devkit-env`, run the following command first!
```
source /etc/profile.d/devkit-env.sh
```

Once these are installed, simply run `make` or `make library` in the same directory as this README to build the library.

## Incorporating into your Project
### 1. Add as a submodule
I recommend adding Aether as a Git Submodule by running the following commands (note your project must have a git repository initialized):
```
git submodule add https://github.com/tallbl0nde/Aether.git
git submodule update --init
git commit -m "Add Aether library"
```
### 2. Edit Makefile
Add the following lines in **your** Makefile after `LIBS` & `LIBDIRS` respectively.

```
LIBS    += -lAether -lnx `sdl2-config --libs` -lSDL2_ttf `freetype-config --libs`\
           -lSDL2_gfx -lSDL2_image -lpng -ljpeg -lwebp
```
```
LIBDIRS += Aether
```
### 3. Done!
**NOTE:** This will setup Aether to clone and compile in the root (top) directory in your project's repository.

To move the submodule to a different directory (for example, we will move Aether to `libs` in the root of project's directory), run the following command:
```
git mv Aether/ libs/
```

Then, change the `LIBDIRS` line you added in step 2 to the following:
```
LIBDIRS += libs/Aether
```
## Credits
[eXhumer](https://www.github.com/eXhumer/) - Initial documentation & automatic build testing along with documentation generation (with GitHub Actions)

[rtrussell](https://www.github.com/rtrussell) - SDL2_gfx extensions