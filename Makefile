EXEC = test_mm

SRC = mm.c test_mm.c syscall.c test_util.c

CC = gcc

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC)  $(SRC) -o $(EXEC)

clean:
	rm -f $(EXEC)
