#include <stdio.h>

#include "libreria.h"

#pragma warning(disable : 4996)

int main() {
	
	char ruta[100];
	dns_mas_rapida dnsMasRapida;
	inicializar_struct_dns_mas_rapida(&dnsMasRapida);

	// Preguntar inicialmente el nombre y la ruta del archivo dnsips.txt.
	printf("Introduzca ruta de archivo: ");
	scanf("%s", ruta);

	// Mostrar el contenido de este archivo por pantalla.
	printf("Contenido del archivo %s:\n", ruta);
	dnsMasRapida = leerArchivoDNS(ruta, dnsMasRapida);

	//Mostrar todos los adaptadores de red en este equipo.
	printf("\n\nTodas las interfaces disponibles en este equipo:\n");
	system("netsh interface show interface");

	//Preguntar al usuario el adaptador de red que quiere modificar.
	printf("Introduzca el nombre del adaptador (ej: Ethernet): ");
	scanf("%s", dnsMasRapida.adaptador);

	//Mostrar los servidores dns configurados en este.
	printf("Mostrando la DNS configurada en este equipo:\n");
	filtrarDireccionDNS(dnsMasRapida.adaptador);

	//Leer el archivo proporcionado inicialmente, y chequear si esas ips son accesibles 
	//y en caso de que lo sean es necesario escribirlas en un archivo temporal.
	printf("Guardando todas las IPs accesibles en una temporal...\n");
	FILE* tmp = comprobarDireccion(dnsMasRapida);

	//Abrir el archivo temporal creado en el punto anterior 
	//y chequear de entre los dns configurados cual es más rápido.
	printf("Buscando la direccion DNS mas rapida...\n");
    dnsMasRapida = obtenerDnsMasRapida(tmp, dnsMasRapida);
	printf("--------------------------------\n");
	printf("La direccion mas rapida es: %s\n", dnsMasRapida.mejorDNS);
	printf("--------------------------------\n");
	
	// Eliminar ficheros temporales.
	printf("Eliminado ficheros temporales...\n");
	limpiarTemporales();
	
	//Cambiar la direccíon en caso de no tener configurado el dns más rápido.
	printf("Procediendo a modificar el servidor DNS del adaptador...\n");
	actualizarAdaptador(dnsMasRapida.adaptador, dnsMasRapida.mejorDNS);
	
	// Pausar programa
	printf("Pulse [ENTER] para continuar...\n");
	system("pause");
}