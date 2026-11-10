#ifndef FACTUFUNCIONES_H_
#define FACTUFUNCIONES_H_

#include "Estructuras.h"
#include "utiles_Proyecto.h"
#include <string.h>

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

#define CLAVE_ADMIN "admin123"
#define MAXIMO_INTENTOS 4
#define CANCELAR -2
#define SALIR 0
#define FLECHA_ARRIBA 72
#define FLECHA_ABAJO 80
#define ENTER 13
#define ANCHO_LINEA 40
#define MAX_DETALLES_FACTURA 20
#define ACTIVO 1
#define INACTIVO 0

// Resultados de validarLogin
#define CONTRA_INCORRECTA 0
#define USUARIO_NO_ENCONTRADO -1

// Resultados de registrarUsuario
#define EXITO 1
#define USUARIO_EXISTE 2
#define SALIDA -1

// *********************************************************************************
// PROTOTIPOS
// *********************************************************************************
// USUARIOS
int buscarPosicionRegistro(FILE *archivoUsuario, char *nombre_usuario_baja);
int validarLogin(FILE *archivoUsuarios, char *nombreUsuario, char *clave);
int registrarUsuario(FILE *archivoUsers);
int iniciarSesion();
int LogeoAdmin();
bool LogeoGestor(FILE *archivoUsuarios);
//**********************************************************************************
// MENUS
void MenuUsuario();
void MenuAdmin();
//**********************************************************************************
// FUNCIONES VARIAS
void AltaGestor(FILE *archivoUsuarios);
void pantallaCarga();
void prepararConsola(void);
void mostrarMenuAdmin(const char *titulo, const char *opciones[], int cant, int sel);
void mostrarMenu(const char *titulo, const char *opciones[], int cant, int sel);
int contarRegistrosUsuarios(FILE *archivoUsers);
int elegirOpcion(const char *titulo, const char *opciones[], int cant);
void verListadoDeProductos(FILE *archivoProductos);
bool generarFactura(int nroFactura, Factura *factura, DetalleFactura detalles[], int capacidadDetalles, int *cantidadDetalles, 
                    int *cantidadFacturas, FILE *archivoFacturas, FILE *archivoProductos, FILE *archivoDetalles);
void verListadoFacturas(FILE *archivoFacturas, FILE *archivoDetalles);

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

/*int buscarUsuario(Usuario usuarios[], int cantidadUsuarios, char nombreBuscado[])
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
}*/
// *********************************************************************************
// Funciones para el menu con cursores
// *********************************************************************************
void mostrarMenu(const char *titulo, const char *opciones[], int cant, int sel)
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

int elegirOpcion(const char *titulo, const char *opciones[], int cant)
{
    int sel = 0;
    int tecla;

    printf("\033[2J\033[?25l"); /* limpia pantalla y oculta el cursor */

    do
    {
        mostrarMenu(titulo, opciones, cant, sel);
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
            mostrarMenu(titulo, opciones, cant, sel);
            tecla = ENTER;
        }

    } while (tecla != ENTER);

    printf("\033[?25h"); /* vuelve a mostrar el cursor */
    return sel + 1;
}

// *********************************************************************************
// Usa buscarUsuario por dentro y, si lo encuentra, compara la contraseña.
// Devuelve el permi de la cuenta (PERMI_ADMIN o PERMI_GESTOR) si el login fue
// exitoso, USUARIO_NO_ENCONTRADO (-1) o CONTRA_INCORRECTA (0) en caso contrario.
// *********************************************************************************
int validarLogin(FILE *archivoUsuarios, char *nombreUsuario, char *clave)
{
    int posicion = buscarPosicionRegistro(archivoUsuarios, nombreUsuario);
    Usuario user;
    if (posicion == USUARIO_NO_ENCONTRADO)
    {
        return USUARIO_NO_ENCONTRADO;
    }
    fseek(archivoUsuarios, posicion * sizeof(Usuario), SEEK_SET);
    fread(&user, sizeof(Usuario), 1, archivoUsuarios);
    if ((strcmp(user.nombreUsuario, nombreUsuario) == 0) && strcmp(user.claveHash, clave) == 0)
    {
        return user.rol_usuario;
    }

    return CONTRA_INCORRECTA;
}

