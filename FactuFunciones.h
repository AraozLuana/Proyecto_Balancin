#ifndef FACTUFUNCIONES_H_
#define FACTUFUNCIONES_H_
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "Estructuras.h"
#include "utiles_Proyecto.h"

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <unistd.h>
#endif
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

/* Constantes para evitar magic numbers en cases y funciones varias */
#define op1 1
#define op2 2
#define op3 3
#define op4 4
#define op5 5
#define op6 6
#define op7 7
const char *pathArchivoUsers = "usersData.dat";

#define CLAVE_ADMIN "admin123"
#define MAXIMO_INTENTOS 4
#define CANCELAR (-2)
#define SALIR 0
#define CANT_ITEMS_MENU 4
#define FILA_MENU 3
#define FLECHA_ARRIBA 72
#define FLECHA_ABAJO 80
#define ENTER 13
#define ANCHO_LINEA 40
#define MAX_DETALLES_FACTURA 20

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
int registrarUsuario(Usuario usuarios[], int *cantidadUsuarios, char nombreNuevo[], char claveNueva[], int permiNuevo, FILE *archivoUsers);
int iniciarSesion();
int LogeoAdmin(Usuario usuarios[], int *cantidadUsuarios, FILE *archivoUsuarios);
bool LogeoGestor(Usuario usuarios[], int cantidadUsuarios);
void MenuUsuario();
void MenuAdmin(Usuario usuarios[], int *cantidadUsuarios, FILE *archivoUsuarios);
void AltaGestor(Usuario usuarios[], int *cantidadUsuarios, FILE *archivoUsuarios);
void pantallaCarga();
void mostrarMenu(char VecMenu[][50]);
int seleccionaItem(char VecMenu[][50]);
bool generarFactura(int nroFactura, Factura *factura, DetalleFactura detalles[], int capacidadDetalles, int *cantidadDetalles);

// *********************************************************************************
// Recorre el arreglo USUARIOS buscando un nombre que coincida con el
// que le pasás, y devuelve la posición donde lo encontró (o -1)
// *********************************************************************************
void prepararConsola(void)
{
    HANDLE hSalida = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD modo = 0;

    SetConsoleOutputCP(CP_UTF8);
    GetConsoleMode(hSalida, &modo);
    SetConsoleMode(hSalida, modo | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

int buscarUsuario(Usuario usuarios[], int cantidadUsuarios, char nombreBuscado[])
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
// *********************************************************************************
// Funciones para el menu con cursores
// *********************************************************************************
int elegirOpcionAdmin(const char *titulo, const char *opciones[], int cant)
{
    int sel = 0;
    int tecla;

    printf("\033[2J\033[?25l"); /* limpia pantalla y oculta el cursor */

    do
    {
        mostrarMenuAdmin(titulo, opciones, cant, sel);
        tecla = _getch();

        if (tecla == 0 || tecla == 224) /* prefijo de tecla especial */
        {
            tecla = _getch();

            if (tecla == FLECHA_ARRIBA)
                sel = (sel == 0) ? cant - 1 : sel - 1;
            else if (tecla == FLECHA_ABAJO)
                sel = (sel == cant - 1) ? 0 : sel + 1;

            tecla = 0; /* para que el código extendido no se confunda con Enter */
        }
        else if (tecla >= '1' && tecla < '1' + cant) /* atajo numérico */
        {
            sel = tecla - '1';
            mostrarMenuAdmin(titulo, opciones, cant, sel);
            tecla = ENTER;
        }

    } while (tecla != ENTER);

    printf("\033[?25h"); /* vuelve a mostrar el cursor */
    return sel + 1;
}
void mostrarMenuAdmin(const char *titulo, const char *opciones[], int cant, int sel)
{
    int i;

    printf("\033[H"); /* cursor arriba a la izquierda: redibuja sin parpadeo */
    linea(ANCHO_LINEA);
    printf("  %s\n", titulo);
    linea(ANCHO_LINEA);

    for (i = 0; i < cant; i++)
    {
        if (i == sel)
            printf("\033[7m\033[33m > %d. %s \033[0m\033[K\n", i + 1, opciones[i]); /* invertido */
        else
            printf("\033[33m   %d. %s \033[0m\033[K\n", i + 1, opciones[i]);
    }

    linea(ANCHO_LINEA);
    printf(" Flechas: mover  |  Enter: elegir\n");
}
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
        return usuarios[posicion].permi;
    }

    return CONTRA_INCORRECTA;
}

