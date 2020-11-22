CC=gcc
CFLAGS=-std=c11 -g -Wall -Wextra -Wpedantic -Wshadow -MMD

cscheme: LDLIBS += -lreadline

tests/test_%: LDLIBS += -lcmocka
tests/test_%: LDFLAGS += -Wl,--wrap=exit_with_error

# Keep object files
.PRECIOUS: %.o
# Cancle implicit rule to compile executable from source
%: %.c

src = $(wildcard *.c)
obj = $(src:.c=.o)
dep = $(obj:.o=.d)
bin = cscheme

test_src = $(wildcard tests/*.c)
test_obj = $(test_src:.c=.o)
test_dep = $(test_obj:.o=.d)
test_bin = $(test_obj:.o=)

all: $(bin) $(test_bin)

$(bin): $(obj)

# Link test executable with all object files
# except the ones for the $(bin) executables.
$(test_bin): $(filter-out $(bin:=.o),$(obj))


.PHONY: all clean test

tests: $(test_bin)
	@for f in $^; do ./$$f; done

clean:
	$(RM) $(obj) $(dep) $(bin)
	$(RM) $(test_obj) $(test_dep) $(test_bin)

-include $(dep)
-include $(test_dep)
