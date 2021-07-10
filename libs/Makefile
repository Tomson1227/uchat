#dependencies
LIBCJSON_DIR = cJSON
LIBCJSON = cJSON.a
LIBAPI_DIR = API
LIBAPI = API.a

#TARGETS
all: $(LIBCJSON) $(LIBAPI)

$(LIBCJSON):
	@make -C $(LIBCJSON_DIR)
	@mv $(LIBCJSON_DIR)/$@ ./$@

$(LIBAPI):
	@make -C $(LIBAPI_DIR)
	@mv $(LIBAPI_DIR)/$@ ./$@

.PHONY: clean uninstall reinstall

#remove all temp files
clean:
	@make $@ -C $(LIBCJSON_DIR)
	@make $@ -C $(LIBAPI_DIR)

#delete all files
uninstall:
	@make $@ -C $(LIBCJSON_DIR)
	@make $@ -C $(LIBAPI_DIR)
	@rm -f $(LIBCJSON) $(LIBAPI)

#rebuild project
reinstall: uninstall all
