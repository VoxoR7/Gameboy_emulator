# D:\Documents\Informatique\Code\dev\mingw64\bin\mingw32-make.exe

include Makefile.conf

strip = D:\Documents\Informatique\Code\dev\winlibs-x86_64-posix-seh-gcc-12.1.0-mingw-w64ucrt-10.0.0-r2\mingw64\bin\strip.exe

CC = D:\Documents\Informatique\Code\dev\mingw64\bin\gcc.exe

SDL_LIB = D:\Documents\Informatique\Code\dev\SDL2-2.0.22\x86_64-w64-mingw32\lib
SDL_INC = D:\Documents\Informatique\Code\dev\SDL2-2.0.22\x86_64-w64-mingw32\include

SDL_MIX_LIB = D:\Documents\Informatique\Code\dev\SDL2_mixer-2.0.4\x86_64-w64-mingw32\lib
SDL_MIX_INC = D:\Documents\Informatique\Code\dev\SDL2_mixer-2.0.4\x86_64-w64-mingw32\include

SDL_TTF_LIB = D:\Documents\Informatique\Code\dev\SDL2_ttf-2.0.18\x86_64-w64-mingw32\lib
SDL_TTF_INC = D:\Documents\Informatique\Code\dev\SDL2_ttf-2.0.18\x86_64-w64-mingw32\include

OBJ = obj
SRC = src
INC = inc
BIN = bin

LD_FLAGS = -lmingw32 -lSDL2main -lsdl2 -lSDL2_mixer -lSDL2_ttf

PROG_NAME = VGB_emulator.exe

all: $(BIN)\$(PROG_NAME)
	strip $(BIN)\$(PROG_NAME)

$(BIN)\$(PROG_NAME): $(OBJ)\main.o $(OBJ)\get_rom.o $(OBJ)\cpu.o $(OBJ)\memory.o $(OBJ)\timer.o $(OBJ)\interrupt.o $(OBJ)\ppu.o $(OBJ)\touche.o $(OBJ)\display.o $(OBJ)\audio.o $(OBJ)\real_time.o
	$(CC) $^ -o $@ -L$(SDL_LIB) -L$(SDL_MIX_LIB) -L$(SDL_TTF_LIB) $(LD_FLAGS)

$(OBJ)\main.o: $(SRC)\main.c $(INC)\main.h
	$(CC) -c $< -o $@ -I$(INC) -I$(SDL_INC) $(C_FLAGS)

$(OBJ)\get_rom.o: $(SRC)\get_rom.c $(INC)\get_rom.h
	$(CC) -c $< -o $@ -I$(INC) $(C_FLAGS)

$(OBJ)\cpu.o: $(SRC)\cpu.c $(INC)\cpu.h
	$(CC) -c $< -o $@ -I$(INC) $(C_FLAGS)

$(OBJ)\memory.o: $(SRC)\memory.c $(INC)\memory.h
	$(CC) -c $< -o $@ -I$(INC) $(C_FLAGS)

$(OBJ)\timer.o: $(SRC)\timer.c $(INC)\timer.h
	$(CC) -c $< -o $@ -I$(INC) $(C_FLAGS)

$(OBJ)\interrupt.o: $(SRC)\interrupt.c $(INC)\interrupt.h
	$(CC) -c $< -o $@ -I$(INC) $(C_FLAGS)

$(OBJ)\ppu.o: $(SRC)\ppu.c $(INC)\ppu.h
	$(CC) -c $< -o $@ -I$(INC) $(C_FLAGS)

$(OBJ)\touche.o: $(SRC)\touche.c $(INC)\touche.h
	$(CC) -c $< -o $@ -I$(INC) -I$(SDL_INC) $(C_FLAGS)

$(OBJ)\display.o: $(SRC)\display.c $(INC)\display.h
	$(CC) -c $< -o $@ -I$(INC) -I$(SDL_INC) -I$(SDL_TTF_INC) $(C_FLAGS)

$(OBJ)\audio.o: $(SRC)\audio.c $(INC)\audio.h
	$(CC) -c $< -o $@ -I$(INC) -I$(SDL_INC) -I$(SDL_MIX_INC) $(C_FLAGS)

$(OBJ)\real_time.o: $(SRC)\real_time.c $(INC)\real_time.h
	$(CC) -c $< -o $@ -I$(INC) -I$(SDL_INC) $(C_FLAGS)

clean:
	del $(OBJ)\*.o

allclean: clean
	del $(BIN)\$(PROG_NAME)
	
remake: allclean all