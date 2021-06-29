CC=gcc
WIN_CC=mingw32-gcc
PROJECT_CODE=hypergunners
WINDOWS_EXE=HyperGunners.exe
SDL_LIB=`sdl2-config --libs` -lSDL2_image
SDL_LIB_WIN=-lmingw32 -lSDL2main -lSDL2 -lSDL2_image
SRC=src/*.c

all: build-win

build-mac:
	$(CC) $(SDL_LIB) $(SRC) -o $(PROJECT_CODE)

build-win:
	$(CC) -m32 $(SRC) -o $(WINDOWS_EXE) $(SDL_LIB_WIN)

test-mac:
	./$(PROJECT_CODE)

test-win:
	$(WINDOWS_EXE)
