# MedLexer - Proyecto 1 LFP

Aplicacion de escritorio en C++17 con Qt y CMake para analisis lexico del lenguaje MedLang.

## Estructura del repositorio

- `src/`: codigo fuente del proyecto (core + GUI).
- `docs/`: documentacion tecnica (AFD, arquitectura, manuales).
- `tests/`: casos de prueba del analizador.
- `examples/`: archivos `.med` de ejemplo.

## Requisitos

- CMake 3.16+
- Compilador C++17 (MinGW, GCC o MSVC)
- Qt 5.15+ o Qt 6.x (modulo Widgets)

## Compilacion (Windows, PowerShell)

```powershell
cmake -S . -B build
cmake --build build
```

## Ejecucion

```powershell
./build/MedLexer.exe
```

## Estado actual (Parte 1)

- Estructura base del proyecto creada.
- Integracion inicial Qt + CMake configurada.
- Clases base del analizador declaradas.
- Diseno del AFD documentado en `docs/AFD_MedLang.md`.
- Casos de prueba preliminares en `tests/casos_prueba.md`.

Las siguientes partes implementaran el AFD completo en `LexicalAnalyzer::nextToken()`, recuperacion de errores, GUI completa y reportes HTML/DOT.
