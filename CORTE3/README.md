# CI5652 - Identical Machine Scheduling
Proyecto de Diseño de Algoritmos II (CI5652) con soluciones aproximadas para el problema de Identical Machine Scheduling. Trimestre Abr-Jul 2024. Universidad Simón Bolívar.

## 📝 Integrantes
- Ana Shek (19-10096)
- Santiago Finamore (18-10125)
- Jeamhowards Montiel (19-10234)

# 🤔 Descripción del problema
Dado un conjunto de `n` tareas y `m` máquinas idénticas, el objetivo es asignar cada tarea a una máquina y determinar la secuencia de tareas en cada máquina de manera que se minimice la tardanza total (the total tardiness). Cada tarea `j` tiene un tiempo de procesamiento `p_j` y una fecha de vencimiento `d_j`. La tardanza de una tarea se calcula como `max(0, C_j - d_j)`, donde `C_j` es el tiempo de finalización del trabajo `j`.

# 📋 INFORME DEL PROYECTO - TERCER CORTE
El programa está implementado en C++ y consta de los siguientes archivos para este tercer corte:

- `memetic.cpp`: Archivo principal del programa que contiene la implementación de una solución utilizando el algoritmo memético para el problema.
- `aco.cpp`: Archivo principal del programa que contiene la implementación de una solución utilizando el algoritmo de optimicación de colonia de hormigas para el problema.
- `scattered.cpp`: Archivo principal del programam que contiene la implementación de la metaheurística de búsqueda dispersa aplicada al problema a tratar.

📂 En la carpeta `benchmarks` se encuentran los casos de pruebas de la primera corte del proyecto para medir y comparar el rendimiento de diferentes algoritmos para solucionar el problema descrito.

## Definición del operador de cruce utilizado en el algoritmo genético para una recombinación de al menos 3 padres, el método de mejora luego de la recombinación y la implementación del algoritmo memético

### 😂 Algoritmo Memético ( ͡° ͜ʖ ͡°)

### 👨‍👩‍👦👨‍👩‍👦👨‍👩‍👦 Operador de cruce utilizado en el algoritmo genético para una recombinación de al menos 3 padres

- **Operador de cruce utilizado en el algoritmo genético**: Cruce parcialmente mapeado.

- **Cruce parcialmente mapeado para múltiples padres**: Se implementa el cruce parcialmente mapeado para múltiples padres, cuyo proceso consiste en:

    1. Sea `k` el número de padres. 

    2. Se seleccionan aleatoriamente `k` puntos de corte para dividir cada padre en `k + 1` subpartes. 

    ![Ejemplo de subpartes](./img/ejemplo_subpartes.png)

      La última subparte lo vamos a tomar en cuenta como subparte 1 también.

    3. Se crea una lista de mapeo (`ch_map`) utilizando la función `create_mapping_list`. Esta lista mapea los genes (o tareas) de diferentes padres entre sí. 

        3.1. Sea `g` el número de genes o tareas de un individuo.

        3.2. Se crea un arreglo `job_mapped` para rastrear qué tareas ya han sido mapeados y un contador de tareas mapeados `cnt_mapped`

        3.3. Se selecciona aleatoriamente un padre `start_parent` y un gen inicial del padre seleccionado `start_gene`.

        3.4. Luego se ejecuta un bucle con `g` iteraciones
        
            3.4.1. En el bucle, se marca la tarea del gen actual `start_gene` como mapeado.

            3.4.2. Se selecciona un padre final `end_parent` aleatoriamente y diferente del padre inicial y se busca en el padre final el gen que corresponde a la misma tarea que el gen inicial, al encontrarlo se guarda el gen en `end_gene`.

            3.4.3. Se incrementa `cnt_mapped` a 1. Si todos las tareas han sido mapeados, se sale del bucle.

            3.4.4. Si aún faltan tareas por mapear o marcar, se elige un nuevo padre inicial diferente del final y se selecciona un nuevo gen inicial cuyo gen tarea no haya sido marcada aún.

            3.4.5. Y se agrega un mapeo con `ch_map[end_gene.job->id] = start_gene`

        3.5. Después del bucle, se agrega un mapeo final de la tarea del último gen a la tarea del gen inicial (el primer gen que se mapea). Esto para crear un mapeo cíclico entre tareas.

        ![Mapeo de genes](./img/mapeo_genes.png)

    4. Luego, se crea un orden aleatorio en los genes para cada subparte con la función `create_random_order` y este orden aleatorio se guarda en un arreglo `order`

    5. Se crean los `k` hijos.

    6. Primero cada hijo `i` se copia la primera subparte del padre `i` (antes del primer punto de corte y después del último punto de corte).

    7. Luego, para cada hijo `i`, las subpartes entre los puntos de corte se copian de otros padres en un patrón diagonal.

    8. Posteriormente, se legaliza cada hijo para asegurar que no hayan trabajos duplicados.

        8.1. Se crea un nuevo orden aleatorio `new_order` para procesar las subpartes del hijo `i`.

        8.2. Se marcan inicialmente la primera subparte procesada, o sea, la primera subparte de cada hijo permanece igual.

        8.3. Para cada subparte restante según el orden en `new_order`.

            8.3.1. Se revisa cada gen de la subparte actual según el orden en `order`.

            8.3.2. Si la tarea ya está marcado (duplicado), se reemplaza con el trabajo mapeado según la lista de mapeo (`ch_map`). Este proceso continúa hasta encontrar un trabajo no marcado.
            
            8.3.3. Al encontrar un trabajo no marcado, se marca el nuevo trabajo y se actualiza el gen en el hijo actual.
 
            8.3.4. Se marca la subparte actual procesada.

        8.4. Se verifica que todas las tareas estén marcadas (para asegurar que no hayan tareas duplicadas y tampoco tareas faltantes).

    10. Se devuelven los hijos generados.

