# ASCII Space Destroyer

The space shooter game made of ASCII characters.

![ASCII Space Destroyer logo](project_icon_ASCII.png)

## Content

Cross platform C++ source code of the game. It's a home-made open-source game, feel free to clone, modify, and propose pull requests.

## Features

+ Several available space ships
+ Different levels with several waves (french ambiance text)
+ Several save slots
+ Customizable ships (just edit/add txt files in resources folder)
+ 8 different weapons
+ 1 fighting shield
+ unlock new weapons and CPU load (number of blocs on your ship)

## Technology

Libraries used :
+ [SDL 1.2](https://www.libsdl.org/)
+ SDL GFX

## Related

My own website where you can find more about this game, like executables (french).
[Jovian Hersemeule - Projets - ASCII Space Destroyer(FR)](http://jovian-hersemeule.eu/projets/pro_ascii_space.php)

## Installation

### Ubuntu

#### Build

You need `make` and `SDL1` and `SDL_gfx` to build this project.

Example on Ubuntu :
```sh
$ sudo apt install make g++ libsdl-dev libsdl-gfx1.2-dev
$ make
```

To execute :
```sh
$ make exec
```

#### To play

Execute the following commands to install SDL and SDL GFX :

`sudo apt install libsdl1.2debian libsdl-gfx1.2-5`
