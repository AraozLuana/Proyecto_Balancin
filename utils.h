#ifndef UTILS_H_ // guardas de inclusión, para evitar que el contenido del archivo se incluya más de una vez
#define UTILS_H_

#include <stdio.h>
#include <stdbool.h> // Para usar el tipo de dato bool
#include <string.h>  // Este #include servirá para más adelante…
#include <ctype.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#else
#include <unistd.h>
#endif

typedef char cadena[200]; // Para tratar a los arrays de chars como 'cadena'

///////////////////////////////////////////////////////////////////////////
////////////////////////////  PROTOTIPOS  /////////////////////////////////
///////////////////////////////////////////////////////////////////////////

int leerEntero(const char *mensaje, int *numero);
float leerFloat(const char *mensaje, float *numero);
char leerCaracter(const char *mensaje);
int leerEnteroEntre(const char *mensaje, int valorMin, int valorMax);
float leerFloatEntre(const char *mensaje, float valorMin, float valorMax);
bool confirmaUsuario(const char *mensaje);
float sumarValores(float a, float b);

///////////////////////////////////////////////////////////////////////////
////////////////////////  PROTOTIPOS VECTORES /////////////////////////////
///////////////////////////////////////////////////////////////////////////

void leerVectorDeEnteros(const char *mensaje, int vec[], int cant);
void leerVectorDeFloats(const char *mensaje, float vec[], int cant);
void leerVectorDeCaracteres(const char *mensaje, char vec[], int cant);
void leerVectorDeEnterosEntre(const char *mensaje, int valorMin, int valorMax, int vec[], int cant);
void leerVectorDeFloatsEntre(const char *mensaje, float valorMin, float valorMax, float vec[], int cant);
void mostrarVectorDeEnteros(int vec[], int tam, char *tit);
void mostrarVectorDeFloats(float vec[], int tam, char *tit);
void mostrarVectorDeCaracteres(char vec[], int cant);
void ordenarVectorDeEnterosAscendenteConBurbujeo(int vec[], int cant);
void ordenarVectorDeEnterosDescendenteConBurbujeo(int vec[], int cant);
void ordenarPorBurbujeoEnteros(int vec[], int cant);
void ordenarPorBurbujeoFloats(float vec[], int cant);
void swapEntero(int *a, int *b);
void swapFloat(float *a, float *b);
int sumatoria(int vec[], int cant);
float promedio(int vec[], int cant);
void cargarVectorEntero(int vec[], int cantidad);
void cargarVectorFloat(float vec[], int cantidad);
void mostrarValoresMayoresQue(int vec[], int cant, int valor);
void cargarVectorRandomEntero(int vec[], int tam, int cs, int ci);
void cargarVectorRandomFloats(float vec[], int tam, float cs, float ci);
void mostrarVector(int vec[], int tam, char *tit);
void invertirEnElMismoVector(int vector[], int tam);

///////////////////////////////////////////////////////////////////////////
/////////////////////////  PROTOTIPOS CADENAS /////////////////////////////
///////////////////////////////////////////////////////////////////////////

void leerCadena(char *destino, int tamano, const char *mensaje);
void leerCadenaEntre(const char *mensaje, int minLength, int maxLength, char *destino, int tamano);
int sonIguales(char *texto, char *texto2);

///////////////////////////////////////////////////////////////////////////
////////////////////////  PROTOTIPOS MATRICES /////////////////////////////
///////////////////////////////////////////////////////////////////////////

void leerMatrizDeEnteros(const char *mensaje, int f, int c, int mat[f][c]);
void leerMatrizDeFloats(const char *mensaje, int f, int c, float mat[f][c]);
void generarMatrizAleatoriaDeEnteros(int f, int c, int mat[f][c], int cs, int ci);
void generarMatrizAleatoriaDeFloats(int f, int c, float mat[f][c], float cs, float ci);
void mostrarMatrizDeEnteros(int f, int c, int mat[f][c]);
void mostrarMatrizDeFloats(int f, int c, float mat[f][c]);

