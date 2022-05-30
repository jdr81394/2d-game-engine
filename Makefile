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
			./src/AssetStore/*.cpp
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua5.3
OBJ_NAME = gameengine

################################################################################
# Declare some Makefile rules
################################################################################
build:
	g++ -Wall -std=c++17 $(SRC_FILES) -I"./libs/" -I"/opt/homebrew/include/" -L"/opt/homebrew/lib" -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer  -lm -llua -o 2dgameengine

run:
	./2dgameengine

clean:
	rm $(OBJ_NAME)
