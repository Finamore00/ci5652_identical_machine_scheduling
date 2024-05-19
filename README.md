# ci5652_identical_machine_scheduling
Proyecto de Diseño de Algoritmos II (CI5652) con soluciones aproximadas para el problema de Identical Machine Scheduling. Trimestre Abr-Jul 2024. Universidad Simón Bolívar

# 🤔 Descripción del problema
Dado un conjunto de `n` tareas y `m` máquinas idénticas, el objetivo es asignar cada tarea a una máquina y determinar la secuencia de tareas en cada máquina de manera que se minimice la tardanza total (the total tardiness). Cada tarea `j` tiene un tiempo de procesamiento `p_j` y una fecha de vencimiento `d_j`. La tardanza de una tarea se calcula como `max(0, C_j - d_j)`, donde `C_j` es el tiempo de finalización del trabajo `j`.

# 📋 INFORME DEL PROYECTO - PRIMER CORTE
El programa está implementado en C++ y consta de los siguientes archivos:

- `exact.cpp`: Archivo principal del programa que contiene la implementación de una solución exacta para el problema.
- `heuristic.cpp`: Archivo principal del programa que contiene la implementación de una heurística para el problema.
- `vicinities.cpp`: Archivo del programa que contiene la implementación de la estructura de vecindad para el problema.
- `local_search.cpp`: Archivo principal del programa que contiene la implementación de la búsqueda local para el problema.
- `Job.h`: Archivo de encabezado que define la estructura `Job`.
- `utilities.h`: Archivo de encabezado que contiene funciones utilitarias para leer archivos de entrada y manipulación de cadenas.

📂 En la carpeta `benchmarks` se encuentran los casos de pruebas que se utilizaran para medir y comparar el rendimiento de diferentes algoritmos para solucionar el problema descrito.

## 👌 Implementación de una solución exacta para el problema
El algoritmo exacto para el problema de Identical Machine Scheduling es un algoritmo de fuerza bruta que genera todas las permutaciones posibles de las tareas y calcula la tardanza total para cada permutación. El algoritmo selecciona la permutación que minimiza la tardanza total y devuelve la secuencia de tareas correspondiente.

Este algoritmo utiliza backtracking + bitmask para encontrar la solución exacta al problema de programación de trabajos en máquinas idénticas. El objetivo es minimizar la tardanza total.

El algoritmo propuesto toma una lista de trabajos, la distribución de trabajos por cada máquina y una máscara que representa los trabajos que ya han sido asignados. La máscara es un número binario donde cada bit representa si un trabajo ha sido asignado o no.

El algoritmo de backtracking funciona de la siguiente manera:

Si todos los trabajos han sido asignados (es decir, la máscara es igual a `(1 << jobs.size()-1)`), entonces devuelve la distribución de trabajos por cada máquina.

De lo contrario, para cada trabajo que aún no ha sido asignado (es decir, `!(mask & (1 << i))` es verdadero), intenta asignarlo a cada programación y realiza una llamada recursiva a la misma función con el trabajo asignado.

Después de cada llamada recursiva, compara la tardanza total de la solución actual con la tardanza total de la mejor solución encontrada hasta ahora. Si la solución actual es mejor, entonces la guarda como la mejor solución.

Finalmente, después de haber probado todas las asignaciones posibles, devuelve la mejor solución encontrada.

Este algoritmo garantiza encontrar la solución óptima porque genera y verifica todas las asignaciones posibles de trabajos a las maquinas. Sin embargo, su tiempo de ejecución es exponencial en el número de trabajos, por lo que solo es práctico para problemas de tamaño pequeño.

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

Para compilar y ejecutar el programa, se debe ejecutar los siguientes comandos en la terminal:

```bash
make
```

```bash
cd target
```

```bash
./LocalSearch <path_to_benchmarks> <algorithm>
```

Donde `<path_to_benchmarks>` es la ruta a la carpeta que contiene los casos de prueba y `<algorithm>` es el número del algoritmo a ejecutar:

1. Algoritmo Exacto
2. Heurística
3. Búsqueda Local

## 📄 Analisis de resultados

A continuación se presenta el análisis de los resultados obtenidos al ejecutar el programa con los casos de prueba en la carpeta `benchmarks`. Se tomará en cuenta los valores de tardanza óptima obtenidos del paper de referencia de donde se obtuvo el `benchmark` para comparar los resultados obtenidos con los algoritmos implementados.


### 📊 Métricas Comparativas

Las métricas claves en el análisis incluyen:

- **Tardanza Total (Total Tardiness)**: La suma de las tardanzas de todas las tareas.
- **Diferencia con la Solución Óptima (Optimal Solution Difference)**: La diferencia entre la solución obtenida y la solución óptima.

### 📈 Resultados

