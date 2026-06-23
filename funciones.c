/* funciones.c
 * Implementación para SGIC - Sistema de Gestión de Inventario y Clientes
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "funciones.h"

/* Definición de arrays y contadores (provenientes de la cabecera) */
Vehiculo vehiculos[MAX_VEHICULOS];
int vehiculos_count = 0;

Cliente clientes[MAX_CLIENTES];
int clientes_count = 0;

Venta ventas[MAX_VENTAS];
int ventas_count = 0;

/* ---------------------- Helpers ---------------------- */
void trim_newline(char *s) {
    if (!s) return;
    size_t len = strlen(s);
    while (len > 0 && (s[len-1] == '\n' || s[len-1] == '\r')) {
        s[len-1] = '\0';
        len--;
    }
}

int stricmp_local(const char *a, const char *b) {
    if (!a && !b) return 0;
    if (!a) return -1;
    if (!b) return 1;
    while (*a && *b) {
        char ca = tolower((unsigned char)*a);
        char cb = tolower((unsigned char)*b);
        if (ca != cb) return (unsigned char)ca - (unsigned char)cb;
        a++; b++;
    }
    return (unsigned char)tolower((unsigned char)*a) - (unsigned char)tolower((unsigned char)*b);
}

/* ---------------------- Generadores de ID ---------------------- */
int generarIDVehiculo(void) {
    int max = 0;
    for (int i = 0; i < vehiculos_count; ++i) if (vehiculos[i].id > max) max = vehiculos[i].id;
    return max + 1;
}

int generarIDCliente(void) {
    int max = 0;
    for (int i = 0; i < clientes_count; ++i) if (clientes[i].id > max) max = clientes[i].id;
    return max + 1;
}

int generarIDVenta(void) {
    int max = 0;
    for (int i = 0; i < ventas_count; ++i) if (ventas[i].id_venta > max) max = ventas[i].id_venta;
    return max + 1;
}

/* ---------------------- Persistencia ---------------------- */
static int parse_double(const char *s, double *out) {
    if (!s || !out) return -1;
    char *end; double v = strtod(s, &end);
    if (end == s) return -1;
    *out = v; return 0;
}

int cargarDatos(void) {
    FILE *f;

    /* Vehículos (binario): primero un int con la cantidad, luego los structs */
    f = fopen(FILE_VEHICULOS, "rb");
    if (f) {
        int cnt = 0;
        if (fread(&cnt, sizeof(int), 1, f) == 1) {
            if (cnt > MAX_VEHICULOS) cnt = MAX_VEHICULOS;
            if (cnt > 0) {
                size_t got = fread(vehiculos, sizeof(Vehiculo), (size_t)cnt, f);
                vehiculos_count = (int)got;
            } else {
                vehiculos_count = 0;
            }
        }
        fclose(f);
    } else {
        vehiculos_count = 0;
    }

    /* Clientes (binario) */
    f = fopen(FILE_CLIENTES, "rb");
    if (f) {
        int cnt = 0;
        if (fread(&cnt, sizeof(int), 1, f) == 1) {
            if (cnt > MAX_CLIENTES) cnt = MAX_CLIENTES;
            if (cnt > 0) {
                size_t got = fread(clientes, sizeof(Cliente), (size_t)cnt, f);
                clientes_count = (int)got;
            } else {
                clientes_count = 0;
            }
        }
        fclose(f);
    } else {
        clientes_count = 0;
    }

    /* Ventas (binario) */
    f = fopen(FILE_VENTAS, "rb");
    if (f) {
        int cnt = 0;
        if (fread(&cnt, sizeof(int), 1, f) == 1) {
            if (cnt > MAX_VENTAS) cnt = MAX_VENTAS;
            if (cnt > 0) {
                size_t got = fread(ventas, sizeof(Venta), (size_t)cnt, f);
                ventas_count = (int)got;
            } else {
                ventas_count = 0;
            }
        }
        fclose(f);
    } else {
        ventas_count = 0;
    }

    return 0;
}

