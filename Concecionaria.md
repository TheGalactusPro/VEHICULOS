# Análisis de Funcionalidades: Sistema de Gestión de Inventario y Clientes (SGIC)

**Materia:** ISWZ1102 – PROGRAMACIÓN 1[cite: 1]
**Profesor:** Carlos Andrés Guaita Ayala[cite: 1]
**Autores:** Joseph y Sebastian Almagro
**Empresa:** Concesionaria Ruedas de Oro[cite: 1]

---

## 1. Objetivos del Sistema
El SGIC tiene como propósito modernizar las operaciones de la concesionaria, reemplazando el registro manual por un sistema modular en C que gestione eficientemente el inventario de vehículos y los requerimientos de los clientes[cite: 1]. Todo esto asegurando la persistencia de datos mediante el uso de archivos[cite: 1].

## 2. Estructuras de Datos Propuestas (`structs`)
Para resolver el problema computacional, se requiere definir al menos tres estructuras principales[cite: 1]:

* `Vehiculo`: `id`, `marca` (ej. Chevrolet), `modelo`, `tipo` (ej. camioneta), `condicion` (nuevo/usado), `precio` (ej. 14000.00), `estado` (disponible/vendido)[cite: 1].
* `Cliente`: `id`, `nombre` (ej. Ruben), `edad` (ej. 56), `presupuesto`, `preferencia_marca`, `preferencia_tipo`[cite: 1].
* `Venta`: `id_venta`, `id_cliente`, `id_vehiculo`, `fecha`, `vendedor` (ej. José)[cite: 1].

## 3. Módulos y Funcionalidades Requeridas (SGIC)

### Módulo 1: Gestión de Archivos (Persistencia)
* **`cargarDatos()`:** Leer la información de los vehículos, clientes y ventas desde archivos al iniciar el programa[cite: 1].
* **`guardarDatos()`:** Escribir y actualizar toda la información transaccionada en los archivos antes de cerrar el programa o después de una venta[cite: 1].

### Módulo 2: Gestión de Inventario de Vehículos
* **`registrarVehiculo()`:** Añadir nuevos vehículos (nuevos o usados) al inventario[cite: 1].
* **`mostrarInventario()`:** Listar todos los vehículos registrados[cite: 1].
* **`buscarVehiculoPorFiltro()`:** Función crítica para resolver el caso de prueba[cite: 1]. Debe permitir filtrar el inventario por:
    * Tipo de vehículo (ej. "camioneta")[cite: 1].
    * Condición (ej. "usada")[cite: 1].
    * Marca (ej. "Chevrolet")[cite: 1].
    * Presupuesto máximo (ej. $14.000,00)[cite: 1].

### Módulo 3: Gestión de Clientes
* **`registrarCliente()`:** Guardar los datos de un cliente potencial, incluyendo sus preferencias y presupuesto[cite: 1].

### Módulo 4: Gestión de Ventas
* **`procesarVenta()`:** Vincular a un cliente con un vehículo disponible[cite: 1].
* **`registrarTransaccion()`:** Cambiar el estado del vehículo a "vendido" y guardar el registro de la venta en el sistema[cite: 1].

## 4. Flujo de Ejecución (Basado en el Caso de Prueba)
1.  **Ingreso:** El vendedor (José) atiende al cliente (Ruben)[cite: 1].
2.  **Registro:** Se ingresan los datos de Ruben (56 años, presupuesto $14.000,00) y sus preferencias (camioneta, usada, Chevrolet)[cite: 1].
3.  **Búsqueda:** El sistema filtra el inventario basándose en las 4 condiciones (tipo, estado, marca, precio <= 14000)[cite: 1].
4.  **Selección:** Se despliegan las opciones que cumplen los criterios[cite: 1].
5.  **Cierre:** Ruben elige un vehículo y el sistema registra formalmente la venta, actualizando los archivos[cite: 1].