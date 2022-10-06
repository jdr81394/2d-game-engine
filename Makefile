################################################################################
# Declare some Makefile variables
################################################################################
CC = g++
LANG_STD = -std=c++17
COMPILER_FLAGS = -Wall -Wfatal-errors
INCLUDE_PATH = -I"./libs/" -L"/opt/homebrew/Cellar/lua"
SRC_FILES = ./src/*.cpp \
			./src/Game/*.cpp \
			./src/Logger/*.cpp \
			./src/ECS/*.cpp \
			./src/AssetStore/*.cpp \
			./libs/imgui/*.cpp
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua5.3
OBJ_NAME = gameengine

################################################################################
# Declare some Makefile rules
################################################################################
build:
	g++ -Wall -std=c++17 $(SRC_FILES) -L"/opt/homebrew/Cellar/gcc/11.3.0/lib/gcc/11/gcc/aarch64-apple-darwin21/11/include-fixed/SDL.h"  -I"./libs/" -I"/opt/homebrew/include/" -L"/opt/homebrew/lib" -lSDL -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer  -lm -llua -o 2dgameengine

run:
	./2dgameengine

clean:
	rm $(OBJ_NAME)
