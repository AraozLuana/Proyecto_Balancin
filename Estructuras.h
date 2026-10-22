#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H
 
/* ============================================================
*  TAMAÑOS DE CAMPOS DE TEXTO
* ============================================================ */
#define MAX_USUARIOS 50
#define LARGO_USUARIO   30
#define LARGO_CLAVE     30
#define LARGO_NOMBRE    60
#define LARGO_DESC      80
#define PERMI_GESTOR 2
#define PERMI_ADMIN 1

 
/* ============================================================
*  ROLES DE USUARIO
* ============================================================ */
typedef enum {
    ROL_ADMIN  = 1,
    ROL_GESTOR = 2
} RolUsuario;
 
/* ============================================================
*  USUARIO
* ============================================================ */
typedef struct {
    char       nombreUsuario[LARGO_USUARIO];  /* PK lógica, única */
    char       claveHash[LARGO_CLAVE];        /* hash, NUNCA texto plano */
    RolUsuario rol;
    int        activo;                        /* 1 = activo, 0 = baja */
    int permi;
} Usuario;
 
/* ============================================================
*  PERSONA (reutilizable: cliente, snapshot en factura, etc.)
* ============================================================ */
typedef struct {
    char  nombreYApellido[LARGO_NOMBRE];
    int   numeroDNI;
} Persona;
 
/* ============================================================
*  CLIENTE
* ============================================================ */
typedef struct {
    int      codigo;          /* PK, único, > 0 */
    Persona  datos;
    int      activo;          /* 1 = activo, 0 = baja lógica */
} Cliente;
 
/* ============================================================
*  ARTÍCULO
* ============================================================ */
typedef struct {
    int   codigo;             /* PK */
    char  descripcion[LARGO_DESC];
    float precio;             /* NETO, sin IVA */
    int   stock;
    int   activo;             /* 1 = activo, 0 = dado de baja */
} Articulo;
 
/* ============================================================
*  FECHA
* ============================================================ */
typedef struct {
    int dia;
    int mes;
    int anio;
} Fecha;
 
/* ============================================================
*  ESTADO DE FACTURA
* ============================================================ */
typedef enum {
    FACTURA_EMITIDA = 0,
    FACTURA_PAGADA  = 1,
    FACTURA_ANULADA = 2
} EstadoFactura;
 
/* ============================================================
*  CABECERA DE FACTURA       ->  facturas.dat
* ============================================================ */
typedef struct {
    int           nroFactura;      /* PK, único y correlativo  cuando hablo de PK es Primrary Key o clave Primaria*/
    Fecha         fechaEmision;
    int           codigoCliente;   /* FK -> Cliente.codigo */
    Persona       clienteSnapshot; /* copia histórica del cliente */
    float         netoGravado;     /* TGNG = suma de subtotalLinea yo lo llamo Total Genral Neto Gravrado */
    float         alicuotaIVA;     /* 0.21, 0.105, 0.27, 0.0 */
    float         importeIVA;      /* netoGravado * alicuotaIVA */
    float         totalFactura;    /* TGF = netoGravado + importeIVA */
    EstadoFactura estado;
} Factura;
 
/* ============================================================
*  LÍNEA DE DETALLE          ->  detalle.dat
*  (una por cada ítem facturado, agrupadas por nroFactura)
* ============================================================ */
typedef struct {
    int   nroFactura;              /* FK -> Factura.nroFactura el FK es una clave Foranea, que significa, que es primaria en otra tabla*/
    int   codigoArticulo;          /* FK -> Articulo.codigo */
    char  descripcion[LARGO_DESC]; /* snapshot o foto del nombre */
    int   cantidad;
    float precioUnitario;          /* snapshot, neto */
    float subtotalLinea;           /* cantidad * precioUnitario */
} DetalleFactura;
 
#endif /*ESTRUCTURAS_H*/