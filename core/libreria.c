#include "libreria.h"

void inicializar_struct_dns_mas_rapida(dns_mas_rapida* dnsMasRapida) {

    // Inicializamos el struct con el valor 0
    memset(dnsMasRapida, 0, sizeof(dns_mas_rapida));
}

dns_mas_rapida leerArchivoDNS(char ruta[SIZE], dns_mas_rapida dnsMasRapida) {

    // Creamos e inicializamos las variables
    FILE* fp; // Variable para manejar ficheros
    char linea[SIZE] = ""; // Variable donde se guardara cada linea leida

    // Comprobamos si podemos acceder al fichero
    // Abrimos el fichero con fopen y pasamos como parametros: ruta, modo
    // Ruta = ruta donde se encuentra el fichero a abrir
    // Modo = que queremos hacer con ese fichero, "r" es igual a modo lectura
    if ((fp = fopen(ruta, "r")) != NULL) {

        // Recorremos el fichero hasta llegar a su final
        for (int i = 0; feof(fp) != 1; i++) {
            // Leemos una linea del fichero y la guardamos en una variable
            fgets(linea, SIZE, fp);

            // Mostramos el resultado al usuario por consola
            printf(linea);

            // Intorducir el valor dentro del array
            strcat(dnsMasRapida.direcionesDNS[i], linea);
        }

        // Retornamos el struct con los nuevos valores
        return dnsMasRapida;

        // Cerramos el archivo abierto
        fclose(fp);
    }
    else {

        // Pausar programa
        system("pause");

        // En caso de no poder abrir el archivo, salimos de la aplicacion
        // mostrando un mensaje de error
        printf("No se pudo abrir el archivo con la lista de servidores DNS.\n");
        exit(1);
    }

}

void escribirEnTemporal(char text[SIZE], FILE* archivoTemporal) {

    // Declarar e inicializar las variables
    int length = strlen(text); // Variable que contiene la longitud de caracteres del texto recibido
    int counter = -1; // Contador

    // Comprobar si se ha creado el fichero temporal
    if (archivoTemporal != NULL) {

        // Mientras la logitud del texto a guardar sea menor al contador
        // guardar caracter en el fichero temporal
        while (++counter < length) {

            // Guardar caracter por caracter en el fichero temporal
            fputc(text[counter], archivoTemporal);
        }

        // Retornar archivo temporal
        return archivoTemporal;
    }
    else {

        // Pausar programa
        system("pause");

        // En caso de no poder abrir el archivo, salimos de la aplicacion
        // mostrando un mensaje de error
        printf("No se pudo abrir el archivo!");
        exit(1);
    }

};

void filtrarDireccionDNS(char adaptador[SIZE]) {

    // Creamos e inicializamos las variables
    FILE* fp; // Variable para acceder al fichero
    char comando[SIZE] = "netsh interface ipv4 show dnsservers "; // Variable que tendra el comando a ejecutar
    char linea[SIZE] = ""; // Variable donde se guardara cada linea leida
    char nombrefichero[SIZE] = "infodns.txt"; // Variable donde guardar el nombre del fichero a cual exportar

    // Completamos el comando con las variables
    strcat(comando, adaptador); // Concatenamos el comando con el nombre del adaptador
    strcat(comando, " > "); // Concatenamos con el icono de exportacion
    strcat(comando, nombrefichero); // Concatenamos con el nombre del fichero a cual exportar

    // Ejecutar el comando batch
    if (system(comando) == 0) {

        // Abrir y leer el archivo temporal creado anteriormente
        if ((fp = fopen(nombrefichero, "r")) != NULL) {

            // Recorremos el fichero hasta llegar a su final
            while (feof(fp) != 1) {

                // Leemos una linea del fichero y la guardamos en una variable
                fgets(linea, SIZE, fp);

                // Si la linea actual contiene "Servidores DNS", imprimimos por pantalla dicha linea
                if (strstr(linea, "Servidores DNS") != 0) {

                    // Imprimimos el servidor DNS
                    printf("%s\n", removeLeadingSpaces(linea));
                }
            }

            // Cerramos el archivo abierto
            fclose(fp);
            // Eliminamos el archivo sobrante
            remove("infodns.txt");
        }
        else {

            // Pausar programa
            system("pause");

            // En caso de no poder abrir el archivo, salimos de la aplicacion mostrando un mensaje de error
            printf("No se pudo abrir el archivo infodns.\n");
            exit(1);
        }
    }
    else {

        // Pausar programa
        system("pause");

        // En caso de obtener error al ejecutar la aplicacion, salimos mostrando un mensaje de error
        printf("No se pudo ejecutar el comando para mostrar los adaptadores DNS.\n");
        exit(1);
    }
}

FILE* comprobarDireccion(dns_mas_rapida dnsMasRapida) {

    // Creamos e inicializamos las variables
    FILE* tmp = tmpfile(); // Variable que iniciara un archivo temporal
    char linea[SIZE] = ""; // Variable donde se guardara cada linea leida

    // Recorremos cada ip almacenada dentro del struct
    for (int i = 0; i < DNS_LIST; i++) {

        // Variable que tendra el comando a ejecutar
        char comando[SIZE] = "ping > nul " ;

        // Concatenamos el comando con con la direccion
        strcat(comando, dnsMasRapida.direcionesDNS[i], "\n");
        // Ejecutamos el comando batch
        if (system(comando) == 0) {

            // Imprimir por pantalla un mensaje de exito junto a la ip
            printf("%s -> Ha respondido.\n", dnsMasRapida.direcionesDNS[i]);
            // Guardamos las IPs disponibles en un archivo temporal
            escribirEnTemporal(dnsMasRapida.direcionesDNS[i], tmp);
        }
        else {

            // En caso contrario, mostramos un mensaje de error
            printf("%s -> K.O.\n", dnsMasRapida.direcionesDNS[i]);
        }
    }

    return tmp;
}

