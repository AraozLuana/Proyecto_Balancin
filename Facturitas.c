#include <stdio.h>
#include "utiles_Proyecto.h"
#include "FactuFunciones.h" // esta tiene incluido "Estructuras.h" y define CANCELAR

int main()
{
    utf8();
    int permi;
    int op;
    int resultado;
    FILE *archivoUsuarios;
    FILE *archivoProductos;
    FILE *archivoFacturas;
    FILE *archivoDetalles;

    if ((archivoUsuarios = fopen("bd_Users.dat", "rb+")) == NULL)
    {
        archivoUsuarios = fopen("bd_Users.dat", "wb+");
    }
    if ((archivoProductos = fopen("bd_Products.dat", "rb+")) == NULL)
    {
        archivoProductos = fopen("bd_Products.dat", "wb+");
    }
    if ((archivoFacturas = fopen("bd_Facturas.dat", "rb+")) == NULL)
    {
        archivoFacturas = fopen("bd_Facturas.dat", "wb+");
    }
    if ((archivoDetalles = fopen("bd_Detalles.dat", "rb+")) == NULL)
    {
        archivoDetalles = fopen("bd_Detalles.dat", "wb+");
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
            resultado = LogeoAdmin();
            if (resultado == CANCELAR)
            {
                permi = CANCELAR; // se agotaron los intentos: cierra el programa
            }
            else if (resultado == EXITO)
            {
                do
                {
                    limpiarPantalla();
                    op = MenuAdmin();

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
                    if (op != op3)
                        pausa();
                } while (op != op3);
            }
            break;

        case PERMI_GESTOR:
            if (LogeoGestor(archivoUsuarios))
            {
                limpiarPantalla();
                printf("----Bienvenide a Balancin----\n");

                do
                {
                    op = MenuUsuario();
                    limpiarPantalla();

                    switch (op)
                    {
                    // FACTURAS
                    case SECC_FACTURAS + op1:
                        printf("CREAR FUNCION GENERAR NUEVA FACTURA");
                        break;
                    case SECC_FACTURAS + op2:
                        printf("CREAR FUNCION VER LISTADO DE FACTURAS");
                        break;
                    case SECC_FACTURAS + op3:
                        printf("CREAR FUNCION ELIMINAR FACTURA DE LA BD");
                        break;
                    case SECC_FACTURAS + op4:
                        printf("CREAR FUNCION VER LISTADO DE CLIENTES");
                        break;
                    case SECC_FACTURAS + op5:
                        printf("CREAR FUNCION AGREGAR CLIENTE");
                        break;
                    case SECC_FACTURAS + op6:
                        printf("CREAR FUNCION ELIMINAR CLIENTE");
                        break;
                    case SECC_FACTURAS + op7:
                        printf("CREAR FUNCION MODIFICAR CLIENTE");
                        break;

                    // ESTADISTICAS
                    case SECC_ESTADISTICAS + op1:
                        rewind(archivoFacturas);
                        rewind(archivoDetalles);
                        productoEstrella(archivoFacturas, archivoDetalles);
                        break;
                    case SECC_ESTADISTICAS + op2:
                    rewind(archivoFacturas);
                    rewind(archivoDetalles);
                        ProductoMasVendido(archivoFacturas, archivoDetalles);
                        break;
                    case SECC_ESTADISTICAS + op3:
                        rewind(archivoFacturas);
                        mejoresClientes(archivoFacturas);
                        break;
                    case SECC_ESTADISTICAS + op4:
                        rewind(archivoFacturas);
                        listado_VentasPorZona(archivoFacturas);
                        break;

                    // PRODUCTOS
                    case SECC_PRODUCTOS + op1:
                        rewind(archivoProductos);
                        verListadoDeProductos(archivoProductos);
                        break;
                    case SECC_PRODUCTOS + op2:
                        printf("**********Registrar Producto**********\n\n");
                        rewind(archivoProductos);
                        resultado = registrarProducto(archivoProductos);

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
                        break;
                    case SECC_PRODUCTOS + op3:
                    {
                        printf("**********Dar de baja un Producto**********\n\n");
                        int codigo = 0;
                        int posicionRegistro = 0;
                        Articulo product;
                        leerEntero("Ingrese el codigo del articulo que quiere dar de baja (-1 para salir)", &codigo);
                        if (codigo == SALIDA)
                        {
                            printf("No se hizo la baja  (cancelado)\n");
                        }
                        rewind(archivoProductos);
                        posicionRegistro = buscarPosicionRegistroProductos(archivoProductos, codigo);
                        if (posicionRegistro == NO_ENCONTRADO)
                        {
                            printf("No se ha encontrado el producto\n");
                        }
                        else
                        {
                            fseek(archivoProductos, posicionRegistro * sizeof(Articulo), SEEK_SET);
                            fread(&product, sizeof(Articulo), 1, archivoProductos);
                            product.estado = INACTIVO; // se trabaja una BAJA LOGICA
                            fseek(archivoProductos, posicionRegistro * sizeof(Articulo), SEEK_SET);
                            fwrite(&product, sizeof(Articulo), 1, archivoProductos);
                            fflush(archivoProductos);

                            printf("Producto desactivado correctamente\n");
                        }
                        break;
                    }
                    }
                    if (op != SALIR)
                        pausa();
                } while (op != SALIR);
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
    fclose(archivoProductos);
    return 0;
}