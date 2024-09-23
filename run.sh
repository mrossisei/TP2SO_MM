#!/bin/bash

# Compilar el archivo mm.c
gcc mm.c test_mm.c syscall.c test_util.c -o mm

# Verificar si la compilación fue exitosa
if [ $? -eq 0 ]; then
    echo "Compilación exitosa. Ejecutando el programa..."
    # Ejecutar el programa
    /Users/matias/Desktop/TP2SO_MM/mm
else
    echo "Error en la compilación."
    exit 1
fi

# Limpiar los archivos generados
echo "Limpiando archivos generados..."
rm mm

echo "Proceso completado."
# Instrucción para correr el script desde la terminal
# echo "bash /Users/matias/Desktop/TP2SO_MM/run.sh"