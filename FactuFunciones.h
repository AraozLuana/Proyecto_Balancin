#ifndef FACTUFUNCIONES_H_
#define FACTUFUNCIONES_H_
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

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

#define CLAVE_ADMIN "admin123"
#define MAXIMO_INTENTOS 4
#define CANCELAR -2
#define SALIR 0
#define FLECHA_ARRIBA 72
#define FLECHA_ABAJO 80
#define ENTER 13
#define ANCHO_LINEA 40
#define ACTIVO 1
#define INACTIVO 0

// Resultados de validarLogin
#define CONTRA_INCORRECTA 0
#define NO_ENCONTRADO -1

// Resultados de ambas funciones de registrar
#define EXITO 1
#define EXISTE_BD 2
#define SALIDA -1

// *********************************************************************************
// PROTOTIPOS
// *********************************************************************************
int buscarPosicionRegistroUsuario(FILE *archivoUsuario, char *nombre_usuario_baja);
int validarLogin(FILE *archivoUsuarios, char *nombreUsuario, char *clave);
int registrarUsuario(FILE *archivoUsers);
int iniciarSesion();
int LogeoAdmin();
bool LogeoGestor(FILE *archivoUsuarios);
void MenuUsuario();
void MenuAdmin();
void AltaGestor(FILE *archivoUsuarios);
void pantallaCarga();
void prepararConsola(void);
int elegirOpcion(const char *titulo, const char *opciones[], int cant);
void verListadoDeProductos(FILE *archivoProductos);
void mostrarMenu(const char *titulo, const char *opciones[], int cant, int sel);
int registrarProducto(FILE *archivoProducts);
int buscarPosicionRegistroProductos(FILE *archivoProductos, int codigoProductoBaja);
void BajaGestor(FILE *archivoUsuarios);
int contarRegistrosUsuarios(FILE *archivoUsers);
void mostrar_pose1();
void mostrar_pose2();

