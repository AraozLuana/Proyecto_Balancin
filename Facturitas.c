#include <stdio.h>
#include "utiles_Proyecto.h"
#include "FactuFunciones.h" // esta tiene incluido "Estructuras.h" y define CANCELAR

int main()
{
    utf8();
    int permi;
    FILE *archivoUsuarios;
    if ((archivoUsuarios = fopen("bd_Users.dat", "rb+")) == NULL)
    {
        archivoUsuarios = fopen("bd_Users.dat", "wb+");
    }

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
            if (LogeoAdmin() == CANCELAR)
            {
                permi = CANCELAR; // se agotaron los intentos: cierra el programa
            }
            break;

        case PERMI_GESTOR:
            if (LogeoGestor(archivoUsuarios))
            {
                limpiarPantalla();
                printf("----Bienvenide a Balancin----\n");
                MenuUsuario(); // Llamada al menú de usuario con productos y detalles vacíos
                //(Hay que agregar los archivos de los productos y facturas para que funcionen las stas)
            }
            break;

        case CANCELAR:
            break;

        default:
            printf("Error: Permiso inválido. Saliendo del programa...\n");
            retardo2(0.2);
            permi = CANCELAR; // cierra el programa
            break;
        }
    } while (permi != CANCELAR);

    printf("\n¡Hasta luego!\n");

    fclose(archivoUsuarios);
    return 0;
}
