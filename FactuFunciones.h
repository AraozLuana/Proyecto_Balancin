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

/* Constantes para evitar magic numbers en cases y funciones varias */
#define op1 1
#define op2 2
#define op3 3
#define op4 4
#define op5 5
#define op6 6
#define op7 7
#define ERROR (-1)

#define CLAVE_ADMIN "admin123"
#define MAXIMO_INTENTOS 4
#define CANCELAR (-2)
#define SALIR 0
#define CANT_ITEMS_MENU 4
#define FILA_MENU 3

// Resultados de validarLogin
#define CONTRA_INCORRECTA 0
#define USUARIO_NO_ENCONTRADO (-1)

// Resultados de registrarUsuario
#define EXITO 1
#define USUARIO_EXISTE 0
#define SIN_ESPACIO (-1)

// *********************************************************************************
// PROTOTIPOS
// *********************************************************************************
int buscarUsuario(Usuario usuarios[], int cantidadUsuarios, char nombreBuscado[]);
int validarLogin(Usuario usuarios[], int cantidadUsuarios, char nombreIngresado[], char claveIngresada[]);
int registrarUsuario(Usuario usuarios[], int *cantidadUsuarios, char nombreNuevo[], char claveNueva[], int permiNuevo);
int iniciarSesion();
int LogeoAdmin(Usuario usuarios[], int *cantidadUsuarios);
int seleccionaItem(char VecMenu[][50]);
int buscarCliente(int codigoCliente);
int codigoEstrella();
bool LogeoGestor(Usuario usuarios[], int cantidadUsuarios);
void MenuUsuario();
void MenuAdmin(Usuario usuarios[], int *cantidadUsuarios);
void AltaGestor(Usuario usuarios[], int *cantidadUsuarios);
void BajaGestor(Usuario usuarios[], int *cantidadUsuarios);
void mostrar_pose1();
void mostrar_pose2();
void registrarFactura();
void pantallaCarga();
void mostrarMenu(char VecMenu[][50]);
void clienteEstrella();
void buscarCodigo(char *nombreBuscado);
char *nombreConCodigo(int codigoCliente);

// *********************************************************************************
// Recorre el arreglo USUARIOS buscando un nombre que coincida con el
// que le pasás, y devuelve la posición donde lo encontró (o -1)
// *********************************************************************************