### 🚀⬆ Mejora luego de la recombinación

Para este paso, se utiliza la búsqueda local implementada en la primera corte del proyecto.

1. Sea un hijo `h` producido en la recombinación.

2. Se decodifica el genotipo del hijo `h` en un fenotipo válido.

3. Luego, se le aplica la búsqueda local al fenotipo convertido.

4. Y por último, el resultado de la búsqueda local se transforma nuevamente en un genotipo.

### 🛠️ Parámetros del algoritmo memético

Aparte de recibir las tareas y la cantidad de máquinas, y los parámetros del algoritmo genético: 
- population_size: El tamaño de la población

- mutation_rate: El porcentaje de mutación.

- max_iter: El máximo número de iteraciones o generaciones.

El algoritmo memético recibe adicionalmente los siguientes parámetros:
- nro_parents_crossover: Un entero para la cantidad de padres para ser seleccionadas para la recombinación en cada generación.

- opt_freq: Un entero que indica la frequencia de aplicar la mejora con la búsqueda local. Responde a la pregunta de ¿cada cuántas generaciones se lanza la optimización?

- opt_rate: Un float para el porcentaje de aplicar la mejora en la búsqueda local sobre los descendientes. ¿Cuántos invididuos generados son optimizados en cada generación?

- random_opt_rate: Un valor booleano que indica si aplicar la búsqueda local con un porcentaje aleatorio en  cada generación o aplicarlo según el opt_rate. Y si es aleatorio, el valor del parámetro es true, en caso contrario es false. Además, si es aleatorio, el valor de porcentaje para la búsqueda local en cada generación debe ser mayor que 10%, esto para asegurar que siempre se aplique una cantidad significativa de optimización local.

### 👣 Pasos del algoritmo memético

Los primeros pasos del algoritmo memético son similares al algoritmo genético de la segunda corte del proyecto:

1. Se genera una población aleatoria inicial.

2. Se ordena la población por su fitness.

3. Luego, se comienza un bucle. Hasta que se terminen el máximo número de generaciones o se encuentre la solución óptima (un fitness igual a 0), se procede a: 

    3.1. Calcular el fitness de cada individuo y la suma del fitness total de la población.

    3.2. Crear una nueva población, el cual se generan nuevos individuos hasta llenar el 90% de la nueva población. El 10% restante se completa con los mejores individuos de la población anterior.

Acá el paso 3.2. cambia un poco para el algoritmo memético:

3.2.1. Se seleccionan múltiples padres usando la función `choose_multi_parents`. La selección de padres es la misma condición para el algoritmo genético.
> Si definimos f(i) como la aptitud del individuo i de la población S, entonces la probabilidad p(i) de que i sea escogido como padre viene dada por: p(i) = 1 - f(i)/sum(f(j) for j in S) 

