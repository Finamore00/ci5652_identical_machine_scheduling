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

### 🔄 ILS

La implementación del algoritmo de Búsqueda Local Iterada (ILS) recibe la información de las `n` tareas, la cantidad de `m` máquinas, y varios parámetros que controlan el proceso de búsqueda, como el número máximo de iteraciones y la fuerza de perturbación inicial. A continuación se describen los pasos de la implementación:

1. **Inicialización:** 
    - El algoritmo empieza creando una solución inicial `S` utilizando el método `mddScheduling`. 
    - Luego, aplica el algoritmo `RNA` a esta solución inicial para obtener una solución mejorada `S`.
    - Esta solución `S` es considerada la mejor solución conocida hasta el momento (`best_schedule`).

2. **Iteraciones principales:** 
    - Para cada iteración, se realiza una perturbación a la solución actual `current_schedule` aplicando un número `p` de movimientos aleatorios.
    - Después de la perturbación, se aplica el algoritmo `RNA` para mejorar la solución perturbada.

3. **Evaluación de soluciones:** 
    - Si la solución mejorada tiene una tardanza total (`total_tardiness`) menor que la mejor solución conocida, se actualiza la mejor solución y se restablece la fuerza de perturbación `p` a su valor inicial.
    - Si no mejora, se incrementa un contador `i`.

4. **Ajuste de la perturbación:** 
    - Si el contador `i` alcanza el límite `itermax`, se incrementa la fuerza de perturbación `p`. Si `p` excede un valor máximo `pmax`, se restablece a su valor inicial.

5. **Repetición:** 
    - Se repiten los pasos 2-4 hasta que se agoten las iteraciones.

El algoritmo `ILS` busca explorar el espacio de soluciones mediante la combinación de perturbaciones y optimización local, ayudando a escapar de óptimos locales y encontrar soluciones mejores.

## Definición de reglas para movimientos que han de ser tabús e implementación de una búsqueda tabú.

### 🚫 Tabu Search

La implementación del algoritmo de Búsqueda Tabú recibe la información de las `n` tareas, la cantidad de `m` máquinas, y varios parámetros que controlan el proceso de búsqueda, como el número máximo de iteraciones y la tenencia de la lista tabú. A continuación se describen los pasos de la implementación:

1. **Inicialización:** 
    - El algoritmo empieza creando una solución inicial `S` utilizando el método `mddScheduling`. 
    - Esta solución `S` es considerada la mejor solución conocida hasta el momento (`best_schedule`).

2. **Lista Tabú:** 
    - Se inicializa una lista tabú para almacenar los movimientos que están prohibidos temporalmente.

3. **Iteraciones principales:** 
    - Para cada iteración, se identifica la máquina con mayor tardanza (`tardiest_machine`).
    - Se generan múltiples vecinos de la solución actual mediante movimientos aleatorios en la máquina con mayor tardanza.
    - Se selecciona el mejor vecino que no esté en la lista tabú o que mejora la mejor solución conocida.

4. **Actualización de la lista Tabú:** 
    - Si el mejor vecino no está en la lista tabú o mejora la mejor solución conocida, se actualiza la solución actual y se añade el movimiento a la lista tabú.
    - Si la lista tabú excede su tamaño máximo (`tabu_tenure`), se elimina el movimiento más antiguo.

5. **Actualización de la mejor solución:** 
    - Si el vecino seleccionado mejora la mejor solución conocida, se actualiza la mejor solución.

6. **Repetición:** 
    - Se repiten los pasos 3-5 hasta que se agoten las iteraciones.

El algoritmo `Tabu Search` busca explorar el espacio de soluciones evitando ciclos y escapando de óptimos locales mediante el uso de una lista tabú que prohíbe ciertos movimientos temporalmente.

## Definición de un proceso de enfriado progresivo e implementación de un recocido simulado.

### ❄️ Simulated Annealing

La implementación del algoritmo de Recocido Simulado (Simulated Annealing) recibe la información de las `n` tareas, la cantidad de `m` máquinas, y varios parámetros que controlan el proceso de búsqueda, como la temperatura inicial y el factor de reducción de temperatura. A continuación se describen los pasos de la implementación:

1. **Inicialización:** 
    - El algoritmo empieza creando una solución inicial `S` utilizando el método `mddScheduling`. 
    - Esta solución `S` es considerada la mejor solución conocida hasta el momento (`best_schedule`).
    - Se inicializa la temperatura `t` con un valor inicial `t0`.

