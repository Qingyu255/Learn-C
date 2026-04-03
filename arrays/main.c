#include <stdio.h>
#define G_SIZE 5 // this is a compile-time constant

// size_t is an unsigned integer type chosen by the implementation to be big enough to represent the size of any single object in memory.
// this signature will work too: void print_array_elements(int *arr, size_t size) {
void print_float_array_elements(float arr[], size_t size) {
    printf("------------\n");
    for (size_t i = 0; i < size; i++) {
        printf("value at arr[%zu]: %f\n", i, arr[i]);
    }
    printf("------------\n");
}

int main(void) {
    size_t f_size = 5;
    float f[f_size]; // declare an array of 5 floats; note that f here is a VLA (variable length array)
    // this creates an array of 5 floats, but if it is a local variable inside a function, the elements are uninitialized.
    // outside of a function, 
    // f is an array of 5 floats, not a pointer
    // in many expressions, f decays to a pointer to its first element (think &f[0])
    // by declaring f is typically on the stack, and its contents are whatever bytes happened to already be there.
    
    // here what is printed is considered undefined behaviour as: f[i] will just be whatever bytes happened to already be there.
    print_float_array_elements(f, f_size);
    // they can look like:
    // value at arr[0]: -5666148273874754688547268067328.000000
    // value at arr[1]: 0.000000
    // value at arr[2]: -663787393185307396128592887808.000000
    // value at arr[3]: 0.000000
    // value at arr[4]: 141980973573503971716235264.000000
    
    // If you want zeros, do: float f[5] = {0};
    float g[G_SIZE] = {0}; // not a VLA
    // G_SIZE here if we are doing = {0} must be a compile time constant (unlike f_size)
    // it is not legal to make a runtime-sized array and also use an initializer list
    // but it is legal to make an array whose size is known only at runtime
    print_float_array_elements(g, G_SIZE); // these should all be 0s

    // C doesn’t stop you from accessing arrays out of bounds. It might not even warn you!
    float a[5] = {22, 37, 3490, 18, 95}; // note: though each intialiser like 22 is a integer literal, C converts each int value to float when initializing the array
    print_float_array_elements(a, 10); // // BAD NEWS: printing too many elements!(trying to print past index 4)

    // Working with multi-dimensional arrays
    // You can add as many dimensions as you want to your arrays.
    int d[10];
    int c[4][5][6];

    int b[2][5] = {      // Initialize a 2D array
        {0, 1, 2, 3, 4},
        {5, 6, 7, 8, 9}
    };
    size_t row_count = sizeof(b) / sizeof(b[0]);
    size_t col_count = sizeof(b[0]) / sizeof(b[0][0]);
    printf("row count: %zu, col count: %zu\n", row_count, col_count);
    // note that: But this trick only works in the scope in which the array was defined. If you pass the array to a function, it doesn’t work. Even if you make it “big” in the function signature:

    // Passing Multidimensional Arrays to Functions:
    // C needs to know all the dimensions (except the first one) so it has enough information to know where in memory to look to find a value.
    // These 2 cases are equivalent:
    // void print_2D_array(int a[2][3]);
    // void print_2D_array(int a[][3]);
    // Here, the compiler really only needs the second dimension so it can figure out how far in memory to skip for each increment of the first dimension. In general, it needs to know all the dimensions except the first one.
    // Also, remember that the compiler does minimal compile-time bounds checking (if you’re lucky), and C does zero runtime checking of bounds. No seat belts! Don’t crash by accessing array elements out of bounds!
    return 0;
}