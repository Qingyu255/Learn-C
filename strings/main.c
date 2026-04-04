#include <stdio.h>
#include <string.h>


int main(void) {
    // A string variable is just a pointer to the first character in the String

    char *s = "Hello, world!";
    // in C, you can index a pointer as long as it points into a valid sequence of elements.
    printf("%c\n", s[0]);
    printf("%c\n", s[1]);

    // Another option is this, nearly equivalent to the above char* usage:
    char e[12] = "Hello world";
    // notice that this only has 11 chars but we allocated 12
    // are there any unused bytes? NO! this is because the 12th byte is used for the null terminator '\0'
    // but for:
    char d[15] = "Hello world"; // the array is bigger than needed, the remaining bytes are zero-initialized
    // then memory is: 'H' 'e' 'l' 'l' 'o' ' ' 'w' 'o' 'r' 'l' 'd' '\0' '\0' '\0' '\0'
    // note:
        // char g[5];   // global -> {0,0,0,0,0}

        // int main(void) {
        //     char l[5];         // local -> garbage
        //     static char s[5];  // static local -> {0,0,0,0,0}
        // }

    printf("%c\n", d[0]);

    size_t e_length = strlen(d);
    printf("last char in e: %c\n", d[e_length - 1]);
    printf("e_length: %zu\n", e_length);
    printf("size of e (includes the string null terminator): %zu\n", sizeof(e)); // returns the size of the entire array e, in bytes.
    int temp = e_length == sizeof(e) / sizeof(e[0]) - 1; // minus one for the c string terminator \0
    printf("%d\n", temp); // this should be true or 1
    for (int i = strlen(d); i < 15; i++){
        printf("%dth val: %c\n", i + 1, d[i]); // these are the zero initialised terms, which for char means: '\0'; 
        // That is why people say:
            // technically precise for the whole array/object: zero-initialized
            // when talking specifically about those char elements: they contain null characters
        // So this is best phrased as:
            // The remaining elements are zero-initialized.   
    }

    // Copying a string:
    // You can’t copy a string through the assignment operator (=). All that does is make a copy of the pointer to the first character… so you end up with two pointers to the same string:
    // char *h = "Hello, world!";// know why this is wrong if you want to modify h as Dereferencing a pointer to a string literal results in a segmentation fault because you are trying to write to a read-only memory segment.
    char h[] = "Hello, world!";
    char *t;

    // This makes a copy of the pointer, not a copy of the string!
    t = h;

    // We modify t
    t[0] = 'z';

    // But printing s shows the modification!
    // Because t and s point to the same string!
    printf("%s\n", h);  // "zello, world!"

    // HENCE:
    // If you want to make a copy of a string, you have to copy it a byte at a time—this means that you’re going to take the individual bytes of the string from one place in memory and duplicate them somewhere else in memory.
    // This is made easier with the strcpy() function.68
    char l[sizeof(h)]; // this is a VLA
    strcpy(l, h);
    printf("%s\n", s);

    return 0;
}