3.2.2. Se aplica el cruce parcialmente mapeado multi-parental.

Luego del paso 3.2, se continua dentro del bucle con los siguientes pasos:

3.3. Se aplica mutación a los nuevos individuos generados 
> Mutación: Reasigna el trabajo seleccionado dentro de una máquina a otra máquina aleatoriamente escogida.

3.4. Se optimiza la generación si es que le toca optimizarse (`opt_freq`):
- Se determina cuántos individuos optimizar (basado en `random_opt_rate` y `opt_rate`).
- Se aplica búsqueda local a los individuos seleccionados.

3.5. La nueva población reemplaza a la anterior.

3.6. Se ordena la nueva población por su aptitud o fitness.

3.7. Se repite desde el paso 3.1 a 3.6 hasta que se finalice el bucle.

## Definición de la medida de distancia entre soluciones, el re-enlazado de caminos para un porcentaje dado de los pares de puntos de referencia de cada "generación", e implementación del algoritmo de búsqueda dispersa.

### Definición de distancia entre individuos

Dados los genotipos de dos individuos, `g1` y `g2`, la distancia entre los individuos representados por estos genotipos se define como el número de trabajos que están asignados a máquinas diferentes dentro de los mismos. Explicando de manera resumida:

1. Para cada par `g = <job_id, machine>` en `g1` (recordar la definición de gen en la entrega anterior)
    * Encontrar el gen `g'` en `g2` tal que `g'.job_id == job_id`
    * Si `g'.machine != g.machine`, entonces sumar 1 a la distancia total

Inicialmente se consideró hacer una versión alternativa donde si para dos individuos un trabajo estaba asignado en la misma máquina, pero el dicho trabajo estaba en *posiciones distintas* dentro de la máquina en cuestión, entonces se agregaría un valor de `0.5` adicional a la distancia. Sin embargo, se descartó la idea por temas de rendimiento.

### Construcción del Conjunto de Referencia para el algoritmo de Búsqueda Dispersa

Para el algoritmo de Búsqueda Dispersa implementado se decidió emplear conjuntos de referencia que toman en cuenta para su constitución tanto la diversidad de las soluciones como su calidad. Dado que es la idea fundamental del algoritmo, la diversidad se mantuvo como el determinante principal de los elementos que constituyen en conjunto.

En cada iteración del algoritmo el conjunto de referencia es obtenido como un subconjunto de una *piscina de soluciones*, de tamaño igual o mayor al del conjunto de referncia. Al momento de generar el conjunto inicial, la piscina está conformada por soluciones generadas aleatoriamente que son sometidas a una mejora mediante búsqueda local con el fin de aumentar la calidad general de los individuos involucrados; durante las iteraciones posteriores del algoritmo la piscina de soluciones está compuesta por la unión del conjunto de referencia mismo con el conjunto de soluciones hijas que salen resultado del proceso de cruce. Una vez tenida la piscina de soluciones el conjunto de referencia es construido de la siguiente manera:

1. Todas las soluciones en la piscina son ordenadas de acuerdo a su aptitud.
2. Se utilizan las soluciones más aptas de la piscina para conformar el 10% inicial del conjunto de referencia. Las soluciones escogidas son posteriormente eliminadas de la piscina
3. Luego hasta lograr el tamaño definido para el conjnunto de referencia
    1. Hallar, para cada elemento en la piscina, la mínima distancia existente entre él y alguno de los elementos ya presentes en el conjunto de referencia
    2. De todos los elementos de la piscina, se escoge aquel que entre las distancias mínimas calculadas, tenga la más grande.
    3. Incluir el elemento escogido en el conjunto de referencia y eliminarlo de la piscina de soluciones

La proporción del 10% de individuos basados en un criterio de calidad está *hardwired* en el código y fue escogida de manera arbitraria, sin embargo, se cree que podría ser de provecho explorar diferentes proporciones de calidad y diversidad para futuras iteraciones del algoritmo.

### Cruce e Intesificación mediante Reenlazado de Caminos

