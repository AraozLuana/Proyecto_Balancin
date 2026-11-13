# Balancín

> *Tu negocio, en equilibrio.*

Sistema de gestión y facturación para pequeños emprendimientos, con interacción por consola.
Permite registrar **clientes** y **productos**, emitir **facturas** y consultar **estadísticas** de ventas, guardando toda la información en archivos binarios.

Proyecto final de **Programación I** · INSPT-UTN · Tecnicatura Superior en Informática Aplicada · Comisión 1.605 · 2026.

**Integrantes:** Araoz Luana, Gamboggi Sofia, Fernandez Tomas y Palazzo Ramiro.

---

## Contenido del repositorio

| Archivo | Descripción |
|---|---|
| `Facturitas.c` | Programa principal. |
| `FactuFunciones.h` | Funciones del sistema: login, menús, ABM, facturación y estadísticas. |
| `Estructuras.h` | Constantes y definición de los tipos de datos (`Usuario`, `Persona`, `Cliente`, `Articulo`, `Fecha`, `Factura`, `DetalleFactura`). |
| `utiles_Proyecto.h` | Biblioteca de la cátedra (adaptada): lectura segura, limpieza de pantalla, pausas, etc. |
| `Informe_Tecnico_Balancin.pdf` | Informe técnico del proyecto. |

---

## Funcionalidades

**Administrador**
- Alta y baja (lógica) de gestores.
- Listado de usuarios.

**Gestor**
- **Facturas:** generar nueva factura, ver listado, anular factura.
- **Clientes:** ver listado, agregar y dar de baja.
- **Productos:** ver listado, agregar y dar de baja.
- **Estadísticas:** producto estrella (el que más recaudó), producto más vendido y los tres mejores clientes.

**Características generales**
- Menú con cursores: navegación con flechas, Enter o atajo numérico.
- Persistencia en archivos binarios `.dat`.
- Bajas lógicas: los registros no se borran, se marcan como inactivos.
- Las facturas guardan una copia (*snapshot*) de los datos del cliente y de cada producto.
- Validación de entradas y opción de cancelar con `-1` en la mayoría de los formularios.

---

## Requisitos

- **Sistema operativo:** Windows (el código usa `conio.h` y `windows.h`).
- **Compilador de C** con soporte C99, por ejemplo GCC (MinGW).
- **Terminal** con soporte de UTF-8 y secuencias ANSI (Windows Terminal o consola moderna de Windows).

---

## Compilación y ejecución

1. Clonar el repositorio o descargar los archivos en una misma carpeta.
2. Compilar desde esa carpeta:

   ```bash
   gcc Facturitas.c -o Balancin.exe
   ```

3. Ejecutar:

   ```bash
   Balancin.exe
   ```

Los archivos de datos (`bd_Users.dat`, `bd_Clients.dat`, `bd_Products.dat`, `bd_Facturas.dat` y `bd_Detalles.dat`) se crean automáticamente en la carpeta de ejecución.

---

## Guía de uso

### Primer uso

Al iniciar por primera vez no hay datos cargados. Seguí este orden:

1. **Iniciar sesión como Administrador** y dar de alta al menos un gestor.
2. **Iniciar sesión como Gestor** con el usuario creado.
3. Cargar **clientes** y **productos**.
4. Generar **facturas** y consultar las **estadísticas**.

### Navegación

- Usá las **flechas ↑ ↓** para moverte por los menús y **Enter** para elegir. 
- En los formularios, ingresar **`-1`** cancela la operación y vuelve al menú.

### Credenciales de administrador

El inicio de sesión de administrador pide un nombre de la lista de administradores habilitados (`Sofia`, `Ramiro`, `Luana` o `Tomas`) y la clave `admin123`.
Se tienen 4 intentos; si se agotan, el programa se cierra.

### Generar una factura

1. Ingresar el código de un cliente **activo**.
2. Ingresar código y cantidad de cada producto (máximo 20 líneas). Ingresar `0` para terminar.
3. El sistema descuenta el stock, calcula el total y muestra la factura.

Al **anular** una factura se restablece el stock de sus productos y la factura queda marcada como anulada (no se elimina).

---

## Informe técnico

La documentación completa del proyecto (arquitectura, modelo de datos, decisiones de diseño y etapas de desarrollo) está en el archivo `Informe_Tecnico_Balancin.pdf`.