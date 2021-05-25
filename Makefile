CC=gcc
PROJECT-CODE=hyperfire-dual-gunners-83
WINDOWS-EXE=HyperFireDualGunners83.exe
SDL_LIB=`sdl-config --libs`
SDL_LIB_WIN=-lmingw32 -lSDLmain -lSDL
SRC=src/*.c

all: build

build:
	$(CC) $(SDL_LIB) $(SRC) -o $(PROJECT-CODE)

build-win:
	$(CC) $(SRC) -o $(WINDOWS-EXE) $(SDL_LIB_WIN)

test:
	./$(PROJECT-CODE)
