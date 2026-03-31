# Diseno del AFD - MedLang (Parte 1)

Este documento define el diseno base del automata finito determinista (AFD) para el analizador lexico de MedLang.

## Restricciones obligatorias del proyecto

- Implementacion manual del AFD en `LexicalAnalyzer::nextToken()`.
- Prohibido `std::regex` para la tokenizacion principal.
- Reconocimiento caracter por caracter.
- Reporte exacto de linea y columna.
- Recuperacion de errores (continuar analisis tras error).

## Tokens objetivo (minimo obligatorio)

1. Palabras reservadas (`HOSPITAL`, `PACIENTES`, `MEDICOS`, `CITAS`, `DIAGNOSTICOS`, `paciente`, `medico`, `cita`, `diagnostico`, `con`).
2. Codigos con guion (`MED-001`, `PAC-123`).
3. Fecha (`AAAA-MM-DD`) validando rangos basicos.
4. Hora (`HH:MM`) validando `00:00` a `23:59`.
5. Enteros positivos.
6. Cadenas entre comillas dobles.
7. Enumeraciones de especialidad.
8. Enumeraciones de dosis.
9. Tipos de sangre restringidos.
10. Delimitadores (`{ } [ ] : ; ,`).

## Estados principales propuestos

- `S0` estado inicial.
- `S_ID` lectura de letras/guion bajo para reservadas/enums.
- `S_NUM` lectura de entero o inicio de fecha/hora.
- `S_DATE_1..S_DATE_4` secuencia para `AAAA-MM-DD`.
- `S_TIME_1..S_TIME_2` secuencia para `HH:MM`.
- `S_STR` lectura de cadena.
- `S_STR_ESC` manejo de escapes opcional.
- `S_CODE_A..S_CODE_N` secuencia para `AAA-999`.
- `S_DELIM` delimitadores de un caracter.
- `S_ERR` recuperacion de error.
- `S_EOF` fin de archivo.

## Tabla de transiciones resumida

| Estado | Entrada | Siguiente | Accion |
|---|---|---|---|
| S0 | Espacio/salto | S0 | Ignorar |
| S0 | Letra o `_` | S_ID | Iniciar lexema |
| S0 | Digito | S_NUM | Iniciar lexema |
| S0 | `"` | S_STR | Iniciar cadena |
| S0 | Delimitador | S_DELIM | Emitir delimitador |
| S0 | Otro | S_ERR | Registrar caracter ilegal |
| S_ID | Letra/digito/_ | S_ID | Acumular |
| S_ID | Otro | S0 | Clasificar (reservada/enum/identificador invalido) |
| S_NUM | Digito | S_NUM | Acumular |
| S_NUM | `-` despues de AAAA | S_DATE_1 | Continuar fecha |
| S_NUM | `:` despues de HH | S_TIME_1 | Continuar hora |
| S_NUM | Otro | S0 | Emitir entero |
| S_STR | `"` | S0 | Emitir cadena |
| S_STR | `\n` o EOF | S_ERR | Cadena sin cerrar |
| S_STR | Otro | S_STR | Acumular |

## Estrategia de recuperacion de errores

- **Error de caracter ilegal**: registrar error y avanzar un caracter.
- **Cadena sin cerrar**: registrar error critico y consumir hasta salto de linea/EOF.
- **Fecha u hora invalida**: registrar error y devolver token `Unknown` o token especializado invalido.
- **Enum no reconocida**: registrar error con sugerencia de valores validos.

## Clasificacion posterior (post-estado)

Cuando el AFD reconoce un lexema de letras, se evaluara en este orden:

1. Palabra reservada.
2. Especialidad valida.
3. Dosis valida.
4. Caso especial de tipo de sangre (si fue cadena).
5. Error de identificador no permitido.

## Plan de implementacion (Parte 2)

1. Implementar transicion explicita por estados con `switch`.
2. Crear funciones de apoyo:
   - `isReservedWord()`
   - `isSpecialtyEnum()`
   - `isDoseEnum()`
   - `isValidBloodType()`
   - `isValidDate()`
   - `isValidTime()`
3. Probar contra casos validos e invalidos definidos en `tests/casos_prueba.md`.
