#dependencies
LIBCJSON_DIR = cJSON
LIBCJSON = libcJSON.a
LIBSQLITE_DIR = SQLite
LIBSQLITE = libSQLite.a

#TARGETS
all: $(LIBCJSON) $(LIBSQLITE)

$(LIBCJSON):
	@make -C $(LIBCJSON_DIR)
	@mv $(LIBCJSON_DIR)/$@ ./$@

$(LIBSQLITE):
	@make -C $(LIBSQLITE_DIR)
	@mv $(LIBSQLITE_DIR)/$@ ./$@

.PHONY: clean uninstall reinstall

#remove all temp files
clean:
	@make $@ -C $(LIBCJSON_DIR)
	@make $@ -C $(LIBSQLITE_DIR)

#delete all files
uninstall:
	@make $@ -C $(LIBCJSON_DIR)
	@make $@ -C $(LIBSQLITE_DIR)
	@rm -f $(LIBCJSON) $(LIBSQLITE)

#rebuild project
reinstall: uninstall all