///////////////////////////////////////////////////////////////////////////
//////////////////////  PROTOTIPOS MAS FUNCIONES///////////////////////////
///////////////////////////////////////////////////////////////////////////

void fechaActual(int *dia, int *mes, int *anio);
void cargarFecha(cadena mensaje, int *dia, int *mes, int *anio);
void mostrarFecha(cadena mensaje, int dia, int mes, int anio);
void utf8();
void activarColoresANSI();
void pausa();
void retardo1(float seg);
void retardo2(float seg);
void linea(int x);
void limpiarBuffer();
void limpiarPantalla();
int generarNumAleatorioEntero(int valorMin, int valorMax);
int generarNumAleatorioFloat(float ci, float cs);
float raiz_cuadrada(float *n1);

///////////////////////////////////////////////////////////////////////////
/////////////////////////  IMPLEMENTACIONES  //////////////////////////////
///////////////////////////////////////////////////////////////////////////

int leerEntero(const char *mensaje, int *numero)
{
    char buffer[100];
    int flag = 1;

    while (flag)
    {
        printf("%s", mensaje);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL)
        {
            if (sscanf(buffer, "%d", numero) == 1)
            {
                flag = 0;
            }
            else
            {
                printf("Entrada inválida. Intente nuevamente.\n");
            }
        }
    }
    return *numero;
}
float leerFloat(const char *mensaje, float *numero)
{
    char buffer[100];
    int flag = 1;

    while (flag)
    {
        printf("%s", mensaje);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL)
        {
            if (sscanf(buffer, "%f", numero) == 1)
            {
                flag = 0;
            }
            else
            {
                printf("Entrada inválida. Intente nuevamente.\n");
            }
        }
    }
    return *numero;
}

char leerCaracter(const char *mensaje)
{
    /* Muestra el mensaje al usuario, lee un char y lo retorna */
    char caracter;
    printf("%s: ", mensaje);
    fflush(stdin); // No es obligatorio pero por las dudas...
    scanf("%c", &caracter);
    return caracter;
}

int leerEnteroEntre(const char *mensaje, int valorMin, int valorMax)
{
    /* Muestra el mensaje al usuario. Luego lee enteros mientras éstos estén
       fuera del rango [valorMin, valorMax] informando al usuario de
       su error. Cuando finalmente lea un entero válido, lo retorna */
    int numero;
    do
    {
        printf("%s", mensaje);
        scanf("%d", &numero);
        if (numero < valorMin || numero > valorMax)
        {
            printf("Valor invalido. Intente nuevamente.\n");
        }
    } while (numero < valorMin || numero > valorMax);
    return numero;
}

float leerFloatEntre(const char *mensaje, float valorMin, float valorMax)
{
    /* Muestra el mensaje al usuario. Luego, lee floats mientras éstos estén
       fuera del rango [valorMin, valorMax] informando al usuario de
       su error. Cuando finalmente lea un float válido, lo retorna */
    float numero;
    do
    {
        printf("\n%s", mensaje);
        scanf("%f", &numero);
        if (numero < valorMin || numero > valorMax)
        {
            printf("Valor invalido. Intente nuevamente.\n");
        }
    } while (numero < valorMin || numero > valorMax);
    return numero;
}

bool confirmaUsuario(const char *mensaje)
{
    /* Muestra el mensaje al usuario junto a la leyenda "[S/N]" que
       representa "Si" o "No". Luego, lee chars mientras éstos no sean los
       previstos ('S' o 'N', incluyendo minúsculas) informando al
       usuario de su error. Retorna si el usuario seleccionó que sí. */
    char confirmacion;
    do
    {
        printf("\n%s: ", mensaje);
        fflush(stdin);
        scanf("%c", &confirmacion);
        confirmacion = toupper(confirmacion);
        if (confirmacion != 'S' && confirmacion != 'N')
        {
            printf("Respuesta invalida. Intente nuevamente.\n");
        }
    } while (confirmacion != 'S' && confirmacion != 'N');
    return (confirmacion == 'S');
}

