// Este archivo es a modo de borrador para trabajar mini funciones
// de impresión de mensajes sin modificar lo que está trabajando el resto.
#include <stdio.h>
#include "utiles_Proyecto.h"
#define CANT 9

void onda(int);
void enDesarrollo();
void adios();
void bienvenida();

int main(){
	utf8();
	bienvenida();
	enDesarrollo();
	adios();
	return 0;
}

void onda(int x)
{
    int i;
    for (i = 0; i < x; i++)
        printf(".·´¯·..");
    printf("\n");
}

void enDesarrollo()
{
	printf("\n1010101010101010101010101010101010101010101010101010");
	printf("\n        ESTA OPCIÓN SIGUE EN DESARROLLO");
    printf("\n1010101010101010101010101010101010101010101010101010");	
}

void adios()
{
	printf("\n.·´¯·...·´¯·...·´¯·...·´¯·...·´¯·...·´¯·..");
	printf("\n G R A C I A S  por haber usado  BALANCIN");
	printf("\n             Hasta lueguito! <3        ");
	printf("\n.·´¯·...·´¯·...·´¯·...·´¯·...·´¯·...·´¯·..");
}

void bienvenida()
{
	onda(CANT);
	printf( ".·´¯·...·´¯·.. B  I  E  N  V  E  N  I  D  O / A ..·´¯·...·´¯·..\n");
	onda(CANT);
	printf("Al futuro de tu negocio - BALANCIN - Tus cuentas, equilibradas\n");
	onda(CANT);
}