NAME		= uchat

SRC_DIR		= src/
OBJ_DIR		= obj/
INC_DIR		= inc/

SRC			= $(wildcard $(SRC_DIR)*.c)
OBJ			= $(SRC:src/%.c=$(OBJ_DIR)%.o)
INC_H		= $(wildcard $(INC_DIR)*.h)

LIB_DIR		= ..
LIB_LIST	= libmx

LIB_DIRS	= $(foreach libdirs, $(LIB_LIST), $(LIB_DIR)/$(libdirs))
LIB_BIN		= $(join $(join $(LIB_DIRS), /), $(addsuffix .a, $(LIB_LIST)))
LIB_INC		= $(addsuffix $(INC_DIR), $(LIB_DIRS))

CC			= clang
GFLAGS		= -std=c11 -pipe -Wall -Wextra -Werror -Wpedantic\
	-Wno-unused-command-line-argument -Wno-unused-variable \
	-Wno-unused-function -Wno-unused-parameter -g -lpthread

IFLAGS		= $(addprefix -I, $(LIB_INC) $(INC_DIR))

COMPILE		= $(CC) $(GFLAGS) $(IFLAGS) $(LIB_BIN)
MAKE_M		= make -sf Makefile -C
RM			= /bin/rm -rf

all: $(NAME)

$(NAME): $(LIB_LIST) $(OBJ_DIR) $(OBJ) 
	@$(COMPILE) $(OBJ) -o $(NAME) -L../libmx -lmx
	@printf "\r\33[2K$@ \033[35;1mcreated\033[0m\n"

$(LIB_BIN): $(LIB_LIST)

$(LIB_LIST): $(LIB_DIRS)
	@$(MAKE_M) $(LIB_DIR)/$@

$(OBJ_DIR):
	@mkdir -p $@ $(foreach dir, $(DIRS), $@/$(dir))

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(INC_H) $(LIB_BIN)
	@$(COMPILE) -L./libmx -lmx -o $@ -c $<
	@printf "\r\33[2K$(NAME) \033[37;1mcompile \033[0m$(<:$(SRC_DIR)/%.c=%)"

clean:
	@$(MAKE_M) $(LIB_DIR)/$(LIB_LIST) $@
	@$(RM) $(OBJ_DIR)
	@printf "obj in $(NAME) \033[36;1mdeleted\033[0m\n"

uninstall:
	@$(MAKE_M) $(LIB_DIR)/$(LIB_LIST) $@
	@$(RM) $(OBJ_DIR) $(NAME)
	@printf "$(NAME) \033[36;1muninstalled\033[0m\n"

reinstall: uninstall all

.PHONY: all clean uninstall reinstall $(LIB_LIST)