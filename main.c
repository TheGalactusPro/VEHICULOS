/* main.c

Menú interactivo para SGIC y caso de prueba Ruben
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funciones.h"

/* Helpers de entrada */
static void read_line(char *buf, int size)
{
if (!fgets(buf, size, stdin))
{
buf[0] = '\0';
return;
}
trim_newline(buf);
}

/* Lee un entero usando fgets+sscanf. Requiere valor >= minValue. */
static int leerEntero(const char *prompt, int minValue)
{
char buf[128];
int val;
char extra;

while (1)
{
    if (prompt)
        printf("%s", prompt);

    if (!fgets(buf, sizeof(buf), stdin))
        return minValue;

    trim_newline(buf);

    if (sscanf(buf, " %d %c", &val, &extra) == 1)
    {
        if (val >= minValue)
            return val;

        printf("Entrada inválida: debe ser >= %d. Intente nuevamente.\n", minValue);
    }
    else
    {
        printf("Entrada inválida. Ingrese un número entero.\n");
    }
}

}

/* Lee un double usando fgets+sscanf. Requiere valor >= minValue. */
static double leerFlotante(const char *prompt, double minValue)
{
char buf[128];
double val;
char extra;

while (1)
{
    if (prompt)
        printf("%s", prompt);

    if (!fgets(buf, sizeof(buf), stdin))
        return minValue;

    trim_newline(buf);

    if (sscanf(buf, " %lf %c", &val, &extra) == 1)
    {
        if (val >= minValue)
            return val;

        printf("Entrada inválida: debe ser >= %.2f. Intente nuevamente.\n", minValue);
    }
    else
    {
        printf("Entrada inválida. Ingrese un número válido.\n");
    }
}

}

void opcion_registrar_vehiculo(void)
{
Vehiculo v = {0};
char tmp[128];

printf("Marca: ");
read_line(tmp, sizeof(tmp));
strncpy(v.marca, tmp, STR_SHORT - 1);

printf("Modelo: ");
read_line(tmp, sizeof(tmp));
strncpy(v.modelo, tmp, STR_SHORT - 1);

printf("Tipo (ej. camioneta): ");
read_line(tmp, sizeof(tmp));
strncpy(v.tipo, tmp, STR_SMALL - 1);

printf("Condicion (nuevo/usado): ");
read_line(tmp, sizeof(tmp));
strncpy(v.condicion, tmp, STR_SMALL - 1);

v.precio = leerFlotante("Precio: ", 0.01);

v.id = 0;
v.estado[0] = '\0';

int id = registrarVehiculo(&v);

if (id > 0)
    printf("Vehículo registrado con ID %d\n", id);
else
    printf("Error registrando vehículo\n");

}

void opcion_registrar_cliente(void)
{
Cliente c = {0};
char tmp[128];

printf("Nombre: ");
read_line(tmp, sizeof(tmp));
strncpy(c.nombre, tmp, STR_MED - 1);

c.edad = leerEntero("Edad: ", 1);
c.presupuesto = leerFlotante("Presupuesto: ", 0.01);

c.id = 0;

int id = registrarCliente(&c);

if (id > 0)
    printf("Cliente registrado con ID %d\n", id);
else
    printf("Error registrando cliente\n");

}

void opcion_buscar_filtrar(void)
{
char tipo[64] = "";
char condicion[64] = "";
char marca[64] = "";

double presupuesto = 0.0;
int resultados[100];

printf("Filtro - Tipo (enter para omitir): ");
read_line(tipo, sizeof(tipo));

printf("Filtro - Condicion (nuevo/usado) (enter para omitir): ");
read_line(condicion, sizeof(condicion));

printf("Filtro - Marca (enter para omitir): ");
read_line(marca, sizeof(marca));

printf("Filtro - Presupuesto max (0 para omitir): ");
presupuesto = leerFlotante("", 0.0);

int n = buscarVehiculoPorFiltro(
    tipo,
    condicion,
    marca,
    presupuesto,
    resultados,
    100);

if (n == 0)
{
    printf("No se encontraron vehículos que cumplan el filtro.\n");
    return;
}

printf("Se encontraron %d vehículos:\n", n);

for (int i = 0; i < n; i++)
{
    imprimirVehiculo(&vehiculos[resultados[i]]);
}

}

void opcion_procesar_venta(void)
{
int idc = leerEntero("ID cliente: ", 1);
int idv = leerEntero("ID vehículo: ", 1);

char vendedor[STR_MED];

printf("Vendedor: ");
read_line(vendedor, sizeof(vendedor));

if (procesarVenta(idc, idv, vendedor) == 0)
    printf("Venta procesada correctamente.\n");
else
    printf("Error procesando venta. Verifique IDs y estado del vehículo.\n");

}

/* NUEVA OPCIÓN */
void opcion_reporte_vendedor(void)
{
char vendedor[STR_MED];

printf("Nombre del vendedor: ");
read_line(vendedor, sizeof(vendedor));

mostrarVentasPorVendedor(vendedor);

}

void caso_prueba_ruben(void)
{
Cliente r = {0};

strncpy(r.nombre, "Ruben", STR_MED - 1);
r.edad = 56;
r.presupuesto = 14000.0;

strncpy(r.preferencia_marca, "Chevrolet", STR_SHORT - 1);
strncpy(r.preferencia_tipo, "camioneta", STR_SMALL - 1);

if (r.edad <= 0)
    r.edad = 56;

if (r.presupuesto <= 0.0)
    r.presupuesto = 14000.0;

int id_cliente = registrarCliente(&r);

printf("Cliente Ruben registrado con ID %d\n", id_cliente);

int resultados[100];

int n = buscarVehiculoPorFiltro(
    "camioneta",
    "usado",
    "Chevrolet",
    14000.0,
    resultados,
    100);

if (n == 0)
{
    printf("No hay opciones disponibles para Ruben.\n");
    return;
}

printf("Opciones encontradas para Ruben (%d):\n", n);

for (int i = 0; i < n; i++)
{
    imprimirVehiculo(&vehiculos[resultados[i]]);
}

int idx = resultados[0];
int id_veh = vehiculos[idx].id;

if (procesarVenta(id_cliente, id_veh, "Jose") == 0)
{
    printf(
        "Venta registrada: cliente %d compró vehículo %d (vendedor Jose)\n",
        id_cliente,
        id_veh);
}
else
{
    printf("Error registrando la venta de Ruben.\n");
}

}

int main(void)
{
cargarDatos();

int opcion = 0;

while (1)
{
    printf("\n--- SGIC - Menú ---\n");
    printf("1) Registrar vehículo\n");
    printf("2) Mostrar inventario\n");
    printf("3) Registrar cliente\n");
    printf("4) Buscar vehículos por filtro\n");
    printf("5) Procesar venta\n");
    printf("6) Guardar y salir\n");
    printf("7) Caso de prueba: Ruben\n");
    printf("8) Reporte de ventas por vendedor\n");
    printf("Elija una opción: ");

    opcion = leerEntero(NULL, 1);

    switch (opcion)
    {
        case 1:
            opcion_registrar_vehiculo();
            break;

        case 2:
            mostrarInventario();
            break;

        case 3:
            opcion_registrar_cliente();
            break;

        case 4:
            opcion_buscar_filtrar();
            break;

        case 5:
            opcion_procesar_venta();
            break;

        case 6:
            guardarDatos();
            printf("Datos guardados. Saliendo.\n");
            return 0;

        case 7:
            caso_prueba_ruben();
            break;

        case 8:
            opcion_reporte_vendedor();
            break;

        default:
            printf("Opción no válida.\n");
            break;
    }
}

return 0;

}