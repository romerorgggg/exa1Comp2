# Instrucciones de Configuración Inicial

## 📦 Setup del Proyecto Completo

### Paso 1: Crear la estructura de directorios

```bash
mkdir -p proyecto-particulas/{include,src,scripts,results,analysis,documents,obj,bin}
cd proyecto-particulas
```

### Paso 2: Copiar los archivos del proyecto

Copia los siguientes archivos a sus respectivas ubicaciones:

#### Código C++
```
include/
├── Bola.h
└── Caja.h

src/
├── main.cpp
├── Bola.cpp
└── Caja.cpp
```

#### Scripts Python
```
scripts/
├── visualizar.py
├── analisis_velocidades.py
├── conservacion_energia.py
└── generar_diagrama.py
```

#### Documentación
```
documents/
└── analisis_fisico.tex
```

#### Archivos de configuración
```
Makefile
Doxyfile
.gitignore
README.md
```

### Paso 3: Dar permisos de ejecución a scripts

```bash
chmod +x scripts/*.py
```

### Paso 4: Verificar dependencias

#### C++ y compilación
```bash
g++ --version        # Debe ser >= 7.0 con soporte C++17
make --version
doxygen --version    # Para documentación
```

#### Python
```bash
python3 --version    # Debe ser >= 3.6
pip3 install numpy matplotlib scipy
```

### Paso 5: Compilar el proyecto

```bash
make
```

Si la compilación es exitosa, verás:
```
Compilando src/main.cpp...
Compilando src/Bola.cpp...
Compilando src/Caja.cpp...
Enlazando...
Compilación exitosa!
Ejecute: ./bin/simulacion
```

### Paso 6: Ejecutar una prueba rápida

```bash
./bin/simulacion 1
```

Esto ejecutará el Experimento 1 (Gas Diluido) y generará archivos en `results/`

### Paso 7: Generar diagramas

```bash
python3 scripts/generar_diagrama.py
```

### Paso 8: Ejecutar todos los experimentos

```bash
make run
```

### Paso 9: Analizar resultados

```bash
make analyze
```

### Paso 10: Generar documentación

```bash
make docs
```

Abre `docs/html/index.html` en tu navegador.

### Paso 11: Compilar documento LaTeX

```bash
cd documents
pdflatex analisis_fisico.tex
pdflatex analisis_fisico.tex  # Segunda vez para referencias
cd ..
```

## 🔧 Solución de Problemas Comunes

### Error: "command not found: g++"
**Solución:** Instalar compilador
```bash
# Ubuntu/Debian
sudo apt-get install build-essential

# macOS
xcode-select --install

# Windows (usar WSL o MinGW)
```

### Error: "No module named 'numpy'"
**Solución:** Instalar dependencias Python
```bash
pip3 install --user numpy matplotlib scipy
```

### Error: "doxygen: command not found"
**Solución:** Instalar Doxygen
```bash
# Ubuntu/Debian
sudo apt-get install doxygen graphviz

# macOS
brew install doxygen graphviz
```

### Error de compilación: "C++17 features not supported"
**Solución:** Actualizar g++ o especificar versión
```bash
# Verificar versión
g++ --version

# Si necesitas usar g++-9 o mayor
# Edita Makefile y cambia CXX = g++ por CXX = g++-9
```

### Los scripts de Python no generan gráficas
**Solución:** Verificar que matplotlib use backend correcto
```bash
# Si estás en servidor sin X11
export MPLBACKEND=Agg
python3 scripts/visualizar.py
```

## 📊 Verificación de Resultados

Después de ejecutar todo, deberías tener:

```
results/
├── gas_diluido.dat
├── gas_denso.dat
├── euler.dat
├── verlet.dat
├── presion_5.dat
├── presion_10.dat
├── presion_15.dat
├── velocidades.dat
├── trayectorias_*.png
├── dist_velocidades_*.png
├── energia_*.png
├── comparacion_metodos.png
└── comparacion_velocidades.png

documents/
├── diagrama_flujo.png
├── diagrama_clases.png
└── analisis_fisico.pdf

docs/html/
└── index.html
```

## 🚀 Inicializar Git y GitHub

### Paso 1: Inicializar repositorio local
```bash
git init
git add .
git commit -m "Implementación inicial: Simulación N partículas"
```

### Paso 2: Crear repositorio en GitHub
1. Ve a https://github.com/new
2. Nombra tu repositorio: `fisica-comp2-particulas`
3. No inicialices con README (ya lo tienes)
4. Crea el repositorio

### Paso 3: Conectar y subir
```bash
git remote add origin https://github.com/TU-USUARIO/fisica-comp2-particulas.git
git branch -M main
git push -u origin main
```

### Paso 4: Invitar al profesor
En GitHub: Settings → Collaborators → Add people → Buscar al profesor

## 📝 Checklist Final

Antes de la entrega, verificar:

- [ ] Código compila sin errores ni warnings
- [ ] Todos los experimentos corren exitosamente
- [ ] README.md está completo y actualizado
- [ ] Documentación Doxygen genera sin errores
- [ ] Documento LaTeX compila correctamente
- [ ] Todas las gráficas se generan en results/
- [ ] Diagramas están en documents/
- [ ] Repositorio Git tiene commits del equipo
- [ ] Profesor tiene acceso al repositorio
- [ ] Presentación preparada (5-8 diapositivas)

## 🎯 Preparación para la Defensa

### Diapositivas sugeridas:

1. **Título y objetivos** (1 diapositiva)
2. **Diseño del software** - Clases y arquitectura (1 diapositiva)
3. **Métodos numéricos** - Euler vs Verlet (1 diapositiva)
4. **Resultados principales** - Gráficas clave (2-3 diapositivas)
5. **Validación** - Conservación y Maxwell-Boltzmann (1 diapositiva)
6. **Conclusiones** (1 diapositiva)

### Preguntas típicas que prepararas:

-