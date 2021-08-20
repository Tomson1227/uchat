#dependencies
NAME = uchat_server

#directories
IDIR = inc
ODIR = obj
SDIR = src
TDIR = test

#lib includes
LIB_DIR		= ../libs/
LIBLIST 	= ../libs/libcJSON.a ../libs/libSQLite.a

#compiler settings
CC = clang
CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic\
	-Wno-unused-command-line-argument -Wno-unused-variable \
	-Wno-unused-function -Wno-unused-parameter -g
DYNAMICS = -pthread -ldl
FLAGS = $(CFLAGS) -I$(IDIR) 

#headers
DEPS := $(wildcard $(IDIR)/*.h)
#sources
SRCS := $(wildcard $(SDIR)/*.c)
#objects
_OBJS = $(SRCS:.c=.o)
OBJS = $(subst $(SDIR),$(ODIR),$(_OBJS))

#TARGETS
all: $(NAME)

#make objects
$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	@mkdir -p obj
	@printf "\33[2KCompiling \33[0;32m$<\33[m\n"
	@$(CC) $(FLAGS) -c $< -o $@ $(PKGCONFIG) $(LIBLIST)

#make excecutable
$(NAME): $(OBJS) 
	@$(CC) $(FLAGS) $^ -o $@ $(DYNAMICS) $(LIBLIST)
	@printf "\r\33[2KExcecutable for \33[0;32m$@\33[m done!\n"
	@printf "Usage: ./$@ <port>\n"

.PHONY: clean uninstall reinstall

#delete all files
uninstall: clean
	@printf "\r\33[2K\33[0;33mUninstalling $(NAME)...\33[m\n"
	@rm -f $(NAME)

#remove all temp files
clean:
	@rm -rf $(ODIR)
	@printf "\r\33[2K\33[0;33mRemoving temporary $(NAME) files...\33[m\n"

#rebuild project
reinstall: uninstall all
