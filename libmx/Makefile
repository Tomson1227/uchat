NAME	=	libmx.a

CC 		= 	clang
CFLG	=	-std=c11 -Wall -Wextra -Werror -Wpedantic -g

SRCD	=	src
INCD	=	inc
OBJD	=	obj

INC		=	libmx.h
INCS	=	$(addprefix $(INCD)/, $(INC))
SRCS	=	$(wildcard $(SRCD)/*.c)
OBJS	=	$(addprefix $(OBJD)/, $(SRCS:$(SRCD)/%.c=%.o))

all: install
	
install: $(NAME)

$(NAME): $(OBJS)
	@ar -rcs $@ $^

$(OBJD)/%.o: $(SRCD)/%.c $(INCS)
	@$(CC) $(CFLG) -c $< -o $@ -I$(INCD)

$(OBJS): | $(OBJD)

$(OBJD):
	@mkdir -p $@

clean:
	@rm -rf $(OBJD)

uninstall: clean
	@rm -rf $(NAME)

reinstall: uninstall install

testlib: 
	@clang -std=c11 -Wall -Wextra -Werror -Wpedantic test/test.c libmx.a
	@mv a.out test/a.out
	@./test/a.out
	@rm -rf test/a.out

test: $(INCS)
	@cp ../main_function/main.c .
	@$(CC) $(FLAG) main.c -L. -lmx -I$(INCD)
	@./a.out
	@rm -f a.out main.c

test1: $(INCS)
	@cp ../main_function/tmain.c .
	@$(CC) $(FLAG) tmain.c -L. -lmx -I$(INCD)
	@./a.out
	@rm -f a.out tmain.c