#ifndef FACTUFUNCIONES_H_
#define FACTUFUNCIONES_H_
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>

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
#define MAX_DETALLES_FACTURA 20
#define ACTIVO 1
#define INACTIVO 0

#define MAX_CLIENTES 300
#define MAX_ARTICULOS 300

// Resultados de validarLogin
#define CONTRA_INCORRECTA 0
#define NO_ENCONTRADO -1

// Resultados de ambas funciones de registrar
#define EXITO 1
#define EXISTE_BD 2
#define SALIDA -2

// *********************************************************************************
// PROTOTIPOS
// *********************************************************************************
int buscarPosicionRegistroUsuario(FILE *archivoUsuario, char *nombre_usuario_baja);
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
void mostrarMenu(const char *titulo, const char *opciones[], int cant, int sel);
int contarRegistrosUsuarios(FILE *archivoUsers);
int elegirOpcion(const char *titulo, const char *opciones[], int cant);
void verListadoDeProductos(FILE *archivoProductos);
int registrarProducto(FILE *archivoProducts);
bool generarFactura(int nroFactura, Factura *factura, DetalleFactura detalles[], int capacidadDetalles, int *cantidadDetalles,
                    int *cantidadFacturas, FILE *archivoFacturas, FILE *archivoProductos, FILE *archivoDetalles,
                    FILE *archivoClientes);
void verListadoFacturas(FILE *archivoFacturas, FILE *archivoDetalles);
void eliminarFactura(FILE *archivoFacturas, FILE *archivoDetalles, FILE *archivoProductos, int nroFactura);
int registrarProducto(FILE *archivoProducts);
int buscarPosicionRegistroProductos(FILE *archivoProductos, int codigoProductoBaja);
void BajaGestor(FILE *archivoUsuarios);
void verListadoUsuarios(FILE *archivoUsers);
int buscarPosicionRegistroClientes(FILE *archivoClientes, int codigoClienteBaja);
void verListadoClientes(FILE *archivoClientes);
int registrarCliente(FILE *archivoCliente);
void mostrar_pose1();
void mostrar_pose2();

void productoEstrella(FILE *archivoFacturas, FILE *archivoDetalles);
void mejoresClientes(FILE *archivoFacturas);
void ProductoMasVendido(FILE *archivoFacturas, FILE *archivoDetalles);

// Producto mas vendido: El articulo que mas se vendió por unidad, sin importar el total de dinero recaudado.
void ProductoMasVendido(FILE *archivoFacturas, FILE *archivoDetalles)
{
    int i;
    int cantidadDeArticulos = 0; // cuántas casillas de la lista de artículos están usadas
    int posicionArticuloEncontrado;
    int posicionMejorArticulo;
    bool facturaValida;
    bool hayArticulosSinAnalizar = false; // se pone en true si aparecen más artículos que MAX_ARTICULOS
    DetalleFactura detalleActual;
    Factura facturaBuscada;
    int artCodigo[MAX_ARTICULOS]; // vectores auxiliares: una casilla por artículo distinto
    char artNombre[MAX_ARTICULOS][LARGO_DESC];
    int artUnidades[MAX_ARTICULOS];

    if (archivoFacturas == NULL || archivoDetalles == NULL)
    {
        printf("No se pudo acceder a los archivos de facturas o detalles.\n");
    }
    else
    {
        printf("**PRODUCTO MAS VENDIDO**\n");

        rewind(archivoDetalles);
        while (fread(&detalleActual, sizeof(DetalleFactura), 1, archivoDetalles) == 1)
        {
            // Se ignoran los detalles eliminados (facturas anuladas)
            if (detalleActual.estado == DETALLE_ACTIVO)
            {
                // La factura de esta línea tiene que existir y no estar anulada
                facturaValida = false;
                rewind(archivoFacturas);
                while (!facturaValida && fread(&facturaBuscada, sizeof(Factura), 1, archivoFacturas) == 1)
                {
                    if (facturaBuscada.nroFactura == detalleActual.nroFactura &&
                        facturaBuscada.estado != FACTURA_ANULADA)
                    {
                        facturaValida = true;
                    }
                }

                if (facturaValida)
                {
                    posicionArticuloEncontrado = NO_ENCONTRADO;
                    i = 0;
                    while (i < cantidadDeArticulos && posicionArticuloEncontrado == NO_ENCONTRADO)
                    {
                        if (artCodigo[i] == detalleActual.codigoArticulo)
                        {
                            posicionArticuloEncontrado = i;
                        }
                        i++;
                    }

                    if (posicionArticuloEncontrado == NO_ENCONTRADO)
                    {
                        if (cantidadDeArticulos < MAX_ARTICULOS)
                        {
                            artCodigo[cantidadDeArticulos] = detalleActual.codigoArticulo;
                            strcpy(artNombre[cantidadDeArticulos], detalleActual.descripcion);
                            artUnidades[cantidadDeArticulos] = detalleActual.cantidad;
                            cantidadDeArticulos++;
                        }
                        else
                        {
                            hayArticulosSinAnalizar = true;
                        }
                    }
                    else
                    {
                        artUnidades[posicionArticuloEncontrado] += detalleActual.cantidad;
                    }
                }
            }
        }
    }

    printf("\n");
    if (cantidadDeArticulos == 0)
    {
        printf("----No hay facturas en existencia----\n");
    }
    else
    {
        posicionMejorArticulo = 0;
        for (i = 1; i < cantidadDeArticulos; i++)
        {
            if (artUnidades[i] > artUnidades[posicionMejorArticulo])
            {
                posicionMejorArticulo = i;
            }
        }
        printf("Artículo: %s\n", artNombre[posicionMejorArticulo]);
        printf("Código: %d\n", artCodigo[posicionMejorArticulo]);
        printf("Unidades vendidas: %d\n", artUnidades[posicionMejorArticulo]);
        if (hayArticulosSinAnalizar)
        {
            printf("\nAviso: hay más de %d artículos distintos, los que no entraron en la lista no se tuvieron en cuenta.\n", MAX_ARTICULOS);
        }
    }
}

