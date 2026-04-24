#
# EPITECH PROJECT, 2026
# minishell1
# File description:
# Main building makefile
# Auto-Generated with Episetup by Amélie
# Author:
# Amélie Ambleton--Guth
#

NAME = 42sh
SRC_FILENAMES = \
	main.c \
	ms_explode.c \
	ms_utils.c \
	ms_path_explorer.c \
	ms_env_manager.c \
	ms_env_commands.c \
	ms_dir_commands.c \
	ms_command_parser.c \
	ms_errors.c \
	ms_strutils.c \
	ms_grammar.c \
	ms_grammar_tools.c \
	ms_runner.c \
	ms_type_utils.c \
	ms_pipeline.c \
	ms_path_expansion.c \
	ms_redirections.c \
	ms_linereader.c \
	ms_prompts.c \
	ms_keymap.c \
	ms_verifiers.c \
	ms_teardown.c \
	ms_input_reader.c \
	ms_le_cmds_history.c \
	ms_le_cmds_misc.c \
	ms_le_cmds_movement.c \
	ms_le_esc_sequence.c \
	ms_le_keybinds.c \
	ms_le_tools.c \
	ms_history.c
HEADERS = minishell1.h minishell2.h benjalib.h


SRC_DIR = src
OBJ_DIR = build
LIBS_DIR = lib
HEADERS_DIR = include

SRC_FILES = $(SRC_FILENAMES:%=$(SRC_DIR)/%)
OBJ_FILES = $(SRC_FILENAMES:%.c=$(OBJ_DIR)/%.o)
HEADER_FILES = $(HEADERS:%=$(HEADERS_DIR)/%)

LIBMY_DIR = $(LIBS_DIR)/benjalib
LIBMY_MAKE = $(MAKE) --no-print-directory -C $(LIBMY_DIR)
LIBMY_BIN = libbenja.a

CFLAGS += -I$(HEADERS_DIR)
LINKER_DIRS += -Llib
LINKER_FLAGS += -lbenja

all: $(NAME)

$(NAME): $(OBJ_FILES) $(LIBS_DIR)/$(LIBMY_BIN)
	@echo "[Linking] $@"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ_FILES) $(LINKER_DIRS) $(LINKER_FLAGS)
	@echo "[$@] Build Success"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER_FILES) | $(OBJ_DIR)
	@echo "[Compiling] $<"
	@$(CC) $(CFLAGS) -c $< -o $@ $(EXTRA_FLAGS)

$(LIBS_DIR)/$(LIBMY_BIN):
	@echo "[Lib] Building $<"
	@$(LIBMY_MAKE) all | awk '$$0="  "$$0'
	@echo "[Lib] Build Success"
	@cp $(LIBMY_DIR)/$(LIBMY_BIN) $(LIBS_DIR)/

$(OBJ_DIR):
	@mkdir -p $@

clean:
	@$(LIBMY_MAKE) clean | awk '$$0="  "$$0'
	@rm -rf $(OBJ_DIR) *.gcda *.gcno *.gcov *.gcov.json.gz *.profraw
	@echo "[Clean] Removed intermediary and debug outputs"

fclean: clean
	@$(LIBMY_MAKE) fclean | awk '$$0="  "$$0'
	@rm -f $(NAME)
	@rm -f $(LIBS_DIR)/$(LIBMY_BIN)
	@echo "[Clean] Removed all output file, running banana-check-repo"
	@banana-check-repo | awk '$$0="  "$$0'
	@echo "[Clean] Repository clean"

re: fclean all

tests_run:
	@echo "[Tests] [WARN] Not implemented yet."

.PHONY: re fclean clean all tests_run
