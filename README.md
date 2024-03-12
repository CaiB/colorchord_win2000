ColorChord
==========

Table of Contents
-----------------

- [ColorChord](#colorchord)
  - [Table of Contents](#table-of-contents)
  - [About this Fork](#about-this-fork)
  - [What is ColorChord?](#what-is-colorchord)
  - [Background](#background)
  - [Current State of Affairs](#current-state-of-affairs)
  - [ColorChord: Embedded](#colorchord-embedded)
- [Build Instructions](#build-instructions)
  - [Building ColorChord Embedded](#building-colorchord-embedded)
  - [Building with Linux](#building-with-linux)
  - [Building with Windows](#building-with-windows)
    - [clang](#clang)
    - [TCC](#tcc)
    - [MSYS2](#msys2)
  - [Using](#using)
  - [Additional Videos](#additional-videos)

About this Fork
---------------
This is a silly idea I had, and somehow made it work. My "usecase" is to compile and run ColorChord using Visual Studio 6.0 on Windows 2000. Notably VC6 is not a C99 compiler, which introduced some additional difficulties.

If you happen to have an equivalent setup, you should be able to simply open the workspace `colorchord2/windows2000/ColorChord_Win2000.dsw`, build and run. Alternatively, I've also [uploaded a binary you can simply use as-is in the releases section](https://github.com/CaiB/colorchord_win2000/releases).

There is no WASAPI on Windows 2000, so loopback audio isn't supported, only microphone inputs. A few other things,such as network and HID outputs, have also been omitted since I couldn't be bothered to fix them.

Feel free to submit PRs if you actually care about this for some reason, but this was mostly just a silly experiment. Feel free to share pictures of it running on real hardware if you do end up using it :)

What is ColorChord?
-------------------

Chromatic Sound to Light Conversion System.  It's really that simple.  Unlike so many of the sound responsive systems out there, ColorChord looks at the chromatic properties of the sound.  It looks for notes, not ranges.  If it hears an "E" it doesn't care what octave it's in, it's an E.  This provides a good deal more interesting patterns between instruments and music than would be available otherwise.

Background Video here:

[![What Is ColorChord?](https://img.youtube.com/vi/qE3zEM8qM00/0.jpg)](https://www.youtube.com/watch?v=qE3zEM8qM00)

ColorChord on an ESP8266:

[![Websockets for Awesome on an ESP8266](https://img.youtube.com/vi/8ISbmQTbjDI/0.jpg)](https://www.youtube.com/watch?v=8ISbmQTbjDI)

More videos below!

Background
----------

Developed over many years, ColorChord 2 is now at the alpha stages.  ColorChord 2 uses the same principles as ColorChord 1.  A brief writeup on that can be seen here: http://cnlohr.blogspot.com/2010/11/colorchord-sound-lighting.html

The major differences in ColorChord 2 is the major rewrite to move everything back to the CPU and a multitude of algorithmic optimizations to make it possible to run on something other than the brand newest of systems.

Feuge in D Minor (ColorChord 2 running a strip of WS2812 LEDs):

[![ColorChord](https://img.youtube.com/vi/s8qWqYTpCN4/0.jpg)](https://www.youtube.com/watch?v=s8qWqYTpCN4)

ColorChord 2 running a voronoi diagram with Mayhem's Dr. Rocker

[![ColorChord 2 Preview: Dr Rocker by Mayhem](https://img.youtube.com/vi/UI4eqOP2AU0/0.jpg)](https://www.youtube.com/watch?v=UI4eqOP2AU0)


Current State of Affairs
------------------------

Currently, ColorChord 2 is designed to run on Linux or Windows.  It's not particularly tied to an architecture, but does pretty much need a dedicated FPU to achieve any decent performance.  Right now there aren't very many output options available for it.  The most interesting one used for debugging is a voronoi-diagram-like thing called "DisplayShapeDriver."

ColorChord: Embedded
--------------------

There is work on an embedded version of ColorChord, which avoids floating point operations anywhere in the output pipeline.  Though I have made efforts to port it to AVRs, it doesn't seem feasable to operate on AVRs without some shifty tricks which I'd like to avoid, so I have retargeted my efforts to 32-bit systems, such as the STM32F303, STM32F407, and (somehow) the ESP8266.  ColorChord Embedded uses a different codebase, located in the [embeddedcommon](/embeddedcommon) and distributed among the various embedded* folders.

Build Instructions
==================

Building ColorChord Embedded
----------------------------

The embedded version of Colorchord has a different build system than the desktop versions of Colorchord. See the build instructions for each of the supported embedded architectures in their respective folders

- [esp8266](embedded8266/README.md#build-instructions)
- [stm32f303](embeddedstm32f303/README.md)
- [stm32f407](embeddedstm32f407/) - TODO Add readme

Building with Linux
-----------------
From the linux terminal use `apt-get` to install the following packages for Debian/Ubuntu/Mint:
```
apt-get install build-essential libpulse-dev libasound2-dev libx11-dev libxext-dev libxinerama-dev libusb-1.0-0-dev libudev-dev
```

To get colorchord, type:

```
git clone --recurse-submodules https://github.com/cnlohr/colorchord
```
This will download the colorchord source code into the colorchord directory

You really want to be using colorchord2, so we need to move to it's build directory to compile the source code.
```
cd colorchord/colorchord2
```
To make colorchord, type:

```
make
```
Building with Windows
-------------------

There are 3 options available for building on Windows, clang, or TCC, MSYS2.
### clang
Start by [downloading](https://clang.llvm.org/) the clang compiler, and installing it.
You can also install using `winget` using the following command in PowerShell
```
winget install -e --id LLVM.LLVM
```

If you have the Windows SDK installed, you should not need to do any additional work.  
If you do not, you'll want to either [install it](https://developer.microsoft.com/en-US/windows/downloads/windows-11-sdk/) to get the official headers. Realistically, it's easyist to get the headers by installing Visual Studio Communitiy and selecting the C/C++ Desktop Application devlopment package and installing it from there [Visual Studio](https://visualstudio.microsoft.com/).

Once the Windows headers are installed run the clang batch script, and it should output to `colorchord2/colorchord.exe`.
```
./compile-clang.bat
```

### TCC
Start by [downloading TCC](http://savannah.nongnu.org/projects/tinycc/), and extracting it to `C:\tcc`.
You can put it anywhere you like, but the compile script defaults to this location. If you install 
to the default location, you can skip the next step.

Edit the batch script at `colorchord2/windows/compile.bat`:
- Edit line 17 (`CC`) to be the location where you put TCC. If there are spaces in the path, wrap the entire path in quotes.

Note that TCC is not able to use the Windows SDK, and as such using the unofficial headers is required, and automatically enabled when compiling with TCC. If you encounter issues, try the clang method above instead. TCC does not support open-gl rendering and is limited to software rendering.

### MSYS2
With either 64bit or 32bit [MSYS2](https://msys2.github.io/) installed, run the _MSYS2 MSYS_ launcher and use `pacman` to set up a MinGW32 toolchain, if you don't have one already:
```
pacman -S mingw-w64-i686-toolchain
```
If you see "`/mingw32 exists in filesystem`", you must temporarily rename or relocate the _mingw32.exe_ file at MSYS2's root folder level and try again. Restore _mingw32.exe_ before moving on.

Next, run the _MSYS2 MinGW 32bit_ launcher to access the toolchain. The previously launched _MSYS_ terminal can be closed.

To make colorchord, navigate to your working copy and type:
```
mingw32-make colorchord.exe
```

Using
-----

To run colorchord, use the following syntax:

```
./colorchord [config file, by default 'default.conf'] [any additional parameters]
```

If you edit default.conf while the program is running and resave it, it will use the settings in the newly saved file.

Note that the colorchord executable file is located in the colorchord2 directory in the colorchord repository. 

Additional Videos
-----------------

[![Come Thou Fount of Every Blessing](https://img.youtube.com/vi/aaB4Afyh5z0/0.jpg)](https://www.youtube.com/watch?v=aaB4Afyh5z0)

[![ColorChord 2 - David Chapman Plays Sax](https://img.youtube.com/vi/DYoK4cZoZ_0/0.jpg)](https://www.youtube.com/watch?v=DYoK4cZoZ_0)


