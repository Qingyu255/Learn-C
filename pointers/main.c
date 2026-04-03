# include <stdio.h>

// You have to declare a function, or at least its prototype, before you call it.
// Otherwise the compiler may report an error because it doesn't know the function's type.
int increment_without_pointer(int num) {
    return ++num;
}

// this is a prototype; somewhat like a java interface
void increment_with_pointer(int *num);

int main(void) { // returns the exit/return code
    printf("Hello, pointers!\n");

    int i = 0;
    printf("Digit at i : %d\n", i);

    int y = increment_without_pointer(i);
    printf("Digit at y after `increment_without_pointer`: %d\n", y);

    // What even is a pointer: it ias a variable that stores a memory address (in virtual memory) that points to a value
    int *pointer_to_i = &i; // ampersand is the address-of operator
    increment_with_pointer(pointer_to_i); // know why increment_without_pointer(*pointer_to_i); is wrong:
    printf("Digit at i after `increment_with_pointer`: %d\n", i);
    // *pointer_to_i means: “go to the address stored in pointer_to_i and get the int there”
    // IMPT concept to know:
    // pointer_to_i has type int *
    // *pointer_to_i has type int

    // OR YOU CAN do this directly:
    increment_with_pointer(&i);

    printf("Digit at i after second `increment_with_pointer`: %d\n", i);

    return 0; // This returns 0 by default even if you dont specify return 0
}

// You only need the prototype when the function is used before its full definition.
void increment_with_pointer(int *num) {
    // know why `*num++;` is wrong: its because postfix ++ binds tighter than *.
    (*num)++;
}

// Pointer sizes in C:
// on a 64-bit system, many pointers are 8 bytes
// on a 32-bit system, many pointers are 4 bytes
// Pointer size = how many bytes the program uses to store an address
// Pointer size is usually based on the platform's address model (ABI).
// It often matches the native register width, but not always.
