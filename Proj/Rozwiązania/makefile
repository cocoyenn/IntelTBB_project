SUBDIRS := $(wildcard */)

.PHONY: all
all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: clean
clean:
	for subdir in $(SUBDIRS); do \
    	$(MAKE) clean -C $$subdir; \
    done

.PHONY: run
run:
	for subdir in $(SUBDIRS); do \
    	$(MAKE) run -C $$subdir; \
    done