int buscarUsuario(Usuario usuarios[], int cantidadUsuarios, char nombreBuscado[])
{
    int i = 0;
    int posicionEncontrada = ERROR;

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
// *********************************************************************************
// Funciones para el menu con cursores
// *********************************************************************************
void mostrarMenu(char VecMenu[][50])
{
    int i = 0;
    printf("\033[7m\033[33m %s \033[0m\033[27m\n", VecMenu[i]);
    for (i = 1; i < CANT_ITEMS_MENU; i++)
    {
        printf("\033[33m %s \033[0m\n", VecMenu[i]);
    }
    linea(50);
}
int seleccionaItem(char VecMenu[][50])
{
    int tecla;
    int i = 0;
    do
    {
        tecla = getch();

        if (tecla == 0 || tecla == 224) /* prefijo de tecla especial */
        {
            tecla = getch();

            switch (tecla)
            {
            case 80: /* flecha abajo */
                /* despinta el ítem actual */
                printf("\033[%d;1H\033[K\033[33m %s \033[0m\n", FILA_MENU + i, VecMenu[i]);

                i++;
                if (i > CANT_ITEMS_MENU - 1)
                    i = 0;

                /* pinta el nuevo ítem seleccionado */
                printf("\033[%d;1H\033[K\033[7m\033[33m %s \033[0m\033[27m\n", FILA_MENU + i, VecMenu[i]);
                break;

            case 72: /* flecha arriba */
                printf("\033[%d;1H\033[K\033[33m %s \033[0m\n", FILA_MENU + i, VecMenu[i]);

                i--;
                if (i < 0)
                    i = CANT_ITEMS_MENU - 1;

                printf("\033[%d;1H\033[K\033[7m\033[33m %s \033[0m\033[27m\n", FILA_MENU + i, VecMenu[i]);
                break;
            }
        }

    } while (tecla != 13); /* Enter */

    return i;
}
// *********************************************************************************
// Usa buscarUsuario por dentro y, si lo encuentra, compara la contraseña.
// Devuelve el permi de la cuenta (PERMI_ADMIN o PERMI_GESTOR) si el login fue
// exitoso, USUARIO_NO_ENCONTRADO (-1) o CONTRA_INCORRECTA (0) en caso contrario.
// *********************************************************************************
int validarLogin(Usuario usuarios[], int cantidadUsuarios, char nombreIngresado[], char claveIngresada[])
{
    int posicion = buscarUsuario(usuarios, cantidadUsuarios, nombreIngresado);

    if (posicion == USUARIO_NO_ENCONTRADO)
    {
        return USUARIO_NO_ENCONTRADO;
    }

    if (strcmp(usuarios[posicion].claveHash, claveIngresada) == 0)
    {
        return usuarios[posicion].rol; // Devuelve el permi del usuario (PERMI_ADMIN o PERMI_GESTOR)
    }

    return CONTRA_INCORRECTA;
}

// *********************************************************************************
// Registra un usuario nuevo y actualiza la cantidad de usuarios
// Devuelve EXITO, USUARIO_EXISTE o SIN_ESPACIO
// *********************************************************************************
int registrarUsuario(Usuario usuarios[], int *cantidadUsuarios, char nombreNuevo[], char claveNueva[], int permiNuevo)
{
    if (*cantidadUsuarios >= MAX_USUARIOS)
    {
        return SIN_ESPACIO;
    }

    if (buscarUsuario(usuarios, *cantidadUsuarios, nombreNuevo) != (-1))
    {
        return USUARIO_EXISTE;
    }

    strcpy(usuarios[*cantidadUsuarios].nombreUsuario, nombreNuevo);
    strcpy(usuarios[*cantidadUsuarios].claveHash, claveNueva);
    usuarios[*cantidadUsuarios].rol = permiNuevo;
    usuarios[*cantidadUsuarios].activo = 1;
    (*cantidadUsuarios)++;
    return EXITO;
}

// *********************************************************************************
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

        if (op < op1 || op > op3)
        {
            limpiarPantalla();
            printf("Opción no válida. Inténtelo nuevamente.\n\n");
        }
    } while (op < op1 || op > op3);

    if (op == op1)
    {
        return PERMI_ADMIN;
    }
    if (op == op2)
    {
        return PERMI_GESTOR;
    }
    return CANCELAR;
}

// *********************************************************************************
// MENÚ GESTOR
// *********************************************************************************

void MenuUsuario()
{
    int op = 0;
    char VecMenuPrincipal[4][50] = {"1. Facturas", "2. Estadisticas", "3. Productos", "0. Salir"};
    utf8();
    limpiarPantalla();
    printf("----Bienvenide Gestor----\n");
    do
    {
        // limpiarPantalla();
        printf("\n");
        mostrarMenu(VecMenuPrincipal);
        int indice = seleccionaItem(VecMenuPrincipal);
        op = (indice + 1) % CANT_ITEMS_MENU;
        switch (op)
        {
        case op1:
            printf("\nSECCION FACTURAS\n");
            printf("\n");
            printf("1. Generar nueva factura\n");
            printf("2. Ver listado de facturas\n");
            printf("3. Eliminar factura de la base de datos\n");
            printf("4. Ver listado de clientes\n");
            printf("5. Agregar cliente\n");
            printf("6. Eliminar cliente\n");
            printf("7. Modificar cliente");
            printf("0-Volver\n");
            leerEntero("Seleccione una opción: ", &op);
            switch (op)
            {
            case op1:
                printf("CREAR FUNCION GENERAR NUEVA FACTURA");
                pausa();
                break;
            case op2:
                printf("CREAR FUNCION VER LISTADO DE FACTURAS");
                pausa();
                break;
            case op3:
                printf("CREAR FUNCION ELIMINAR FACTURA DE LA BD");
                pausa();
                break;
            case op4:
                printf("CREAR FUNCION VER LISTADO DE CLIENTES");
                break;
            case op5:
                printf("CREAR FUNCION AGREGAR CLIENTE");
                break;
            case op6:
                printf("CREAR FUNCION ELIMINAR CLIENTE");
                break;
            case op7:
                printf("CREAR FUNCION MODIFICAR CLIENTE");
                break;

            case SALIR:
                printf("\nVolviendo...\n");
                break;

            default:
                printf("Error: Fuera de rango. Reintente.\n");
                break;
            }
            pausa();
            break;
        case op2:
            printf("\nSECCION ESTADISTICAS\n");
            printf("\n");
            printf("1. Ver producto estrella\n");
            printf("2. Ver producto mas vendido\n");
            printf("3. Ver mejores clientes\n");
            printf("4. Ver ventas por zona\n");
            printf("5. Ver cliente estrella\n");
            printf("0-Volver\n");
            leerEntero("Seleccione una opción: ", &op);
            switch (op)
            {
            case op1:
                printf("Desarrollar ver producto estrella\n");
                break;
            case op2:
                printf("Desarrollar ver producto mas vendido\n");
                break;
            case op3:
                printf("Desarrollar ver mejores clientes SEGUN CRITERIO MENSAJE LUANA EN EL GRUPO\n");
                break;
            case op4:
                printf("Desarrollar ver ventas por zona");
                break;
            case op5:
                break;
            case SALIR:
                printf("Volviendo\n");
                break;

            default:
                printf("Error: Fuera de rango. Reintente.\n");
                break;
            }
            pausa();
            break;
        case op3:
            printf("\nSECCION PRODUCTOS");
            printf("1. Ver listado de productos\n");
            printf("2. Agregar producto nuevo\n");
            printf("3. Ver listado de materia prima\n");
            printf("4. Agregar materia prima\n");
            printf("5. Eliminar materia prima\n");
            printf("6. Volver\n");
            leerEntero("Seleccione una opción: ", &op);
            switch (op)
            {
            case op1:
                printf("Desarrollar ver listado de productos\n");
                break;
            case op2:
                printf("Desarrollar agregar producto nuevo\n");
                break;
            case op3:
                printf("Desarrollar ver listado de materia prima\n");
                break;
            case op4:
                printf("Desarrollar agregar materia prima\n");
                break;
            case op5:
                printf("Desarrollar eliminar materia prima\n");
                break;
            case SALIR:
                printf("Volviendo....");

            default:
                printf("Error: Fuera de rango. Reintente.\n");
                break;
            }
            pausa();
            break;
        default:
            printf("Error: Fuera de rango. Reintente.\n");
            break;
        }
    } while (op != SALIR);
}

