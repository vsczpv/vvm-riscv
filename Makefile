VERSION = "0.2.0"

SOURCE =	src
BUILD =		build
INCLUDE =	include

CC =		cc
OPT_CARGS :=
CARGS = 	$(OPT_CARGS) -I$(INCLUDE) -DVERSION='$(VERSION)' -Wall -Wextra -std=c2x -Wno-misleading-indentation -Wno-missing-field-initializers

OBJS = 		$(shell find $(SOURCE) -type f -name '*.c' | sed 's/\.c*$$/\.o/; s/$(SOURCE)\//$(BUILD)\//')
HEADERS =	$(shell find $(INCLUDE) -type f -name '*.h')

.PHONY: all clean builddir sample1

all: vvm-riscv

builddir:
	@if [ ! -d "build" ] ; then mkdir build ; fi

clean:
	rm -vf vvm-riscv $(OBJS)

	make -C samples/sample1 clean

$(OBJS): $(BUILD)/%.o: $(SOURCE)/%.c $(HEADERS)
	$(CC) -c $(CARGS) $< -o $@

sample1: vvm-riscv
	make -C samples/sample1 run

vvm-riscv: builddir $(OBJS)
	$(CC) $(CARGS) $(OBJS) -o $@