int guardarDatos(void) {
    FILE *f;

    /* Vehículos (binario): escribir count + array */
    f = fopen(FILE_VEHICULOS, "wb");
    if (!f) return -1;
    int cnt_v = vehiculos_count;
    if (fwrite(&cnt_v, sizeof(int), 1, f) != 1) { fclose(f); return -1; }
    if (cnt_v > 0) {
        if (fwrite(vehiculos, sizeof(Vehiculo), (size_t)cnt_v, f) != (size_t)cnt_v) { fclose(f); return -1; }
    }
    fclose(f);

    /* Clientes (binario) */
    f = fopen(FILE_CLIENTES, "wb");
    if (!f) return -1;
    int cnt_c = clientes_count;
    if (fwrite(&cnt_c, sizeof(int), 1, f) != 1) { fclose(f); return -1; }
    if (cnt_c > 0) {
        if (fwrite(clientes, sizeof(Cliente), (size_t)cnt_c, f) != (size_t)cnt_c) { fclose(f); return -1; }
    }
    fclose(f);

    /* Ventas (binario) */
    f = fopen(FILE_VENTAS, "wb");
    if (!f) return -1;
    int cnt_t = ventas_count;
    if (fwrite(&cnt_t, sizeof(int), 1, f) != 1) { fclose(f); return -1; }
    if (cnt_t > 0) {
        if (fwrite(ventas, sizeof(Venta), (size_t)cnt_t, f) != (size_t)cnt_t) { fclose(f); return -1; }
    }
    fclose(f);

    return 0;
}

/* ---------------------- Vehículos / Inventario ---------------------- */
int registrarVehiculo(const Vehiculo *v) {
    if (!v) return -1;
    if (vehiculos_count >= MAX_VEHICULOS) return -1;
    Vehiculo copy = *v;
    if (copy.id <= 0) copy.id = generarIDVehiculo();
    /* Asegurar estado por defecto */
    if (strlen(copy.estado) == 0) strncpy(copy.estado, "disponible", STR_SMALL-1);
    vehiculos[vehiculos_count++] = copy;
    return copy.id;
}

void mostrarInventario(void)
{
    printf("--- Inventario (%d) ---\n", vehiculos_count);
    /* Cabecera con anchos fijos /
    printf("%-5s | %-12s | %-12s | %-12s | %-10s | %10s | %-10s\n",
    "ID", "Marca", "Modelo", "Tipo", "Condicion", "Precio", "Estado");
    / Línea separadora con los mismos anchos */
    printf("%-5s | %-12s | %-12s | %-12s | %-10s | %10s | %-10s\n",
           "-----", "------------", "------------", "------------", "----------", "----------", "----------");
    for (int i = 0; i < vehiculos_count; ++i)
    {
        imprimirVehiculo(&vehiculos[i]);
    }
}

int buscarVehiculoPorFiltro(const char *tipo,
                           const char *condicion,
                           const char *marca,
                           double presupuesto,
                           int resultados[],
                           int maxResultados) {
    int found = 0;
    for (int i = 0; i < vehiculos_count && found < maxResultados; ++i) {
        Vehiculo *v = &vehiculos[i];
        /* Ignorar vehículos que ya están vendidos */
        if (stricmp_local(v->estado, "vendido") == 0) continue;

        if (tipo && strlen(tipo) > 0 && stricmp_local(tipo, v->tipo) != 0) continue;
        if (condicion && strlen(condicion) > 0 && stricmp_local(condicion, v->condicion) != 0) continue;
        if (marca && strlen(marca) > 0 && stricmp_local(marca, v->marca) != 0) continue;
        if (presupuesto > 0.0 && v->precio > presupuesto) continue;

        resultados[found++] = i;
    }
    return found;
}

int buscarVehiculoPorID(int id) {
    for (int i = 0; i < vehiculos_count; ++i) if (vehiculos[i].id == id) return i;
    return -1;
}

void imprimirVehiculo(const Vehiculo *v)
{
    if (!v)
        return;
    /* Imprime respetando los mismos anchos que la cabecera */
        printf("%-5d | %-12s | %-12s | %-12s | %-10s | %10.2f | %-10s\n",
               v->id,
               v->marca,
               v->modelo,
               v->tipo,
               v->condicion,
               v->precio,
               v->estado);
}

