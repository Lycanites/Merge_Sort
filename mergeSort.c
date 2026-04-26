#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int llenarArray(int *arr, int tamaño)
{
    for (int i = 0; i < tamaño; i++)
    {
        printf("Ingrese el elemento %d: ", i + 1);
        scanf("%d", &arr[i]);
    }
    return 0;
}

int merge(int *arr, int *mid, int *fin)
{
    int leftSize = mid - arr;
    int rightSize = fin - mid;

    int *left = (int *)malloc(leftSize * sizeof(int));
    int *right = (int *)malloc(rightSize * sizeof(int));

    memcpy(left, arr, leftSize * sizeof(int));
    memcpy(right, mid, rightSize * sizeof(int));

    int *l = left, *r = right, *f = arr;

    while (l < left + leftSize && r < right + rightSize)
    {
        if (*l <= *r)
        {
            *f = *l;
            l++;
        }
        else
        {
            *f = *r;
            r++;
        }
        f++;
    }

    while (l < left + leftSize)
    {
        *f = *l;
        l++;
        f++;
    }

    while (r < right + rightSize)
    {
        *f = *r;
        r++;
        f++;
    }

    free(left);
    free(right);
    return 0;
}

int mergeSort(int *inicio, int *final)
{
    if (final - inicio <= 1)
        return 1;

    int *mid = inicio + (final - inicio) / 2;

    mergeSort(inicio, mid);
    mergeSort(mid, final);
    merge(inicio, mid, final);
    return 0;
}

int imprimir(int *inicio, int *final)
{
    for (int *p = inicio; p < final; p++)
    {
        printf("%d", *p);
        printf("\n");
    }
}

int main()
{
    int arr[10];
    int *inicio = arr;
    int *final = arr + 10;
    int opc;

    while (1)
    {
        printf("\nSeleccione una opción:\n");
        printf("1. Llenar el array\n");
        printf("2. Ordenar el array\n");
        printf("3. Imprimir el array\n");
        printf("4. Salir\n");

        scanf("%d \n", &opc);

        switch (opc)
        {
        case 1:
            llenarArray(arr, 10);
            break;

        case 2:
            mergeSort(inicio, final);
            printf("Array ordenado\n");
            break;

        case 3:
            imprimir(inicio, final);
            break;

        case 4:
            printf("Saliendo...\n");
            return 0;

        default:
            printf("Opción inválida\n");
        }
    }
}