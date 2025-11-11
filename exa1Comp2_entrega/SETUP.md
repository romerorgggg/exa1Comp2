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


# Si estás en servidor sin X11
export MPLBACKEND=Agg
python3 scripts/visualizar.py
```
