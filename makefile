BIN=bin
IDIR =include
CFLAGS=-I$(IDIR) -Wextra -ansi -pedantic -Wuninitialized -Werror
CFLAGS_tables = -Wno-unused-parameter
CFLAGS_main = -Wall
LIBS = -lpthread
ODIR =obj
_OBJ = main.o tables.o buffer_blocks.o modified_alg.o modified_multi_block_alg.o original_alg.o original_multi_block_alg.o product_executor.o alg_threads.o tables_manager.o cpu_specs.o stresser.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
SRC = src
_OUTPUT = cpu_stress
OUTPUT = $(BIN)/$(_OUTPUT)
ICX := $(shell command -v icx 2>/dev/null)
ifeq ($(ICX),)
    CC := gcc -O3 #-g -O0
else
    CC := icx
endif

$(shell mkdir -p $(ODIR))

$(ODIR)/%.o: $(SRC)/%.c
	$(CC) -c -o $@ $< $(CFLAGS) $(CFLAGS_$*)

$(shell mkdir -p $(BIN))

$(OUTPUT): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) -Wall $(LIBS)

.PHONY: clean

clean:
	rm -r $(ODIR)
