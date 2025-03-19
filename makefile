CC = clang
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))
OUT = $(BIN_DIR)/pathtracer.exe
CFLAGS = -O3
RUN_ARGS = $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))

.PHONY: all
all: $(OUT)

$(OUT): $(OBJ) | $(BIN_DIR)
	$(CC) $(OBJ) -o $@ $(CFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(BIN_DIR):
	if not exist $@ mkdir $@

$(OBJ_DIR):
	if not exist $@ mkdir $@

.PHONY: clean
clean:
	if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
	if exist $(BIN_DIR) rmdir /s /q $(BIN_DIR)

.PHONY: run
run: clean all
	$(OUT) $(RUN_ARGS)

%:
	@:
