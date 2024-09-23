#!/bin/bash

# Function to clean up
cleanup() {
    echo "Limpiando archivos generados..."
    make clean
    echo "Proceso completado."
}

trap cleanup SIGINT

chmod +x run.sh
make all
echo "Compilación exitosa. Ejecutando el programa..."
./test_mm

cleanup