void mostrarValoresMayoresQue(int vec[], int cant, int valor)
{
    for (int i = 0; i < cant; i++)
    {
        if (vec[i] > valor)
        {
            printf("%d\n", vec[i]);
        }
    }
}

int sumatoria(int vec[], int cant)
{
    int sumatoria = 0;
    for (int i = 0; i < cant; i++)
    {
        sumatoria += vec[i];
    }
    return sumatoria;
}

float promedio(int vec[], int cant)
{
    return sumatoria(vec, cant) / (float)cant;
}

float sumarValores(float a, float b)
{
    return a + b;
}

///////////////////////////////////////////////////////////////////////////
////////////////////  IMPLEMENTACIONES VECTORES  /////////////////////////
/////////////////////////////////////////////////////////////////////////

void leerVectorDeEnteros(const char *mensaje, int vec[], int cant)
{
    /* Lee 'cant' enteros, mostrando el mismo mensaje cada vez,
    y guarda los valores en cada posición de 'vec' */
    for (int i = 0; i < cant; i++)
    {
        printf("%s (%d de %d): ", mensaje, i + 1, cant);
        scanf("%d", &vec[i]);
    }
}
void leerVectorDeFloats(const char *mensaje, float vec[], int cant)
{
    /* Lee 'cant' floats, mostrando el mismo mensaje cada vez,
    y guarda los valores en cada posición de 'vec' */
    for (int i = 0; i < cant; i++)
    {
        printf("%s (%d de %d): ", mensaje, i + 1, cant);
        vec[i] = leerFloat(mensaje, &vec[i]);
    }
}
void leerVectorDeCaracteres(const char *mensaje, char vec[], int cant)
{
    /* Lee 'cant' chars, mostrando el mismo mensaje cada vez,
    y guarda los valores en cada posición de 'vec' */
    for (int i = 0; i < cant; i++)
    {
        printf("%s (%d de %d): ", mensaje, i + 1, cant);
        scanf(" %c", &vec[i]);
    }
}
void leerVectorDeEnterosEntre(const char *mensaje, int valorMin, int valorMax, int vec[], int cant)
{
    /* Lee 'cant' enteros, dentro del rango [valorMin, valorMax],
    mostrando el mismo mensaje cada vez, y guarda los
    valores en cada posición de 'vec' */
    for (int i = 0; i < cant; i++)
    {
        vec[i] = leerEnteroEntre(mensaje, valorMin, valorMax);
    }
}
void leerVectorDeFloatsEntre(const char *mensaje, float valorMin, float valorMax, float vec[], int cant)
{
    for (int i = 0; i < cant; i++)
    {
        vec[i] = leerFloatEntre(mensaje, valorMin, valorMax);
    }
}

void cargarVectorEntero(int vec[], int cantidad)
{
    for (int i = 0; i < cantidad; i++)
    {
        leerEntero("Ingrese un valor entero: ",&vec[i]);
    }
}

void cargarVectorFloat(float vec[], int cantidad)
{
    for (int i = 0; i < cantidad; i++)
    {
        leerFloat("Ingrese un valor float: ", &vec[i]);
    }
}

void cargarVectorRandomEntero(int vec[], int tam, int cs, int ci)
{
    int i;
    for (i = 0; i < tam; i++)
    {
        vec[i] = rand() % (cs - ci + 1) + ci;
    }
}

void cargarVectorRandomFloats(float vec[], int tam, float cs, float ci)
{
    int i;
    for (i = 0; i < tam; i++)
    {
        vec[i] = ((float)rand() / RAND_MAX) * (cs - ci) + ci;
    }
}

