#include <stdio.h>
#include <string.h>

#include "utiles_Proyecto.h"
#include "FactuFunciones.h" // esta tiene incluido "Estructuras.h" y define CANCELAR

int main()
{
    utf8();

    Usuario usuarios[MAX_USUARIOS];
    int cantidadUsuarios = 0; // el admin es fijo (CLAVE_ADMIN), no se guarda en el arreglo
    int permi;
    //FILE *archivoLogs;
    //FILE *archivoProductos;
    //FILE *archivoClientes;
    //FILE *archivoFacturas;
    FILE *archivoUsers = NULL;

    // Ciclo general
    do
    {
        pantallaCarga();
        limpiarPantalla();
        printf("***********************\n");
        printf("        BALANCIN        \n");
        printf("***********************\n");
        printf("\n");

        // el nivel de permisos es asignado por el tipo de sesión
        permi = iniciarSesion();

        switch (permi) // Según el nivel de permiso es el menú al que va
        {
        case PERMI_ADMIN:
            if (LogeoAdmin(usuarios, &cantidadUsuarios, archivoUsers) == CANCELAR)
            {
                permi = CANCELAR; // se agotaron los intentos: cierra el programa
            }
            break;

        case PERMI_GESTOR:
            if (LogeoGestor(usuarios, cantidadUsuarios))
            {
                limpiarPantalla();
                printf("----Bienvenide a Balancin----\n");
                MenuUsuario();
            }
            break;

        case CANCELAR:
            break;

        default:
            printf("ERROR DE CUENTA\n");
            break;
        }
    } while (permi != CANCELAR);

    printf("\n¡Hasta luego!\n");

    return 0;
}
