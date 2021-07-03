#dependencies
LIBCJSON_DIR = cJSON
LIBCJSON = cJSON.a

#TARGETS
all: $(LIBCJSON)

$(LIBCJSON):
	make -C $(LIBCJSON_DIR)
	mv $(LIBCJSON_DIR)/$@ ./$@

.PHONY: clean uninstall reinstall

#remove all temp files
clean:
	@make $@ -C $(LIBCJSON_DIR)

#delete all files
uninstall:
	@make $@ -C $(LIBCJSON_DIR)
	@rm -f $(LIBCJSON)

#rebuild project
reinstall: uninstall all
