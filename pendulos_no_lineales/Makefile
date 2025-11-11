# ===============================
#  Makefile - Péndulos acoplados
# ===============================
# Compilación del simulador interactivo en C++
# Autor: Norman Romero, Jorge Garzón
CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Iinclude
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:src/%.cpp=build/%.o)
TARGET = bin/pendulos_final

# Compilación completa
all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $^

build/%.o: src/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpieza de archivos generados
clean:
	rm -rf build bin results/*.csv results/*.png pendulos_results.zip

.PHONY: all clean