// *********************************************************************************
// Alta Gestor
// Usa buffers propios: antes pisaba la variable "clave" de LogeoAdmin con la
// contraseña del gestor, y eso rompía la condición del ciclo de login.
// *********************************************************************************
void AltaGestor(Usuario usuarios[], int *cantidadUsuarios)
{
    char nombre[LARGO_USUARIO];
    char clave[LARGO_CLAVE];
    bool registroResuelto = false;

    printf("**********Registrar Gestor**********\n\n");
    do
    {
        leerCadena(nombre, LARGO_USUARIO, "Nombre de gestor (-1 para salir): ");
        if (strcmp(nombre, "-1") == 0)
        {
            registroResuelto = true;
        }
        else if (nombre[0] == '\0')
        {
            printf("El nombre no puede estar vacío.\n");
        }
        else
        {
            leerCadena(clave, LARGO_CLAVE, "Contraseña (-1 para salir): ");
            if (strcmp(clave, "-1") == 0)
            {
                registroResuelto = true;
            }
            else if (clave[0] == '\0')
            {
                printf("La contraseña no puede estar vacía.\n");
            }
            else
            {
                int resultado = registrarUsuario(usuarios, cantidadUsuarios, nombre, clave, PERMI_GESTOR);
                if (resultado == SIN_ESPACIO)
                {
                    printf("ERROR: sin espacio para mas usuarios\n");
                    registroResuelto = true;
                }
                else if (resultado == USUARIO_EXISTE)
                {
                    printf("Usuario ya existente, reintente\n");
                }
                else
                {
                    printf("\n¡Registro exitoso!\n");
                    registroResuelto = true;
                }
            }
        }
    } while (!registroResuelto);
    pausa();
}

