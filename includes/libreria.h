#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "extra.h";

#pragma warning(disable : 4996)

#define SIZE 150
#define DNS_LIST 4 // Cantidad de direcciones que contiene el fichero DNSips.txt

typedef struct {
	char direcionesDNS[DNS_LIST][SIZE];
	char adaptador[SIZE];
	char mejorDNS[SIZE];
} dns_mas_rapida;

void inicializar_struct_dns_mas_rapida(dns_mas_rapida* dnsMasRapida);
dns_mas_rapida leerArchivoDNS(char ruta[SIZE], dns_mas_rapida dnsMasRapida);
void filtrarDireccionDNS(char adaptador[SIZE]);
void escribirEnTemporal(char text[SIZE], FILE* archivoTemporal);
dns_mas_rapida obtenerDnsMasRapida(FILE* archivoTemporal, dns_mas_rapida dnsMasRapida);
int actualizarAdaptador(char adaptador[SIZE], char dns[SIZE]);
FILE* comprobarDireccion(dns_mas_rapida dnsMasRapida);
void limpiarTemporales();