Para la formación de nuevas soluciones se utilizan los mismos mecanismos de selección y cruce empleados en el algoritmo memético. Posteriormente, se emplea un proceso de intensificación sobre el conjunto de soluciones generadas aplicando Reenlazado de Caminos sobre un porcentaje definido de los posibles pares existentes sobre el conjunto de soluciones descendientes (El porcentajede pares a explorar se pasa al algoritmo como parámetro).

Dadas dos genotipos `g1` y `g2` representando dos individuos de la población, el proceso de reenlazado de caminos funciona explorando un conjunto de soluciones intermedias que "conectan" a `g1` y `g2` a través de estructuras de vecindad. Para generar este camino eficientemente, el reenlazado de caminos es realizado efectuando intercambios y reasignaciones de máquinas en `g1` vorazmente con el fin de irlo asemejando a `g2` en cada paso del proceso. Concretamente, para dos individuos el reenlazado de caminos se ejecuta de la siguiente forma:

Para cada gen `g = <job_id, machine>` en el individuo objetivo

1. Ubicar el gen `g'` en el individuo de partida con el id de trabajo `job_id`
2. Si la posición de `g'` en el individuo de partida es distinta a la posición de `g` en el individuo objetivo, traer `g'` a la posición correcta mediante una operación de intercambio y generar una solución intermedia
3. Si la máquina a la cual está asignada el trabajo en `g'` difiere de la máquina a la que está asignado el trabajo en `g`, cambiar la máquina de `g'` a la encontada en `g` y generar una solución intermedia.

Para cada una de las soluciones intermedias generadas se mide su aptitud y se mantiene almacenada la solución más apta encontrada hasta el momento (esto puede incluir a las mismas soluciones de partida y objetivo). Si al terminar de explorar todas las soluciones intermedias se encuentra una solución mejor a las dos involucradas, una de ellas es reemplazada al azar por la nueva solución más apta. Este proceso es repetido hasta que se explora el porcentaje de pares establecido.

### Recapitulación del algoritmo de Búsqueda Dispersa

Para sintetizar, el algoritmo de Búsqueda Dispersa Implementado se resume en las siguientes operaciones:

1. Generar un conjunto de soluciones aleatorias con el tamaño de piscina definido
2. Mejorar cada una de estas soluciones con un proceso breve de búsqueda local (para esta implementación concreta, se emplearon 60 iteraciones).
3. Construir el conjunto inicial de referencias
4. Hasta que la mejor solución tenga morosidad 0 o se efectúen el máximo de iteraciones
    1. Crear la nueva generación de individuos mediante los operadores de selección y cruce
    2. Sobre los individuos generados, intentar obtener mejores y más diversas soluciones empleando reenlace de caminos sobre el porcentaje de pares definido como argumento.
    3. Actualizar el conjunto de referencias usando como piscina de soluciones la unión del conjunto actual con el conjunto de descendientes intensificado.
5. Retornar la mejor solución hallada

## Definición del comportamiento de la feromona/heurística e implemente con ello una optimización de colonia de hormigas
### 🐜 Optimización de Colonia de Hormigas

#### Feromona 

- **Estructura**: Se implementa como una matriz bidimensional donde cada elemento τ[i][j] representa el nivel de feromona para asignar la tarea i a la máquina j.

- **Inicialización**: Al comenzar, todos los valores de feromona se establecen en 1.0, indicando que inicialmente no hay preferencia por ninguna asignación.

- **Actualización local**: Después de cada asignación de tarea, se actualiza la feromona usando la fórmula:
  τ[job][machine] = (1 - ρ_local) * τ[job][machine] + ρ_local * Δτ
  Donde Δτ es 1 dividido por el retraso máximo calculado inicialmente y ρ_local es un parámetro que controla la tasa de evaporación local.

- **Actualización global**: Al final de cada iteración, se actualiza la feromona basándose en la mejor solución encontrada:
  τ[i][j] = (1 - ρ_global) * τ[i][j] + ρ_global * (1 / mejor_retraso) 
    Donde ρ_global es un parámetro que controla la tasa de evaporación global y mejor_retraso es el retraso total de la mejor solución encontrada.

- **Uso**: En la selección de tareas, la feromona se eleva a la potencia α para ajustar su influencia en la decisión.

#### Heurística