void BajaGestor(Usuario usuarios[], int *cantidadUsuarios)
{
    printf("**********Dar de baja un Gestor**********\n\n");
    char nombre[LARGO_USUARIO];
    leerCadena(nombre, LARGO_USUARIO, "Ingrese el nombre del gestor a dar de baja (-1 para salir): ");
    if (strcmp(nombre, "-1") == 0)
    {
        printf("Operación cancelada.\n");
        pausa();
    }
    else if (strcmp(nombre, "admin") == 0)
    {
        printf("No se puede dar de baja al administrador.\n");
        pausa();
    }
    else
    {
        int opcion;
        int posicion = buscarUsuario(usuarios, *cantidadUsuarios, nombre);
        if (posicion == ERROR)
        {
            printf("Gestor no encontrado.\n");
            pausa();
        }
        else
        {
            printf("Gestor encontrado: %s\n", usuarios[posicion].nombreUsuario);
            printf("¿Eliminar de manera logica o fisica?\n");
            printf("1. Logica (marcar como inactivo)\n");
            printf("2. Fisica (eliminar del arreglo)\n");
            leerEntero("Seleccione una opción: ", &opcion);
            switch (opcion)
            {
            case 1:
                usuarios[posicion].activo = 0;
                printf("Gestor dado de baja de manera logica exitosamente.\n");
                pausa();
                break;
            case 2:
                // Mover todos los usuarios posteriores una posición hacia atrás
                for (int i = posicion; i < *cantidadUsuarios - 1; i++)
                {
                    usuarios[i] = usuarios[i + 1];
                }
                (*cantidadUsuarios)--;
                break;
            default:
                printf("Opción no válida. Operación cancelada.\n");
                pausa();
                return;
            }
            printf("Gestor eliminado permanentemente.\n");
            pausa();
        }
    }
}
// *********************************************************************************
// MENÚ ADMIN
// *********************************************************************************
void MenuAdmin(Usuario usuarios[], int *cantidadUsuarios)
{
    int op = 0;

    utf8();
    do
    {
        limpiarPantalla();
        printf("---- Menu de Administrador ----\n");
        printf("\n");
        printf("1. Dar de alta a un gestor\n");
        printf("2. Dar de baja a un gestor\n");
        printf("0. Salir\n");

        // arreglar menu con cursores
        // mostrarMenu(VecMenuPrincipal);
        // indice = seleccionaItem(VecMenuPrincipal);
        // op = (indice + 1) % CANT_ITEMS_MENU;
        leerEntero("Ingrese una opcion", &op);
        switch (op)
        {
        case op1:
            limpiarPantalla();
            AltaGestor(usuarios, cantidadUsuarios);
            break;
        case op2:
            limpiarPantalla();
            BajaGestor(usuarios, cantidadUsuarios);
            break;
        case SALIR:
            printf("\nVolviendo...\n");
            break;
        default:
            printf("\nError: Fuera de rango. Reintente.\n");
            pausa(); // sin esto el limpiarPantalla() del ciclo borra el mensaje
            break;
        }
    } while (op != SALIR);
}

// *********************************************************************************
// LOGUEO ADMIN
// Devuelve CANCELAR si se agotaron los intentos (cierra el programa),
// 0 en cualquier otro caso (vuelve a la pantalla de selección de rol).
// *********************************************************************************
int LogeoAdmin(Usuario usuarios[], int *cantidadUsuarios)
{
    int intentos = MAXIMO_INTENTOS;
    char clave[LARGO_CLAVE];

    limpiarPantalla();

    while (intentos > 0)
    {
        leerCadena(clave, LARGO_CLAVE, "Ingrese la clave de administrador (-1 para volver): ");

        if (strcmp(clave, "-1") == 0)
        {
            return 0;
        }

        if (strcmp(clave, CLAVE_ADMIN) == 0)
        {
            limpiarPantalla();
            MenuAdmin(usuarios, cantidadUsuarios);
            return 0;
        }

        intentos--;
        if (intentos > 0)
        {
            printf("Clave incorrecta. Intentos disponibles: %d\n", intentos);
        }
    }

    limpiarPantalla();
    printf("Clave incorrecta. Sin intentos disponibles.\n\n");
    printf("Saliendo del programa...\n");
    retardo2(0.2);
    return CANCELAR;
}

