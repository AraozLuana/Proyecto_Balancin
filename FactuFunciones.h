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
#define ERROR (-1)

// Secciones de MenuUsuario: el retorno es SECCION + opcion (ej: SECC_PRODUCTOS + op2)
#define SECC_FACTURAS 10
#define SECC_ESTADISTICAS 20
#define SECC_PRODUCTOS 30

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
int MenuUsuario();
int MenuAdmin();
void AltaGestor(FILE *archivoUsuarios);
void pantallaCarga();
void prepararConsola(void);
void mostrarMenuAdmin(const char *titulo, const char *opciones[], int cant, int sel);
void mostrarMenu(const char *titulo, const char *opciones[], int cant, int sel);
int contarRegistrosUsuarios(FILE *archivoUsers);
int elegirOpcion(const char *titulo, const char *opciones[], int cant);
void verListadoDeProductos(FILE *archivoProductos);
int registrarProducto(FILE *archivoProducts);
bool generarFactura(int nroFactura, Factura *factura, DetalleFactura detalles[], int capacidadDetalles, int *cantidadDetalles,
                    int *cantidadFacturas, FILE *archivoFacturas, FILE *archivoProductos, FILE *archivoDetalles,
                    FILE *archivoClientes);
void verListadoFacturas(FILE *archivoFacturas, FILE *archivoDetalles);
void eliminarFactura(FILE *archivoFacturas, FILE *archivoDetalles, int nroFactura);
int registrarProducto(FILE *archivoProducts);
int buscarPosicionRegistroProductos(FILE *archivoProductos, int codigoProductoBaja);
void BajaGestor(FILE *archivoUsuarios);
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
    const char *menu[] = {"Administrador", "Gestor", "Salir"};
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
        leerFlotante("El precio no puede ser ni 0 ni  negativo.Ingrese el precio unitario de este producto (-1 para salir)", &product.precio);
    }

    if (product.precio == SALIDA)
    {
        return SALIDA;
    }
    leerEntero("Ingrese la cantidad vendida de este producto (-1) para salir: ", &product.cantVendida);
    while (product.cantVendida < SALIDA)
    {
        leerEntero("La cantidad vendida no puede negativa. Ingrese la cantidad vendida de este producto (-1) para salir:(-1 para salir)", &product.cantVendida);
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

int MenuUsuario()
{
    int seccion = 0;
    int op = 0;
    int resultado = -1; /* -1 = todavía no hay nada que devolver */
    const char *VecMenuPrincipal[] = {"Facturas", "Estadisticas", "Productos", "Salir"};
    const char *vecMenuFacturas[] = {"Generar nueva factura", "Ver listado de facturas", "Eliminar factura de la base de datos", "Ver listado de clientes", "Agregar cliente", "Eliminar cliente", "Modificar cliente", "Volver"};
    const char *vecMenuEstadisticas[] = {"Ver producto estrella", "Ver producto mas vendido", "Ver mejores clientes", "Ver ventas por zona", "Volver"};
    const char *vecMenuProductos[] = {"Ver listado de productos", "Agregar producto nuevo", "Dar de baja un producto", "Volver"};
// Calcular la cantidad de opciones en cada menú
    int cantOpcionesMenuPrincipal = sizeof(VecMenuPrincipal) / sizeof(VecMenuPrincipal[0]);
    int cantOpcionesFacturas = sizeof(vecMenuFacturas) / sizeof(vecMenuFacturas[0]);
    int cantOpcionesEstadisticas = sizeof(vecMenuEstadisticas) / sizeof(vecMenuEstadisticas[0]);
    int cantOpcionesProductos = sizeof(vecMenuProductos) / sizeof(vecMenuProductos[0]);
 
    utf8();
    prepararConsola();

    do
    {
        limpiarPantalla();
        seccion = elegirOpcion("Menu Gestor", VecMenuPrincipal, cantOpcionesMenuPrincipal);

        switch (seccion)
        {
        case op1:
            op = elegirOpcion("SECCION FACTURAS", vecMenuFacturas, cantOpcionesFacturas);
            if (op != cantOpcionesFacturas) //validacion por si se selecciona la opcion de volver, que no haga nada y vuelva al menu principal
            {
                resultado = SECC_FACTURAS + op;
            }
            break;
        case op2:
            op = elegirOpcion("SECCION ESTADISTICAS", vecMenuEstadisticas, cantOpcionesEstadisticas);
            if (op != cantOpcionesEstadisticas)
            {
                resultado = SECC_ESTADISTICAS + op;
            }
            break;
        case op3:
            op = elegirOpcion("SECCION PRODUCTOS", vecMenuProductos, cantOpcionesProductos);
            if (op != cantOpcionesProductos)
            {
                resultado = SECC_PRODUCTOS + op;
            }
            break;
        default:
            resultado = SALIR;
            break;
        }
    } while (resultado == -1);

    return resultado;
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
int MenuAdmin()
{
    const char *menu[] = {"Dar de alta a un gestor\n",
                          "Dar de baja a un gestor\n",
                          "Salir"};
    int cant = sizeof(menu) / sizeof(menu[0]);
    prepararConsola();

    return elegirOpcion("Menu Administrador", menu, cant);
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
            return EXITO;
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
    rewind(archivoUsers);
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
bool generarFactura(int nroFactura, Factura *factura, DetalleFactura detalles[], int capacidadDetalles, int *cantidadDetalles, int *cantidadFacturas, FILE *archivoFacturas, FILE *archivoProductos, FILE *archivoDetalles, FILE *archivoClientes)
{
    // Inicializo las variables
    Factura nuevaFactura = {0};
    int cantidadLineas = 0;
    float netoGravado = 0;
    bool facturaGenerada = false;
    time_t ahora;
    struct tm *fechaActual;

    if (factura == NULL || detalles == NULL || cantidadDetalles == NULL || cantidadFacturas == NULL ||
        archivoFacturas == NULL || archivoProductos == NULL || archivoDetalles == NULL || archivoClientes == NULL ||
        nroFactura <= 0 || capacidadDetalles <= 0)
    {
        printf("No se puede generar la factura: los datos recibidos no son válidos.\n");
    }
    else
    {

        *cantidadDetalles = 0;                // Inicializo la cantidad de detalles en 0 antes de comenzar a cargar la factura
        nuevaFactura.nroFactura = nroFactura; // Asigno el número de factura recibido como parámetro a la nueva factura

        {
            Cliente clienteEncontrado = {0};
            bool clienteValido = false;

            do
            {
                leerEntero("Ingrese el código del cliente: ", &nuevaFactura.codigoCliente);
                if (nuevaFactura.codigoCliente <= 0)
                {
                    printf("El código del cliente debe ser mayor que cero.\n");
                    continue;
                }

                rewind(archivoClientes);
                while (!clienteValido && fread(&clienteEncontrado, sizeof(Cliente), 1, archivoClientes) == 1)
                {
                    if (clienteEncontrado.codigo == nuevaFactura.codigoCliente &&
                        clienteEncontrado.activo == ACTIVO)
                    {
                        clienteValido = true;
                    }
                }

                if (!clienteValido)
                {
                    printf("No existe un cliente activo con ese código. Intente nuevamente.\n");
                }
            } while (!clienteValido);

            nuevaFactura.clienteSnapshot = clienteEncontrado.datos;
            nuevaFactura.clienteSnapshot.nombreYApellido[LARGO_NOMBRE - 1] = '\0';
        }
        printf("\n");
        nuevaFactura.alicuotaIVA = 0;
        nuevaFactura.importeIVA = 0;
        nuevaFactura.estado = FACTURA_EMITIDA; // Asigno el estado de la factura como emitida, ya que se está generando una nueva factura

        ahora = time(NULL);              // Obtengo la fecha y hora actual del sistema
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
                        while (!productoEncontrado && fread(&producto, sizeof(Articulo), 1, archivoProductos) == 1)
                        {
                            if (producto.codigo == codigoIngresado)
                            {
                                productoEncontrado = true;
                            }
                        }

                        if (!productoEncontrado)
                        {
                            printf("No existe un producto con el código ingresado. Intente nuevamente.\n");
                            lineaValida = false;
                        }
                        else
                        {
                            for (int i = 0; i < LARGO_DESC - 1; i++)
                            {
                                detalle.descripcion[i] = producto.descripcion[i];
                            }
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
        }

        printf("Total de la factura: $%.2f\n", factura.totalFactura);
    }

    if (cantidadFacturas == 0)
    {
        printf("No hay facturas para mostrar.\n");
    }
}

void eliminarFactura(FILE *archivoFacturas, FILE *archivoDetalles, int nroFactura)
{
    Factura factura;
    DetalleFactura detalle;
    bool facturaEncontrada = false;

    if (archivoFacturas == NULL || archivoDetalles == NULL)
    {
        printf("No se pudo acceder a los archivos de facturas o detalles.\n");
        return;
    }

    rewind(archivoFacturas);
    while (!facturaEncontrada && fread(&factura, sizeof(Factura), 1, archivoFacturas) == 1)
    {
        if (factura.nroFactura == nroFactura)
        {
            facturaEncontrada = true;
        }
    }

    if (!facturaEncontrada)
    {
        printf("No se encontró la factura con número %d.\n", nroFactura);
        return;
    }

    // Marcar la factura como anulada
    factura.estado = FACTURA_ANULADA;
    fseek(archivoFacturas, -sizeof(Factura), SEEK_CUR);
    fwrite(&factura, sizeof(Factura), 1, archivoFacturas);
    fflush(archivoFacturas);

    // Eliminar los detalles asociados a la factura
    rewind(archivoDetalles);
    while (fread(&detalle, sizeof(DetalleFactura), 1, archivoDetalles) == 1)
    {
        if (detalle.nroFactura == nroFactura)
        {
            // Aquí podrías implementar una lógica para eliminar o marcar los detalles como eliminados
            // Por ejemplo, podrías establecer un campo de estado en DetalleFactura si lo tuvieras
        }
    }

    printf("Factura N° %d anulada correctamente.\n", nroFactura);
}

#endif /* FACTUFUNCIONES_H_ */