- **Cálculo**: Se define como el inverso del retraso modificado de la fecha de vencimiento (modified due date tardiness o MDD):
  η = 1 / MDD(tiempo_actual_máquina, tarea)

- **Función MDD**: Calcula el retraso potencial de una tarea si se asignara a una máquina en un momento dado. 

- **Uso**: En la selección de tareas, la heurística se eleva a la potencia β para ajustar su influencia en la decisión.

- **Equilibrio con la feromona**: La probabilidad de seleccionar una tarea para una máquina se calcula como:
  P[i][j] ∝ (τ[i][j])^α * (η[i][j])^β
  Donde τ es la feromona, η es la heurística, y α y β son parámetros que controlan la importancia relativa de la feromona y la heurística respectivamente.

Esta combinación de feromona y heurística permite al algoritmo equilibrar entre explotar la información aprendida (a través de la feromona) y responder a las características específicas de cada tarea y el estado actual de las máquinas (a través de la heurística). Esto guía al algoritmo hacia soluciones que minimizan el retraso total de las tareas, adaptándose dinámicamente a medida que construye y mejora las soluciones.

#### Pasos del Algoritmo

1. **Inicialización**

El algoritmo comienza creando la matriz de feromonas. Además, se calcula el MDD Tardiness que se usa como referencia para la actualización local de la feromona. 

2. **Ciclo principal**

El corazón del algoritmo es un ciclo que se repite un número fijo de veces. Cada repetición de este ciclo representa una generación completa de soluciones. Durante cada iteración, el algoritmo generará una solución, la mejorará, y usará la información obtenida para influir en las siguientes generaciones.

3. **Construcción de soluciones**

En esta fase, el algoritmo crea múltiples soluciones desde cero. Cada solución se construye de manera similar a cómo una hormiga construiría un camino. Se comienza con todas las tareas sin asignar y todas las máquinas vacías. Luego, para cada tarea, se selecciona primero una máquina, favoreciendo aquellas con menos tiempo de procesamiento acumulado. Después, se elige una tarea para esa máquina, basándose en los niveles de feromona (que indican qué tan buena ha sido esta asignación en el pasado) y la urgencia de la tarea. Una vez hecha la asignación, se actualiza el tiempo de procesamiento de la máquina y se modifica ligeramente el nivel de feromona para esa combinación específica de tarea y máquina.

4. **Actualización local de feromonas**

Después de asignar cada tarea a una máquina, el algoritmo actualiza los niveles de feromona localmente. Esto se hace aumentando los niveles de feromona para las combinaciones de tarea-máquina que se han asignado en esta solución.

5. **Mejora local**

Una vez construida una solución completa, el algoritmo intenta mejorarla. Esto se hace mediante un proceso de búsqueda local.

6. **Evaluación**

Después de mejorar cada solución, el algoritmo calcula su calidad midiendo el retraso total que produce. Si esta solución resulta ser mejor que la mejor encontrada hasta ahora (es decir, si produce un retraso total menor), se guarda como la nueva mejor solución. 

7. **Actualización global de feromonas**

Al final de cada iteración, después de haber construido y evaluado todas las soluciones de esa generación, el algoritmo actualiza la matriz de feromonas basándose en la mejor solución encontrada. Esto se hace aumentando los niveles de feromona para las combinaciones de tarea-máquina que aparecen en la mejor solución.

8. **Evaporación de feromonas**

Para evitar que el algoritmo se quede atrapado en soluciones subóptimas, se implementa un mecanismo de evaporación de feromonas. Esto significa que en cada iteración, todos los niveles de feromona se reducen ligeramente.

9. **Finalización**

Después de completar todas las iteraciones programadas, el algoritmo termina y devuelve la mejor solución que ha encontrado. Esta solución representa la programación de tareas que, según el algoritmo, debería producir el menor retraso total posible dado el conjunto de tareas y máquinas disponibles.

## 🚀 Uso

Para compilar y ejecutar el programa, se debe ejecutar los siguientes comandos en la terminal:

```bash
make
```

```bash
cd target
```

```bash
./PROY3 <path_to_benchmarks> <algorithm>
```

Donde `<path_to_benchmarks>` es la ruta a la carpeta que contiene los casos de prueba y `<algorithm>` es el número del algoritmo a ejecutar:

1. Algoritmo Memético
2. Búsqueda Dispersa
3. Optimización de Colonia de Hormigas

## 📄 Analisis de resultados

A continuación se presenta el análisis de los resultados obtenidos al ejecutar el programa con los casos de prueba en la carpeta `benchmarks` en una **laptop** con **procesador AMD Ryzen 5 5500U**, **disco SSD**, **8GB de memoria RAM** y **WSL 2 Ubuntu**. Se tomará en cuenta los valores de tardanza óptima obtenidos del paper de referencia de donde se obtuvo el `benchmark` para comparar los resultados obtenidos con los algoritmos implementados de ambos cortes del proyecto.


### 📊 Métricas Comparativas
Las métricas claves en el análisis incluyen:

- **Tardanza Total (Total Tardiness)**: La suma de las tardanzas de todas las tareas.
- **Diferencia con la Solución Óptima (Optimal Solution Difference)**: La diferencia entre la solución obtenida y la solución óptima.
- **Tiempo en segundos (Time in seconds)**: El tiempo en segundos tomado por el algoritmo.

Así mismo, se emplearon diferentes parámetros para cada algoritmo implementado en este tercer corte:
**Parámetros del Algoritmo Memético (MA)**:
 * population_size: Tamaño de la población.
 * mutation_rate: Probabilidad de mutación.
 * max_iter: Número máximo de iteraciones permitidas para el algoritmo.
 * nro_parent_crossover: Número de padres que estarán involucrados en el proceso de cruce.
 * opt_freq: Frecuencia con la que se aplicará búsqueda local para la mejora de individuos (medido en número de generaciones).
 * opt_rate: Porcentaje de la población que será mejorado por búsqueda local en cada proceso de intensificación.
 * random_opt_rate: Valor booleano que indica si el porcentaje de población a mejorar será aleatorio para cada proceso de intensificación.

> MA1: population_size = 500, mutation_rate = 0.05, max_iter = 20, nro_parents_crossover = 3, opt_freq = 2, opt_rate = 0.75, random_opt_rate = false

> MA2: population_size = 500, mutation_rate = 0.05, max_iter = 20, nro_parents_crossover = 10, opt_freq = 2, opt_rate = 0.75, random_opt_rate = false

> MA3: population_size = 500, mutation_rate = 0.05, max_iter = 20, nro_parents_crossover = 10, opt_freq = 1, opt_rate = 0.75, random_opt_rate = false

> MA4: population_size = 500, mutation_rate = 0.05, max_iter = 20, nro_parents_crossover = 3, opt_freq = 1, opt_rate = 1, random_opt_rate = false

> MA5: population_size = 250, mutation_rate = 0.05, max_iter = 20, nro_parents_crossover = 5, opt_freq = 2, opt_rate = 1, random_opt_rate = true

> MA6: population_size = 250, mutation_rate = 0.05, max_iter = 20, nro_parents_crossover = 5, opt_freq = 2, opt_rate = 1, random_opt_rate = false

**Parámetros de la Búsqueda Dispersa (SS)**:
 * max_pool_size: Tamaño máximo a emplear para la piscina de soluciones.
 * ref_set_size: Tamaño del conjunto de referencias.
 * parent_count: Número de padres a emplear en el proceso de cruce
 * max_iter: Número máximo de iteraciones a realizar
 * path_relinking_ptg: Porcentaje de pares a explorar para el reenlazado de caminos

> SS1: max_pool_size = 40, ref_set_size = 20, parent_count = 4, max_iter = 20, path_relinking_ptg = 0.3

> SS2: max_pool_size = 40, ref_set_size = 20, parent_count = 4, max_iter = 30, path_relinking_ptg = 0.3

> SS3: max_pool_size = 40, ref_set_size = 20, parent_count = 5, max_iter = 30, path_relinking_ptg = 0.3

> SS4: max_pool_size = 60, ref_set_size = 30, parent_count = 5, max_iter = 20, path_relinking_ptg = 0.3

> SS5: max_pool_size = 60, ref_set_size = 30, parent_count = 5, max_iter = 30, path_relinking_ptg = 0.3

> SS6: max_pool_size = 60, ref_set_size = 30, parent_count = 6, max_iter = 20, path_relinking_ptg = 0.3