void mostrarVector(int vec[], int tam, char *tit)
{
    int i;
    printf("%-30s\n",tit);
    for(i=0;i<tam;i++)
    {
        printf("vec[%2d]=%2d\n", i + 1,vec[i] );
    }
}

void invertirEnElMismoVector(int vector[], int tam)
{
    int aux;
    int j = tam - 1;
    for (int i = 0; i < j; i++)
    {
        aux = vector[i];
        vector[i] = vector[j];
        vector[j] = aux;
        j--;
    }
}

void mostrarVectorDeEnteros(int vec[], int tam, char *tit)
{
    int i;
    printf("%-30s\n", tit);
    for (i = 0; i < tam; i++)
    {
        printf("vec[%2d]=%2d\n", i, vec[i]);
    }
}

void mostrarVectorDeFloats(float vec[], int tam, char *tit)
{
    int i;
    printf("%-30s\n", tit);
    for (i = 0; i < tam; i++)
    {
        printf("vec[%2d]=%.2f\n", i + 1, vec[i]);
    }
}
void mostrarVectorDeCaracteres(char vec[], int cant)
{
    /* Muestra el contenido de 'vec' en renglones separados,
    Incluyendo también el número de posición (empezando desde cero) */
    for (int i = 0; i < cant; i++)
    {
        printf("[%d] => %c\n", i, vec[i]);
    }
}
void ordenarVectorDeEnterosAscendenteConBurbujeo(int vec[], int cant)
{
    /* Ordena los valores de 'vec' de forma ascendente (menor a mayor),
     utilizando algún algoritmo de ordenamiento (investigar) */
    int k = 1, cota = cant;
    while (k)
    {
        k = 0;
        for (int i = 0; i < cota - 1; i++)
        {
            for (int j = 0; j < cota - i - 1; j++)
            {
                if (vec[j] > vec[j + 1])
                {
                    swapEntero(&vec[j], &vec[j + 1]);
                    k=i;
                }
            }
        }
        cota = k;
    }
}
void ordenarVectorDeEnterosDescendenteConBurbujeo(int vec[], int cant)
{
    /* Ordena los valores de 'vec' de forma descendente (mayor a menor),
     utilizando algún algoritmo de ordenamiento (investigar) distinto al
     anterior */
    for (int i = 0; i < cant - 1; i++)
    {
        for (int j = 0; j < cant - i - 1; j++)
        {
            if (vec[j] < vec[j + 1])
            {
                swapEntero(&vec[j], &vec[j + 1]);
            }
        }
    }
}

void ordenarPorBurbujeoEnteros(int vec[], int cant)
{
    for (int i = 0; i < cant - 1; i++)
    {
        for (int j = 0; j < cant - i - 1; j++)
        {
            if (vec[j + 1] < vec[j])
            {
                swapEntero(&vec[j], &vec[j + 1]);
            }
        }
    }

    /*for (int i = 0; i < cant-1; i++) {
        for (int j = 0; j < cant-i-1; j++) {
            if ( strcmp(vec[j+1], vec[j]) < 0 ) {
                swap(&vec[j], &vec[j+1]);
            }
        }
    }*/
}

void ordenarPorBurbujeoFloats(float vec[], int cant)
{
    for (int i = 0; i < cant - 1; i++)
    {
        for (int j = 0; j < cant - i - 1; j++)
        {
            if (vec[j + 1] < vec[j])
            {
                swapFloat(&vec[j], &vec[j + 1]);
            }
        }
    }
}

void swapEntero(int *a, int *b)
{
    int aux = *a;
    *a = *b;
    *b = aux;
}

void swapFloat(float *a, float *b)
{
    float aux = *a;
    *a = *b;
    *b = aux;
}

///////////////////////////////////////////////////////////////////////////
////////////////////  IMPLEMENTACIONES CADENAS  //////////////////////////
/////////////////////////////////////////////////////////////////////////

