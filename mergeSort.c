#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int llenarArrayManual(double *arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("Ingrese el elemento %d: ", i + 1);
        if (scanf("%lf", &arr[i]) != 1)
        {
            fprintf(stderr, "Error al leer el elemento %d\n", i + 1);
            i--;
            while (getchar() != '\n')
                ;
        }
    }
    return 0;
}

int llenarArrayAuto(double *arr, int size)
{
    srand(time(NULL));
    for (int i = 0; i < size; i++)
    {
        // Genera doubles aleatorios entre -100000.0 y 100000.0
        arr[i] = ((double)rand() / RAND_MAX) * 200000.0 - 100000.0;
    }
    printf("Array llenado automaticamente con %d numeros aleatorios.\n", size);
    return 0;
}

int merge(double *arr, double *mid, double *fin)
{
    int leftSize = mid - arr;
    int rightSize = fin - mid;

    double *left = (double *)malloc(leftSize * sizeof(double));
    double *right = (double *)malloc(rightSize * sizeof(double));

    if (!left || !right)
    {
        fprintf(stderr, "Error de memoria en merge\n");
        free(left);
        free(right);
        return -1;
    }

    memcpy(left, arr, leftSize * sizeof(double));
    memcpy(right, mid, rightSize * sizeof(double));

    double *l = left, *r = right, *f = arr;

    while (l < left + leftSize && r < right + rightSize)
    {
        if (*l <= *r)
            *f++ = *l++;
        else
            *f++ = *r++;
    }

    while (l < left + leftSize)
        *f++ = *l++;

    while (r < right + rightSize)
        *f++ = *r++;

    free(left);
    free(right);
    return 0;
}

int mergeSort(double *inicio, double *final)
{
    if (final - inicio <= 1)
        return 0;

    double *mid = inicio + (final - inicio) / 2;

    mergeSort(inicio, mid);
    mergeSort(mid, final);
    merge(inicio, mid, final);
    return 0;
}

void imprimir(double *inicio, double *final)
{
    int idx = 1;
    for (double *p = inicio; p < final; p++, idx++)
        printf("[%d] %.6f\n", idx, *p);
}

// Submenú reutilizable para pedir tamaño y reservar memoria
// Retorna el nuevo tamaño, o 0 si hubo error
int pedirTamanioYAllocar(double **arr)
{
    int n;
    printf("Ingrese el tamanio del array: ");
    if (scanf("%d", &n) != 1 || n <= 0)
    {
        printf("Entrada invalida. Por favor, ingrese un numero entero positivo.\n");
        while (getchar() != '\n')
            ;
        return 0;
    }
    while (getchar() != '\n')
        ;

    free(*arr);
    *arr = (double *)malloc(n * sizeof(double));
    if (!*arr)
    {
        printf("Error de memoria\n");
        return 0;
    }
    return n;
}

int main()
{
    int n = 0;
    double *arr = NULL;
    int opc, subOpc;

    while (1)
    {
        printf("\n===== MENU =====\n");
        printf("1. Llenar el array\n");
        printf("2. Ordenar el array\n");
        printf("3. Imprimir el array\n");
        printf("4. Salir\n");
        printf("Opcion: ");

        if (scanf("%d", &opc) != 1)
        {
            printf("Entrada invalida.\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        while (getchar() != '\n')
            ;

        switch (opc)
        {
        case 1:
            printf("\n--- Llenar Array ---\n");
            printf("  1. Ingresar numeros manualmente\n");
            printf("  2. Generar numeros automaticamente\n");
            printf("  Opcion: ");

            if (scanf("%d", &subOpc) != 1)
            {
                printf("Entrada invalida.\n");
                while (getchar() != '\n')
                    ;
                break;
            }
            while (getchar() != '\n')
                ;

            if (subOpc == 1)
            {
                n = pedirTamanioYAllocar(&arr);
                if (n > 0)
                {
                    llenarArrayManual(arr, n);
                    printf("Array llenado manualmente con %d elementos (double).\n", n);
                }
            }
            else if (subOpc == 2)
            {
                n = pedirTamanioYAllocar(&arr);
                if (n > 0)
                    llenarArrayAuto(arr, n);
            }
            else
            {
                printf("Opcion invalida.\n");
            }
            break;

        case 2:
            if (!arr || n <= 0)
            {
                printf("Primero llene el array (opcion 1).\n");
                break;
            }

            clock_t start = clock();
            mergeSort(arr, arr + n);
            clock_t end = clock();

            double tiempo = ((double)(end - start)) / CLOCKS_PER_SEC;
            printf("Array ordenado en %.6f segundos.\n", tiempo);
            break;

        case 3:
            if (!arr || n <= 0)
            {
                printf("Primero llene el array (opcion 1).\n");
                break;
            }
            imprimir(arr, arr + n);
            break;

        case 4:
            printf("Saliendo...\n");
            free(arr);
            return 0;

        default:
            printf("Opcion invalida. Intente de nuevo.\n");
        }
    }
}