// Producto estrella: el artículo que más dinero dio. Las líneas de facturas anuladas no cuentan.
// Los artículos se identifican por su código.
// Se arma una lista de artículos (artCodigo, artNombre, artUnidades y artTotal, vectores auxiliares paralelos)
// sumando unidades y dinero de cada artículo; al final se elige el de mayor total.
void productoEstrella(FILE *archivoFacturas, FILE *archivoDetalles)
{
    int i;
    int cantidadDeArticulos = 0; // cuántas casillas de la lista de artículos están usadas
    int posicionArticuloEncontrado;
    int posicionMejorArticulo;
    bool facturaValida;
    bool hayArticulosSinAnalizar = false; // se pone en true si aparecen más artículos que MAX_ARTICULOS
    DetalleFactura detalleActual;
    Factura facturaBuscada;
    int artCodigo[MAX_ARTICULOS]; // vectores auxiliares: una casilla por artículo distinto
    char artNombre[MAX_ARTICULOS][LARGO_DESC];
    int artUnidades[MAX_ARTICULOS];
    float artTotal[MAX_ARTICULOS];

    printf("**PRODUCTO ESTRELLA**\n");

    if (archivoFacturas == NULL || archivoDetalles == NULL)
    {
        printf("No se pudo acceder a los archivos de facturas o detalles.\n");
        return;
    }

    // Cada vuelta lee una línea de detalle del archivo (cada línea es un artículo vendido dentro de una factura)
    rewind(archivoDetalles);
    while (fread(&detalleActual, sizeof(DetalleFactura), 1, archivoDetalles) == 1)
    {
        // ¿la factura de esta línea existe y no está anulada?
        // Se recorre el archivo de facturas buscando la que tenga el mismo número que la línea
        facturaValida = false;
        rewind(archivoFacturas);
        while (!facturaValida && fread(&facturaBuscada, sizeof(Factura), 1, archivoFacturas) == 1)
        {
            if (facturaBuscada.nroFactura == detalleActual.nroFactura && facturaBuscada.estado != FACTURA_ANULADA)
            {
                facturaValida = true;
            }
        }

        if (facturaValida)
        {
            // posicionArticuloEncontrado guarda en qué casilla de la lista está este artículo (NO_ENCONTRADO = todavía no está)
            posicionArticuloEncontrado = NO_ENCONTRADO;
            i = 0;
            // i recorre los artículos ya registrados buscando este por su código
            while (i < cantidadDeArticulos && posicionArticuloEncontrado == NO_ENCONTRADO)
            {
                if (artCodigo[i] == detalleActual.codigoArticulo)
                {
                    posicionArticuloEncontrado = i;
                }
                i++;
            }

            // artículo nuevo: se agrega a la lista; si ya estaba, se le suman unidades y dinero
            if (posicionArticuloEncontrado == NO_ENCONTRADO)
            {
                if (cantidadDeArticulos < MAX_ARTICULOS)
                {
                    artCodigo[cantidadDeArticulos] = detalleActual.codigoArticulo;
                    strcpy(artNombre[cantidadDeArticulos], detalleActual.descripcion);
                    artUnidades[cantidadDeArticulos] = detalleActual.cantidad;
                    artTotal[cantidadDeArticulos] = detalleActual.subtotalLinea;
                    cantidadDeArticulos++;
                }
                else
                {
                    hayArticulosSinAnalizar = true; // la lista está llena: este artículo no entra
                }
            }
            else
            {
                artUnidades[posicionArticuloEncontrado] += detalleActual.cantidad;
                artTotal[posicionArticuloEncontrado] += detalleActual.subtotalLinea;
            }
        }
    }

    printf("\n");
    if (cantidadDeArticulos == 0)
    {
        printf("----No hay facturas en existencia----\n");
    }
    else
    {
        // se toma el primer artículo como el mejor y se lo compara con los demás (i arranca en 1)
        posicionMejorArticulo = 0;
        // i recorre el resto de la lista; si encuentra uno con mayor total, pasa a ser el mejor
        for (i = 1; i < cantidadDeArticulos; i++)
        {
            if (artTotal[i] > artTotal[posicionMejorArticulo])
            {
                posicionMejorArticulo = i;
            }
        }
        printf("Artículo: %s\n", artNombre[posicionMejorArticulo]);
        printf("Total recaudado: $%.2f\n", artTotal[posicionMejorArticulo]);
        printf("Unidades vendidas: %d\n", artUnidades[posicionMejorArticulo]);
        if (hayArticulosSinAnalizar)
        {
            printf("\nAviso: hay más de %d artículos distintos, los que no entraron en la lista no se tuvieron en cuenta.\n", MAX_ARTICULOS);
        }
    }
}