**Parámetros de la Optimización de Colonia de Hormigas (ACO)**:
 * iterations: Número máximo de iteraciones a realizar
 * ants: Número de hormigas (soluciones) a generar en cada iteración.
 * alpha: Importancia del camino de feromonas en la toma de decisiones de las hormigas.
 * beta: Importancia de la información heurística en la toma de decisiones de las hormigas.
 * qm0: Probabilidad de escoger el trabajo menos tardío en vez de elegir probabilísticamente.
 * qj0: Probabilidad de seleccionar el trabajo de mayor valor en vez de elegir probabilísticamente.
 * rho_local: Taza de evaporación local de la feromona.
 * rho_global: Taza de evaporación global de la feromona.

> ACO1: iterations = 250, ants = 20, alpha = 1, beta = 3, qm0 = 0.9, qj0 = 0.9, rho_local = 0.01, rho_global = 0.01

> ACO2: iterations = 250, ants = 50, alpha = 1, beta = 3, qm0 = 0.9, qj0 = 0.9, rho_local = 0.01, rho_global = 0.01

> ACO3: iterations = 250, ants = 20, alpha = 1, beta = 3, qm0 = 0.9, qj0 = 0.9, rho_local = 0.1, rho_global = 0.1

> ACO4: iterations = 250, ants = 20, alpha = 3, beta = 1, qm0 = 0.9, qj0 = 0.9, rho_local = 0.01, rho_global = 0.01

> ACO5: iterations = 250, ants = 20, alpha = 1, beta = 3, qm0 = 0.7, qj0 = 0.7, rho_local = 0.01, rho_global = 0.01

> ACO6: iterations = 100, ants = 20, alpha = 1, beta = 3, qm0 = 0.9, qj0 = 0.9, rho_local = 0.01, rho_global = 0.01