void leerCadena(char *destino, int tamano, const char *mensaje)
{
    printf("%s", mensaje);

    if (fgets(destino, tamano, stdin) != NULL)
    {
        size_t longitud = strlen(destino);
        if (longitud > 0 && destino[longitud - 1] == '\n')
            destino[longitud - 1] = '\0';
        else
        {
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
        }
    }
}

void leerCadenaEntre(const char *mensaje, int minLength, int maxLength, char *destino, int tamano)
{
    int len;
    leerCadena(destino, tamano, mensaje);
    len = strlen(destino);
    while (len < minLength || len > maxLength)
    {
        printf("La cadena debe tener entre %d y %d caracteres. Intente nuevamente.\n", minLength, maxLength);
        leerCadena(destino, tamano, mensaje);
        len = strlen(destino);
    }
}

/*int sonIguales(char *texto, char *texto2)
{
    return strcmp(texto, texto2) == 0;
}*/

///////////////////////////////////////////////////////////////////////////
////////////////////  IMPLEMENTACIONES MATRICES //////////////////////////
/////////////////////////////////////////////////////////////////////////

void leerMatrizDeEnteros(const char *mensaje, int f, int c, int mat[f][c])
{
    /* Muestra el mensaje al usuario y lee tantos enteros como
    corresponda, alojándolos en cada posición de 'mat' */

    for (int i = 0; i < f; i++)
    {
        for (int j = 0; j < c; j++)
        {
            printf("Elemento [%d][%d]: ", i, j);
            leerEntero(mensaje,&mat[i][j]);
        }
    }
}

void leerMatrizDeFloats(const char *mensaje, int f, int c, float mat[f][c])
{
    /* Muestra el mensaje al usuario y lee tantos floats como
    corresponda, alojándolos en cada posición de 'mat' */
    for (int i = 0; i < f; i++)
    {
        for (int j = 0; j < c; j++)
        {
            printf("Elemento [%d][%d]: ", i, j);
            leerFloat(mensaje,&mat[i][j]);
        }
    }
}

void generarMatrizAleatoriaDeEnteros(int f, int c, int mat[f][c], int cs, int ci)
{
    for (int i = 0; i < f; i++)
    {
        for (int j = 0; j < c; j++)
        {
            mat[i][j] = rand() % (cs - ci + 1) + ci;
        }
    }
}

void generarMatrizAleatoriaDeFloats(int f, int c, float mat[f][c], float cs, float ci)
{
    for (int i = 0; i < f; i++)
    {
        for (int j = 0; j < c; j++)
        {
            mat[i][j] = ((float)rand() / RAND_MAX) * (cs - ci) + ci;
        }
    }
}

void mostrarMatrizDeEnteros(int f, int c, int mat[f][c])
{
    /* Muestra el contenido de 'mat' en forma de matriz,
    anteponiendo a cada valor los índices [f][c]
    Ejemplo, para una matriz de 2x3:
    [0][0] = 18 [0][1] = 52 [0][2] = 33
    [1][0] = 47 [1][1] = 29 [1][2] = 82
    */
    for (int i = 0; i < f; i++)
    {
        for (int j = 0; j < c; j++)
        {
            printf("[%d][%d] = %d ", i, j, mat[i][j]);
        }
        printf("\n");
    }
}
void mostrarMatrizDeFloats(int f, int c, float mat[f][c])
{
    /* Muestra el contenido de 'mat' en forma de matriz,
    anteponiendo a cada valor los índices [f][c]
    Ejemplo:
    [0][0] = 18.2 [0][1] = 52.3 [0][2] = 33.0
    [1][0] = 47.5 [1][1] = 29.7 [1][2] = 82.9
    */
    for (int i = 0; i < f; i++)
    {
        for (int j = 0; j < c; j++)
        {
            printf("Elemento [%d][%d] = %f ", i, j, mat[i][j]);
        }
        printf("\n");
    }
}

