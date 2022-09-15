include Makefile.conf

OBJ = obj
SRC = src
INC = inc
BIN = bin

SDL_INC = /home/mathieu/Documents/code/outils/SDL2/SDL2/include/
SDL_LIB = /home/mathieu/Documents/code/outils/SDL2/SDL2/lib

LD_FLAGS = -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_ttf '-Wl,-rpath,$$ORIGIN/../lib'

PROG_NAME = VGB_emulator

ifeq ($(STRIP), yes)
all: $(BIN)/VGB_emulator
	strip $(BIN)/$(PROG_NAME)
else
all: $(BIN)/VGB_emulator
endif

$(BIN)/$(PROG_NAME): $(OBJ)/main.o $(OBJ)/get_rom.o $(OBJ)/cpu.o $(OBJ)/memory.o $(OBJ)/timer.o $(OBJ)/interrupt.o $(OBJ)/ppu.o $(OBJ)/touche.o $(OBJ)/display.o $(OBJ)/audio.o $(OBJ)/real_time.o
	$(CC) $(LD_FLAGS_ADDITIONAL) $^ -o $@  $(LD_FLAGS)

$(OBJ)/main.o: $(SRC)/main.c $(INC)/main.h
	$(CC) -c $(C_FLAGS) $< -o $@ -I$(INC)

$(OBJ)/get_rom.o: $(SRC)/get_rom.c $(INC)/get_rom.h
	$(CC) -c $(C_FLAGS) $< -o $@ -I$(INC)

$(OBJ)/cpu.o: $(SRC)/cpu.c $(INC)/cpu.h
	$(CC) -c $(C_FLAGS) $< -o $@ -I$(INC)

$(OBJ)/memory.o: $(SRC)/memory.c $(INC)/memory.h
	$(CC) -c $(C_FLAGS) $< -o $@ -I$(INC)

$(OBJ)/timer.o: $(SRC)/timer.c $(INC)/timer.h
	$(CC) -c $(C_FLAGS) $< -o $@ -I$(INC)

$(OBJ)/interrupt.o: $(SRC)/interrupt.c $(INC)/interrupt.h
	$(CC) -c $(C_FLAGS) $< -o $@ -I$(INC)

$(OBJ)/ppu.o: $(SRC)/ppu.c $(INC)/ppu.h
	$(CC) -c $(C_FLAGS) $< -o $@ -I$(INC)

$(OBJ)/touche.o: $(SRC)/touche.c $(INC)/touche.h
	$(CC) -c $(C_FLAGS) $< -o $@ -I$(INC)

$(OBJ)/display.o: $(SRC)/display.c $(INC)/display.h
	$(CC) -c $(C_FLAGS) $< -o $@ -I$(INC)

$(OBJ)/audio.o: $(SRC)/audio.c $(INC)/audio.h
	$(CC) -c $(C_FLAGS) $< -o $@ -I$(INC)

$(OBJ)/real_time.o: $(SRC)/real_time.c $(INC)/real_time.h
	$(CC) -c $(C_FLAGS) $< -o $@ -I$(INC)

clean:
	rm -f $(OBJ)/*.o

allclean: clean
	rm -f $(BIN)/$(PROG_NAME)
	
remake: allclean all