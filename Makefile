# Makefile para el proyecto de Péndulos Acoplados

# Compilador y flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -O2
LDFLAGS =

# Directorios
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin
DOCDIR = docs
RESDIR = results
DOCSDIR = documents

# Archivos fuente y objeto
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))
EXECUTABLE = $(BINDIR)/pendulos

# --- Reglas Principales ---
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "✅ Ejecutable creado en $(EXECUTABLE)"

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# --- Reglas de Tareas ---
run: all
	@./$(EXECUTABLE)

plot:
	@echo "🐍 Generando gráficos..."
	@python3 scripts/plot.py $(RESDIR)/simulation_data.csv
	@echo "📊 Gráficos guardados en la carpeta $(RESDIR)/"

doc:
	@echo "📄 Generando documentación con Doxygen..."
	@doxygen Doxyfile
	@echo "📘 Documentación generada en la carpeta $(DOCDIR)/html/"

report:
	@echo "📝 Compilando reporte LaTeX..."
	@pdflatex -output-directory=$(DOCSDIR) $(DOCSDIR)/analysis.tex
	@pdflatex -output-directory=$(DOCSDIR) $(DOCSDIR)/analysis.tex
	@echo "📄 Reporte generado en $(DOCSDIR)/analysis.pdf"
	@rm -f $(DOCSDIR)/*.aux $(DOCSDIR)/*.log

clean:
	@echo "🧹 Limpiando el proyecto..."
	@rm -rf $(OBJDIR) $(BINDIR) $(DOCDIR)
	@rm -f $(RESDIR)/*.*
	@rm -f $(DOCSDIR)/*.pdf $(DOCSDIR)/*.aux $(DOCSDIR)/*.log

.PHONY: all run plot doc report clean