///////////////////////////////////////////////////////////////////////////
//////////////////////  IMPLEMENTACIONES EXTRA ///////////////////////////
/////////////////////////////////////////////////////////////////////////
void fechaActual(int *dia, int *mes, int *anio)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    *dia = tm.tm_mday;
    *mes = tm.tm_mon + 1; // Los meses en struct tm van de 0 a 11
    *anio = tm.tm_year + 1900; // El año en struct tm es desde 1900
}

void cargarFecha(cadena mensaje, int *dia, int *mes, int *anio)
{
    printf("%s\n", mensaje);
    do
    {
        *dia = leerEnteroEntre("Ingrese dia (1-31) ", 1, 31);
        *mes = leerEnteroEntre("Ingrese mes (1-12) ", 1, 12);
        // Validar que febrero no tenga más de 29 días
        if (*mes == 2 && *dia > 29)
        {
            printf("Error: Febrero no puede tener mas de 29 dias. Intente nuevamente.\n");
            *dia = leerEnteroEntre("Ingrese dia (1-31) ", 1, 31);
            *mes = leerEnteroEntre("Ingrese mes (1-12) ", 1, 12);
        }
    } while (*mes == 2 && *dia > 29);

    *anio = leerEnteroEntre("Ingrese anio (1900-2100) ", 1900, 2100);
}

void mostrarFecha(cadena mensaje, int dia, int mes, int anio)
{
    printf("%s %02d/%02d/%04d", mensaje, dia, mes, anio);
}

void utf8()
{
#ifdef _WIN32
    system("chcp 65001");
#endif
    limpiarPantalla();
}

void activarColoresANSI()
{
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

void pausa()
{
#ifdef _WIN32
    printf("\n");
    system("pause");
#else
    printf("Presione ENTER para continuar...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ; // limpiar buffer
    getchar();
    printf("\n"); // esperar ENTER real
#endif
}

void retardo1(float seg)
{
    printf("CARGANDO");
#ifdef _WIN32
    for (int i = 0; i < 3; i++)
    {
        Sleep((DWORD)(seg * 1000)); // Sleep recibe millisegundos en Windows
        printf(".");
    }
#else
    for (int i = 0; i < 3; i++)
    {
        usleep((useconds_t)(seg * 1000000)); // usleep recibe microsegundos en Linux
        printf(".");
    }
#endif
    printf("\n");
}

void retardo2(float seg)
{
#ifdef _WIN32
    for (int i = 0; i < 3; i++)
    {
        Sleep((DWORD)(seg * 1000)); // Sleep recibe millisegundos en Windows
    }
#else
    for (int i = 0; i < 3; i++)
    {
        usleep((useconds_t)(seg * 1000000)); // usleep recibe microsegundos en Linux
        printf(".");
    }
#endif
    printf("\n");
}

// *********************************************************************************
// simplemente hace una linea de asteriscos, para mejorar la presentación en consola
// *********************************************************************************
void linea(int x)
{
    int i;
    for (i = 0; i < x; i++)
        printf("*");
    printf("\n");
}
// *********************************************************************************
// Función para limpiar el búfer de entrada (stdin)
// *********************************************************************************
void limpiarBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

// *********************************************************************************
// limpia pantalla de forma multiplataforma
// *********************************************************************************
void limpiarPantalla()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
// Función para darle color a la consola (Windows)
void Color()
{
#ifdef _WIN32
    system("color 91");
#endif
}

int generarNumAleatorioEntero(int ci, int cs)
{
    // srand(time(NULL));
    return (ci + (rand() % (cs - ci + 1)));
}

int generarNumAleatorioFloat(float ci, float cs)
{
    // srand(time(NULL));
    return (((float) rand()/ RAND_MAX)* (cs-ci) + ci);
}

float raiz_cuadrada(float *n1)
{
    float raizCuadrada = sqrt(*n1);

    return raizCuadrada;
}

#endif /* UTILS_H_ */