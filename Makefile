################################################################################
# Declare some Makefile variables
################################################################################
CC = g++
LANG_STD = -std=c++17
COMPILER_FLAGS = -Wall -Wfatal-errors
INCLUDE_PATH = -I"./libs/" -I"/opt/homebrew/include/" -L"/opt/homebrew/lib"
SRC_FILES = ./src/*.cpp \
			./src/Game/*.cpp \
			./src/Logger/*.cpp \
			./src/ECS/*.cpp \
			./src/AssetStore/*.cpp
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer  -lm -llua
OBJ_NAME = gameengine

################################################################################
# Declare some Makefile rules
################################################################################
build:
	g++ $(COMPILER_FLAGS) $(LANG_STD) $(SRC_FILES) $(INCLUDE_PATH) $(LINKER_FLAGS) -o $(OBJ_NAME)

run:
	./2dgameengine

clean:
	rm $(OBJ_NAME)
