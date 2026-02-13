#
# EPITECH PROJECT, 2026
# minishell1
# File description:
# Main building makefile
# Auto-Generated with Episetup by Amélie
# Author:
# Amélie Ambleton--Guth
#

NAME = mysh
SRC_FILENAMES = main.c ms_explode.c ms_utils.c ms_path_explorer.c \
	ms_env_manager.c ms_env_commands.c ms_dir_commands.c \
	ms_input_reader.c
HEADERS = linked_list.h my.h my_printf.h minishell1.h


SRC_DIR = src
OBJ_DIR = build
BIN_DIR = bin
LIBS_DIR = lib
HEADERS_DIR = include

SRC_FILES = $(SRC_FILENAMES:%=$(SRC_DIR)/%)
OBJ_FILES = $(SRC_FILENAMES:%.c=$(OBJ_DIR)/%.o)
BIN_FILE = $(BIN_DIR)/$(NAME)
HEADER_FILES = $(HEADERS:%=$(HEADERS_DIR)/%)

LIBMY_DIR = $(LIBS_DIR)/mylite
LIBMY_MAKE = $(MAKE) -C $(LIBMY_DIR)
LIBMY_BIN = libmy.a

CFLAGS += -g -I$(HEADERS_DIR)
LINKER_FLAGS += -Llib -lmy

codingstyle: CC = epiclang

# Extra flags reserved for gcovr profiling flags

export CC

all: $(NAME)
	
$(NAME): $(BIN_FILE)
	ln -sf $(BIN_FILE) $(NAME)

$(BIN_FILE): $(OBJ_FILES) $(LIBS_DIR)/$(LIBMY_BIN) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_FILE) $(OBJ_FILES) $(LINKER_DIRS) $(LINKER_FLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER_FILES) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(EXTRA_FLAGS)

$(LIBS_DIR)/$(LIBMY_BIN):
	$(LIBMY_MAKE) all
	cp $(LIBMY_DIR)/bin/$(LIBMY_BIN) $(LIBS_DIR)/

$(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) *.gcda *.gcno *.gcov *.gcov.json.gz *.profraw
	$(LIBMY_MAKE) clean 

fclean: clean
	rm -rf $(BIN_DIR)
	$(LIBMY_MAKE) fclean
	rm -f $(LIBS_DIR)/$(LIBMY_BIN)

re:
	$(MAKE) fclean
	$(MAKE) all

run: $(BIN_FILE)
	./$(BIN_FILE)

codingstyle:
	$(MAKE) clean
	rm -rf $(BIN_DIR)
	$(MAKE) all

.PHONY: re fclean clean all codingstyle run
