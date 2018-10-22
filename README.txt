# Recuperación de Información Multimedia - Tarea 1 

## Requisitos

La tarea fue resuelta usando C++11 y OpenCV 3.4.3, y para compilarla se necesita CMake versión 3.12.
Esto es lo único que se necesita, pero la tarea fue resuelta y probada en un computador con Ubuntu 18 y este texto son instrucciones para
ejecutarla en GNU/Linux, por lo que se recomienda usar alguna distro de GNU/Linux.


## Cómo Compilar

Estando en una terminal en el directorio donde se encuentran los archivos entregados ejecutar los siguientes comandos:

$ cmake .
$ cmake --build .

Lo cual genera 3 ejecutables: extraccion, busqueda y deteccion.


## Cómo Ejecutar

Cada uno de los ejecutables generados corresponde a una de las 3 etapas de la tarea mencionadas en el enunciado.


### Extraccion de características

El programa recibe 2 parámetros: el video sobre el cual se van a buscar comerciales y el directorio que contiene los videos de los comerciales.

Por ejemplo, si se tiene un video llamado mega-2014_04_10.mp4 en un directorio llamado television dentro del directorio de trabajo y los 
comerciales en un directorio llamado comerciales se puede ejecutar con el siguiente comando:

$ ./extraccion television/mega-2014_04_11.mp4 comerciales

Este proceso tarda unos 2 o 3 minutos, dependiendo de la velocidad del computador.

Al finalizar se habrán generado varios archivos que representan los descriptores de los videos. Cada archivo se llama exactamente igual que el
video del cual proviene, pero con extensión .txt, y queda guardado en el mismo directorio que el video correspondiente.


### Búsqueda por similitud

El programa recibe 2 parámetros: el archivo de texto que representa al video y el directorio que contiene los videos de los comerciales.
Por la manera en que guarda los datos el programa de extraccion, la forma de ejecutar busqueda es con casi exactamente los mismos parámetros,
pero se cambia la extensión del video por .txt.

Continuando con el ejemplo anterior:

$ ./busqueda television/mega-2014_04_11.txt comerciales

Este proceso tarda unos 4 o 5 minutos, dependiendo de la velocidad del computador.

Al finalizar se habrá generado un archivo de texto en el directorio del video que se llama igual que el éste, pero al final se le agrega 
"_distancias". Con el ejemplo anterior el archivo generado estaría en el directorio television y su nombre sería mega-2014_04_11_distancias.txt.

Este archivo contiene los vecinos mas cercanos de cada frame del video, indicando el nombre del comercial, el numero de frame del comercial y 
la distancia.


### Detección de apariciones

El programa recibe como parámetro el archivo generado en el paso anterior.

Continuando con el ejemplo:

$ ./deteccion television/mega-2014_04_11_distancias.txt

Este proceso tarda solo un par de segundos, y al finalizar se habrá generado en el directorio de trabajo un archivo "detecciones.txt", el cual
contiene el reporte final de detecciones de comerciales para el video.

Así, se puede evaluar el resultado con el comando:

$ python evaluar.py detecciones.txt
