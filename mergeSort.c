#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Contadores globales para medir el rendimiento del algoritmo */
static int comparisons = 0; // Número de comparaciones realizadas
static int writes = 0;      // Número de escrituras en el arreglo

/*
 * Convierte una cadena de texto a un entero positivo válido.
 *
 * Parámetros:
 * - text: cadena de entrada
 * - out: puntero donde se guardará el resultado si es válido
 *
 * Retorna:
 * - 1 si la conversión fue exitosa
 * - 0 si hubo error (cadena inválida, negativa, fuera de rango, etc.)
 */
int parse_positive_int(const char *text, int *out)
{
    char *end = NULL;
    long value = strtol(text, &end, 10);

    // Validaciones:
    // - No se convirtió nada
    // - Hay caracteres extra
    // - Número <= 0
    // - Número demasiado grande
    if (end == text || *end != '\0' || value <= 0 || value > 2000000000L)
    {
        return 0;
    }

    *out = (int)value; // Guardar resultado válido
    return 1;
}

/*
 * Genera un arreglo de números aleatorios en el rango [-range, +range].
 *
 * Parámetros:
 * - arr: arreglo a llenar
 * - size: número de elementos
 * - range: rango de valores (simétrico)
 */
void generate_random_array(int *arr, int size, int range)
{
    int bound = range * 2 + 1; // Total de valores posibles

    printf("INICIANDO generacion de datos\n");

    for (int i = 0; i < size; i++)
    {
        // Genera valores entre -range y +range
        arr[i] = (rand() % bound) - range;
        printf("GENERADO[%d] = %d\n", i, arr[i]);
    }
}

/*
 * Imprime el contenido del arreglo.
 *
 * Parámetros:
 * - arr: arreglo a imprimir
 * - size: tamaño del arreglo
 */
void print_array(const int *arr, int size)
{
    printf("ARREGLO: ");
    for (int i = 0; i < size; i++)
    {
        printf("%d", arr[i]);
        if (i < size - 1)
        {
            printf(" ");
        }
    }
    printf("\n");
}

/*
 * Mezcla (merge) dos subarreglos ordenados en uno solo.
 *
 * Parámetros:
 * - arr: inicio del subarreglo izquierdo
 * - mid: punto medio (inicio del subarreglo derecho)
 * - end: final del arreglo
 * - base: inicio del arreglo original (para imprimir índices)
 *
 * Retorna:
 * - 1 si tuvo éxito
 * - 0 si falla por falta de memoria
 */
int merge(int *arr, int *mid, int *end, int *base)
{
    // Tamaños de los subarreglos
    int left_size = (int)(mid - arr);
    int right_size = (int)(end - mid);

    // Índices relativos para impresión
    int left_start = (int)(arr - base);
    int left_end = left_start + left_size - 1;
    int right_start = (int)(mid - base);
    int right_end = right_start + right_size - 1;

    // Reservar memoria para copias temporales
    int *left = (int *)malloc((size_t)left_size * sizeof(int));
    int *right = (int *)malloc((size_t)right_size * sizeof(int));

    // Verificar memoria
    if (left == NULL || right == NULL)
    {
        free(left);
        free(right);
        return 0;
    }

    // Copiar datos a arreglos temporales
    memcpy(left, arr, (size_t)left_size * sizeof(int));
    memcpy(right, mid, (size_t)right_size * sizeof(int));

    printf("MEZCLANDO [%d..%d] con [%d..%d]\n", left_start, left_end, right_start, right_end);

    // Punteros de recorrido
    int *l = left;
    int *r = right;
    int *w = arr; // Escritura en arreglo original

    // Mezcla principal
    while (l < left + left_size && r < right + right_size)
    {
        comparisons++; // Contar comparación
        printf("COMPARANDO %d y %d\n", *l, *r);

        if (*l <= *r)
        {
            *w = *l;
            l++;
        }
        else
        {
            *w = *r;
            r++;
        }

        writes++; // Contar escritura
        printf("ESCRIBIENDO[%d] = %d\n", (int)(w - base), *w);
        w++;
    }

    // Copiar elementos restantes del subarreglo izquierdo
    while (l < left + left_size)
    {
        *w = *l;
        writes++;
        printf("ESCRIBIENDO[%d] = %d\n", (int)(w - base), *w);
        l++;
        w++;
    }

    // Copiar elementos restantes del subarreglo derecho
    while (r < right + right_size)
    {
        *w = *r;
        writes++;
        printf("ESCRIBIENDO[%d] = %d\n", (int)(w - base), *w);
        r++;
        w++;
    }

    // Liberar memoria temporal
    free(left);
    free(right);

    return 1;
}

