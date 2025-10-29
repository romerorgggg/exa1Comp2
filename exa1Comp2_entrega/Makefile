# Makefile para simulación de partículas en caja
# Física Computacional 2 - 2025

# Compilador y flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -Iinclude
LDFLAGS = -lm

# Directorios
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin
RESULTS_DIR = results

# Archivos
TARGET = $(BIN_DIR)/simulacion
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Colores para output
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[1;33m
NC = \033[0m # No Color

# Regla principal
all: directories $(TARGET)
	@echo "$(GREEN)Compilación exitosa!$(NC)"
	@echo "Ejecute: $(YELLOW)./$(TARGET)$(NC)"

# Crear directorios necesarios
directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR) $(RESULTS_DIR)

# Compilar el ejecutable
$(TARGET): $(OBJECTS)
	@echo "$(YELLOW)Enlazando...$(NC)"
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Compilar archivos objeto
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "$(YELLOW)Compilando $<...$(NC)"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpiar archivos generados
clean:
	@echo "$(RED)Limpiando archivos...$(NC)"
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "$(GREEN)Limpieza completa!$(NC)"

# Limpiar todo incluyendo resultados
cleanall: clean
	@echo "$(RED)Limpiando resultados...$(NC)"
	rm -rf $(RESULTS_DIR)/*
	@echo "$(GREEN)Limpieza total completa!$(NC)"

# Ejecutar todos los experimentos
run: all
	@echo "$(GREEN)Ejecutando todos los experimentos...$(NC)"
	./$(TARGET) 6

# Ejecutar experimento específico
run1: all
	./$(TARGET) 1

run2: all
	./$(TARGET) 2

run3: all
	./$(TARGET) 3

run4: all
	./$(TARGET) 4

run5: all
	./$(TARGET) 5

# Generar documentación con Doxygen
docs:
	@echo "$(YELLOW)Generando documentación...$(NC)"
	doxygen Doxyfile
	@echo "$(GREEN)Documentación generada en docs/html/$(NC)"

# Analizar resultados con Python
analyze:
	@echo "$(YELLOW)Ejecutando scripts de análisis...$(NC)"
	python3 scripts/visualizar.py
	python3 scripts/analisis_velocidades.py
	python3 scripts/conservacion_energia.py
	@echo "$(GREEN)Análisis completado!$(NC)"

# Mostrar ayuda
help:
	@echo "$(YELLOW)Makefile - Simulación de N partículas$(NC)"
	@echo ""
	@echo "Objetivos disponibles:"
	@echo "  $(GREEN)make$(NC)          - Compilar el proyecto"
	@echo "  $(GREEN)make run$(NC)      - Ejecutar todos los experimentos"
	@echo "  $(GREEN)make run1-5$(NC)   - Ejecutar experimento específico"
	@echo "  $(GREEN)make clean$(NC)    - Limpiar archivos de compilación"
	@echo "  $(GREEN)make cleanall$(NC) - Limpiar todo incluyendo resultados"
	@echo "  $(GREEN)make docs$(NC)     - Generar documentación Doxygen"
	@echo "  $(GREEN)make analyze$(NC)  - Analizar resultados con Python"
	@echo "  $(GREEN)make help$(NC)     - Mostrar esta ayuda"

.PHONY: all clean cleanall run run1 run2 run3 run4 run5 docs analyze help directories