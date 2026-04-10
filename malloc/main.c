#include <stdio.h>
#include <stdlib.h> // includes malloc header file

int *make_array(int n) {
    /*
    Return the pointer to the allocated memory address in heap
    */
    int *arr = malloc(sizeof(int) * n);

    if (arr == NULL) {
        return NULL; // malloc failed
    }

    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }
    return arr;
}

int main(void) {
    int n = 4;
    int *a = make_array(n);
    if (a == NULL) {
        fprintf(stderr, "Heap memory allocation failed");
        return 1;
    }

    a[0] = 67;
    *(a + 2) = 68; // another way to index an array
    for (int i = 0; i < n; i++) {
        printf("val at index i: %d\n", a[i]);
    }

    // remember to free allocated memory after they are no longer needed
    free(a);
    // But p still holds the old address even after free. That is why we often write:
    // This is good practice so you do not accidentally reuse a freed pointer.
    a = NULL;
    return 0;
}
