#!/bin/bash
#
# usage, e.g.: ./make.sh minigames

PROGRAM=$1

clear; clear; g++ -x c -g -std=c99 -O3 -fmax-errors=5 -pedantic -Wall -Wextra -DSAF_PLATFORM_SDL2=1 -o ${PROGRAM} ${PROGRAM}.h -lSDL2 -lX11 -lcsfml-graphics -lcsfml-window -lcsfml-system -lcsfml-audio -lncurses && ./${PROGRAM}