void mejoresClientes(FILE *archivoFacturas)
{
    int i;
    int cantidadDeClientes = 0;
    int posicionClienteEncontrado;
    int posicionMejorCliente[3] = {-1, -1, -1}; // Inicializamos en -1 para indicar que están vacíos
    bool hayClientesSinAnalizar = false;
    Factura facturaActual;
    int clienteNro[MAX_CLIENTES];
    char clienteNombre[MAX_CLIENTES][LARGO_NOMBREYAPELLIDO];
    float clienteTotal[MAX_CLIENTES];
    int clienteFacturas[MAX_CLIENTES];

    printf("**MEJORES CLIENTES**\n");

    if (archivoFacturas == NULL)
    {
        printf("No se pudo leer el archivo de facturas.\n");
        return;
    }

    // Cada vuelta lee una factura del archivo
    rewind(archivoFacturas);
    while (fread(&facturaActual, sizeof(Factura), 1, archivoFacturas) == 1)
    {
        // Solo se analizan las facturas que no están anuladas
        if (facturaActual.estado != FACTURA_ANULADA)
        {
            posicionClienteEncontrado = NO_ENCONTRADO;
            i = 0;
            while (i < cantidadDeClientes && posicionClienteEncontrado == NO_ENCONTRADO)
            {
                if (clienteNro[i] == facturaActual.codigoCliente)
                {
                    posicionClienteEncontrado = i;
                }
                i++;
            }

            if (posicionClienteEncontrado == NO_ENCONTRADO)
            {
                if (cantidadDeClientes < MAX_CLIENTES)
                {
                    clienteNro[cantidadDeClientes] = facturaActual.codigoCliente;
                    strcpy(clienteNombre[cantidadDeClientes], facturaActual.clienteSnapshot.nombreYApellido);
                    clienteTotal[cantidadDeClientes] = facturaActual.totalFactura;
                    clienteFacturas[cantidadDeClientes] = 1;
                    cantidadDeClientes++;
                }
                else
                {
                    hayClientesSinAnalizar = true;
                }
            }
            else
            {
                clienteTotal[posicionClienteEncontrado] += facturaActual.totalFactura;
                clienteFacturas[posicionClienteEncontrado]++;
            }
        }
    }

    printf("\n");
    if (cantidadDeClientes == 0)
    {
        printf("----No hay facturas en existencia----\n");
    }
    else
    {
        // Algoritmo correcto para encontrar los 3 mayores
        for (i = 0; i < cantidadDeClientes; i++)
        {
            if (posicionMejorCliente[0] == -1 || clienteTotal[i] > clienteTotal[posicionMejorCliente[0]])
            {
                posicionMejorCliente[2] = posicionMejorCliente[1];
                posicionMejorCliente[1] = posicionMejorCliente[0];
                posicionMejorCliente[0] = i;
            }
            else if (posicionMejorCliente[1] == -1 || clienteTotal[i] > clienteTotal[posicionMejorCliente[1]])
            {
                posicionMejorCliente[2] = posicionMejorCliente[1];
                posicionMejorCliente[1] = i;
            }
            else if (posicionMejorCliente[2] == -1 || clienteTotal[i] > clienteTotal[posicionMejorCliente[2]])
            {
                posicionMejorCliente[2] = i;
            }
        }

        // Mostrar resultados asegurándose de que existan los clientes
        if (posicionMejorCliente[0] != -1)
        {
            printf("Cliente estrella:\n");
            printf("Cliente: %s\n", clienteNombre[posicionMejorCliente[0]]);
            printf("Cliente Nro: %d\n", clienteNro[posicionMejorCliente[0]]);
            printf("Total gastado: $%.2f\n", clienteTotal[posicionMejorCliente[0]]);
            printf("Cantidad de facturas: %d\n", clienteFacturas[posicionMejorCliente[0]]);
            printf("\n");
        }

        if (posicionMejorCliente[1] != -1)
        {
            printf("Segundo cliente:\n");
            printf("Cliente: %s\n", clienteNombre[posicionMejorCliente[1]]);
            printf("Cliente Nro: %d\n", clienteNro[posicionMejorCliente[1]]);
            printf("Total gastado: $%.2f\n", clienteTotal[posicionMejorCliente[1]]);
            printf("Cantidad de facturas: %d\n", clienteFacturas[posicionMejorCliente[1]]);
            printf("\n");
        }

        if (posicionMejorCliente[2] != -1)
        {
            printf("Tercer cliente:\n");
            printf("Cliente: %s\n", clienteNombre[posicionMejorCliente[2]]);
            printf("Cliente Nro: %d\n", clienteNro[posicionMejorCliente[2]]);
            printf("Total gastado: $%.2f\n", clienteTotal[posicionMejorCliente[2]]);
            printf("Cantidad de facturas: %d\n", clienteFacturas[posicionMejorCliente[2]]);
            printf("\n");
        }

        if (hayClientesSinAnalizar)
        {
            printf("Aviso: hay más de %d clientes distintos, los que no entraron en la lista no se tuvieron en cuenta.\n", MAX_CLIENTES);
        }
    }
}
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
                respuesta[i] = toupper(respuesta[i]);
            }
            if (strcmp(respuesta, "SI") == 0 || strcmp(respuesta, "S") == 0)
            {
                strcpy(busqueda.claveHash, user.claveHash);
                busqueda.activo = ACTIVO;

                fseek(archivoUsers, (long)posicion * sizeof(Usuario), SEEK_SET);
                fwrite(&busqueda, sizeof(Usuario), 1, archivoUsers);
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
    rewind(archivoProductos);
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
        leerEntero("La cantidad vendida no puede ser negativa. Ingrese la cantidad vendida de este producto (-1) para salir: ", &product.cantVendida);
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
    // fwrite(&product, sizeof(Articulo), 1, archivoProducts);

    return fwrite(&product, sizeof(Articulo), 1, archivoProducts);
    ;
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
int buscarPosicionRegistroCliente(FILE *archivoClientes, int codigoClienteBaja)
{
    Cliente client;
    int posicion = 0;
    while (fread(&client, sizeof(Cliente), 1, archivoClientes)) // fread retorna 1 si esta bien, mientras que exista. cuando no haya mas que sea endoffile, termina (porque sería 0)
    {

        if (client.codigo == codigoClienteBaja)
        {
            return posicion;
        }
        posicion++;
    }
    if (client.codigo != codigoClienteBaja)
    {
        return NO_ENCONTRADO;
    }
}
void verListadoClientes(FILE *archivoClientes)
{
    Cliente cliente;
    rewind(archivoClientes);
    if (fread(&cliente, sizeof(Cliente), 1, archivoClientes) == 0)
    {
        printf("No hay clientes cargados para mostrar\n");
    }
    rewind(archivoClientes);
    printf("=============================================================\n");
    printf("                    CONSULTA DE CLIENTES                     \n");
    printf("=============================================================\n");
    printf("%-8s %-30s %-12s %-8s\n", "CODIGO", "NOMBRE Y APELLIDO", "DNI", "ESTADO");
    printf("-------------------------------------------------------------\n");

    while (fread(&cliente, sizeof(Cliente), 1, archivoClientes) == 1)
    {

        printf("%-8d %-30s %-12d %-8s\n",
               cliente.codigo,
               cliente.datos.nombreYApellido,
               cliente.datos.numeroDNI,
               cliente.activo == ACTIVO ? "ACTIVO" : "INACTIVO");
    }
    printf("==========================================================================\n");
}
int registrarCliente(FILE *archivoCliente)
{
    Cliente client;
    Cliente busqueda;
    char respuesta[LARGO_RESPUESTA];
    int posicion = 0;
    int i;

    client.activo = ACTIVO;

    leerCadena(client.datos.nombreYApellido, LARGO_NOMBREYAPELLIDO, "Nombre y apellido del cliente (-1 para salir): ");
    while (client.datos.nombreYApellido[0] == '\0')
    {
        printf("El nombre y apellido  no puede estar vacío.\n");
        leerCadena(client.datos.nombreYApellido, LARGO_NOMBREYAPELLIDO, "Nombre y apellido del cliente (-1 para salir): ");
    }
    if (strcmp(client.datos.nombreYApellido, "-1") == 0)
    {
        return SALIDA;
    }
    leerEntero("Ingrese el DNI del cliente a registrar (-1 para salir)", &client.datos.numeroDNI);
    while (client.datos.numeroDNI == 0)
    {
        printf("El dni no puede ser 0  no puede estar vacío.\n");
        leerEntero("Ingrese el DNI del cliente a registrar (-1 para salir)", &client.datos.numeroDNI);
    }
    if (client.datos.numeroDNI == SALIDA)
    {
        return SALIDA;
    }
    leerEntero("Ingrese el codigo cliente a registrar (-1 para salir)", &client.codigo);
    while (client.codigo == 0)
    {
        printf("El codigo del cliente no puede ser 0  no puede estar vacío.\n");
        leerEntero("Ingrese el codigo cliente a registrar (-1 para salir)", &client.codigo);
    }
    if (client.codigo == SALIDA)
    {
        return SALIDA;
    }

    rewind(archivoCliente);
    while (fread(&busqueda, sizeof(Cliente), 1, archivoCliente) == 1)
    {
        if (busqueda.codigo == client.codigo)
        {
            if (busqueda.activo == ACTIVO)
            {
                return EXISTE_BD;
            }

            leerCadena(respuesta, LARGO_RESPUESTA, "Este cliente esta desactivado. Desea activarlo [SI/NO]? ");
            for (i = 0; respuesta[i] != '\0'; i++)
            {
                respuesta[i] = toupper((unsigned char)respuesta[i]);
            }
            if (strcmp(respuesta, "SI") == 0 || strcmp(respuesta, "S") == 0)
            {
                busqueda.activo = ACTIVO;

                fseek(archivoCliente, (long)posicion * sizeof(Cliente), SEEK_SET);
                fwrite(&busqueda, sizeof(Cliente), 1, archivoCliente);
                return EXITO;
            }
            return SALIDA;
        }
        posicion++;
    }

    fseek(archivoCliente, 0L, SEEK_END);
    fwrite(&client, sizeof(Cliente), 1, archivoCliente);
    fflush(archivoCliente);
    return EXITO;
}

// *********************************************************************************
// MENÚ GESTOR
// *********************************************************************************

void MenuUsuario()
{
    int op = 0;
    int subOp = 0;
    char *VecMenuPrincipal[] = {"Facturas", "Estadisticas", "Productos", "Salir"};
    char *vecMenuFacturas[] = {"Generar nueva factura", "Ver listado de facturas", "Eliminar factura de la base de datos", "Ver listado de clientes", "Agregar cliente", "Eliminar cliente", "Volver"};
    char *vecMenuEstadisticas[] = {"Ver producto estrella", "Ver producto mas vendido", "Ver mejores clientes", "Volver"};
    char *vecMenuProductos[] = {"Ver listado de productos", "Agregar producto nuevo", "Dar de baja un producto", "Volver"};

    utf8();
    limpiarPantalla();
    int cantOpcionesMenuPrincipal = sizeof(VecMenuPrincipal) / sizeof(VecMenuPrincipal[0]);
    int cantOpcionesFacturas = sizeof(vecMenuFacturas) / sizeof(vecMenuFacturas[0]);
    int cantOpcionesEstadisticas = sizeof(vecMenuEstadisticas) / sizeof(vecMenuEstadisticas[0]);
    int cantOpcionesProductos = sizeof(vecMenuProductos) / sizeof(vecMenuProductos[0]);
    // Creacion de los archivos
    FILE *archivoProductos;
    FILE *archivoFacturas;
    FILE *archivoClientes;
    FILE *archivoDetalles;
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

                subOp = elegirOpcion("SECCION FACTURAS", vecMenuFacturas, cantOpcionesFacturas);
                switch (subOp)
                {
                case op1:
                {
                    Factura facturaActual = {0};
                    Factura facturaGuardada;
                    DetalleFactura detallesFactura[MAX_DETALLES_FACTURA];
                    int cantidadDetallesFactura = 0;
                    int cantidadFacturas = 0;
                    int siguienteNroFactura = 1;

                    limpiarPantalla();

                    if ((archivoFacturas = fopen("bd_Facturas.dat", "rb+")) == NULL)
                    {
                        archivoFacturas = fopen("bd_Facturas.dat", "wb+");
                    }

                    if ((archivoDetalles = fopen("bd_Detalles.dat", "rb+")) == NULL)
                    {
                        archivoDetalles = fopen("bd_Detalles.dat", "Wb+");
                    }

                    if ((archivoProductos = fopen("bd_Products.dat", "rb+")) == NULL)
                    {
                        printf("No se pudo abrir el archivo de productos. Cargue productos antes de generar facturas.\n");
                    }
                    else if ((archivoClientes = fopen("bd_Clients.dat", "rb")) == NULL)
                    {
                        printf("No se pudo abrir el archivo de clientes. Cargue clientes antes de generar facturas.\n");
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

                        if (generarFactura(siguienteNroFactura, &facturaActual, detallesFactura,
                                           MAX_DETALLES_FACTURA, &cantidadDetallesFactura,
                                           &cantidadFacturas, archivoFacturas, archivoProductos, archivoDetalles,
                                           archivoClientes))
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
                    if (archivoClientes != NULL)
                    {
                        fclose(archivoClientes);
                    }
                    break;
                }
                case op2:
                    limpiarPantalla();
                    {
                        if ((archivoFacturas = fopen("bd_Facturas.dat", "rb")) == NULL)
                        {
                            printf("No hay facturas para mostrar.\n");
                        }
                        else if ((archivoDetalles = fopen("bd_Detalles.dat", "rb")) == NULL)
                        {
                            printf("No se pudo abrir el archivo de detalles de facturas.\n");
                        }
                        else
                        {
                            verListadoFacturas(archivoFacturas, archivoDetalles);
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
                    limpiarPantalla();
                    {
                        if ((archivoFacturas = fopen("bd_Facturas.dat", "rb+")) == NULL)
                        {
                            printf("No hay facturas cargadas para eliminar.\n");
                        }
                        else if ((archivoDetalles = fopen("bd_Detalles.dat", "rb+")) == NULL)
                        {
                            printf("No se pudo abrir el archivo de detalles de facturas.\n");
                        }
                        else if ((archivoProductos = fopen("bd_Products.dat", "rb+")) == NULL)
                        {
                            printf("No se pudo abrir el archivo de prodcutos.\n");
                        }
                        else
                        {
                            int nroFactura;
                            leerEntero("Ingrese el numero de la factura que desea anular: ", &nroFactura);
                            eliminarFactura(archivoFacturas, archivoDetalles, archivoProductos, nroFactura);
                        }

                        if (archivoFacturas != NULL)
                        {
                            fclose(archivoFacturas);
                        }
                        if (archivoDetalles != NULL)
                        {
                            fclose(archivoDetalles);
                        }
                        if (archivoProductos != NULL)
                        {
                            fclose(archivoProductos);
                        }
                        break;
                    }
                case op4:
                    if ((archivoClientes = fopen("bd_Clients.dat", "rb+")) == NULL)
                    {
                        archivoClientes = fopen("bd_Clients.dat", "wb+");
                    }
                    verListadoClientes(archivoClientes);
                    fclose(archivoClientes);
                    break;
                case op5:
                    if ((archivoClientes = fopen("bd_Clients.dat", "rb+")) == NULL)
                    {
                        archivoClientes = fopen("bd_Clients.dat", "wb+");
                    }
                    printf("**********Registrar Cliente**********\n\n");

                    int resultado = registrarCliente(archivoClientes);

                    if (resultado == EXISTE_BD)
                    {
                        printf("Cliente ya existente, reintente\n");
                    }
                    else if (resultado == SALIDA)
                    {
                        printf("No se registro el cliente (cancelado)");
                    }
                    else
                    {
                        printf("\n¡Registro exitoso!\n");
                    }
                    fclose(archivoClientes);
                    pausa();
                    break;
                case op6:
                    if ((archivoClientes = fopen("bd_Clients.dat", "rb+")) == NULL)
                    {
                        archivoClientes = fopen("bd_Clients.dat", "wb+");
                    }
                    printf("**********Dar de baja un Cliente**********\n\n");
                    int codigo = 0;
                    int posicionRegistro = 0;
                    Cliente client;
                    leerEntero("Ingrese el codigo del cliente que quiere dar de baja (-1 para salir)", &codigo);
                    if (codigo == SALIDA)
                    {
                        printf("No se hizo la baja  (cancelado)\n");
                    }
                    posicionRegistro = buscarPosicionRegistroCliente(archivoClientes, codigo);
                    if (posicionRegistro == NO_ENCONTRADO)
                    {
                        printf("No se ha encontrado el cliente\n");
                    }
                    else
                    {
                        fseek(archivoClientes, posicionRegistro * sizeof(Cliente), SEEK_SET);
                        fread(&client, sizeof(Cliente), 1, archivoClientes);
                        client.activo = INACTIVO; // se trabaja una BAJA LOGICA, no estamos borrando por completo el registro ya que no hay manera de deshacerlo. la buena practica es desactivar registros, no eliminarlos por completo
                        fseek(archivoClientes, posicionRegistro * sizeof(Cliente), SEEK_SET);
                        fwrite(&client, sizeof(Cliente), 1, archivoClientes);
                        printf("Cliente desactivado correctamente\n");
                    }
                    fclose(archivoClientes);
                    break;
                }
                if (subOp != cantOpcionesFacturas)
                    pausa();
            } while (subOp != cantOpcionesFacturas);
            break;

        case op2:
            archivoFacturas = fopen("bd_Facturas.dat", "rb");
            archivoDetalles = fopen("bd_Detalles.dat", "rb");
            do
            {

                subOp = elegirOpcion("SECCION ESTADISTICAS", vecMenuEstadisticas, cantOpcionesEstadisticas);
                switch (subOp)
                {
                case op1:
                    rewind(archivoFacturas);
                    rewind(archivoDetalles);
                    productoEstrella(archivoFacturas, archivoDetalles);
                    break;
                case op2:
                    rewind(archivoFacturas);
                    rewind(archivoDetalles);
                    ProductoMasVendido(archivoFacturas, archivoDetalles);
                    break;
                case op3:
                    rewind(archivoFacturas);
                    mejoresClientes(archivoFacturas);
                    break;
                }

                if (subOp != cantOpcionesEstadisticas)
                    pausa();
            } while (subOp != cantOpcionesEstadisticas);

            if (archivoFacturas != NULL)
                fclose(archivoFacturas);
            if (archivoDetalles != NULL)
                fclose(archivoDetalles);
            break;
        case op3:

            if ((archivoProductos = fopen("bd_Products.dat", "rb+")) == NULL)
            {
                archivoProductos = fopen("bd_Products.dat", "wb+");
            }

            do
            {
                rewind(archivoProductos);
                subOp = elegirOpcion("SECCION PRODUCTOS", vecMenuProductos, cantOpcionesProductos);
                switch (subOp)

                {
                case op1:
                    rewind(archivoProductos);
                    verListadoDeProductos(archivoProductos);

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
                        printf("Producto desactivado correctamente\n");
                    }
                    break;
                default:
                    printf("Error: Fuera de rango. Reintente.\n");
                    break;
                }

                if (subOp != cantOpcionesProductos)
                {
                    pausa();
                }

            } while (subOp != cantOpcionesProductos);
            fclose(archivoProductos);
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

        printf("Gestor desactivado correctamente\n");
    }
}
void verListadoUsuarios(FILE *archivoUsers)
{
    Usuario user;
    rewind(archivoUsers);
    if (fread(&user, sizeof(Usuario), 1, archivoUsers) == 0)
    {
        printf("No hay usuarios cargados para mostrar\n");
    }
    printf("==============================================================\n");
    printf("                    CONSULTA DE USUARIOS                      \n");
    printf("==============================================================\n");
    printf("%-22s %-10s %-8s\n", "USUARIO", "ROL", "ESTADO");
    printf("--------------------------------------------------------------\n");

    while (fread(&user, sizeof(Usuario), 1, archivoUsers) == 1) // mientras haya registros
    {
        printf("%-22s %-10s %-8s\n",
               user.nombreUsuario, // por una cuestion de seguridad, no muestro las contraseñas
               user.rol_usuario == 1 ? "ADMIN" : "GESTOR",
               user.activo == 1 ? "ACTIVO" : "INACTIVO");
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
                          "Ver listado de usuarios\n",
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
        case op3:
            verListadoUsuarios(archivoUsuarios);
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
    int resultado = 0;
    int i;
    bool administradorEncontrado;
    bool accesoConcedido = false;
    bool volver = false;
    const char *administradores[] = {"Sofia", "Ramiro", "Luana", "Tomas"};
    int cantidadAdministradores = sizeof(administradores) / sizeof(administradores[0]);
    char nombreUsuario[LARGO_USUARIO];
    char clave[LARGO_CLAVE];

    limpiarPantalla();

    while (intentos > 0 && !accesoConcedido && !volver)
    {
        leerCadena(nombreUsuario, LARGO_USUARIO, "Ingrese el usuario administrador (-1 para volver): ");

        if (strcmp(nombreUsuario, "-1") == 0)
        {
            volver = true;
        }
        else
        {
            leerCadena(clave, LARGO_CLAVE, "Ingrese la clave de administrador: ");
            if (strcmp(clave, "-1") == 0)
            {
                volver = true;
            }
            else
            {
                administradorEncontrado = false;
                for (i = 0; i < cantidadAdministradores; i++)
                {
                    if (strcmp(nombreUsuario, administradores[i]) == 0)
                    {
                        administradorEncontrado = true;
                    }
                }

                if (administradorEncontrado && strcmp(clave, CLAVE_ADMIN) == 0)
                {
                    accesoConcedido = true;
                }
                else
                {
                    intentos--;
                    if (intentos > 0)
                    {
                        printf("Usuario o clave incorrectos. Intentos disponibles: %d\n", intentos);
                    }
                }
            }
        }
    }

    if (accesoConcedido)
    {
        limpiarPantalla();
        MenuAdmin();
    }
    else if (!volver)
    {
        limpiarPantalla();
        printf("Usuario o clave incorrectos. Sin intentos disponibles.\n\n");
        printf("Saliendo del programa...\n");
        retardo2(0.2);
        resultado = CANCELAR;
    }

    return resultado;
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
            nuevaFactura.clienteSnapshot.nombreYApellido[LARGO_NOMBREYAPELLIDO - 1] = '\0';
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
                bool lineaValida = true; // Variable para verificar si la línea de detalle es válida (producto encontrado y cantidad válida)

                detalle.estado = DETALLE_ACTIVO; // Asigno el estado del detalle como activo, ya que se está generando un nuevo detalle de factura

                leerEntero("Ingrese el código del producto (0 para finalizar): ", &codigoIngresado);
                detalle.codigoArticulo = codigoIngresado; // Asigno el código del producto ingresado a la estructura de detalle de factura
                printf("\n");
                if (codigoIngresado != 0)
                {

                    if (detalle.codigoArticulo < 0)
                    {
                        printf("El código del producto debe ser mayor que cero.\n");
                        lineaValida = false;
                    }

                    if (lineaValida) // Si la línea es válida, busco el producto en el archivo de productos
                    {
                        Articulo producto = {0};
                        bool productoEncontrado = false;

                        rewind(archivoProductos);
                        while (!productoEncontrado && fread(&producto, sizeof(Articulo), 1, archivoProductos) == 1) // Recorro el archivo de productos hasta encontrar el producto con el código ingresado
                        {
                            if (producto.codigo == codigoIngresado)
                            {
                                productoEncontrado = true; // Si se encuentra el producto, marco la variable como verdadera y salgo del bucle
                            }
                        }

                        if (!productoEncontrado) // Si no se encuentra el producto, muestro un mensaje de error y marco la línea como inválida
                        {
                            printf("No existe un producto con el código ingresado. Intente nuevamente.\n");
                            lineaValida = false;
                        }
                        else if (producto.estado != ACTIVO)
                        {
                            printf("El producto está inactivo y no puede incluirse en una factura.\n");
                            lineaValida = false;
                        }
                        else
                        {
                            for (int i = 0; i < LARGO_DESC - 1; i++) // Copio la descripción del producto al detalle de factura, asegurándome de no exceder el tamaño del arreglo
                            {
                                detalle.descripcion[i] = producto.descripcion[i]; // Copio la descripción del producto al detalle de factura
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

                            if (detalle.cantidad > producto.stock)
                            {
                                printf("La cantidad ingresada supera el stock disponible (%d). Intente nuevamente.\n", producto.stock);
                                lineaValida = false;
                            }
                            else
                            {
                                // Actualizo el stock del producto en el archivo de productos
                                producto.stock -= detalle.cantidad;
                                fseek(archivoProductos, -sizeof(Articulo), SEEK_CUR);     // Muevo el puntero del archivo al inicio del registro del producto encontrado
                                fwrite(&producto, sizeof(Articulo), 1, archivoProductos); // Sobrescribo el registro del producto con el stock actualizado
                                fflush(archivoProductos);                                 // Aseguro que los cambios se escriban en el archivo
                            }

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
                    fflush(archivoDetalles) != 0 || fseek(archivoFacturas, 0, SEEK_END) != 0 ||
                    fwrite(&nuevaFactura, sizeof(Factura), 1, archivoFacturas) != 1 || fflush(archivoFacturas) != 0)
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
        if (factura.estado != FACTURA_ANULADA)
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
                    if (detalle.nroFactura == factura.nroFactura && detalle.estado == DETALLE_ACTIVO)
                    {
                        printf("%-10d | %-50s | $%16.2f | %10d | $%11.2f\n",
                               detalle.codigoArticulo, detalle.descripcion,
                               detalle.precioUnitario, detalle.cantidad, detalle.subtotalLinea);
                    }
                }
            }
            printf("Total de la factura: $%.2f\n", factura.totalFactura);
        }
    }

    if (cantidadFacturas == 0)
    {
        printf("No hay facturas para mostrar.\n");
    }
}

void eliminarFactura(FILE *archivoFacturas, FILE *archivoDetalles, FILE *archivoProductos, int nroFactura)
{
    Factura factura;
    DetalleFactura detalle;
    bool facturaEncontrada = false;
    bool continuar = true;
    long posicionFactura = 0;

    if (archivoFacturas == NULL || archivoDetalles == NULL || archivoProductos == NULL || nroFactura <= 0)
    {
        printf("No se pudo anular la factura: los datos recibidos no son válidos.\n");
        continuar = false;
    }

    if (continuar)
    {
        rewind(archivoFacturas);
        while (!facturaEncontrada && fread(&factura, sizeof(Factura), 1, archivoFacturas) == 1)
        {
            if (factura.nroFactura == nroFactura)
            {
                posicionFactura = ftell(archivoFacturas) - sizeof(Factura);
                facturaEncontrada = true;
            }
        }

        if (!facturaEncontrada)
        {
            printf("No se encontró la factura con número %d.\n", nroFactura);
            continuar = false;
        }
        else if (factura.estado == FACTURA_ANULADA)
        {
            printf("La factura N° %d ya está anulada.\n", nroFactura);
            continuar = false;
        }
    }

    if (continuar)
    {
        rewind(archivoDetalles);
        while (continuar && fread(&detalle, sizeof(DetalleFactura), 1, archivoDetalles) == 1)
        {
            if (detalle.nroFactura == nroFactura && detalle.estado == DETALLE_ACTIVO)
            {
                long posicionDetalle = ftell(archivoDetalles) - sizeof(DetalleFactura);
                Articulo producto;
                bool productoEncontrado = false;
                long posicionProducto = 0;

                rewind(archivoProductos);
                while (!productoEncontrado && fread(&producto, sizeof(Articulo), 1, archivoProductos) == 1)
                {
                    if (producto.codigo == detalle.codigoArticulo)
                    {
                        posicionProducto = ftell(archivoProductos) - sizeof(Articulo);
                        productoEncontrado = true;
                    }
                }

                if (!productoEncontrado)
                {
                    printf("No se encontró el producto %d; la factura quedó sin anular.\n",
                           detalle.codigoArticulo);
                    continuar = false;
                }
                else if (detalle.cantidad <= 0 || producto.stock > INT_MAX - detalle.cantidad)
                {
                    printf("No se pudo restaurar el stock del producto %d por datos inválidos.\n",
                           detalle.codigoArticulo);
                    continuar = false;
                }
                else
                {
                    producto.stock += detalle.cantidad;
                    if (fseek(archivoProductos, posicionProducto, SEEK_SET) != 0 ||
                        fwrite(&producto, sizeof(Articulo), 1, archivoProductos) != 1 ||
                        fflush(archivoProductos) != 0)
                    {
                        printf("No se pudo actualizar el stock del producto %d.\n", detalle.codigoArticulo);
                        continuar = false;
                    }
                    else
                    {
                        detalle.estado = DETALLE_ELIMINADO;
                        if (fseek(archivoDetalles, posicionDetalle, SEEK_SET) != 0 ||
                            fwrite(&detalle, sizeof(DetalleFactura), 1, archivoDetalles) != 1 ||
                            fflush(archivoDetalles) != 0)
                        {
                            printf("No se pudo actualizar el detalle de la factura N° %d.\n", nroFactura);
                            continuar = false;
                        }
                        else if (fseek(archivoDetalles, posicionDetalle + sizeof(DetalleFactura), SEEK_SET) != 0)
                        {
                            printf("No se pudo continuar leyendo los detalles de la factura N° %d.\n", nroFactura);
                            continuar = false;
                        }
                    }
                }
            }
        }
    }

    if (continuar)
    {
        factura.estado = FACTURA_ANULADA;
        if (fseek(archivoFacturas, posicionFactura, SEEK_SET) != 0 ||
            fwrite(&factura, sizeof(Factura), 1, archivoFacturas) != 1 ||
            fflush(archivoFacturas) != 0)
        {
            printf("No se pudo actualizar el estado de la factura N° %d.\n", nroFactura);
            continuar = false;
        }
    }

    if (continuar)
    {
        printf("Factura N° %d anulada correctamente.\n", nroFactura);
    }
}
#endif /* FACTUFUNCIONES_H_ */