/* ---------------------- Clientes ---------------------- */
int registrarCliente(const Cliente *c) {
    if (!c) return -1;
    if (clientes_count >= MAX_CLIENTES) return -1;
    Cliente copy = *c;
    if (copy.id <= 0) copy.id = generarIDCliente();
    clientes[clientes_count++] = copy;
    return copy.id;
}

int buscarClientePorID(int id) {
    for (int i = 0; i < clientes_count; ++i) if (clientes[i].id == id) return i;
    return -1;
}

/* ---------------------- Ventas ---------------------- */
int registrarTransaccion(const Venta *vt) {
    if (!vt) return -1;
    if (ventas_count >= MAX_VENTAS) return -1;
    Venta copy = *vt;
    if (copy.id_venta <= 0) copy.id_venta = generarIDVenta();
    ventas[ventas_count++] = copy;
    return copy.id_venta;
}

int procesarVenta(int id_cliente, int id_vehiculo, const char *vendedor) {
    int idx_v = buscarVehiculoPorID(id_vehiculo);
    if (idx_v < 0) return -1; /* vehículo no existe */
    if (stricmp_local(vehiculos[idx_v].estado, "disponible") != 0) return -1; /* no disponible */

    int idx_c = buscarClientePorID(id_cliente);
    if (idx_c < 0) return -1; /* cliente no existe */

    /* Marcar como vendido */
    strncpy(vehiculos[idx_v].estado, "vendido", STR_SMALL-1);

    /* Crear venta */
    Venta vt = {0};
    vt.id_vehiculo = id_vehiculo;
    vt.id_cliente = id_cliente;
    if (vendedor) strncpy(vt.vendedor, vendedor, STR_MED-1);

    /* Fecha actual YYYY-MM-DD */
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(vt.fecha, STR_DATE, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    vt.id_venta = generarIDVenta();
    if (registrarTransaccion(&vt) < 0) return -1;

    /* Persistir cambios inmediatamente */
    if (guardarDatos() != 0) return -1;

    return 0;
}

/* ---------------------- Reportes de Ventas ---------------------- */

double calcularComisionVenta(int id_vehiculo)
{
    int idx = buscarVehiculoPorID(id_vehiculo);

    if (idx < 0)
        return 0.0;

    return vehiculos[idx].precio * COMISION_PORCENTAJE;
}

void mostrarVentasPorVendedor(const char *vendedor)
{
    if (!vendedor || strlen(vendedor) == 0)
    {
        printf("Nombre de vendedor inválido.\n");
        return;
    }

    int totalVentas = 0;
    double totalComisiones = 0.0;

    printf("\n===============================================================\n");
    printf("REPORTE DE VENTAS DEL VENDEDOR: %s\n", vendedor);
    printf("===============================================================\n");

    printf("%-8s | %-12s | %-8s | %-9s | %-12s | %-12s\n",
           "Venta",
           "Fecha",
           "Cliente",
           "Vehiculo",
           "Precio",
           "Comision");

    printf("--------------------------------------------------------------------------\n");

    for (int i = 0; i < ventas_count; i++)
    {
        if (stricmp_local(ventas[i].vendedor, vendedor) == 0)
        {
            int idxVehiculo = buscarVehiculoPorID(ventas[i].id_vehiculo);

            double precio = 0.0;
            double comision = 0.0;

            if (idxVehiculo >= 0)
            {
                precio = vehiculos[idxVehiculo].precio;
                comision = precio * COMISION_PORCENTAJE;
            }

            printf("%-8d | %-12s | %-8d | %-9d | %-12.2f | %-12.2f\n",
                   ventas[i].id_venta,
                   ventas[i].fecha,
                   ventas[i].id_cliente,
                   ventas[i].id_vehiculo,
                   precio,
                   comision);

            totalVentas++;
            totalComisiones += comision;
        }
    }

    printf("--------------------------------------------------------------------------\n");
    printf("TOTAL DE VENTAS      : %d\n", totalVentas);
    printf("TOTAL DE COMISIONES  : $%.2f\n", totalComisiones);
    printf("===============================================================\n");
}