include Makefile.conf

OBJ = obj
SRC = src
INC = inc
BIN = bin

SDL_INC = /home/mathieu/Documents/code/outils/SDL2/SDL2/include/
SDL_LIB = /home/mathieu/Documents/code/outils/SDL2/SDL2/lib

LD_FLAGS = -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_ttf '-Wl,-rpath,$$ORIGIN/../lib'

PROG_NAME = VGB_emulator

all: $(BIN)/VGB_emulator
	strip $(BIN)/$(PROG_NAME)

$(BIN)/$(PROG_NAME): $(OBJ)/main.o $(OBJ)/get_rom.o $(OBJ)/cpu.o $(OBJ)/memory.o $(OBJ)/timer.o $(OBJ)/interrupt.o $(OBJ)/ppu.o $(OBJ)/touche.o $(OBJ)/display.o $(OBJ)/audio.o $(OBJ)/real_time.o
	$(CC) $^ -o $@  $(LD_FLAGS)

$(OBJ)/main.o: $(SRC)/main.c $(INC)/main.h
	$(CC) -c $< -o $@ -I$(INC) $(C_FLAGS)

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
	$(CC) -c $< -o $@ -I$(INC) $(C_FLAGS)

$(OBJ)/audio.o: $(SRC)/audio.c $(INC)/audio.h
	$(CC) -c $< -o $@ -I$(INC) $(C_FLAGS)

$(OBJ)/real_time.o: $(SRC)/real_time.c $(INC)/real_time.h
	$(CC) -c $< -o $@ -I$(INC) $(C_FLAGS)

clean:
	rm -f $(OBJ)/*.o

allclean: clean
	rm -f $(BIN)/$(PROG_NAME)
	
remake: allclean all