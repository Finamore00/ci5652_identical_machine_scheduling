# CI5652 - Identical Machine Scheduling
Proyecto de Diseño de Algoritmos II (CI5652) con soluciones aproximadas para el problema de Identical Machine Scheduling. Trimestre Abr-Jul 2024. Universidad Simón Bolívar.

## 📝 Integrantes
- Ana Shek (19-10096)
- Santiago Finamore (18-10125)
- Jeamhowards Montiel (19-10234)

# 🤔 Descripción del problema
Dado un conjunto de `n` tareas y `m` máquinas idénticas, el objetivo es asignar cada tarea a una máquina y determinar la secuencia de tareas en cada máquina de manera que se minimice la tardanza total (the total tardiness). Cada tarea `j` tiene un tiempo de procesamiento `p_j` y una fecha de vencimiento `d_j`. La tardanza de una tarea se calcula como `max(0, C_j - d_j)`, donde `C_j` es el tiempo de finalización del trabajo `j`.

# 📋 INFORME DEL PROYECTO - SEGUNDO CORTE
El programa está implementado en C++ y consta de los siguientes archivos para este segundo corte:

- `grasp.cpp`: Archivo principal del programa que contiene la implementación de una solución utilizando GRASP para el problema.

📂 En la carpeta `benchmarks` se encuentran los casos de pruebas de la primera corte del proyecto para medir y comparar el rendimiento de diferentes algoritmos para solucionar el problema descrito.

## Definición de una perturbación e implementación de una búsqueda local iterada (ILS).

## Definición de reglas para movimientos que han de ser tabús e implementación de una búsqueda tabú.

## Definición de un proceso de enfriado progresivo e implementación de un reconocido simulado.

## Definición de un método de construción para una RCL e implementación de GRASP.

## Definición de un fenotipo/genotipo, operadores de cruce y mutación e implementación de un algoritmo genético.

## 🚀 Uso

Para compilar y ejecutar el programa, se debe ejecutar los siguientes comandos en la terminal:

```bash
make
```

```bash
cd target
```

```bash
./PROY2 <path_to_benchmarks> <algorithm>
```

Donde `<path_to_benchmarks>` es la ruta a la carpeta que contiene los casos de prueba y `<algorithm>` es el número del algoritmo a ejecutar:

1. Búsqueda Local Iterada (ILS)
2. Búsqueda Tabú 
3. Reconocido Simulado
4. GRASP
5. Algoritmo Genético

## 📄 Analisis de resultados

A continuación se presenta el análisis de los resultados obtenidos al ejecutar el programa con los casos de prueba en la carpeta `benchmarks` en una **laptop** con **procesador AMD Ryzen 5 5500U**, **disco SSD**, **8GB de memoria RAM** y **WSL 2 Ubuntu**. Se tomará en cuenta los valores de tardanza óptima obtenidos del paper de referencia de donde se obtuvo el `benchmark` para comparar los resultados obtenidos con los algoritmos implementados.


### 📊 Métricas Comparativas

Las métricas claves en el análisis incluyen:

- **Tardanza Total (Total Tardiness)**: La suma de las tardanzas de todas las tareas.
- **Diferencia con la Solución Óptima (Optimal Solution Difference)**: La diferencia entre la solución obtenida y la solución óptima.

### 📈 Resultados


## 📌 Conclusiones



## 📚 Referencias
