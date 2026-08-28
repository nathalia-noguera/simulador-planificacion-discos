Simulador de Algoritmos de Planificación en Discos

Proyecto en C++ que simula el movimiento del cabezal de un disco duro y compara el rendimiento de distintos algoritmos de planificación de solicitudes de lectura/escritura.

( 1 ) Algoritmos implementados

- FCFS (First-Come, First-Served): atiende las solicitudes en el orden en que llegaron.
- SSTF (Shortest Seek Time First): atiende siempre la solicitud más cercana al cabezal.
- SCAN (algoritmo del ascensor): el cabezal se mueve en una dirección atendiendo todo lo que encuentra y, al llegar al extremo del disco, invierte el sentido.

Para cada algoritmo se calcula el movimiento total del cabezal y el tiempo/latencia promedio, para poder compararlos entre sí.

( 2 ) Estructura del proyecto

simulador-planificacion-discos/
├── src/        # Código fuente (.cpp).
├── include/    # Archivos de cabecera (.hpp).
├── data/       # Archivos de solicitudes de prueba.
├── results/    # Resultados generados por el programa.
├── Makefile
└── README.md

( 3 ) Requisitos

- g++ (compilador de C++).
- make

( 4 ) Cómo compilar

make

Esto genera el archivo simulador.exe.

( 5 ) Cómo ejecutar

./simulador.exe

El programa muestra un menú para:
- Cargar solicitudes desde un archivo, generarlas al azar o escribirlas a mano.
- Configurar los datos del disco (cilindros, posición inicial del cabezal, etc.).
- Ejecutar cada algoritmo por separado o los tres juntos para compararlos.
- Guardar los resultados en la carpeta results/.

( 6 ) Cómo limpiar los archivos compilados

make clean