// *********************************************************************************
// Registra un usuario nuevo y actualiza la cantidad de usuarios
// Devuelve EXITO, USUARIO_EXISTE o SIN_ESPACIO
// *********************************************************************************
int registrarUsuario(Usuario usuarios[], int *cantidadUsuarios, char nombreNuevo[], char claveNueva[], int permiNuevo, FILE *archivoUsers)
{
    archivoUsers = fopen(pathArchivoUsers, "rb+");

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
    usuarios[*cantidadUsuarios].permi = permiNuevo;
    fwrite(&(usuarios[*cantidadUsuarios]), sizeof(Usuario), 1, archivoUsers);
    (*cantidadUsuarios)++;
    fclose(archivoUsers);

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
    int siguienteNroFactura = 1;
    Factura facturaActual;
    DetalleFactura detallesFactura[MAX_DETALLES_FACTURA];
    int cantidadDetallesFactura = 0;
    char VecMenuPrincipal[4][50] = {"1. Facturas", "2. Estadisticas", "3. Productos", "0. Salir"};
    // char vecMenuFacturas[4][50];
    // char vecMenuEstadisticas[4][50];
    // char vecMenuProductos[4][50];
    limpiarPantalla();
    printf("----Bienvenide Gestor----\n");
    do
    {
        printf("\n");
        mostrarMenu(VecMenuPrincipal);
        int indice = seleccionaItem(VecMenuPrincipal);
        op = (indice + 1) % CANT_ITEMS_MENU;
        limpiarPantalla();
        switch (op)
        {
        case op1:
            limpiarPantalla();
            printf("\nSECCION FACTURAS\n");
            printf("\n");
            printf("1. Generar nueva factura\n");
            printf("2. Ver listado de facturas\n");
            printf("3. Eliminar factura de la base de datos\n");
            printf("4. Ver listado de clientes\n");
            printf("5. Agregar cliente\n");
            printf("6. Eliminar cliente\n");
            printf("7. Modificar cliente\n");
            printf("0. Volver\n");
            leerEntero("Seleccione una opción: ", &op);
            switch (op)
            {
            case op1:
            limpiarPantalla();
                if (generarFactura(siguienteNroFactura, &facturaActual, detallesFactura, MAX_DETALLES_FACTURA, &cantidadDetallesFactura))
                {
                    siguienteNroFactura++;
                }
                pausa();
                break;
            case op2:
                limpiarPantalla();
                printf("CREAR FUNCION VER LISTADO DE FACTURAS");
                pausa();
                break;
            case op3:
                limpiarPantalla();
                printf("CREAR FUNCION ELIMINAR FACTURA DE LA BD");
                pausa();
                break;
            case op4:
                limpiarPantalla();
                printf("CREAR FUNCION VER LISTADO DE CLIENTES");
                break;
            case op5:
                limpiarPantalla();
                printf("CREAR FUNCION AGREGAR CLIENTE");
                break;
            case op6:
                limpiarPantalla();
                printf("CREAR FUNCION ELIMINAR CLIENTE");
                break;
            case op7:
                limpiarPantalla();
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
            limpiarPantalla();
            printf("\nSECCION ESTADISTICAS\n");
            printf("\n");
            printf("1. Ver producto estrella\n");
            printf("2. Ver producto mas vendido\n");
            printf("3. Ver mejores clientes\n");
            printf("4. Ver ventas por zona\n");
            printf("0-Volver\n");
            leerEntero("Seleccione una opción: ", &op);
            switch (op)
            {
            case op1:
            {
                limpiarPantalla();
                // Producto que genera más ganancias
                /*char *productoEstrella = buscarProductoEstrella(productos, cantidadProductos, detalles, cantidadDetalles);
                if (productoEstrella != NULL)
                {
                    printf("El producto estrella es: %s\n", productoEstrella);
                }
                else
                {
                    printf("No se encontraron productos o detalles de facturas.\n");
                }*/
                printf("Desarrollar ver producto estrella\n");
                break;
            }
            case op2:
                limpiarPantalla();
                printf("Desarrollar ver producto mas vendido\n");
                break;
            case op3:
                limpiarPantalla();
                printf("Desarrollar ver mejores clientes SEGUN CRITERIO MENSAJE LUANA EN EL GRUPO\n");
                break;
            case op4:
                limpiarPantalla();
                printf("Desarrollar ver ventas por zona");
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
            limpiarPantalla();
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
                limpiarPantalla();
                printf("Desarrollar ver listado de productos\n");
                break;
            case op2:
                limpiarPantalla();
                printf("Desarrollar agregar producto nuevo\n");
                break;
            case op3:
                limpiarPantalla();
                printf("Desarrollar ver listado de materia prima\n");
                break;
            case op4:
                limpiarPantalla();
                printf("Desarrollar agregar materia prima\n");
                break;
            case op5:
                limpiarPantalla();
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
void AltaGestor(Usuario usuarios[], int *cantidadUsuarios, FILE *archivoUsuarios)
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
                int resultado = registrarUsuario(usuarios, cantidadUsuarios, nombre, clave, PERMI_GESTOR, archivoUsuarios);
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
        int posicion = buscarUsuario(usuarios, *cantidadUsuarios, nombre);
        if (posicion == -1)
        {
            printf("Gestor no encontrado.\n");
            pausa();
        }
        else
        {
            // Mover todos los usuarios posteriores una posición hacia atrás
            for (int i = posicion; i < *cantidadUsuarios - 1; i++)
            {
                usuarios[i] = usuarios[i + 1];
            }
            (*cantidadUsuarios)--;
            printf("Gestor dado de baja exitosamente.\n");
            pausa();
        }
    }
}
// *********************************************************************************
// MENÚ ADMIN
// *********************************************************************************
void MenuAdmin(Usuario usuarios[], int *cantidadUsuarios, FILE *archivoUsuarios)
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
        leerEntero("Ingrese una opcion: ", &op);
        switch (op)
        {
        case op1:
            limpiarPantalla();
            AltaGestor(usuarios, cantidadUsuarios, archivoUsuarios);
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
int LogeoAdmin(Usuario usuarios[], int *cantidadUsuarios, FILE *archivoUsuarios)
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
            MenuAdmin(usuarios, cantidadUsuarios, archivoUsuarios);
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
        retardo2(0.1);
        limpiarPantalla();
        mostrar_pose2();
        retardo2(0.1);
    }
}

// Buscar el producto con mayor importe vendido (precio por cantidad vendida).
/* char *buscarProductoEstrella(Articulo productos[], int cantidadProductos, DetalleFactura detalles[], int cantidadDetalles)
{
    if (cantidadProductos <= 0 || cantidadDetalles <= 0)
    {
        return NULL;
    }

    Articulo *productoEstrella = NULL;
    float mayorImporteVendido = 0.0;

    for (int i = 0; i < cantidadProductos; i++)
    {
        int cantidadVendida = 0;
        for (int j = 0; j < cantidadDetalles; j++)
        {
            if (detalles[j].codigoArticulo == productos[i].codigo)
            {
                cantidadVendida += detalles[j].cantidad;
            }
        }

        float importeVendido = productos[i].precio * cantidadVendida;
        if (cantidadVendida > 0 && (productoEstrella == NULL || importeVendido > mayorImporteVendido))
        {
            productoEstrella = &productos[i];
            mayorImporteVendido = importeVendido;
        }
    }

    if (productoEstrella != NULL)
    {
        return productoEstrella->descripcion;
    }
    else
    {
        return NULL;
    }
} */

// Carga los datos del cliente y sus productos, calcula los importes y devuelve si se genero la factura.
bool generarFactura(int nroFactura, Factura *factura, DetalleFactura detalles[], int capacidadDetalles, int *cantidadDetalles)
{
    //Inicializo las variables
    Factura nuevaFactura = {0};
    int cantidadLineas = 0;
    float netoGravado = 0;
    bool facturaGenerada = false;
    time_t ahora;
    struct tm *fechaActual;

    if (factura == NULL || detalles == NULL || cantidadDetalles == NULL || nroFactura <= 0 || capacidadDetalles <= 0)
    {
        printf("No se puede generar la factura: los datos recibidos no son válidos.\n");
    }
    else
    {

        *cantidadDetalles = 0;
        nuevaFactura.nroFactura = nroFactura;

        do
        {
            leerEntero("Ingrese el código del cliente: ", &nuevaFactura.codigoCliente);
            if (nuevaFactura.codigoCliente <= 0)
            {
                printf("El código del cliente debe ser mayor que cero.\n");
            }
        } while (nuevaFactura.codigoCliente <= 0);

        leerCadena(nuevaFactura.clienteSnapshot.nombreYApellido, LARGO_NOMBRE, "Ingrese el nombre del cliente: ");
        printf("\n");
        nuevaFactura.alicuotaIVA = 0;
        nuevaFactura.importeIVA = 0;
        nuevaFactura.estado = FACTURA_EMITIDA;

        ahora = time(NULL);
        fechaActual = localtime(&ahora);

        if (fechaActual == NULL)
        {
            printf("No se pudo obtener la fecha de emisión.\n");
        }
        else
        {
            nuevaFactura.fechaEmision.dia = fechaActual->tm_mday;
            nuevaFactura.fechaEmision.mes = fechaActual->tm_mon + 1;
            nuevaFactura.fechaEmision.anio = fechaActual->tm_year + 1900;

            while (cantidadLineas < capacidadDetalles)
            {
                DetalleFactura detalle = {0};
                float precioUnitario = 0;
                float nuevoNeto = 0;
                bool lineaValida = true;

                leerEntero("Ingrese el código del producto (0 para finalizar): ", &detalle.codigoArticulo);
                printf("\n");
                if (detalle.codigoArticulo == 0)
                {
                    break;
                }
                else
                {
                    if (detalle.codigoArticulo < 0)
                    {
                        printf("El código del producto debe ser mayor que cero.\n");
                        lineaValida = false;
                    }
                }

                if (lineaValida)
                {
                    leerCadena(detalle.descripcion, LARGO_DESC, "Ingrese la descripción del producto: ");
                    printf("\n");
                    if (detalle.descripcion[0] == '\0')
                    {
                        printf("La descripción no puede estar vacía.\n");
                        lineaValida = false;
                    }
                }

                if (lineaValida)
                {
                    do
                    {
                        leerFlotante("Ingrese el precio unitario: $", &precioUnitario);
                        if (precioUnitario < 0)
                        {
                            printf("El precio no puede ser negativo.\n");
                        }
                    } while (precioUnitario < 0);

                    do
                    {
                        leerEntero("Ingrese la cantidad: ", &detalle.cantidad); 
                        if (detalle.cantidad <= 0)
                        {
                            printf("La cantidad debe ser mayor que cero.\n");
                        }
                    } while (detalle.cantidad <= 0);

                    detalle.nroFactura = nroFactura;
                    detalle.precioUnitario = precioUnitario;
                    detalle.subtotalLinea = precioUnitario * detalle.cantidad;
                    nuevoNeto = netoGravado + detalle.subtotalLinea;
                }

                if (lineaValida)
                {
                    detalles[cantidadLineas] = detalle;
                    netoGravado = nuevoNeto;
                    cantidadLineas++;
                }
            }

            //Comprueba si se ingresaron productos en la factura y, si es así, calcula el total y muestra la factura generada.
            if (cantidadLineas == 0)
            {
                printf("La factura no contiene productos; no se generó.\n");
            }
            else
            {
                nuevaFactura.netoGravado = netoGravado;
                nuevaFactura.totalFactura = netoGravado + nuevaFactura.importeIVA;
                *factura = nuevaFactura;
                *cantidadDetalles = cantidadLineas;

                printf("\nFactura N° %d generada el %02d/%02d/%04d\n",
                       factura->nroFactura, factura->fechaEmision.dia, factura->fechaEmision.mes, factura->fechaEmision.anio);
                printf("Cliente: %d - %s\n", factura->codigoCliente, factura->clienteSnapshot.nombreYApellido);
                printf("%-10s | %20s | %10s | %10s | %6s\n", "Código", "Descripción", "Precio unitario", "Cantidad", "Total"); //Muestra la factura generada con los detalles de cada producto
                for (int i = 0; i < cantidadLineas; i++) // Recorre los detalles de la factura
                {
                    printf("%-10d | %20s | $%10.2f | %10d | $%6.2f\n",
                        detalles[i].codigoArticulo, detalles[i].descripcion,
                        detalles[i].precioUnitario, detalles[i].cantidad, detalles[i].subtotalLinea);
                }
                printf("Total de la factura: $%.2f\n", factura->totalFactura);
                facturaGenerada = true;
            }
        }
    }

    return facturaGenerada;
}

#endif /* FACTUFUNCIONES_H_ */
