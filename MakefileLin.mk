OBJ = obj
SRC = src
INC = inc
BIN = bin

# SDL_INC = /home/mathieu/Documents/C/SDL/SDL2-devel-2.0.22-mingw/SDL2-2.0.22/x86_64-w64-mingw32/include
# SDL_LIB = /home/mathieu/Documents/C/SDL/SDL2-devel-2.0.22-mingw/SDL2-2.0.22/x86_64-w64-mingw32/lib

LD_FLAGS = -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_ttf '-Wl,-rpath,$$ORIGIN/so'
C_FLAGS = -std=c17 -Wall -O2 -D__REALITY -D__LIN #-D__DEBUG -D__STEP 

PROG_NAME = VGB_emulator

all: $(BIN)/VGB_emulator
	strip $(BIN)/$(PROG_NAME)

$(BIN)/$(PROG_NAME): $(OBJ)/main.o $(OBJ)/get_rom.o $(OBJ)/cpu.o $(OBJ)/memory.o $(OBJ)/timer.o $(OBJ)/interrupt.o $(OBJ)/ppu.o $(OBJ)/touche.o $(OBJ)/display.o $(OBJ)/audio.o
	$(CC) $^ -o $@  $(LD_FLAGS)

$(OBJ)/main.o: $(SRC)/main.c $(INC)/main.h
	$(CC) -c $< -o $@ -I$(INC)  $(C_FLAGS)

$(OBJ)/get_rom.o: $(SRC)/get_rom.c $(INC)/get_rom.h
	$(CC) -c $< -o $@ -I$(INC) $(C_FLAGS)

$(OBJ)/cpu.o: $(SRC)/cpu.c $(INC)/cpu.h
	$(CC) -c $< -o $@ -I$(INC) $(C_FLAGS)

$(OBJ)/memory.o: $(SRC)/memory.c $(INC)/memory.h
	$(CC) -c $< -o $@ -I$(INC) $(C_FLAGS)

$(OBJ)/timer.o: $(SRC)/timer.c $(INC)/timer.h
	$(CC) -c $< -o $@ -I$(INC) $(C_FLAGS)

$(OBJ)/interrupt.o: $(SRC)/interrupt.c $(INC)/interrupt.h
	$(CC) -c $< -o $@ -I$(INC) $(C_FLAGS)

$(OBJ)/ppu.o: $(SRC)/ppu.c $(INC)/ppu.h
	$(CC) -c $< -o $@ -I$(INC) $(C_FLAGS)

$(OBJ)/touche.o: $(SRC)/touche.c $(INC)/touche.h
	$(CC) -c $< -o $@ -I$(INC) $(C_FLAGS)

$(OBJ)/display.o: $(SRC)/display.c $(INC)/display.h
	$(CC) -c $< -o $@ -I$(INC)  $(C_FLAGS)

$(OBJ)/audio.o: $(SRC)/audio.c $(INC)/audio.h
	$(CC) -c $< -o $@ -I$(INC) $(C_FLAGS)

clean:
	rm -f $(OBJ)/*.o

allclean: clean
	rm -f $(BIN)/$(PROG_NAME)
	
remake: allclean all