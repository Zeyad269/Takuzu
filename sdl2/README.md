# Takuzu

This is our Takuzu game that illustrates a 2-Dimensional grid whith options in the side by using SDL2

## Dependencies

```bash
apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev # for Linux Debian/Ubuntu
brew install sdl2 sdl2_image sdl2_ttf                     # for MacOS
```

## Compilation

With CMake:

```bash
mkdir build ; cd build ; cmake .. ; make VERBOSE=ON ; ./game_sdl
```

Different games that can be loaded are given :

```bash
    - 4x4_default.txt
    - 4x4_Empty.txt
    - 4x4_isWrapping.txt
    - 6x6_default.txt
    - 6x6_isWrapping.txt
```


## Game notice

    - S_ZERO is represented by an asteroide
    - S_ONE is represented by a star
    - S_IMMUTABLE_ZERO and S_IMMUTABLE_ONE are represented the same -way as the basic ones but with a different color 

## Objectif : Try to fill the 2D Grid

## Inside the Grid :
    - Right click button :
        -If this button of the mouse is pressed once in an empty case it sets an asteroide in that case
        -If this button of the mouse is pressed a second time in the same case it sets a star
        -If this button of the mouse is pressed a third time in that same case it sets as an empty case   
    - Left click button :
        -If this button of the mouse is pressed once in an empty case it sets a star in that case
        -If this button of the mouse is pressed a second time in the same case it sets an asteroide 
        -If this button of the mouse is pressed a third time in that same case it sets as an empty case 

## Options
    There are some options that allows you to :
        - Display a support window
        - Solve the game
        - Undo a move
        - Redo a move
        - Reset the game
        - Save the game


Enjoy this Takuzu Game!

