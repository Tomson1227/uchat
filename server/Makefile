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

#compiler settings
CC = gcc
CFLAGS = -std=c11
CCFLAGS = -std=c11 -Wall -Wextra -Wpedantic
FLAGS = $(CFLAGS) -I$(IDIR) $(addprefix -I, $(LIBDIR))
TEST_FLAGS = $(CCFLAGS)

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

#make objects
$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	@mkdir -p obj
	@printf "\33[2KCompiling \33[0;32m$<\33[m\n"
	@$(CC) $(FLAGS) -c $< -o $@

#make excecutable
$(NAME): $(OBJS)
	@$(CC) $(FLAGS) $^ $(LIB) -o $@
	@printf "\r\33[2KExcecutable for \33[0;32m$@\33[m done!\n"
	@printf "Usage: ./$@ <port> <address>\n"
# WARNING: Edit "printf" for your needs

#make test
$(TEST): $(TEST_SRCS)
	@printf "\33[2KCompiling \33[0;32m$<\33[m\n"
	@$(CC) $(TEST_FLAGS) $< -o $@
	@printf "\r\33[2KExcecutable for \33[0;32m$@\33[m done!\n"

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
