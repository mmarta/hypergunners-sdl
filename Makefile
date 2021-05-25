CC=gcc
WIN_CC=mingw32-gcc
PROJECT_CODE=hyperfire-dual-gunners-83
WINDOWS_EXE=HyperFireDualGunners83.exe
SDL_LIB=`sdl2-config --libs` -lSDL2_image
SDL_LIB_WIN=-lmingw32 -lSDL2main -lSDL2 -lSDL2_image
SRC=src/*.c

all: build

build:
	$(CC) $(SDL_LIB) $(SRC) -o $(PROJECT_CODE)

build-win:
	$(CC) -m32 $(SRC) -o $(WINDOWS_EXE) $(SDL_LIB_WIN)

test:
	./$(PROJECT_CODE)
