
PROGRAM := maputil

.PHONY: default
default: $(PROGRAM)

SOURCES := $(wildcard src/*.c)
OBJECTS := $(SOURCES:src/%.c=obj/%.o)
DEPENDS := $(SOURCES:src/%.c=deps/%.d)

MAKEFILES := Makefile

CUSTOM_OBJ := obj/main.o obj/maputil.o obj/error.o obj/cmdline.o obj/cmdlineobjectproperties.o

CFLAGS := -O3 -g -std=gnu99 -Wall -Wno-unused-function
CFLAGS += -I./include

$(OBJECTS): $(MAKEFILES)

$(PROGRAM): $(CUSTOM_OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJECTS): obj/%.o: src/%.c
	$(CC) -o $@ $(CFLAGS) -c $<

.PHONY: depend
depend: $(DEPENDS)

$(DEPENDS): $(MAKEFILES)

$(DEPENDS): deps/%.d: src/%.c
	$(CC) $(CFLAGS) -MM $< | \
		sed -e 's|\(.*\)\.o:|deps/\1.d obj/\1.o:|g' > $@

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPENDS)
endif

.PHONY: clean
clean:
	rm -f maputil obj/*.o deps/*.d