/*
 * Implementación recursiva de Merge Sort.
 *
 * Parámetros:
 * - start: inicio del subarreglo
 * - end: final del subarreglo
 * - base: inicio del arreglo original
 *
 * Retorna:
 * - 1 si tuvo éxito
 * - 0 si falla (memoria insuficiente)
 */
int merge_sort(int *start, int *end, int *base)
{
    // Caso base: 0 o 1 elemento (ya está ordenado)
    if (end - start <= 1)
    {
        printf("CASO BASE en indice %d\n", (int)(start - base));
        return 1;
    }

    // Dividir el arreglo en dos mitades
    int *mid = start + (end - start) / 2;

    printf("DIVIDIENDO [%d..%d] y [%d..%d]\n",
           (int)(start - base),
           (int)(mid - base) - 1,
           (int)(mid - base),
           (int)(end - base) - 1);

    // Ordenar recursivamente ambas mitades
    if (!merge_sort(start, mid, base))
        return 0;

    if (!merge_sort(mid, end, base))
        return 0;

    // Mezclar ambas mitades ordenadas
    return merge(start, mid, end, base);
}

/*
 * Función principal del programa.
 *
 * Flujo:
 * 1. Valida argumentos
 * 2. Convierte entradas
 * 3. Reserva memoria
 * 4. Genera datos aleatorios
 * 5. Ejecuta merge sort
 * 6. Muestra resultados y métricas
 */
int main(int argc, char **argv)
{
    int size = 0;
    int range = 0;

    // Validar número de argumentos
    if (argc != 3)
    {
        printf("USO: %s <cantidad> <rango>\n", argv[0]);
        return 1;
    }

    // Convertir y validar argumentos
    if (!parse_positive_int(argv[1], &size) ||
        !parse_positive_int(argv[2], &range))
    {
        printf("ERROR: cantidad y rango deben ser enteros positivos.\n");
        return 1;
    }

    // Reservar memoria para el arreglo
    int *arr = (int *)malloc((size_t)size * sizeof(int));
    if (arr == NULL)
    {
        printf("ERROR: no se pudo reservar memoria para %d elementos.\n", size);
        return 1;
    }

    // Inicializar semilla aleatoria
    srand((unsigned int)time(NULL));

    printf("ARGUMENTOS: cantidad=%d rango=%d\n", size, range);

    // Generar arreglo aleatorio
    generate_random_array(arr, size, range);

    printf("INICIANDO merge sort\n");

    // Medir tiempo de ejecución
    clock_t start = clock();

    if (!merge_sort(arr, arr + size, arr))
    {
        printf("ERROR: fallo en merge sort por memoria insuficiente.\n");
        free(arr);
        return 1;
    }

    clock_t end = clock();

    printf("MERGE SORT TERMINADO\n");

    // Mostrar arreglo ordenado
    print_array(arr, size);

    // Mostrar métricas
    printf("COMPARACIONES: %d\n", comparisons);
    printf("ESCRITURAS: %d\n", writes);
    printf("TIEMPO_SEGUNDOS: %.6f\n",
           (double)(end - start) / CLOCKS_PER_SEC);

    // Liberar memoria
    free(arr);

    return 0;
}