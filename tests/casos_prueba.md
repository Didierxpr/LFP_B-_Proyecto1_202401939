# Casos de prueba preliminares (Parte 1)

Documento base de pruebas para ejecutar en la Parte 2 y Parte 3.

## Formato

- **Entrada**: contenido del archivo `.med`.
- **Esperado**: comportamiento del lexer.

## Casos minimos (10)

1. **Archivo valido completo**  
   Esperado: tokenizacion sin errores lexicos.

2. **Token invalido aislado (`@`)**  
   Esperado: error por caracter ilegal y continuidad del analisis.

3. **Cadena sin cerrar**  
   Esperado: error critico de cadena sin cerrar, continuar con siguiente linea.

4. **Fecha con mes invalido (`2025-13-01`)**  
   Esperado: error de fecha fuera de rango.

5. **Fecha con dia invalido (`2025-02-32`)**  
   Esperado: error de fecha fuera de rango.

6. **Hora invalida (`25:00`)**  
   Esperado: error de hora fuera de rango.

7. **Especialidad no reconocida (`CARDIO`)**  
   Esperado: error de enumeracion invalida.

8. **Tipo de sangre invalido (`"O++"`)**  
   Esperado: error de tipo de sangre invalido.

9. **Multiples errores en una sola entrada**  
   Esperado: acumulacion de todos los errores, sin detener ejecucion.

10. **Caso borde de 1000 lineas**  
    Esperado: tiempo de analisis dentro del objetivo (<3s segun enunciado).
