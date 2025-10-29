#  Sistema de Gestión de Sanatorios

##  Introducción

Este proyecto tiene como objetivo desarrollar un **Sistema de Gestión de Sanatorios**, pensado para optimizar la administración de pacientes, profesionales, turnos y recursos médicos dentro de una institución sanitaria.

El sistema busca ofrecer una solución integral que facilite la organización interna, reduzca errores administrativos y mejore la comunicación entre las distintas áreas del sanatorio.

##  Propósito de este archivo

En este archivo se detalla **todo lo necesario para el correcto funcionamiento del sistema**, incluyendo:

- Los **requisitos de instalación** y dependencias necesarias.
- Las **instrucciones de configuración** del entorno.
- Los **pasos para ejecutar** el sistema correctamente.


Este documento es esencial para desarrolladores o administradores que deseen instalar, mantener o desplegar el sistema de manera adecuada.

##  Pasos de instalación

###  Paso 1: Instalar el entorno — MSYS2 (x86_64)

El primer paso para preparar el entorno de desarrollo es instalar **MSYS2**, una plataforma que proporciona un entorno similar a Linux en Windows y facilita la instalación de herramientas como GCC, CMake, Git, entre otras.

####  Instrucciones:

1. Descargar el instalador oficial desde el sitio:  
    [https://www.msys2.org](https://www.msys2.org)

2. Ejecutar el instalador y seguir los pasos recomendados por defecto.  
   (Generalmente se instala en `C:\msys64`).

3. Una vez instalado, abrir la terminal **MSYS2 UCRT64** y actualizar los paquetes con los siguientes comandos:

   ```bash
   pacman -Syu

---

### Paso 2: Instalar dependencias necesarias

Una vez instalado y actualizado MSYS2, se deben instalar las dependencias necesarias para compilar y ejecutar el sistema. (Siempre que te pida reiniciar la terminal hacelo)

####  Instalación de compiladores y herramientas base
Observacion verifique que tiene un interprete de Python instalado en su maquina. 

Ejecutar el siguiente comando en la terminal **MSYS2 UCRT64**:

   ```bash
   pacman -S --needed \
   mingw-w64-ucrt-x86_64-gcc \
   mingw-w64-ucrt-x86_64-cmake \
   mingw-w64-ucrt-x86_64-make \
   mingw-w64-ucrt-x86_64-ncurses \
   mingw-w64-ucrt-x86_64-gdb
   ```

Ejecutar el siguiente comando en la terminal MSYS2 UCRT64:

   ```bash
   pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-make
   pacman -S mingw-w64-ucrt-x86_64-curl
   mingw-w64-ucrt-x86_64-nlohmann-json
   pacman -S ca-certificates
   ```
    
Añadir el comando pip al entorno de la terminal UCRT64 para poder instalar librerías de Python.


    pacman -S mingw-w64-ucrt-x86_64-python-pip


Instalar la librería específica que permite a nuestro script de Python leer las credenciales de forma segura desde el archivo .env.

   ```bash
   pip install python-dotenv
   ```
---
###  Paso 3: Configurar Variables de entorno
En el archivo .env va a encontrar las variables de entorno del sistema. Tiene que configurarlas con sus datos(CLAVE=VALOR).

**Gmail para el envio de recordartorios**

Para la contraseña del mail  tiene que usar la contraseña de aplicacion, para obtenerla tiene que ir a configuraciones de google y buscar contraseña de aplicacion y le va a dar 
la opcion de crearla.

**Para la API_KEY**

Debe crear su cuenta gratuita en:
 https://positionstack.com/
Copiar la API key (por ejemplo: afc545760e5a981467674e483fa40a78) y la guarda en el archivo.


   ```bash
   SENDER_EMAIL=tuEmail@gmail.com
   SENDER_PASSWORD=contraseña de aplicacion
   API_KEY=afc545760e5a981467674e483fa40a78
   ```
---
###  Paso 4: Compilar y ejecutar
Una vez tengan instalados todas las dependencias necesarias. Para correr el programa deben primero copiar el path donde descargaron 
el proyecto (ejemplo: /c/Users/tomyL/SistemaSanatorio)

Abrir la terminal UCRT64 y entrar al proyecto ejecutando el siguiente comando.

   ```bash
   cd "(pega aqui el path)"
   ```
Una vez dentro del proyecto compilamos.

   ```bash
   rm -rf build
   cmake -S . -B build -G "MinGW Makefiles" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCURSES_NEED_WIDE=TRUE -DCURSES_NEED_NCURSES=TRUE
   cmake --build build -j
   ```
Y corremos el archvio ejecutable
   ```bash
   ./build/SistemaSanatorio.exe
   ```

---
## Posibles errores y formas de solucionarlos
### SOLUCIÓN SSL (si da error “Problem with the SSL CA cert”)
Si  aparece ese mensaje ejecutar el siguiente comando. 

   ```bash
   pacman -S ca-certificates
   update-ca-trust
   ```

### No such file or directory

menu.h: No such file or directory → compilar con CMake provisto (ya incluye ui/) o incluir como #include "ui/menu.h"


