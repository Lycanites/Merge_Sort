#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int comparisons = 0;
static int writes = 0;

int parse_positive_int(const char *text, int *out)
{
    char *end = NULL;
    long value = strtol(text, &end, 10);
    if (end == text || *end != '\0' || value <= 0 || value > 2000000000L)
    {
        return 0;
    }
    *out = (int)value;
    return 1;
}

void generate_random_array(int *arr, int size, int range)
{
    int bound = range * 2 + 1;
    printf("INICIANDO generacion de datos\n");
    for (int i = 0; i < size; i++)
    {
        arr[i] = (rand() % bound) - range;
        printf("GENERADO[%d] = %d\n", i, arr[i]);
    }
}

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

int merge(int *arr, int *mid, int *end, int *base)
{
    int left_size = (int)(mid - arr);
    int right_size = (int)(end - mid);
    int left_start = (int)(arr - base);
    int left_end = left_start + left_size - 1;
    int right_start = (int)(mid - base);
    int right_end = right_start + right_size - 1;
    int *left = (int *)malloc((size_t)left_size * sizeof(int));
    int *right = (int *)malloc((size_t)right_size * sizeof(int));

    if (left == NULL || right == NULL)
    {
        free(left);
        free(right);
        return 0;
    }

    memcpy(left, arr, (size_t)left_size * sizeof(int));
    memcpy(right, mid, (size_t)right_size * sizeof(int));

    printf("MEZCLANDO [%d..%d] con [%d..%d]\n", left_start, left_end, right_start, right_end);

    int *l = left;
    int *r = right;
    int *w = arr;

    while (l < left + left_size && r < right + right_size)
    {
        comparisons++;
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
        writes++;
        printf("ESCRIBIENDO[%d] = %d\n", (int)(w - base), *w);
        w++;
    }

    while (l < left + left_size)
    {
        *w = *l;
        writes++;
        printf("ESCRIBIENDO[%d] = %d\n", (int)(w - base), *w);
        l++;
        w++;
    }

    while (r < right + right_size)
    {
        *w = *r;
        writes++;
        printf("ESCRIBIENDO[%d] = %d\n", (int)(w - base), *w);
        r++;
        w++;
    }

    free(left);
    free(right);
    return 1;
}

int merge_sort(int *start, int *end, int *base)
{
    if (end - start <= 1)
    {
        printf("CASO BASE en indice %d\n", (int)(start - base));
        return 1;
    }

    int *mid = start + (end - start) / 2;
    printf("DIVIDIENDO [%d..%d] y [%d..%d]\n", (int)(start - base), (int)(mid - base) - 1, (int)(mid - base), (int)(end - base) - 1);
    if (!merge_sort(start, mid, base))
    {
        return 0;
    }
    if (!merge_sort(mid, end, base))
    {
        return 0;
    }
    return merge(start, mid, end, base);
}

int main(int argc, char **argv)
{
    int size = 0;
    int range = 0;

    if (argc != 3)
    {
        printf("USO: %s <cantidad> <rango>\n", argv[0]);
        return 1;
    }

    if (!parse_positive_int(argv[1], &size) || !parse_positive_int(argv[2], &range))
    {
        printf("ERROR: cantidad y rango deben ser enteros positivos.\n");
        return 1;
    }

    int *arr = (int *)malloc((size_t)size * sizeof(int));
    if (arr == NULL)
    {
        printf("ERROR: no se pudo reservar memoria para %d elementos.\n", size);
        return 1;
    }

    srand((unsigned int)time(NULL));
    printf("ARGUMENTOS: cantidad=%d rango=%d\n", size, range);
    generate_random_array(arr, size, range);

    printf("INICIANDO merge sort\n");
    clock_t start = clock();
    if (!merge_sort(arr, arr + size, arr))
    {
        printf("ERROR: fallo en merge sort por memoria insuficiente.\n");
        free(arr);
        return 1;
    }
    clock_t end = clock();

    printf("MERGE SORT TERMINADO\n");
    print_array(arr, size);
    printf("COMPARACIONES: %d\n", comparisons);
    printf("ESCRITURAS: %d\n", writes);
    printf("TIEMPO_SEGUNDOS: %.6f\n", (double)(end - start) / CLOCKS_PER_SEC);

    free(arr);
    return 0;
}