dns_mas_rapida obtenerDnsMasRapida(FILE* archivoTemporal, dns_mas_rapida dnsMasRapida) {

    // Creamos e inicializamos las variables
    int mejorTiempo = INT_MAX; // Variable de referencia que guarda los mejores tiempo, INT_MAX es el limite maximo
    char* mejorDNS = ""; // Varible de referencia que guardara la mejor ip (dependiendo de la variable "mejorTiempo")
    int tiempo = 0; // Variable que contiene el tiempo devuelto por el comando ping
    char linea[SIZE] = ""; // Variable que contiene la linea del archivo

    // Movemos el puntero al inicio del archivo temporal
    rewind(archivoTemporal);

    // Recorremos el fichero hasta llegar a su final
    for (int i = 0; feof(archivoTemporal) != 1; i++) {
        // Leemos una linea del fichero y la guardamos en una variable
        fgets(linea, SIZE, archivoTemporal);
        // Eliminamos los saltos de linea que estan al final de la linea
        strtok(linea, "\n");
        // Ejecutamos la funcion de filtrarVelocidad para obtener el tiempo de respuesta de la IP
        tiempo = filtrarVelocidad(linea);

        // Comprobamos si el tiempo devuelto es menor que el anterior
        if (tiempo < mejorTiempo) {

            // En caso de que la IP sea menor actualizamos las variables
            mejorTiempo = tiempo; // Actualizamos la variable tiempo (tiempo en ms)
            mejorDNS = linea; // Actualizamos la variable de la IP
        }
    }

    // Guardamos el valor de la mejor IP en el struct.
    strcat(dnsMasRapida.mejorDNS, mejorDNS);
    // Devolvemos el struct actualizado
    return dnsMasRapida;
}

int filtrarVelocidad(char ip[SIZE]) {

    // Creamos e inicializamos las variables
    FILE* fp; // Variable de tipo FILE
    char* tiempo = ""; // Variable que contiene el tiempo devuelto por el comando ping
    char linea[SIZE] = ""; // Variable que contiene la linea del archivo
    char comando[SIZE] = "ping "; // Variable que tendra el comando a ejecutar

    // Completamos el comando a ejecutar
    strcat(comando, ip); // Concatenamos el comando con el servidor DNS
    strcat(comando, " > ping.txt"); // Concatenamos el comando con la exportacion del resultado a un fichero

    // Comprobamos que el comando se haya ejecutado correctamente
    if (system(comando) == 0) {

        // Comprobamos si podemos acceder al fichero
        // Abrimos el fichero con fopen y pasamos como parametros: ruta, modo
        // Ruta = ruta donde se encuentra el fichero a abrir
        // Modo = que queremos hacer con ese fichero, "r" es igual a modo lectura
        if ((fp = fopen("ping.txt", "r")) != NULL) {

            // Recorremos el fichero hasta llegar a su final
            while (feof(fp) != 1) {

                // Leemos una linea del fichero y la guardamos en una variable
                fgets(linea, SIZE, fp);
                // Comprobamos que la linea actual contenga "Media = "
                if (strstr(linea, "Media = ") != NULL) {

                    // Filtramos el resultado de la linea
                    tiempo = strstr(linea, "Media = "); // Devuelve la parte de "Media"
                    tiempo = strtok(tiempo, "ms"); // Utilizando el resultado anterior, eliminamos el texto "ms"
                    tiempo = strtok(tiempo, "Media = "); // Utilizanod el resultado anterior, eliminamos el texto "Media = "

                    // Convertimos el texto a integro
                    // Devolvemos el valor de tipo integro
                    return atoi(tiempo);
                }
            }

            // Cerramos el archivo abierto
            fclose(fp);
            // Eliminamos el archivo sobrante
            //remove("ping.txt");
        }
        else {

            // Pausar programa
            system("pause");

            // En caso de no poder abrir el archivo, salimos de la aplicacion mostrando un mensaje de error
            printf("No se pudo abrir el archivo con la lista de servidores DNS.\n");
            exit(1);
        }

        // Cerramos el archivo abierto
        fclose(fp);
    }
}

int actualizarAdaptador(char adaptador[SIZE], char dns[SIZE]) {

    // Creamos e inicializamos las variables
    char comando[SIZE] = "netsh interface ipv4 add dnsserver name="; // Variable que tendra el comando a ejecutar

    // Completamos el comando a ejecutar
    strcat(comando, adaptador); // Concatenamos el comando con el nombre del adaptador
    strcat(comando, " address= "); // Concatenamos el comando con el parametro de la direccion
    strcat(comando, dns); // Concatenamos el comando con la direccion DNS

    // Comprobamos que el comando se haya ejecutado correctamente
    if (system(comando) == 0) {

        // Mostrar mensaje de exito
        printf("Servidor DNS configurado correctamente\n");
        return 0;
    }
    else {

        // Pausar programa
        system("pause");

        // En caso de obtener error al ejecutar la aplicacion, salimos mostrando un mensaje de error
        printf("1. Compruebe que su aplicacion se haya ejecutado con permisos de administrador.\n");
        printf("2. Es posible que ya este asignada esta direccion en su adaptador.\n");
        exit(1);
    }
}


void limpiarTemporales() {

    // Creamos e inicializamos las variables
    char ficherosParaEliminar[2][SIZE] = { "infodns.txt", "ping.txt" }; // Array con nombres de ficheros a eliminar

    // Recorrer el arreglo con los ficheros a eliminar
    for (int i = 0; i < 3; i++) {
        // Eliminar archivo temporal y comprobar si se ha ejecutado correctamente
        remove(ficherosParaEliminar[i]);
    }
}