### 📈 Resultados
Los resultados obtenidos al ejecutar el programa con los casos de prueba en la carpeta `benchmarks` se encuentran en el directorio `results`, sin embargo, debido a la cantidad de datos obtenidos, se almacenó los datos más relevantes en el siguiente enlace: [Resultados](https://docs.google.com/spreadsheets/d/1uta3jDjtNU2J74XQsV7Od8gzj1OaBIcp_lE5jGSw7Gw/edit#gid=294798782)

A modo de resumen y para facilitar la visualización de los resultados, se presentan las siguentes imágenes comparativas resumidas a continuación:

#### Promedio de diferencia entre la solución obtenida y la solución óptima cada n tareas y m máquinas
- Resultados del primer corte:
    - Solución heurística
    
    - Solución de búsqueda local partiendo de una solución heurística
    
    - Solución de búsqueda local partiendo de una solución aleatoria.

![Diff Opt Corte1](./img/DiffOptCorte1.png)

- Resultados del segundo corte:
    - Iterated Local Search (ILS), Tabu Search (TS) y Simulated Annealing (SA) usando diferentes parámetros:

    ![Diff Opt ILS &TS &SA](./img/DiffOptILS&TS&SA.png)

    - GRASP usando diferentes parámetros:

    ![Diff Opt GRASP](./img/DiffOptGRASP.png)

    - Genetic Algorithm (GA) usando diferentes parámetros:

    ![Diff Op tGA](./img/DiffOptGA.png)

- Resultados del tercer corte: 

    - Algoritmo Memético usando diferentes parámetros:

    ![Diff Opt MA](./img/DiffOptMA.png)

    - Búsqueda Dispersa usando diferentes parámetros:

    ![Diff Opt SS](./img/DiffOptSS.png)

    - Optimización de Colonia de Hormigas usando diferentes parámetros:

    ![Diff Opt ACO](./img/DiffOptACO.png)


#### Resultados ordenados por promedio de diferencias entre la solución obtenida y la solución óptima por n = 20

![Sorted Diff opt by n = 20](./img/SortedDiffn20Corte3.png)

#### Resultados ordenados por promedio de diferencias entre la solución obtenida y la solución óptima por n = 25

![Sorted Diff opt by n = 25](./img/SortedDiffn25Corte3.png)

#### Promedio de tiempo en segundos para cada n tareas y m máquinas
- Resultados del primer corte:
    - Solución heurística
    
    - Solución de búsqueda local partiendo de una solución heurística
    
    - Solución de búsqueda local partiendo de una solución aleatoria.

![Time Corte1](./img/TimeCorte1.png)

- Resultados del segundo corte:

    - Iterated Local Search (ILS), Tabu Search (TS) y Simulated Annealing (SA) usando diferentes parámetros:

    ![Time ILS & TS & SA](./img/TimeILS&TS&SA.png)

    - GRASP usando diferentes parámetros:

    ![Time GRASP](./img/TimeGRASP.png)

    - Genetic Algorithm (GA) usando diferentes parámetros:

    ![Diff Op tGA](./img/TimeGA.png)


- Resultados del tercer corte: 

    - Algoritmo Memético usando diferentes parámetros:

    ![Time MA](./img/TimeMA.png)

    - Búsqueda Dispersa usando diferentes parámetros:

    ![Time SS](./img/Time%20SS.png)

    - Optimización de Colonia de Hormigas usando diferentes parámetros:

    ![Time ACO](./img/TimeACO.png)

#### Resultados ordenados por promedio de tiempo por n = 20

![Sorted Time by n = 20](./img/SortedTimen20Corte3.png)

#### Resultados ordenados por promedio de tiempo por n = 25

![Sorted Time by n = 25](./img/SortedTimen25Corte3.png)


## 📌 Conclusiones

- **Según el promedio de diferencias entre la solución óptima y la solución obtenida**:
    - **Para n = 20** El algoritmo memético y la optimización de colonia de hormigas se posicionan como los algoritmos más exactos de las soluciones implementadas, teniendo 3 y 2 posiciones de los 5 mejores resultados respectivamente. Se evidencia una clara pérdida de exactitud en el algoritmo de búsqueda dispersa, con todas sus ejecuciones quedando en los últimos 6 lugares del ranking. La pérdida de exactitud demostrada por el algorimto de búsqueda dispersa es además bastante pronunciada, con el *mejor* desempeño de la búsqueda dispersa (15.2844) siendo un 30.4% más tardío que el *peor* resultado del algoritmo memético (11.7217) y un 66.67% más tardío que el *peor* resultado de la optimización de colonia de hormigas (9.1724)

    - **Para n = 25** La optimización de colonia de hormigas muestra una clara superioridad por encima de las otras dos metaheurísticas, teniendo la totalidad de los 5 mejores resultados obtenidos. Inmediatamente después le siguen casi todas las instancias del algoritmo memético, y finalmente la búsqueda dispersa vuelve a posicionarse como el algoritmo menos exacto. En este caso el *mejor* resultado de búsqueda dispersa (36.1084) es un 68.64% más tardío que el *peor* resultado del algoritmo memético (21.4107) y un 310.2% más tardío que el *peor* resultado de la optimización de colonia de hormigas (11.64).

- **Según el tiempo promedio de ejecución**
    - **Para n=20:** La búsqueda dispersa se posiciona como el algoritmo más rápido de las 3 metaheurísticas, con 3 de sus soluciones posicionándose en las 5 respuestas más rápidas y con todas sus soluciones entranado en el top 10. Con excepción del caso ACO6, el algoritmo memético supera en velocidad a la optimización de colonia de hormigas en todas sus respuestas, dejando a la optimización de colonia de hormigas como el algorimto más tardío en líneas generales. 
        
    - **Para n=25:** La búsqueda dispersa se posiciona nuevamente como el algoritmo más rápido de los 3, teniendo en esta ocación 4 de las 5 soluciones más rápidas registradas. En esta ocasión las diferencias de velocidad entre el algorimto memético y la optimización de colonia de hormigas son mucho más difusas, siendo complicado establecer con certeza el predominio de una metaheurística sobre la otra. 

## 📚 Referencias

1. [C. Ting, C. Su and C. Lee. Multi-parent extension of partially mapped crossover for combinatorial optimization problems. Expert Systems with Applications 37, p. 1879–1886, 2010.](https://mx.nthu.edu.tw/~ckting/pubs/eswa2010.pdf)
2. [Martí, R. Laguna, M. Glover F. Principles of Scatter Search. Dpto. de Estadística e Investigación Operativa, Facultad de Matemáticas, Universidad de Valencia, 2003](https://www.uv.es/rmarti/paper/docs/ss8.pdf)
