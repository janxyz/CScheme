CC=gcc
CFLAGS=-std=c11 -g -Wall -Wextra -Wpedantic -Wshadow -MMD
# pattern-specific variable value for all targets prefixed with test_
test_%: LDLIBS += -lcmocka
test_%: CFLAGS += -Wl,--wrap=exit_with_error

bin = test_objects \
      test_eval \
      test_utils \
      mopl
obj = obj.o \
      string.o \
      pair.o \
      symbol.o \
      error.o \
      utils.o \
      eval.o \
      test_objects.o \
      test_eval.o \
      test_utils.o \
      mopl.o
dep=$(obj:.o=.d)

.PHONY: clean test

mopl: obj.o string.o pair.o symbol.o error.o

test: test_objects test_eval test_utils
	./test_objects
	./test_eval
	./test_utils

test_objects: obj.o string.o pair.o symbol.o error.o
test_eval: obj.o string.o pair.o symbol.o error.o eval.o utils.o
test_utils: obj.o string.o pair.o symbol.o error.o utils.o

clean:
	rm -f $(obj) $(dep) $(bin)

-include $(dep)
