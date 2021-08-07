#dependencies
LIBCJSON_DIR = cJSON
LIBCJSON = cJSON.a
# LIBAPI_DIR = API
# LIBAPI = API.a
LIBSQLITE_DIR = SQLite
LIBSQLITE = SQLite.a

#TARGETS
all: $(LIBCJSON) $(LIBSQLITE)

$(LIBCJSON):
	@make -C $(LIBCJSON_DIR)
	@mv $(LIBCJSON_DIR)/$@ ./$@

# $(LIBAPI):
# 	@make -C $(LIBAPI_DIR)
# 	@mv $(LIBAPI_DIR)/$@ ./$@

$(LIBSQLITE):
	@make -C $(LIBSQLITE_DIR)
	@mv $(LIBSQLITE_DIR)/$@ ./$@

.PHONY: clean uninstall reinstall

#remove all temp files
clean:
	@make $@ -C $(LIBCJSON_DIR)
	# @make $@ -C $(LIBAPI_DIR)
	@make $@ -C $(LIBSQLITE_DIR)
#delete all files
uninstall:
	@make $@ -C $(LIBCJSON_DIR)
	@make $@ -C $(LIBSQLITE_DIR)
	@rm -f $(LIBCJSON) $(LIBSQLITE)

#rebuild project
reinstall: uninstall all
