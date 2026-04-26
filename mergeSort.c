#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    int arr[] = {38, 27, 43, 3, 9, 82, 10};
    int *inicio = arr;
    int *final = arr + sizeof(arr) / sizeof(*arr);

    printf("Array original:\n");
    imprimir(inicio, final);

    mergeSort(inicio, final);
    printf("Array ordenado:\n");
    imprimir(inicio, final);

    return 0;
}