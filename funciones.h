#ifndef FUNCIONES_H
#define FUNCIONES_H

/* Configuración */
#define MAX_VEHICULOS 500
#define MAX_CLIENTES 500
#define MAX_VENTAS 1000

/* Porcentaje de comisión del vendedor */
#define COMISION_PORCENTAJE 0.05

/* Nombres de archivos para persistencia (binarios .dat) */
#define FILE_VEHICULOS "vehiculos.dat"
#define FILE_CLIENTES "clientes.dat"
#define FILE_VENTAS "ventas.dat"

/* Tamaños de campos */
#define STR_SHORT 32
#define STR_MED   64
#define STR_SMALL 16
#define STR_DATE  20

/* Estructuras de dominio */
typedef struct {
    int id;
    char marca[STR_SHORT];
    char modelo[STR_SHORT];
    char tipo[STR_SMALL];
    char condicion[STR_SMALL];
    double precio;
    char estado[STR_SMALL];
} Vehiculo;

typedef struct {
    int id;
    char nombre[STR_MED];
    int edad;
    double presupuesto;
    char preferencia_marca[STR_SHORT];
    char preferencia_tipo[STR_SMALL];
} Cliente;

typedef struct {
    int id_venta;
    int id_cliente;
    int id_vehiculo;
    char fecha[STR_DATE];
    char vendedor[STR_MED];
} Venta;

/* Arrays en memoria y contadores */
extern Vehiculo vehiculos[MAX_VEHICULOS];
extern int vehiculos_count;

extern Cliente clientes[MAX_CLIENTES];
extern int clientes_count;

extern Venta ventas[MAX_VENTAS];
extern int ventas_count;

/* ----- Persistencia ----- */
int cargarDatos(void);
int guardarDatos(void);

/* ----- Vehículos / Inventario ----- */
int registrarVehiculo(const Vehiculo *v);
void mostrarInventario(void);

int buscarVehiculoPorFiltro(const char *tipo,
                            const char *condicion,
                            const char *marca,
                            double presupuesto,
                            int resultados[],
                            int maxResultados);

int buscarVehiculoPorID(int id);
void imprimirVehiculo(const Vehiculo *v);

/* ----- Clientes ----- */
int registrarCliente(const Cliente *c);
int buscarClientePorID(int id);

/* ----- Ventas ----- */
int procesarVenta(int id_cliente,
                  int id_vehiculo,
                  const char *vendedor);

int registrarTransaccion(const Venta *vt);

/* ----- Reportes de Ventas ----- */

/* Calcula la comisión de una venta según el precio del vehículo */
double calcularComisionVenta(int id_vehiculo);

/* Muestra todas las ventas realizadas por un vendedor,
   incluyendo total de ventas y total de comisiones */
void mostrarVentasPorVendedor(const char *vendedor);

/* ----- Generadores de ID ----- */
int generarIDVehiculo(void);
int generarIDCliente(void);
int generarIDVenta(void);

/* ----- Utilidades ----- */
void trim_newline(char *s);
int stricmp_local(const char *a, const char *b);

#endif /* FUNCIONES_H */