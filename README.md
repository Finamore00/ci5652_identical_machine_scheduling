# ci5652_identical_machine_scheduling
Proyecto de Diseño de Algoritmos II (CI5652) con soluciones aproximadas para el problema de Identical Machine Scheduling. Trimestre Abr-Jul 2024. Universidad Simón Bolívar

# 🤔 Descripción del problema
Dado un conjunto de `n` tareas y `m` máquinas idénticas, el objetivo es asignar cada tarea a una máquina y determinar la secuencia de tareas en cada máquina de manera que se minimice la tardanza total (the total tardiness). Cada tarea `j` tiene un tiempo de procesamiento `p_j` y una fecha de vencimiento `d_j`. La tardanza de una tarea se calcula como `max(0, C_j - d_j)`, donde `C_j` es el tiempo de finalización del trabajo `j`.

# 📋 INFORME DEL PROYECTO - PRIMER CORTE
El programa está implementado en C++ y consta de los siguientes archivos:

- `heuristic.cpp`: Archivo principal del programa que contiene la implementación de una heurística para el problema.
- `Job.h`: Archivo de encabezado que define la estructura `Job`.
- `utilities.h`: Archivo de encabezado que contiene funciones utilitarias para leer archivos de entrada y manipulación de cadenas.

📂 En la carpeta `benchmarks` se encuentran los casos de pruebas que se utilizaran para medir y comparar el rendimiento de diferentes algoritmos para solucionar el problema descrito.

## 👌 Implementación de una solución exacta para el problema

## 🧠 Implementación de una heurística para el problema
El algoritmo MDD (Modified Due Date) es un enfoque heurístico, el cual funciona de la siguiente manera:

1. Se tiene una lista de tareas no programadas `U`.
2. Para cada máquina `j`, dividir `U` en dos subconjuntos `U1j` y `U2j` para `j` = 1, 2, ..., `m`.
    > `U1j` contiene las tareas que no se pueden completar en su fecha de vencimiento en la máquina `j`.

    > `U2j` contiene las tareas que sí se pueden completar antes de su fecha de vencimiento en la máquina `j`.
3. De `U1j` y `U2j`, encontrar los subconjuntos `γj` y `λj` que contienen las tareas con el tiempo de procesamiento mínimo y la fecha de vencimiento mínima, respectivamente.
4. Seleccionar una tarea `gj` de `γj` o `λj` que minimice el valor de MDD en la máquina `j`. El valor de MDD en la máquina `j` de una tarea `i` está dada por `MDD = max(Cj + pi, di)` 
    > `Cj` es la suma del procesamiento de tiempo de las tareas que ya han sido programados en la máquina `j`.

    > `pi` es el tiempo de procesamiento de la tarea `i` con su fecha de vencimiento `di`.
5. Programar la tarea `gl` en la máquina `l` que produzca el valor MDD mínimo. 
6. Eliminar la tarea `gl` de la lista `U` de trabajos no programados.
7. Repetir los pasos 2-6 hasta que no queden trabajos por programar.

## 🏘️ Definición de una estructura de vecindad e implementación de la búsqueda local para el problema

## 🚀 Uso

Para compilar un `.cpp` y correr el programa, usa el siguiente comando:

```bash
g++ -std=c++17 program archivo.cpp
./program