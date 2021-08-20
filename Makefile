#dependencies
SERVER_DIR = server
CLIENT_DIR = client
LDIR = libs
SERVER = uchat_server
CLIENT = uchat
TEST_SERVER = test_server
TEST_CLIENT = test_client

.PHONY: clean uninstall reinstall $(SERVER) $(CLIENT)

#TARGETS
all: $(SERVER) $(CLIENT)

$(SERVER):
	@make -C $(LDIR)
	@make -C $(SERVER_DIR)
	@mv $(SERVER_DIR)/$@ ./$@

$(CLIENT):
	@make -C $(LDIR)
	@make -C $(CLIENT_DIR)
	@mv $(CLIENT_DIR)/$@ ./$@

#TESTS
test: $(TEST_SERVER) $(TEST_CLIENT)

$(TEST_SERVER):
	@make $@ -C $(SERVER_DIR)
	@mv $(SERVER_DIR)/$@ ./$@

$(TEST_CLIENT):
	@make $@ -C $(CLIENT_DIR)
	@mv $(CLIENT_DIR)/$@ ./$@


#remove all temp files
clean:
	@make $@ -C $(LDIR)
	@make $@ -C $(SERVER_DIR)
	@make $@ -C $(CLIENT_DIR)

#delete all files
uninstall:
	@make $@ -C $(LDIR)
	@make $@ -C $(SERVER_DIR)
	@make $@ -C $(CLIENT_DIR)
	@rm -f $(TEST_SERVER) $(TEST_CLIENT) $(SERVER) $(CLIENT)

#rebuild project
reinstall: uninstall all
