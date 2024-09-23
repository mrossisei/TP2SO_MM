# Nombre del ejecutable
EXEC = test_mm

# Archivos fuente
SRC = mm.c test_mm.c syscall.c test_util.c

# Compilador
CC = gcc

# Opciones de compilación
CFLAGS = -Wall -g

# Reglas
all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(EXEC)

# Limpia archivos compilados
clean:
	rm -f $(EXEC)