// *********************************************************************************
// LOGUEO GESTOR
// Valida usuario y contraseña contra el arreglo de usuarios (usa validarLogin).
// Devuelve true si el login fue exitoso.
// *********************************************************************************
bool LogeoGestor(Usuario usuarios[], int cantidadUsuarios)
{
    char nombre[LARGO_USUARIO];
    char clave[LARGO_CLAVE];
    int intentos = MAXIMO_INTENTOS;

    if (cantidadUsuarios == 0)
    {
        printf("\nNo hay gestores registrados. Pida al administrador que cree uno.\n");
        pausa();
        return false;
    }

    while (intentos > 0)
    {
        leerCadena(nombre, LARGO_USUARIO, "Usuario (-1 para volver): ");
        if (strcmp(nombre, "-1") == 0)
        {
            return false;
        }
        leerCadena(clave, LARGO_CLAVE, "Contraseña: ");

        if (validarLogin(usuarios, cantidadUsuarios, nombre, clave) == PERMI_GESTOR)
        {
            return true;
        }

        intentos--;
        if (intentos > 0)
        {
            printf("Usuario o contraseña incorrectos. Intentos disponibles: %d\n", intentos);
        }
    }

    printf("Sin intentos disponibles.\n");
    pausa();
    return false;
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
void mostrar_pose2() // FIGURA PRUEBA
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

void registrarFactura()
{
}

// *********************************************************************************
/* Debe leer el archivo clientes.dat y devolver la posición del cliente con el código dado.
Si no lo encuentra, devuelve -1. */
// *********************************************************************************
int buscarCliente(int codigoCliente)
{
    FILE *archivoClientes = fopen("clientes.dat", "rb");
    if (archivoClientes == NULL)
    {
        return ERROR; // Error al abrir el archivo
    }

    Cliente cliente;
    int posicion = 0;

    while (fread(&cliente, sizeof(Cliente), 1, archivoClientes))
    {
        if (cliente.codigo == codigoCliente)
        {
            fclose(archivoClientes);
            return posicion;
        }
        posicion++;
    }

    fclose(archivoClientes);
    return ERROR;
}

// *********************************************************************************
/* Debe leer el archivo clientes.dat y buscar el o los clientes que coincidadan con el nombre
buscado, los imprime junto con su codigo */
// *********************************************************************************
void buscarCodigo(char *nombreBuscado)
{
    FILE *archivoClientes = fopen("clientes.dat", "rb");
    int impreso = 0;
    if (archivoClientes = !NULL)
    {
        Cliente cliente;

        while (fread(&cliente, sizeof(Cliente), 1, archivoClientes))
        {
            if (strcmp(cliente.datos.nombreYApellido, nombreBuscado) == 0)
            {
                printf("Nombre: %s, Código: %d \n", cliente.datos.nombreYApellido, cliente.codigo);
                impreso++;
            }
        }
        fclose(archivoClientes);
        if (impreso == 0)
        {
            printf("No se encontraron clientes con el nombre: %s\n", nombreBuscado);
        }
        fclose(archivoClientes);
        printf("Error al buscar el código del cliente.\n");
    }
    else
    {
        printf("Error al abrir el archivo de clientes.\n");
        return;
    }
}

// *********************************************************************************
/* Debe leer las facturas con buscarCLiente y devolver el codigo de cual de ellos gastó mas dinero en total.*/
// *********************************************************************************
int codigoEstrella()
{
    Cliente cliente;
    Cliente clienteMax;
    float totalMax = 0.0;
    FILE *archivoClientes = fopen("clientes.dat", "rb");
    if (archivoClientes == NULL)
        return ERROR;
    else
    {
        while (fread(&cliente, sizeof(Cliente), 1, archivoClientes))
        {
            if (cliente.activo == 1 && cliente.saldo > totalMax)
            {
                totalMax = cliente.saldo;
                clienteMax = cliente;
            }
        }
        if (totalMax == 0.0)
        {
            fclose(archivoClientes);
            return ERROR;
        }
        else
        {
            fclose(archivoClientes);
            return clienteMax.codigo;
        }
    }
}

// *********************************************************************************
/* Devuelve el nombre del cliente ingresando el codigo*/
// *********************************************************************************
char *nombreConCodigo(int codigoCliente)
{
    Cliente cliente;
    FILE *archivoClientes = fopen("clientes.dat", "rb");
    if (archivoClientes == NULL)
        printf("Error al abrir el archivo de clientes.\n");
    else
    {
        while (fread(&cliente, sizeof(Cliente), 1, archivoClientes))
        {
            if (cliente.codigo == codigoCliente)
            {
                fclose(archivoClientes);
                return cliente.datos.nombreYApellido;
            }
        }
        fclose(archivoClientes);
        printf("No se encontró un cliente con el código %d.\n", codigoCliente);
    }
}

// *********************************************************************************
/* Imprime los datos del cliente que ha gastado más dinero en total. */
// *********************************************************************************
void clienteEstrella()
{
    if (codigoEstrella() == ERROR)
    {
        printf("No hay clientes activos o no hay clientes con saldo mayor a 0.\n");
    }
    else
    {
        int posicion;
        posicion = codigoEstrella();
        printf("Su cliente estrella es %s - codigo: %d\n", nombreConCodigo(posicion), posicion);
    }
}

#endif /* FACTUFUNCIONES_H_ */
