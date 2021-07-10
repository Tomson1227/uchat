NAME = API.a

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = inc

#lib includes
LIBDIR = ../cJSON/inc

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(notdir $(SRC_FILES:%.c=%.o)))
INC_FILES = $(wildcard $(INC_DIR)/*.h)

CC = gcc
CFLAGS = -std=c11 -std=c11 -Wall -Wextra -Wpedantic
FLAGS = $(CFLAGS) -I$(INC_DIR) $(addprefix -I, $(LIBDIR))

AR = ar
AFLAGS = rcs

all: $(NAME)

$(NAME): $(OBJ_FILES)
	@$(AR) $(AFLAGS) $@ $^
	@printf "\r\33[2K$@\t \033[32;1mcreated\033[0m\n"

$(OBJ_FILES): | $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_FILES)
	@$(CC) $(FLAGS) -c $< -o $@
	@printf "\r\33[2K$(NAME)\033[33;1m\t compile \033[0m$(<:$(SRC_DIR)/%.c=%)\r"

$(OBJ_DIR):
	@mkdir -p $@

clean:
	@rm -rf $(OBJ_DIR)
	@printf "$(OBJ_DIR) in $(NAME)\t \033[31;1mdeleted\033[0m\n"

uninstall: clean
	@rm -f $(NAME)
	@printf "$(NAME)\t \033[31;1muninstalled\033[0m\n"

reinstall: uninstall all

.PHONY: all uninstall clean reinstall
