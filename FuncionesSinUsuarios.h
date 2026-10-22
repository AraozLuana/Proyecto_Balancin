#ifndef FACTUFUNCIONES_H_
#define FACTUFUNCIONES_H_
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "Estructuras.h"
#include "utiles_Proyecto.h"

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <unistd.h>
#endif

// Resultados de registrarUsuario
#define EXITO 1
#define USUARIO_EXISTE 0
#define SIN_ESPACIO (-1)
#define CANCELAR (-2)

// *********************************************************************************
// PROTOTIPOS
// *********************************************************************************
int buscarCliente(Usuario usuarios[], int cantidadUsuarios, char nombreBuscado[]);
int iniciarSesion();
void MenuUsuario();
void pantallaCarga();

int buscarCliente(Usuario usuarios[], int cantidadUsuarios, char nombreBuscado[])
{
    int i = 0;
    int posicionEncontrada = (-1);

    while (i < cantidadUsuarios && posicionEncontrada == (-1))
    {
        if (strcmp(usuarios[i].nombreUsuario, nombreBuscado) == 0)
        {
            posicionEncontrada = i;
        }
        i++;
    }

    return posicionEncontrada;
}

/* // *********************************************************************************
// Selección de rol. Devuelve PERMI_ADMIN, PERMI_GESTOR o CANCELAR (salir)
// *********************************************************************************
int iniciarSesion()
{
    int op = 0;

    do
    {
        printf("Seleccione su Usuario:\n");
        printf("1- Administrador\n");
        printf("2- Gestor\n");
        printf("3- Salir\n");
        leerEntero("Opción: ", &op);

        if (op < 1 || op > 3)
        {
            limpiarPantalla();
            printf("Opción no válida. Inténtelo nuevamente.\n\n");
        }
    } while (op < 1 || op > 3);

    if (op == 1)
    {
        return PERMI_ADMIN;
    }
    if (op == 2)
    {
        return PERMI_GESTOR;
    }
    return CANCELAR;
}*/

// *********************************************************************************
// MENÚ GESTOR
// *********************************************************************************
void MenuUsuario()
{
    int op = 0;
    printf("----Bienvenide Gestor----\n");
    do
    {
        printf("\n");
        printf("1-Registrar Factura/s\n");
        printf("2-Eliminar Factura/s\n");
        printf("3-Buscar Factura/s\n");
        printf("4-Cantidad de clientes por zona\n");
        printf("5-Ver producto estrella\n");
        printf("6-Ver cliente estrella\n");
        printf("7-Volver\n");
        leerEntero("Seleccione una opción: ", &op);
        switch (op)
        {
        case 1:
            printf("\nREGISTRO DE FACTURAS A DESARROLLAR\n");
            pausa();
            break;
        case 2:
            printf("\nELIMINACIÓN DE FACTURAS A DESARROLLAR\n");
            pausa();
            break;
        case 3:
            printf("\nBUSQUEDA DE FACTURAS A DESARROLLAR\n");
            pausa();
            break;
        case 4:
            printf("\nCANTIDAD DE CLIENTES POR ZONA A DESARROLLAR\n");
            pausa();
            break;
        case 5:
            printf("\nVER PRODUCTO ESTRELLA A DESARROLLAR\n");
            pausa();
            break;
        case 6:
            printf("\nVER CLIENTE ESTRELLA A DESARROLLAR\n");
            pausa();
            break;
        case 7:
            printf("\nVolviendo...\n");
            break;
        default:
            printf("Error: Fuera de rango. Reintente.\n");
            break;
        }
    } while (op != 7);
}

void mostrar_pose1() 
{
    printf("      /\\_/\\\n");
    printf("     ( o.o )\n");
    printf("      > ^ <\n");
    printf("     /     \\\n");
    printf("    |       |\n");
    printf("    |  | |  |\n");
    printf("    |__|_|__|\n");
    printf("     ||   ||\n");
    printf("     ||   ||\n");
    printf("     ()   ()\n");
}

// Pose 2: Brazos arriba (saludando o moviéndose)
void mostrar_pose2() //FIGURA PRUEBA
{
    printf("     _\\_/_ \n");
    printf("    ( o.o )\n");
    printf("      > ^ <\n");
    printf("     /     \\\n");
    printf("    |       |\n");
    printf("    |  | |  |\n");
    printf("    |__|_|__|\n");
    printf("     ||   ||\n");
    printf("     ||   ||\n");
    printf("     ()   ()\n");
}
void pantallaCarga()
{
    int i = 0;

    for (i = 0; i < 3; i++)
    {
        limpiarPantalla();
        mostrar_pose1();
        retardo2(0.2);
        limpiarPantalla();
        mostrar_pose2();
        retardo2(0.2);
    }
}

#endif /* FACTUFUNCIONES_H_ */