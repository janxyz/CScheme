CC=gcc
CFLAGS=-std=c11 -g -Wall -Wextra -Wpedantic -Wshadow -MMD
# pattern-specific variable value for all targets prefixed with test_
test_%: LDLIBS += -lcmocka
test_%: CFLAGS += -Wl,--wrap=exit_with_error

bin=mopl test_objects
obj = obj.o \
      boolean.o \
      string.o \
      empty_list.o \
      pair.o \
      symbol.o \
      error.o \
      test_objects.o \
      mopl.o
dep=$(obj:.o=.d)

.PHONY: clean test

mopl: obj.o boolean.o string.o empty_list.o pair.o symbol.o error.o

test: test_objects
	./test_objects

test_objects: obj.o boolean.o string.o empty_list.o pair.o symbol.o error.o

clean:
	rm -f $(obj) $(dep) $(bin)

-include $(dep)