Los resultados obtenidos al ejecutar el programa con los casos de prueba en la carpeta `benchmarks` se encuentran en el directorio `results`, sin embargo, debido a la cantidad de datos obtenidos, se almacenó los datos más relevantes en el siguiente enlace: [Resultados](https://docs.google.com/spreadsheets/d/1nE2hubPLtpBfe2wIpuzA0SgOXGMpi-NJaMEHRL38QYM/edit?usp=sharing)

A modo de resumen y para facilitar la visualización de los resultados, se presentan las tablas comparativas resumidas a continuación:

|   n  |  m  | Promedio de diferencia de tardanza para el algoritmo heuristica  vs la tardanza óptima | Promedio de diferencia de tardanza para el algoritmo ls con solución inicial heuristica  vs la tardanza óptima | Promedio de diferencia de tardanza para el algoritmo ls  con solución inicial aleatoria  vs la tardanza óptima |
|:----:|:---:|:-----------:|:-------------------:|:--------------:|
|  20  |  2  |    14,784   |        6,008        |       23       |
|  20  |  3  |    18,544   |        10,28        |       86       |
|  20  |  4  |    20,936   |        13,264       |      114       |
|  20  |  5  |    21,512   |        15,616       |      439,5     |
|  20  |  6  |     20,6    |        15,208       |       61       |
|  20  |  7  |    20,688   |        17,008       |      227,5     |
|  20  |  8  |    20,016   |        15,424       |      333       |
|  20  |  9  |    17,944   |        15,152       |      141       |
|  20  | 10  |    16,176   |        14,056       |      33,5      |
|  25  |  2  |     25,88   |        10,52        |      18,5      |
|  25  |  3  |    23,176   |        13,136       |       34       |
|  25  |  4  |    22,064   |        15,128       |       15       |
|  25  |  5  |    23,528   |        16,36        |       69       |
|  25  |  6  |    23,024   |        17,112       |      315,5     |
|  25  |  7  |    22,656   |        17,08        |       77       |
|  25  |  8  |    21,528   |        16,84        |      298       |
|  25  |  9  |    21,416   |        16,664       |      112       |
|  25  | 10  |    22,896   |        18,216       |      144       |

|   n  |  m  | Promedio de tiempo para el algoritmo heuristica (s) | Promedio de tiempo para el algoritmo ls con solución inicial heuristica (s) | Promedio de tiempo para el algoritmo ls con solución inicial aleatoria (s) |
|:----:|:---:|:---------------------------------------------------------------:|:-------------------------------------------------------------------:|:----------------------------------------------------------------:|
|  20  |  2  |                          0,000177437608                         |                           0,5536462978                            |                          0,684012973                           |
|  20  |  3  |                          0,000250604512                         |                           0,6705884606                            |                          0,8120744103                          |
|  20  |  4  |                          0,000177437608                         |                           0,8305885517                            |                          0,9842081426                          |
|  20  |  5  |                          0,000421948168                         |                           0,9994120584                            |                          1,10762457                            |
|  20  |  6  |                          0,000490450608                         |                           1,181125517                             |                          1,279287094                           |
|  20  |  7  |                              0,000561551                         |                           1,354315666                             |                          1,43728576                            |
|  20  |  8  |                          0,000332425656                         |                           1,55716976                              |                          1,62052424                            |
|  20  |  9  |                          0,00034788928                          |                           1,72209336                              |                          1,76874936                            |
|  20  | 10  |                          0,000428774128                         |                           1,90492776                              |                          2,00072456                            |
|  25  |  2  |                              1,0,000115114784                            |                           0,6300726939                            |                          0,8729056595                          |
|  25  |  3  |                          0,000178468432                         |                           0,8133722274                            |                          1,109488589                           |
|  25  |  4  |                          0,000224764072                         |                           0,9550425673                            |                          1,236124276                           |
|  25  |  5  |                          0,000285201784                         |                           1,114882156                             |                          1,405418945                           |
|  25  |  6  |                          0,000326256624                         |                           1,297659981                             |                          1,403675999                           |
|  25  |  7  |                          0,000390043864                         |                           1,299856236                             |                          1,470842934                           |
|  25  |  8  |                          0,000449967848                         |                           1,369663928                             |                          1,556300776                           |
|  25  |  9  |                          0,000483178408                         |                           1,325604456                             |                          1,560743072                           |
|  25  | 10  |                          0,000521611536                         |                           1,373501304                             |                          1,414723992                           |

- **Algoritmo Exacto**: En el caso del algoritmo exacto propuesto, el tiempo de ejecución para el benchmark propuesto resultó ser excepcionalmente alto. A pesar de haberse ejecutado el algoritmo durante más de una hora, no se logró obtener una solución. Por consiguiente, se tomó la decisión de no incorporar estos resultados en la tabla comparativa. La razón de este comportamiento radica en la complejidad exponencial del algoritmo exacto, lo que lo hace inviable para instancias de gran tamaño. No obstante, se anticipa que este algoritmo pueda proporcionar la solución óptima para los casos de prueba.

- **Algoritmo Heurístico**: El algoritmo heurístico propuesto logró obtener resultados aceptables en comparación con la solución óptima. En promedio, la diferencia de tardanza fue de 19,02 para instancias de tamaño 20 y 22,91 para instancias de tamaño 25. En cuanto al tiempo de ejecución, el algoritmo heurístico se ejecutó en un tiempo promedio de 0,0004 segundos para instancias de tamaño 20 y 0,0004 segundos para instancias de tamaño 25. En general, el algoritmo heurístico demostró ser eficiente y efectivo para instancias de tamaño moderado. 

    A partir de los resultados obtenidos, se puede concluir que el algoritmo heurístico propuesto es una solución viable para instancias de tamaño moderado. A pesar de no garantizar la solución óptima, el algoritmo heurístico logra obtener resultados aceptables en un tiempo de ejecución razonable. Por otro lado, el algoritmo exacto se muestra como una alternativa para instancias de tamaño pequeño, aunque su complejidad exponencial lo hace inviable para instancias de gran tamaño.

- **Algoritmo de Búsqueda Local con Solución Inicial de la Heurística**: El algoritmo de búsqueda local con heurística logró obtener resultados superiores en comparación con el algoritmo heurístico. En promedio, la diferencia de tardanza fue de 13,56 para instancias de tamaño 20 y 15,67 para instancias de tamaño 25. En cuanto al tiempo de ejecución, el algoritmo de búsqueda local con heurística se ejecutó en un tiempo promedio de 1,20 segundos para instancias de tamaño 20 y 1,13 segundos para instancias de tamaño 25. En general, observamos que el algoritmo de búsqueda local con heurística logra mejorar la solución obtenida por el algoritmo heurístico, aunque a costa de un mayor tiempo de ejecución, sin embargo, el tiempo de ejecución sigue siendo razonable si tomamos en cuenta el porcentaje de mejora en la tardanza total, para el caso de 20 tareas es de 28,71% y para el caso de 25 tareas es de 31,60%, lo que nos indica que el algoritmo de búsqueda local con heurística es una buena opción si se busca una mejor aproximación a la solución óptima.

- **Algoritmo de Búsqueda Local con Solución Inicial Aleatoria**: El algoritmo de búsqueda local con solución inicial aleatoria logró obtener resultados bastante más
bajos en comparación con el algoritmo de búsqueda local con heurística. En promedio, la diferencia de tardanza fue de 162,06 para instancias de tamaño 20 y 120,33 para instancias de tamaño 25. En cuanto al tiempo de ejecución, el algoritmo de búsqueda local con solución inicial aleatoria se ejecutó en un tiempo promedio de 1,30 segundos para instancias de tamaño 20 y 1,34 segundos para instancias de tamaño 25. En general, observamos que el algoritmo de búsqueda local con solución inicial aleatoria no logra mejorar la solución obtenida por el algoritmo heurístico. 

    La razón de este comportamiento radica en la calidad de la solución inicial. Al utilizar una solución inicial aleatoria, el algoritmo de búsqueda local se enfrenta a un espacio de búsqueda que en promedio llevará a cuencas de atracción que no son favorables, lo que dificulta la convergencia hacia una solución óptima. Por otro lado, el algoritmo de búsqueda local con heurística logra obtener una solución inicial de mayor calidad, lo que facilita la convergencia hacia una solución óptima. En general, se observa que la calidad de la solución inicial juega un papel crucial en el rendimiento del algoritmo de búsqueda local.

## 📌 Conclusiones

En este proyecto, se ha abordado el problema de Identical Machine Scheduling, el cual consiste en asignar un conjunto de tareas a un conjunto de máquinas idénticas de manera que se minimice la tardanza total. Para resolver este problema, se han implementado diferentes algoritmos, incluyendo un algoritmo exacto, un algoritmo heurístico y un algoritmo de búsqueda local. 

Para el caso del algoritmo exacto, se observó que su complejidad exponencial lo hace inviable para instancias de gran tamaño, como es el caso de los benchmarks propuestos. A pesar de no haber obtenido una solución en un tiempo razonable, se anticipa que el algoritmo exacto pueda proporcionar la solución óptima para instancias de tamaño pequeño. 

Por otra parte, el análisis derivado de los resultados obtenidos con el algoritmo heurístico, el algoritmo de búsqueda local con solución inicial de la heurística y el algoritmo de búsqueda local con solución inicial aleatoria, permitió concluir que el algoritmo de búsqueda local con solución inicial de la heurística es la mejor opción para obtener una solución aproximada al problema de Identical Machine Scheduling. A pesar de que el algoritmo de búsqueda local con solución inicial aleatoria no logró mejorar la solución obtenida por el algoritmo heurístico, se observó que la calidad de la solución inicial juega un papel crucial en el rendimiento del algoritmo de búsqueda local.

## 📚 Referencias

1. [Google](https://www.google.com)


