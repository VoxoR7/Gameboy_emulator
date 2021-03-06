# E:\Documents\Code\C\mingw64\bin\mingw32-make.exe

strip = E:\Documents\Code\C\mingw64\bin\strip.exe

CC = E:\Documents\Code\C\mingw64\bin\gcc.exe

SDL_LIB = E:\Documents\Code\C\SDL2-2.0.14\x86_64-w64-mingw32\lib
SDL_INC = E:\Documents\Code\C\SDL2-2.0.14\x86_64-w64-mingw32\include

LIB_SDL_IMG = E:\Documents\Code\C\SDL2_image-2.0.5\x86_64-w64-mingw32\lib
INC_SDL_IMG = E:\Documents\Code\C\SDL2_image-2.0.5\x86_64-w64-mingw32\include

SDL_MIX_INC = E:\Documents\Code\C\SDL2_mixer\SDL2_mixer-2.0.4\x86_64-w64-mingw32\include
SDL_MIX_LIB = E:\Documents\Code\C\SDL2_mixer\SDL2_mixer-2.0.4\x86_64-w64-mingw32\lib

SDL_TTF_INC = E:\Documents\Code\C\SDL2_ttf\SDL2_ttf-2.0.18\x86_64-w64-mingw32\include
SDL_TTF_LIB = E:\Documents\Code\C\SDL2_ttf\SDL2_ttf-2.0.18\x86_64-w64-mingw32\lib

OBJ = obj
SRC = src
INC = inc
BIN = bin

FAST = -D__FAST

LD_FLAGS = -lmingw32 -lSDL2main -lsdl2 -lSDL2_mixer -lSDL2_ttf '-Wl,-rpath,$$ORIGIN/so' # --coverage
C_FLAGS = -std=c17 -Wall -g -D__REALITY -D__WIN $(FAST) -D__DEBUG # -D__STEP --coverage

PROG_NAME = VGB_emulator.exe

all: $(BIN)\$(PROG_NAME)
#	strip $(BIN)\$(PROG_NAME)

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