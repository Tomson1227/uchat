NAME		= uchat_server

SRC_DIR		= src/
OBJ_DIR		= obj/
INC_DIR		= inc/

SRC			= $(wildcard $(SRC_DIR)*.c)
OBJ			= $(SRC:src/%.c=$(OBJ_DIR)%.o)
INC_H		= $(wildcard $(INC_DIR)*.h)

CC			= clang
GFLAGS		= -std=c11 -pipe -Wall -Wextra -Werror -Wpedantic\
	-Wno-unused-command-line-argument -Wno-unused-variable \
	-Wno-unused-function -Wno-unused-parameter -g -pthread

IFLAGS		= $(addprefix -I, $(INC_DIR))
COMPILE		= $(CC) $(GFLAGS) $(IFLAGS)
RM			= /bin/rm -rf

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJ) 
	@$(COMPILE) $(OBJ) -o $(NAME)
	@printf "\r\033[2K$@ \033[35;1mcreated\033[0m\n"

$(OBJ_DIR):
	@mkdir -p $@ $(foreach dir, $(DIRS), $@/$(dir))

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(INC_H)
	@$(COMPILE) -o $@ -c $<
	@printf "\r\033[2K$(NAME) \033[37;1mcompile \033[0m$(<:$(SRC_DIR)/%.c=%)"

clean:
	@$(RM) $(OBJ_DIR)
	@printf "obj in $(NAME) \033[36;1mdeleted\033[0m\n"

uninstall:
	@$(RM) $(OBJ_DIR) $(NAME)
	@printf "$(NAME) \033[36;1muninstalled\033[0m\n"

reinstall: uninstall all

.PHONY: all clean uninstall reinstall
