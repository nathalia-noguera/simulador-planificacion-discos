# Definir el compilador y los flags de compilación (C++17)
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

# Nombre del ejecutable final
TARGET = simulador

# Archivos fuente necesarios para construir el programa
SRC = src/main.cpp src/Disk.cpp src/Request.cpp src/Scheduler.cpp

# Regla principal por defecto al ejecutar 'make'
all: $(TARGET)

# Regla para compilar el ejecutable a partir de los fuentes y carpetas de inclusión
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

# Regla para limpiar y borrar los archivos binarios generados
clean:
	rm -f $(TARGET)