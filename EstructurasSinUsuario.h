#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_

#define MAX_PRODUCTOS 10
#define MAX_CLIENTES 10
#define MAX_FACTURAS 10
#define LARGO_NOMBRE 50
#define LARGO_NPROD 50
#define LARGO_DESC 100
#define LARGO_M_PRIMA 50

typedef struct 
{
    char nombreYApellido[LARGO_NOMBRE];
    int numeroDNI;
    
}Persona;

typedef struct 
{
    char nombreProducto[LARGO_NPROD];
    char descripcion[LARGO_DESC];
    int cantProd;
    float precioUnitario;
    //Materia prima del producto
    char materiaPrima[LARGO_M_PRIMA];
    int cantMPrima;
}Stock;

typedef struct 
{
    int dia;
    int mes;
    int anio;

}Fecha;

typedef struct 
{
    int nroFactura;
    Fecha fecha_emision;
    Persona cliente;
    float montoTotal;
    char estado[20];
    Stock productos[];
    
} Factura;

#endif /* ESTRUCTURAS_H_ */