// *********************************************************************************
// Registra un usuario nuevo y actualiza la cantidad de usuarios
// Devuelve EXITO, USUARIO_EXISTE o SIN_ESPACIO
// *********************************************************************************
int registrarUsuario(FILE *archivoUsers)
{
    Usuario user;
    Usuario busqueda;
    user.activo = ACTIVO;
    user.rol_usuario = PERMI_GESTOR;

    leerCadena(user.nombreUsuario, LARGO_USUARIO, "Nombre de gestor (-1 para salir): ");
    while (user.nombreUsuario[0] == '\0')
    {
        printf("El nombre no puede estar vacío.\n");
        leerCadena(user.nombreUsuario, LARGO_USUARIO, "Nombre de gestor (-1 para salir): ");
    }
    if (strcmp(user.nombreUsuario, "-1") == 0)
    {
        return SALIDA;
    }

    leerCadena(user.claveHash, LARGO_CLAVE, "Contraseña (-1 para salir): ");
    while (user.claveHash[0] == '\0')
    {
        printf("La contraseña no puede estar vacía.\n");
        leerCadena(user.claveHash, LARGO_CLAVE, "Contraseña (-1 para salir): ");
    }

    if (strcmp(user.claveHash, "-1") == 0)
    {
        return SALIDA;
    }
    while (fread(&busqueda, sizeof(Usuario), 1, archivoUsers)) // fread retorna 1 si esta bien, mientras que exista. cuando no haya mas que sea endoffile, termina (porque sería 0)
    {
        if (strcmp(busqueda.nombreUsuario, user.nombreUsuario) == 0)
        {
            return USUARIO_EXISTE;
        }
    }

    fwrite(&user, sizeof(Usuario), 1, archivoUsers);

    return EXITO;
}

