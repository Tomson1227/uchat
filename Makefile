SERVER_DIR = ./server/
SERVER_NAME = uchat_server

CLIENT_DIR = ./client/
CLIENT_NAME = uchat

SERVER_BIN = $(join $(SERVER_DIR), $(SERVER_NAME))
CLIENT_BIN = $(join $(CLIENT_DIR), $(CLIENT_NAME))

LIB_DIR		= .
LIB_LIST	= libmx

LIB_DIRS	= $(foreach libdirs, $(LIB_LIST), $(LIB_DIR)/$(libdirs)/)
LIB_BIN		= $(join $(LIB_DIRS), $(addsuffix .a, $(LIB_LIST)))
LIB_INC		= $(addsuffix $(INC_DIR), $(LIB_DIRS))

COMPILE	= $(CC) $(GFLAGS) $(IFLAGS) $(LIB_BIN)
MAKE_M	= make -sf Makefile -C
RM		= /bin/rm -rf

all: $(LIB_BIN) $(SERVER_BIN) $(CLIENT_BIN) start_server
	@mv -f $(CLIENT_BIN) .
	@./$(CLIENT_NAME) 127.0.0.1

start_server:
	@mv -f $(SERVER_BIN) .
	@open -a Terminal "./$(SERVER_NAME)" #macOS
	# @gnome-terminal --command=./$(SERVER_NAME)
	@sleep 1

$(SERVER_BIN):
	@$(MAKE_M) $(SERVER_DIR)

$(CLIENT_BIN):
	@$(MAKE_M) $(CLIENT_DIR)

$(LIB_BIN): $(LIB_LIST)

$(LIB_LIST): $(LIB_DIRS)
	@$(MAKE_M) $(LIB_DIR)/$@

clean:
	@$(MAKE_M) $(LIB_DIR)/$(LIB_LIST) $@
	@$(MAKE_M) $(SERVER_DIR) $@
	@$(MAKE_M) $(CLIENT_DIR) $@
	@$(RM) $(OBJ_DIR) $(SERVER_NAME) $(CLIENT_NAME)

uninstall:
	@$(MAKE_M) $(LIB_DIR)/$(LIB_LIST) $@
	@$(MAKE_M) $(SERVER_DIR) $@
	@$(MAKE_M) $(CLIENT_DIR) $@

reinstall: uninstall all

.PHONY: all clean uninstall reinstall $(LIB_LIST)


#	netstat -anp --tcp | grep ./server/uchat_server