// *********************************************************************************
// Funciones para el menu con cursores
// *********************************************************************************
void prepararConsola(void)
{
    HANDLE hSalida = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD modo = 0;

    SetConsoleOutputCP(CP_UTF8);
    GetConsoleMode(hSalida, &modo);
    SetConsoleMode(hSalida, modo | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}
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
int buscarPosicionRegistroUsuario(FILE *archivoUsuario, char *nombre_usuario_baja)
{
    Usuario user;
    int posicion = 0;
    if (strcmp(nombre_usuario_baja, "-1") == 0)
    {
        return SALIDA;
    }
    rewind(archivoUsuario);
    while (fread(&user, sizeof(Usuario), 1, archivoUsuario)) // fread retorna 1 si esta bien, mientras que exista. cuando no haya mas que sea endoffile, termina (porque sería 0)
    {

        if (strcmp(user.nombreUsuario, nombre_usuario_baja) == 0)
        {
            return posicion;
        }

        posicion++;
    }
    return NO_ENCONTRADO;
}
int validarLogin(FILE *archivoUsuarios, char *nombreUsuario, char *clave)
{
    int posicion = buscarPosicionRegistroUsuario(archivoUsuarios, nombreUsuario);
    Usuario user;
    if (posicion < 0)
    {
        return NO_ENCONTRADO;
    }
    fseek(archivoUsuarios, posicion * sizeof(Usuario), SEEK_SET);
    fread(&user, sizeof(Usuario), 1, archivoUsuarios);
    if (user.activo == INACTIVO)
    {
        return NO_ENCONTRADO;
    }
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
    char respuesta[LARGO_RESPUESTA];
    int posicion = 0;
    int i;

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

    rewind(archivoUsers);
    while (fread(&busqueda, sizeof(Usuario), 1, archivoUsers) == 1)
    {
        if (strcmp(busqueda.nombreUsuario, user.nombreUsuario) == 0)
        {
            if (busqueda.activo == ACTIVO)
            {
                return EXISTE_BD;
            }

            leerCadena(respuesta, LARGO_RESPUESTA, "Este usuario esta desactivado. Desea activarlo [SI/NO]? ");
            for (i = 0; respuesta[i] != '\0'; i++)
            {
                respuesta[i] = toupper((unsigned char)respuesta[i]);
            }
            printf("DEBUG respuesta = [%s] largo = %d\n", respuesta, (int)strlen(respuesta));
            if (strcmp(respuesta, "SI") == 0 || strcmp(respuesta, "S") == 0)
            {
                strcpy(busqueda.claveHash, user.claveHash);
                busqueda.activo = ACTIVO;

                fseek(archivoUsers, (long)posicion * sizeof(Usuario), SEEK_SET);
                fwrite(&busqueda, sizeof(Usuario), 1, archivoUsers);
                fflush(archivoUsers);
                return EXITO;
            }
            return SALIDA;
        }
        posicion++;
    }

    fseek(archivoUsers, 0L, SEEK_END);
    fwrite(&user, sizeof(Usuario), 1, archivoUsers);
    fflush(archivoUsers);
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
// FUNCIONES DEL MENU DE PRODUCTOS
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
    printf("%-6s %-22s %-10s %10s %-17s %-8s\n",
           "COD", "NOMBRE", "STOCK", "PRECIO", "CANTIDAD VENDIDA", "ESTADO");
    while (fread(&product, sizeof(Articulo), 1, archivoProductos) == 1) // significa que mientras que haya registro
    {
        printf("%-6d %-22s %-10d %10.2f %-17d %-8s\n",
               product.codigo, product.descripcion, product.stock,
               product.precio, product.cantVendida,
               product.estado == ACTIVO ? "ACTIVO" : "INACTIVO");
    }
}
int registrarProducto(FILE *archivoProducts)
{
    Articulo product;
    Articulo busqueda;
    product.estado = ACTIVO;

    leerEntero("Ingrese el codigo del producto que desea agregar (-1 para salir)", &product.codigo);
    while (product.codigo == 0 && product.codigo < SALIDA)
    {
        leerEntero("El codigo no puede ser ni 0 ni negativo.Ingrese el codigo del producto que desea agregar (-1 para salir)", &product.codigo);
    }
    if (product.codigo == SALIDA)
    {
        return SALIDA;
    }

    leerCadena(product.descripcion, LARGO_USUARIO, "Descripcion del producto (-1 para salir): ");
    while (product.descripcion[0] == '\0')
    {
        printf("La descripcion  no puede estar vacía.\n");
        leerCadena(product.descripcion, LARGO_USUARIO, "Descripcion del producto (-1 para salir): ");
    }
    if (strcmp(product.descripcion, "-1") == 0)
    {
        return SALIDA;
    }
    leerEntero("Ingrese la cantidad de stock disponible de este producto (-1) para salir: ", &product.stock);
    while (product.codigo < SALIDA)
    {
        leerEntero("El stock no pued eser  negativo.Ingrese la cantidad de stock disponible de este producto(-1 para salir)", &product.stock);
    }
    if (product.stock == SALIDA)
    {
        return SALIDA;
    }
    leerFlotante("Ingrese el precio unitario de este producto (-1) para salir: ", &product.precio);
    while (product.codigo < SALIDA && product.precio == 0)
    {
        leerEntero("El precio no puede ser ni 0 ni  negativo.Ingrese el precio unitario de este producto (-1 para salir)", &product.precio);
    }

    if (product.precio == SALIDA)
    {
        return SALIDA;
    }
    leerEntero("Ingrese la cantidad vendida de este producto (-1) para salir: ", &product.cantVendida);
    while (product.cantVendida < SALIDA)
    {
        leerEntero("La cantidad vendida no puede negativa. Ingrese la cantidad vendida de este producto (-1) para salir:(-1 para salir)", &product.precio);
    }

    if (product.cantVendida == SALIDA)
    {
        return SALIDA;
    }

    while (fread(&busqueda, sizeof(Articulo), 1, archivoProducts)) // fread retorna 1 si esta bien, mientras que exista. cuando no haya mas que sea endoffile, termina (porque sería 0)
    {
        if (busqueda.codigo == product.codigo)
        {
            return EXISTE_BD;
        }
    }
    int write = fwrite(&product, sizeof(Articulo), 1, archivoProducts);
    printf("%d", write);

    return EXITO;
}
int buscarPosicionRegistroProductos(FILE *archivoProductos, int codigoProductoBaja)
{
    Articulo product;
    int posicion = 0;
    while (fread(&product, sizeof(Articulo), 1, archivoProductos)) // fread retorna 1 si esta bien, mientras que exista. cuando no haya mas que sea endoffile, termina (porque sería 0)
    {

        if (product.codigo == codigoProductoBaja)
        {
            return posicion;
        }
        posicion++;
    }
    if (product.codigo != codigoProductoBaja)
    {
        return NO_ENCONTRADO;
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
    char *vecMenuProductos[] = {"Ver listado de productos", "Agregar producto nuevo", "Dar de baja un producto", "Volver"};

    utf8();
    limpiarPantalla();
    int cantOpcionesMenuPrincipal = sizeof(VecMenuPrincipal) / sizeof(VecMenuPrincipal[0]);
    int cantOpcionesFacturas = sizeof(vecMenuFacturas) / sizeof(vecMenuFacturas[0]);
    int cantOpcionesEstadisticas = sizeof(vecMenuEstadisticas) / sizeof(vecMenuEstadisticas[0]);
    int cantOpcionesProductos = sizeof(vecMenuProductos) / sizeof(vecMenuProductos[0]);
    // Creacion de los archivos
    FILE *archivoProductos;
    prepararConsola();

    do
    {
        limpiarPantalla();
        op = elegirOpcion("Menu Gestor", VecMenuPrincipal, cantOpcionesMenuPrincipal);

        switch (op)
        {
        case op1:

            printf("\n");

            do
            {
                op = elegirOpcion("SECCION FACTURAS", vecMenuFacturas, cantOpcionesFacturas);
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
                }
                if (op != cantOpcionesFacturas)
                    pausa();
            } while (op != cantOpcionesFacturas);
            break;

        case op2:

            do
            {
                op = elegirOpcion("SECCION ESTADISTICAS", vecMenuEstadisticas, cantOpcionesEstadisticas);
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

            if ((archivoProductos = fopen("bd_Products.dat", "rb+")) == NULL)
            {
                archivoProductos = fopen("bd_Products.dat", "wb+");
            }

            do
            {
                rewind(archivoProductos);
                op = elegirOpcion("SECCION PRODUCTOS", vecMenuProductos, cantOpcionesProductos);
                switch (op)

                {
                case op1:

                    verListadoDeProductos(archivoProductos);
                    pausa();
                    break;
                case op2:
                    printf("**********Registrar Producto**********\n\n");

                    int resultado = registrarProducto(archivoProductos);

                    if (resultado == EXISTE_BD)
                    {
                        printf("Producto ya existente, reintente\n");
                    }
                    else if (resultado == SALIDA)
                    {
                        printf("No se registro el Prodcuto (cancelado)");
                    }
                    else
                    {
                        printf("\n¡Registro exitoso!\n");
                    }

                    pausa();
                    break;
                case op3:
                    printf("**********Dar de baja un Producto**********\n\n");
                    int codigo = 0;
                    int posicionRegistro = 0;
                    Articulo product;
                    leerEntero("Ingrese el codigo del articulo que quiere dar de baja (-1 para salir)", &codigo);
                    if (codigo == SALIDA)
                    {
                        printf("No se hizo la baja  (cancelado)\n");
                    }
                    posicionRegistro = buscarPosicionRegistroProductos(archivoProductos, codigo);
                    if (posicionRegistro == NO_ENCONTRADO)
                    {
                        printf("No se ha encontrado el producto\n");
                    }
                    else
                    {

                        fseek(archivoProductos, posicionRegistro * sizeof(Articulo), SEEK_SET);
                        fread(&product, sizeof(Articulo), 1, archivoProductos);
                        product.estado = INACTIVO; // se trabaja una BAJA LOGICA, no estamos borrando por completo el registro ya que no hay manera de deshacerlo. la buena practica es desactivar registros, no eliminarlos por completo
                        fseek(archivoProductos, posicionRegistro * sizeof(Articulo), SEEK_SET);
                        fwrite(&product, sizeof(Articulo), 1, archivoProductos);
                        fflush(archivoProductos);

                        printf("Producto desactivado correctamente\n");
                    }
                    break;
                default:
                    printf("Error: Fuera de rango. Reintente.\n");
                    break;
                }

                fclose(archivoProductos);
                break;
                if (op != cantOpcionesProductos)
                {
                    pausa();
                }

            } while (op != cantOpcionesProductos);
        }
        if (op != cantOpcionesMenuPrincipal)
        {
            pausa();
        }
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

    if (resultado == EXISTE_BD)
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

// BAJA GESTOR: baja logica.
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
    posicionRegistro = buscarPosicionRegistroUsuario(archivoUsuarios, nombre);
    if (posicionRegistro == NO_ENCONTRADO)
    {
        printf("No se ha encontrado el usuario\n");
    }
    else
    {
        fseek(archivoUsuarios, (long)posicionRegistro * sizeof(Usuario), SEEK_SET);
        fread(&user, sizeof(Usuario), 1, archivoUsuarios);

        user.activo = INACTIVO; // baja lógica

        fseek(archivoUsuarios, (long)posicionRegistro * sizeof(Usuario), SEEK_SET); // volver al inicio del registro
        fwrite(&user, sizeof(Usuario), 1, archivoUsuarios);
        fflush(archivoUsuarios);

        printf("Gestor desactivado correctamente\n");
    }
}
// *********************************************************************************
// MENÚ ADMIN
// *********************************************************************************
void MenuAdmin()
{
    FILE *archivoUsuarios;
    if ((archivoUsuarios = fopen("bd_Users.dat", "rb+")) == NULL) // creacion y apertura del archivo de usuarios. Si existe, lo abre y utiliza, si no existe, lo crea y utiliza
    {
        archivoUsuarios = fopen("bd_Users.dat", "wb+");
    }

    int op = 0;
    const char *menu[] = {"Dar de alta a un gestor\n",
                          "Dar de baja a un gestor\n",
                          "Salir"};
    int cant = sizeof(menu) / sizeof(menu[0]);
    prepararConsola();

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
