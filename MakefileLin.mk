OBJ = obj
SRC = src
INC = inc
BIN = bin

SDL_INC = /home/mathieu/Documents/C/SDL/SDL2-devel-2.0.22-mingw/SDL2-2.0.22/x86_64-w64-mingw32/include
SDL_LIB = /home/mathieu/Documents/C/SDL/SDL2-devel-2.0.22-mingw/SDL2-2.0.22/x86_64-w64-mingw32/lib

LD_FLAGS = -lSDL2 #`sdl2-config --libs`
C_FLAGS = -std=c17 -Wall -O2 -D__DEBUG -D__LIN -D__STEP #`sdl2-config --cflags`

PROG_NAME = VGB_emulator

all: $(BIN)/VGB_emulator
	strip $(BIN)/$(PROG_NAME)

$(BIN)/$(PROG_NAME): $(OBJ)/cpu.o $(OBJ)/main.o
	$(CC) $^ -o $@ -L$(SDL_LIB) $(LD_FLAGS)
#	$(CC) $^ -o $@ $(LD_FLAGS)

$(OBJ)/main.o: $(SRC)/main.c $(INC)/main.h
	$(CC) -c $< -o $@ -I$(INC) -I$(SDL_INC) $(C_FLAGS)

$(OBJ)/cpu.o: $(SRC)/cpu.c $(INC)/cpu.h
	$(CC) -c $< -o $@ -I$(INC) $(C_FLAGS)

clean:
	rm -f $(OBJ)/*.o

allclean: clean
	rm -f $(BIN)/$(PROG_NAME)
	
remake: allclean all