2. **Iteraciones principales:** 
    - Para cada iteración, se generan múltiples vecinos de la solución actual mediante movimientos aleatorios.
    - Se calcula la diferencia de tardanza (`delta`) entre el vecino y la solución actual.

3. **Criterio de aceptación:** 
    - Si el vecino tiene una tardanza menor o igual, se acepta.
    - Si el vecino tiene una tardanza mayor, se acepta con una probabilidad `exp(-delta / t)`.

4. **Actualización de la mejor solución:** 
    - Si la solución actual mejorada tiene una tardanza menor que la mejor solución conocida, se actualiza la mejor solución.

5. **Reducción de la temperatura:** 
    - Después de un número fijo de iteraciones, se reduce la temperatura multiplicándola por un factor `t_step`.
    - Si la temperatura cae por debajo de un umbral `epsilon`, se restablece a su valor inicial `t0`.

6. **Repetición:** 
    - Se repiten los pasos 2-5 hasta que se agoten las iteraciones.

El algoritmo `Simulated Annealing` busca explorar el espacio de soluciones permitiendo peores soluciones con una probabilidad decreciente, lo que ayuda a escapar de óptimos locales y encontrar mejores soluciones globales.

## 🎲 Definición de un método de construción para una RCL e implementación de GRASP.

### 👨‍⚖️ Método de construción para una RCL

Para la definición del RCL en este problema, se utilizó el enfoque heurístico Modified Due Date (MDD) explicado en el primer corte del proyecto:

1. Sea `S` una solución parcialmente construida.
2. Se tiene una lista de tareas no programadas `U` en la solución `S`.
3. Para cada máquina `j`, dividir `U` en dos subconjuntos `U1j` y `U2j` para `j` = 1, 2, ..., `m`.
    > `U1j` contiene las tareas que no se pueden completar en su fecha de vencimiento en la máquina `j`.

    > `U2j` contiene las tareas que sí se pueden completar antes de su fecha de vencimiento en la máquina `j`.
4. De `U1j` y `U2j`, encontrar los subconjuntos `γj` y `λj` que contienen las tareas con el tiempo de procesamiento mínimo y la fecha de vencimiento mínima, respectivamente. 
5. Seleccionar una tarea `gj` de `γj` o `λj` que minimice el valor de MDD en la máquina `j`. El valor de MDD en la máquina `j` de una tarea `i` está dada por `MDD(j, i) = max(Cj + pi, di)` 
    > `Cj` es la suma del procesamiento de tiempo de las tareas que ya han sido programados en la máquina `j`.

    > `pi` es el tiempo de procesamiento de la tarea `i` con su fecha de vencimiento `di`.
6. Luego, sea `C` el conjunto resultante de tener cada par `<g, l>`, donde cada una de estos pares representa que la tarea `g` es la tarea que produce el menor valor MDD en la máquina `l`).
7. Se puede definir el costo de la función de un elemento `<g, l>` en `C` como `c(<g, l>) = MDD(g, l)`.
8. También se define `c_min = min{ c(<g, l>) | <g, l> ∈ C}` y `c_max = max{ c(<g, l>) | <g, l> ∈ C}`.
9. Entonces, el `RCL = { <g, l> ∈ C | c(<g, l>) <= c_min + α(c_max - c_min)}`

### 🎰 GRASP

La implementación del algoritmo GRASP recibe la información de las `n` tareas, la cantidad de las `m` máquinas, el `alpha` para el RCL y `el máximo número de iteraciones`. A continuación se describen los pasos de la implementación: 

1. El algoritmo empieza a generar una solución inicial aleatoria `S`. Por ahora, se tiene que `S` es la mejor solución que se tiene para el problema.
2. Ahora, para cada iteración, se construye una solución voraz aleatoria `S'`, el cual: 

    2.1. Se considera las tareas que aún no han sido programadas y se aplica el enfoque heurístico para la Lista Restringida de Candidatos (RCL). 

    2.2. De esta lista RCL, se escoge aleatoriamente un elemento: `<g, l>`. 

    2.3. Luego, del elemento seleccionado `<g, l>`, se le asigna la tarea `g` a la máquina `l`. 

    2.4. Se elimina la tarea `g` de la lista de tareas sin programar en esta solución parcial construida `S'`.

    2.5. Se repite los pasos 2.1 a 2.4 hasta que no queden tareas sin programar.
3. Se reemplaza `S` por `S'` si el retraso total o total tardiness de las tareas en la solución `S` es mayor que el de `S'`, en caso contrario, no se hace nada.
4. Se repite el paso 2 y 3 hasta que se acaben las iteraciones.

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
