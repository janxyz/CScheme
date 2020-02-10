CC=gcc
CFLAGS=-std=c11 -g -Wall -Wextra -Wpedantic -Wshadow -MMD -Wl,--wrap=exit_with_error
# pattern-specific variable value for all targets prefixed with test_
test_%: LDLIBS += -lcmocka

bin=test_objects
obj = obj.o \
      boolean.o \
      string.o \
      empty_list.o \
      pair.o \
      error.o \
      test_objects.o
dep=$(obj:.o=.d)

.PHONY: clean test

clean:
	rm -f $(obj) $(dep) $(bin)

-include $(dep)

test: test_objects
	./test_objects

test_objects: obj.o boolean.o string.o empty_list.o pair.o error.o
