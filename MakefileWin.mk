# C:\Users\mvaud\Documents\info\tools\winlibs-x86_64-posix-seh-gcc-13.2.0-mingw-w64ucrt-11.0.1-r5\mingw64\bin\mingw32-make.exe

include Makefile.conf

strip = C:\Users\mvaud\Documents\info\tools\winlibs-x86_64-posix-seh-gcc-13.2.0-mingw-w64ucrt-11.0.1-r5\mingw64\bin\strip.exe

CC = C:\Users\mvaud\Documents\info\tools\winlibs-x86_64-posix-seh-gcc-13.2.0-mingw-w64ucrt-11.0.1-r5\mingw64\bin\gcc.exe
CXX = C:\Users\mvaud\Documents\info\tools\winlibs-x86_64-posix-seh-gcc-13.2.0-mingw-w64ucrt-11.0.1-r5\mingw64\bin\g++.exe
LLD = C:\Users\mvaud\Documents\info\tools\winlibs-x86_64-posix-seh-gcc-13.2.0-mingw-w64ucrt-11.0.1-r5\mingw64\bin\lld-link.exe
CMAKE = C:\Users\mvaud\Documents\info\tools\winlibs-x86_64-posix-seh-gcc-13.2.0-mingw-w64ucrt-11.0.1-r5\mingw64\bin\cmake.exe
MAKE = C:\Users\mvaud\Documents\info\tools\winlibs-x86_64-posix-seh-gcc-13.2.0-mingw-w64ucrt-11.0.1-r5\mingw64\bin\mingw32-make.exe

SDL_LIB = C:\Users\mvaud\Documents\info\tools\SDL2-devel-2.30.1-mingw\SDL2-2.30.1\x86_64-w64-mingw32\lib
SDL_INC = C:\Users\mvaud\Documents\info\tools\SDL2-devel-2.30.1-mingw\SDL2-2.30.1\x86_64-w64-mingw32\include

SDL_MIX_LIB = C:\Users\mvaud\Documents\info\tools\SDL2_mixer-devel-2.8.0-mingw\SDL2_mixer-2.8.0\x86_64-w64-mingw32\lib
SDL_MIX_INC = C:\Users\mvaud\Documents\info\tools\SDL2_mixer-devel-2.8.0-mingw\SDL2_mixer-2.8.0\x86_64-w64-mingw32\include

SDL_TTF_LIB = C:\Users\mvaud\Documents\info\tools\SDL2_ttf-devel-2.22.0-mingw\SDL2_ttf-2.22.0\x86_64-w64-mingw32\lib
SDL_TTF_INC = C:\Users\mvaud\Documents\info\tools\SDL2_ttf-devel-2.22.0-mingw\SDL2_ttf-2.22.0\x86_64-w64-mingw32\include

OBJ = obj
SRC = src
INC = inc
BIN = bin
LIB = lib

LD_FLAGS = -lmingw32 -lSDL2main -lsdl2 -lSDL2_mixer -lSDL2_ttf -llog -lm

PROG_NAME = VGB_emulator.exe

all: prepare $(BIN)\$(PROG_NAME)
	strip $(BIN)\$(PROG_NAME)

prepare:
	if not exist $(LIB)\lib-log\build mkdir $(LIB)\lib-log\build
	cd $(LIB)\lib-log\build && $(CMAKE) -G "MinGW Makefiles" -D CMAKE_MAKE_PROGRAM=$(MAKE) -D CMAKE_C_COMPILER=C:/Users/mvaud/Documents/info/tools/winlibs-x86_64-posix-seh-gcc-13.2.0-mingw-w64ucrt-11.0.1-r5/mingw64/bin/gcc.exe -DCMAKE_LINKER=$(LLD) .. && $(MAKE)

$(BIN)\$(PROG_NAME): $(OBJ)\main.o $(OBJ)\get_rom.o $(OBJ)\cpu.o $(OBJ)\memory.o $(OBJ)\timer.o $(OBJ)\interrupt.o $(OBJ)\ppu.o $(OBJ)\touche.o $(OBJ)\display.o $(OBJ)\audio.o $(OBJ)\real_time.o
	$(CC) $^ -o $@ -L$(SDL_LIB) -L$(SDL_MIX_LIB) -L$(SDL_TTF_LIB) -L$(LIB)\lib-log\build $(LD_FLAGS)

$(OBJ)\main.o: $(SRC)\main.c $(INC)\main.h
	$(CC) -c $< -o $@ -I$(INC) -I$(LIB)\lib-log\inc -I$(SDL_INC) $(C_FLAGS)

$(OBJ)\get_rom.o: $(SRC)\get_rom.c $(INC)\get_rom.h
	$(CC) -c $< -o $@ -I$(INC) -I$(LIB)\lib-log\inc $(C_FLAGS)

$(OBJ)\cpu.o: $(SRC)\cpu.c $(INC)\cpu.h
	$(CC) -c $< -o $@ -I$(INC) -I$(LIB)\lib-log\inc $(C_FLAGS)

$(OBJ)\memory.o: $(SRC)\memory.c $(INC)\memory.h
	$(CC) -c $< -o $@ -I$(INC) -I$(LIB)\lib-log\inc $(C_FLAGS)

$(OBJ)\timer.o: $(SRC)\timer.c $(INC)\timer.h
	$(CC) -c $< -o $@ -I$(INC) -I$(LIB)\lib-log\inc $(C_FLAGS)

$(OBJ)\interrupt.o: $(SRC)\interrupt.c $(INC)\interrupt.h
	$(CC) -c $< -o $@ -I$(INC) -I$(LIB)\lib-log\inc $(C_FLAGS)

$(OBJ)\ppu.o: $(SRC)\ppu.c $(INC)\ppu.h
	$(CC) -c $< -o $@ -I$(INC) -I$(LIB)\lib-log\inc $(C_FLAGS)

$(OBJ)\touche.o: $(SRC)\touche.c $(INC)\touche.h
	$(CC) -c $< -o $@ -I$(INC) -I$(LIB)\lib-log\inc -I$(SDL_INC) $(C_FLAGS)

$(OBJ)\display.o: $(SRC)\display.c $(INC)\display.h
	$(CC) -c $< -o $@ -I$(INC) -I$(LIB)\lib-log\inc -I$(SDL_INC) -I$(SDL_TTF_INC) $(C_FLAGS)

$(OBJ)\audio.o: $(SRC)\audio.c $(INC)\audio.h
	$(CC) -c $< -o $@ -I$(INC) -I$(LIB)\lib-log\inc -I$(SDL_INC) -I$(SDL_MIX_INC) $(C_FLAGS)

$(OBJ)\real_time.o: $(SRC)\real_time.c $(INC)\real_time.h
	$(CC) -c $< -o $@ -I$(INC) -I$(LIB)\lib-log\inc -I$(SDL_INC) $(C_FLAGS)

clean:
	if exist lib\lib-log\build\* del /q lib\lib-log\build\*
	if exist $(OBJ)\*.o del $(OBJ)\*.o

allclean: clean
	if exist $(BIN)\$(PROG_NAME) del $(BIN)\$(PROG_NAME)
	
remake: allclean all