#dependencies
NAME = uchat_server
TEST = test_server

#directories
IDIR = inc
ODIR = obj
SDIR = src
TDIR = test

#lib includes
LIBDIR = ../libs/cJSON/inc ../libs/API/inc ../libs/SQLite/inc
LIB = ../libs/SQLite.a ../libs/API.a ../libs/cJSON.a -ldl -pthread

CC			= clang
GFLAGS		= -std=c11 -pipe -Wall -Wextra -Werror -Wpedantic\
	-Wno-unused-command-line-argument -Wno-unused-variable \
	-Wno-unused-function -Wno-unused-parameter -g -pthread -rdynamic -ldl

#headers
DEPS := $(wildcard $(IDIR)/*.h)
#sources
SRCS := $(wildcard $(SDIR)/*.c)
TEST_SRCS := $(wildcard $(TDIR)/*.c)
#objects
_OBJS = $(SRCS:.c=.o)
OBJS = $(subst $(SDIR),$(ODIR),$(_OBJS))

#TARGETS
all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJ) 
	@$(COMPILE) $(OBJ) -o $(NAME)
	@printf "\r\033[2K$@ \033[35;1mcreated\033[0m\n"

#make excecutable
$(NAME): $(OBJS)
	@$(CC) $(FLAGS) $^ $(LIB) -o $@
	@printf "\r\33[2KExcecutable for \33[0;32m$@\33[m done!\n"
	@printf "Usage: ./$@ <port> <address>\n"
# WARNING: Edit "printf" for your needs

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(INC_H)
	@$(COMPILE) -o $@ -c $<
	@printf "\r\033[2K$(NAME) \033[37;1mcompile \033[0m$(<:$(SRC_DIR)/%.c=%)"

.PHONY: clean uninstall reinstall

#delete all files
uninstall: clean
	@printf "\r\33[2K\33[0;33mUninstalling $(TEST)...\33[m\n"
	@rm -f $(TEST)
	@printf "\r\33[2K\33[0;33mUninstalling $(NAME)...\33[m\n"
	@rm -f $(NAME)

#remove all temp files
clean:
	@rm -rf $(ODIR)
	@printf "\r\33[2K\33[0;33mRemoving temporary $(NAME) files...\33[m\n"

#rebuild project
reinstall: uninstall all