// *********************************************************************************
// Selección de rol. Devuelve PERMI_ADMIN, PERMI_GESTOR o CANCELAR (salir)
// *********************************************************************************
int iniciarSesion()
{
    int op = 0;
    char *menu[] = {"Administrador", "Gestor", "Salir"};
    int cant = sizeof(menu) / sizeof(menu[0]);

    do
    {
        op = elegirOpcion("Seleccione su usuario", menu, cant);
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

void verListadoDeProductos(FILE *archivoProductos)
{
    Articulo product;
    if (fread(&product, sizeof(Articulo), 1, archivoProductos) == 0)
    {
        printf("No hay productos cargados para mostrar\n");
    }
    printf("==============================================================\n");
    printf("                    CONSULTA DE PRODUCTOS                     \n");
    printf("==============================================================\n");
    printf("%-6s %-22s %-10s %8s %10s\n", "COD", "NOMBRE", "STOCK", "PRECIO", "CANTIDAD VENDIDA" , "ESTADO");
    while (fread(&product, sizeof(Articulo), 1, archivoProductos) == 1) // significa que mientras que haya registro
    {
        printf("%-6d %-22s %-10s %8d %10.2f %d %d\n", product.codigo, product.descripcion, product.stock, product.precio,product.cantVendida, product.estado);
    }
}

// *********************************************************************************
// MENÚ GESTOR
// *********************************************************************************

void MenuUsuario()
{
    int op = 0;
    char *VecMenuPrincipal[] = {"Facturas", "Estadisticas", "Productos", "Salir"};
    char *vecMenuFacturas[] = {"Generar nueva factura", "Ver listado de facturas", "Eliminar factura de la base de datos", "Ver listado de clientes", "Agregar cliente", "Eliminar cliente", "Modificar cliente", "Volver"};
    char *vecMenuEstadisticas[] = {"Ver producto estrella", "Ver producto mas vendido", "Ver mejores clientes", "Ver ventas por zona", "Volver"};
    char *vecMenuProductos[] = {"Ver listado de productos", "Agregar producto nuevo", "Ver listado de materia prima", "Agregar materia prima", "Eliminar materia prima", "Volver"};

    utf8();
    limpiarPantalla();
    int cantOpcionesMenuPrincipal = sizeof(VecMenuPrincipal) / sizeof(VecMenuPrincipal[0]);
    int cantOpcionesFacturas = sizeof(vecMenuFacturas) / sizeof(vecMenuFacturas[0]);
    int cantOpcionesEstadisticas = sizeof(vecMenuEstadisticas) / sizeof(vecMenuEstadisticas[0]);
    int cantOpcionesProductos = sizeof(vecMenuProductos) / sizeof(vecMenuProductos[0]);
    prepararConsola();

    do
    {
        limpiarPantalla();
        op = elegirOpcion("Menu Gestor", VecMenuPrincipal, cantOpcionesMenuPrincipal);

        switch (op)
        {
        case op1:

            printf("\n");

            op = elegirOpcion("SECCION FACTURAS", vecMenuFacturas, cantOpcionesFacturas);

            do
            {
                switch (op)
                {
                case op1:
                {
                    FILE *archivoFacturas;
                    FILE *archivoProductos;
                    FILE *archivoDetalles;
                    Factura facturaActual = {0};
                    Factura facturaGuardada;
                    DetalleFactura detallesFactura[MAX_DETALLES_FACTURA];
                    int cantidadDetallesFactura = 0;
                    int cantidadFacturas = 0;
                    int siguienteNroFactura = 1;

                    limpiarPantalla();
                    archivoFacturas = fopen("bd_Facturas.dat", "rb+");
                    if (archivoFacturas == NULL)
                    {
                        archivoFacturas = fopen("bd_Facturas.dat", "wb+");
                    }
                    archivoProductos = fopen("bd_Products.dat", "rb");
                    archivoDetalles = fopen("bd_Detalles.dat", "ab+");

                    if (archivoFacturas == NULL)
                    {
                        printf("No se pudo abrir el archivo de facturas.\n");
                    }
                    else if (archivoProductos == NULL)
                    {
                        printf("No se pudo abrir el archivo de productos. Cargue productos antes de generar facturas.\n");
                    }
                    else if (archivoDetalles == NULL)
                    {
                        printf("No se pudo abrir el archivo de detalles de facturas.\n");
                    }
                    else
                    {
                        rewind(archivoFacturas);
                        while (fread(&facturaGuardada, sizeof(Factura), 1, archivoFacturas) == 1)
                        {
                            cantidadFacturas++;
                            if (facturaGuardada.nroFactura >= siguienteNroFactura)
                            {
                                siguienteNroFactura = facturaGuardada.nroFactura + 1;
                            }
                        }

                        if (ferror(archivoFacturas))
                        {
                            printf("No se pudo leer el archivo de facturas.\n");
                        }
                        else if (generarFactura(siguienteNroFactura, &facturaActual, detallesFactura,
                                                MAX_DETALLES_FACTURA, &cantidadDetallesFactura,
                                                &cantidadFacturas, archivoFacturas, archivoProductos, archivoDetalles))
                        {
                            printf("\nFactura guardada correctamente.\n");
                        }
                    }

                    if (archivoProductos != NULL)
                    {
                        fclose(archivoProductos);
                    }
                    if (archivoFacturas != NULL)
                    {
                        fclose(archivoFacturas);
                    }
                    if (archivoDetalles != NULL)
                    {
                        fclose(archivoDetalles);
                    }
                    break;
                }
                case op2:
                    limpiarPantalla();
                    {
                        FILE *archivoFacturas = fopen("bd_Facturas.dat", "rb");
                        FILE *archivoDetalles = fopen("bd_Detalles.dat", "rb");

                        if (archivoFacturas == NULL)
                        {
                            printf("No hay facturas para mostrar.\n");
                        }
                        else
                        {
                            verListadoFacturas(archivoFacturas, archivoDetalles);
                        }

                        if (archivoDetalles == NULL && archivoFacturas != NULL)
                        {
                            printf("No se pudo abrir el archivo de detalles de facturas.\n");
                        }
                        if (archivoFacturas != NULL)
                        {
                            fclose(archivoFacturas);
                        }
                        if (archivoDetalles != NULL)
                        {
                            fclose(archivoDetalles);
                        }
                    }
                    break;
                case op3:
                    printf("CREAR FUNCION ELIMINAR FACTURA DE LA BD");
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
                }
                if (op != cantOpcionesFacturas)
                    pausa();
            } while (op != cantOpcionesFacturas);
            break;

        case op2:

            op = elegirOpcion("SECCION ESTADISTICAS", vecMenuEstadisticas, cantOpcionesEstadisticas);
            do
            {
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
                }
                if (op != cantOpcionesEstadisticas)
                    pausa();
            } while (op != cantOpcionesEstadisticas);
            break;
        case op3:
            op = elegirOpcion("SECCION PRODUCTOS", vecMenuProductos, cantOpcionesProductos);
            FILE *archivoProductos; // creacion y apertura del archivo de productos. Si existe lo abre, si no existe lo crea

            if ((archivoProductos = fopen("bd_Products.dat", "rb+")) == NULL)
            {
                archivoProductos = fopen("bd_Products.dat", "wb+");
            }
            do
            {
                switch (op)
                {
                case op1:
                    verListadoDeProductos(archivoProductos);
                    break;
                case op2:
                    printf("Desarrollar agregar producto nuevo\n");
                    break;
                case op3:
                    printf("Dar de baja producto \n");
                    break;
                default:
                    printf("Error: Fuera de rango. Reintente.\n");
                    break;
                }
                if (op != cantOpcionesProductos)
                    pausa();

            } while (op != cantOpcionesProductos);
            fclose(archivoProductos);
            break;
        }
        if (op != cantOpcionesMenuPrincipal)
            pausa();
    } while (op != cantOpcionesMenuPrincipal);
}

// *********************************************************************************
// Alta Gestor
// Usa buffers propios: antes pisaba la variable "clave" de LogeoAdmin con la
// contraseña del gestor, y eso rompía la condición del ciclo de login.
// *********************************************************************************
void AltaGestor(FILE *archivoUsuarios)
{

    printf("**********Registrar Gestor**********\n\n");

    int resultado = registrarUsuario(archivoUsuarios);

    if (resultado == USUARIO_EXISTE)
    {
        printf("Usuario ya existente, reintente\n");
    }
    else if (resultado == SALIDA)
    {
        printf("No se registro el usario (cancelado)");
    }
    else
    {
        printf("\n¡Registro exitoso!\n");
    }

    pausa();
}

int buscarPosicionRegistro(FILE *archivoUsuario, char *nombre_usuario_baja)
{
    Usuario user;
    int posicion = 0;
    if (strcmp(nombre_usuario_baja, "-1") == 0)
    {
        return SALIDA;
    }
    while (fread(&user, sizeof(Usuario), 1, archivoUsuario)) // fread retorna 1 si esta bien, mientras que exista. cuando no haya mas que sea endoffile, termina (porque sería 0)
    {

        if (strcmp(user.nombreUsuario, nombre_usuario_baja) == 0)
        {
            return posicion;
        }
        else
        {
            return USUARIO_NO_ENCONTRADO;
        }
        posicion++;
    }
}

void BajaGestor(FILE *archivoUsuarios)
{
    printf("**********Dar de baja un Gestor**********\n\n");
    char nombre[LARGO_USUARIO];
    int posicionRegistro = 0;
    Usuario user;
    leerCadena(nombre, LARGO_USUARIO, "Ingrese el nombre del gestor a dar de baja (-1 para salir): ");
    while (nombre[0] == '\0')
    {
        printf("El nombre no puede estar vacío.\n");
        leerCadena(nombre, LARGO_USUARIO, "Ingrese el nombre del gestor a dar de baja (-1 para salir):  ");
    }
    if (strcmp(nombre, "-1") == 0)
    {
        printf("No se registró el usuario (cancelado)\n");
    }
    posicionRegistro = buscarPosicionRegistro(archivoUsuarios, nombre);
    if (posicionRegistro == USUARIO_NO_ENCONTRADO)
    {
        printf("No se ha encontrado el usuario\n");
    }
    else
    {
        fseek(archivoUsuarios, posicionRegistro * sizeof(Usuario), SEEK_SET);
        fread(&user, sizeof(Usuario), 1, archivoUsuarios);
        user.activo = INACTIVO; // se trabaja una BAJA LOGICA, no estamos borrando por completo el registro ya que no hay manera de deshacerlo. la buena practica es desactivar registros, no eliminarlos por completo
        printf("Gestor desactivado correctamente\n");
    }
}

// *********************************************************************************
// MENÚ ADMIN
// *********************************************************************************
void MenuAdmin()
{
    FILE *archivoUsuarios;
    if ((archivoUsuarios = fopen("bd_Users.dat", "rb+")) == NULL)
    {
        archivoUsuarios = fopen("bd_Users.dat", "wb+");
    }

    int op = 0;
    // char vecMenuAdministrador[4][50]= {"1. Dar de alta a un gestor\n", "2. Dar de baja a un gestor\n", "0.Salir"};
    const char *menu[] = {"Dar de alta a un gestor\n",
                          "Dar de baja a un gestor\n",
                          "Salir"};
    int cant = sizeof(menu) / sizeof(menu[0]);
    prepararConsola();
    // abrir archivo de usuarios

    do
    {

        op = elegirOpcion("Menu Administrador", menu, cant);

        switch (op)
        {
        case op1:
            limpiarPantalla();
            AltaGestor(archivoUsuarios);

            break;
        case op2:
            limpiarPantalla();
            BajaGestor(archivoUsuarios);

            break;
        }
        if (op != cant)
            pausa();
    } while (op != cant);
    fclose(archivoUsuarios);
}

// *********************************************************************************
// LOGUEO ADMIN
// Devuelve CANCELAR si se agotaron los intentos (cierra el programa),
// 0 en cualquier otro caso (vuelve a la pantalla de selección de rol).
// *********************************************************************************
int LogeoAdmin()
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
            MenuAdmin();
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

int contarRegistrosUsuarios(FILE *archivoUsers)
{
    int cantidad = 0;
    Usuario user;
    while (fread(&user, sizeof(Usuario), 1, archivoUsers))
    {
        cantidad++;
    }
    return cantidad;
}

// *********************************************************************************
// LOGUEO GESTOR
// Valida usuario y contraseña contra el arreglo de usuarios (usa validarLogin).
// Devuelve true si el login fue exitoso.
// *********************************************************************************
bool LogeoGestor(FILE *archivoUsuarios)
{
    char nombre[LARGO_USUARIO];
    char clave[LARGO_CLAVE];
    int intentos = MAXIMO_INTENTOS;
    int cantidadUsuarios = 0;
    cantidadUsuarios = contarRegistrosUsuarios(archivoUsuarios);

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

        if (validarLogin(archivoUsuarios, nombre, clave) == PERMI_GESTOR)
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
    int i;
    int cantVueltas = 2; // Número de veces que se alternarán las poses

    for (i = 0; i < cantVueltas; i++)
    {
        limpiarPantalla();
        mostrar_pose1();
        retardo2(0.1);
        limpiarPantalla();
        mostrar_pose2();
        retardo2(0.1);
    }
}

// Carga los datos del cliente y sus productos, calcula los importes y devuelve si se genero la factura.
bool generarFactura(int nroFactura, Factura *factura, DetalleFactura detalles[], int capacidadDetalles, int *cantidadDetalles, int *cantidadFacturas, FILE *archivoFacturas, FILE *archivoProductos, FILE *archivoDetalles)
{
    // Inicializo las variables
    Factura nuevaFactura = {0};
    int cantidadLineas = 0;
    float netoGravado = 0;
    bool facturaGenerada = false;
    time_t ahora;
    struct tm *fechaActual;

    if (factura == NULL || detalles == NULL || cantidadDetalles == NULL || cantidadFacturas == NULL ||
        archivoFacturas == NULL || archivoProductos == NULL || archivoDetalles == NULL ||
        nroFactura <= 0 || capacidadDetalles <= 0)
    {
        printf("No se puede generar la factura: los datos recibidos no son válidos.\n");
    }
    else
    {

        *cantidadDetalles = 0; // Inicializo la cantidad de detalles en 0 antes de comenzar a cargar la factura
        nuevaFactura.nroFactura = nroFactura; // Asigno el número de factura recibido como parámetro a la nueva factura

        do
        {
            leerEntero("Ingrese el código del cliente: ", &nuevaFactura.codigoCliente); // Solicito al usuario que ingrese el código del cliente y lo almaceno en la nueva factura
            if (nuevaFactura.codigoCliente <= 0) // Valido que el código del cliente sea mayor que cero, ya que no puede ser negativo ni cero
            {
                printf("El código del cliente debe ser mayor que cero.\n"); 
            }
        } while (nuevaFactura.codigoCliente <= 0); // Repetir la solicitud hasta que se ingrese un código válido

        do
        {
            leerCadena(nuevaFactura.clienteSnapshot.nombreYApellido, LARGO_NOMBRE, "Ingrese el nombre del cliente: "); // Solicito al usuario que ingrese el nombre del cliente y lo almaceno en la nueva factura
        } while (nuevaFactura.clienteSnapshot.nombreYApellido[0] == '\0');
        printf("\n");
        nuevaFactura.alicuotaIVA = 0;
        nuevaFactura.importeIVA = 0;
        nuevaFactura.estado = FACTURA_EMITIDA; // Asigno el estado de la factura como emitida, ya que se está generando una nueva factura

        ahora = time(NULL); // Obtengo la fecha y hora actual del sistema
        fechaActual = localtime(&ahora); // Convierto la fecha y hora actual a una estructura de tiempo local

        if (fechaActual == NULL) // Verifico si la conversión a tiempo local fue exitosa
        {
            printf("No se pudo obtener la fecha de emisión.\n");
        }
        else // Si la conversión fue exitosa, asigno los valores de día, mes y año a la fecha de emisión de la nueva factura
        {
            nuevaFactura.fechaEmision.dia = fechaActual->tm_mday;
            nuevaFactura.fechaEmision.mes = fechaActual->tm_mon + 1;
            nuevaFactura.fechaEmision.anio = fechaActual->tm_year + 1900;

            int codigoIngresado = 0; // Variable para almacenar el código del producto ingresado por el usuario
            do
            {
                DetalleFactura detalle = {0}; // Inicializo la estructura de detalle de factura para almacenar los datos del producto ingresado
                float precioUnitario = 0; //
                float nuevoNeto = 0;
                bool lineaValida = true;

                leerEntero("Ingrese el código del producto (0 para finalizar): ", &codigoIngresado);
                detalle.codigoArticulo = codigoIngresado;
                printf("\n");
                if (codigoIngresado != 0)
                {

                    if (detalle.codigoArticulo < 0)
                    {
                        printf("El código del producto debe ser mayor que cero.\n");
                        lineaValida = false;
                    }

                    if (lineaValida)
                    {
                        Articulo producto = {0};
                        bool productoEncontrado = false;

                        rewind(archivoProductos);
                        while (fread(&producto, sizeof(Articulo), 1, archivoProductos) == 1)
                        {
                            if (producto.codigo == codigoIngresado)
                            {
                                productoEncontrado = true;
                                break;
                            }
                        }

                        if (ferror(archivoProductos))
                        {
                            printf("No se pudo leer el archivo de productos.\n");
                            return false;
                        }

                        if (!productoEncontrado)
                        {
                            printf("No existe un producto con el código ingresado. Intente nuevamente.\n");
                            lineaValida = false;
                        }
                        else
                        {
                            memcpy(detalle.descripcion, producto.descripcion, sizeof(detalle.descripcion));
                            detalle.descripcion[sizeof(detalle.descripcion) - 1] = '\0';
                            detalle.precioUnitario = producto.precio;

                            do
                            {
                                leerEntero("Ingrese la cantidad: ", &detalle.cantidad);
                                if (detalle.cantidad <= 0)
                                {
                                    printf("La cantidad debe ser mayor que cero.\n");
                                }
                            } while (detalle.cantidad <= 0);

                            detalle.nroFactura = nroFactura;
                            detalle.subtotalLinea = detalle.precioUnitario * detalle.cantidad;
                            nuevoNeto = netoGravado + detalle.subtotalLinea;
                        }
                    }

                    if (lineaValida)
                    {
                        detalles[cantidadLineas] = detalle;
                        netoGravado = nuevoNeto;
                        cantidadLineas++;
                    }
                }
            } while (cantidadLineas < capacidadDetalles && codigoIngresado != 0);

            // Comprueba si se ingresaron productos en la factura y, si es así, calcula el total y muestra la factura generada.
            if (cantidadLineas == 0)
            {
                printf("La factura no contiene productos; no se generó.\n");
            }
            else
            {
                nuevaFactura.netoGravado = netoGravado;
                nuevaFactura.totalFactura = netoGravado + nuevaFactura.importeIVA;

                if (fseek(archivoDetalles, 0, SEEK_END) != 0 ||
                    fwrite(detalles, sizeof(DetalleFactura), cantidadLineas, archivoDetalles) != (size_t)cantidadLineas ||
                    fflush(archivoDetalles) != 0 ||
                    fseek(archivoFacturas, 0, SEEK_END) != 0 ||
                    fwrite(&nuevaFactura, sizeof(Factura), 1, archivoFacturas) != 1 ||
                    fflush(archivoFacturas) != 0)
                {
                    printf("No se pudo guardar la factura y sus detalles en los archivos.\n");
                }
                else
                {
                    *factura = nuevaFactura;
                    *cantidadDetalles = cantidadLineas;

                    printf("\nFactura N° %d generada el %02d/%02d/%04d\n",
                           factura->nroFactura, factura->fechaEmision.dia, factura->fechaEmision.mes, factura->fechaEmision.anio);
                    printf("Cliente: %d - %s\n", factura->codigoCliente, factura->clienteSnapshot.nombreYApellido);
                    printf("%-10s | %-50s | %17s | %10s | %12s\n", "Código", "Descripción", "Precio unitario", "Cantidad", "Total");
                    for (int i = 0; i < cantidadLineas; i++) // Recorre los detalles de la factura
                    {
                        printf("%-10d | %-50s | $%16.2f | %10d | $%11.2f\n",
                               detalles[i].codigoArticulo, detalles[i].descripcion,
                               detalles[i].precioUnitario, detalles[i].cantidad, detalles[i].subtotalLinea);
                    }
                    printf("Total de la factura: $%.2f\n", factura->totalFactura);
                    (*cantidadFacturas)++;
                    facturaGenerada = true;
                }
            }
        }
    }

    return facturaGenerada;
}

void verListadoFacturas(FILE *archivoFacturas, FILE *archivoDetalles)
{
    Factura factura;
    DetalleFactura detalle;
    int cantidadFacturas = 0;

    if (archivoFacturas == NULL)
    {
        printf("No se pudo leer el archivo de facturas.\n");
        return;
    }

    rewind(archivoFacturas);
    while (fread(&factura, sizeof(Factura), 1, archivoFacturas) == 1)
    {
        cantidadFacturas++;
        printf("\nFactura N° %d - Fecha: %02d/%02d/%04d - Cliente: %d - %s\n",
               factura.nroFactura, factura.fechaEmision.dia, factura.fechaEmision.mes,
               factura.fechaEmision.anio, factura.codigoCliente, factura.clienteSnapshot.nombreYApellido);
        printf("%-10s | %-50s | %17s | %10s | %12s\n", "Código", "Descripción", "Precio unitario", "Cantidad", "Total");

        if (archivoDetalles != NULL)
        {
            rewind(archivoDetalles);
            while (fread(&detalle, sizeof(DetalleFactura), 1, archivoDetalles) == 1)
            {
                if (detalle.nroFactura == factura.nroFactura)
                {
                    printf("%-10d | %-50s | $%16.2f | %10d | $%11.2f\n",
                           detalle.codigoArticulo, detalle.descripcion,
                           detalle.precioUnitario, detalle.cantidad, detalle.subtotalLinea);
                }
            }
            if (ferror(archivoDetalles))
            {
                printf("Error al leer el archivo de detalles de facturas.\n");
                return;
            }
        }

        printf("Total de la factura: $%.2f\n", factura.totalFactura);
    }

    if (ferror(archivoFacturas))
    {
        printf("Error al leer el archivo de facturas.\n");
    }
    else if (cantidadFacturas == 0)
    {
        printf("No hay facturas para mostrar.\n");
    }
}

#endif /* FACTUFUNCIONES_H_ */
