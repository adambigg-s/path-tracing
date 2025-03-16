CC = clang
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
SRC = $(SRC_DIR)/main.c
OBJ = $(OBJ_DIR)/main.o
OUT = $(BIN_DIR)/pathtracer.exe
CFLAGS = 
RUN_ARGS = $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))

.PHONY: all
all: $(OUT)

$(OUT): $(OBJ) | $(BIN_DIR)
	$(CC) $(OBJ) -o $@ $(CXXFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CXXFLAGS)

$(BIN_DIR):
	if not exist $@ mkdir $@

$(OBJ_DIR):
	if not exist $@ mkdir $@

.PHONY: clean
clean:
	if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
	if exist $(BIN_DIR) rmdir /s /q $(BIN_DIR)

.PHONY: run
run: all
	$(OUT) $(RUN_ARGS)

%:
	@:
