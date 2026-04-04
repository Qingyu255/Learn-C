#include <stdio.h>


struct animal {
    char *name;
    int age;
};

struct car {
    char *name;
    float price;
    int speed;
};

void set_price(struct car *c, float new_price) {
    //(*c).price = new_price; // know the need for the parentheses as for *c.price, it would be interpreted as *(c.price)  
    // The line above is 100% equivalent to the one below:
    // this is the arrow operator: helps refer to fields in pointers to structs
    c->price = new_price;  // That's the one!
}

int main(void) {
    // ways to intialise a struct
    // 1:
    struct animal animal_a;
    animal_a.name = "pig";
    animal_a.age = 2;
    printf("animal_a name: %s\n", animal_a.name);
    //2:
    struct animal animal_b = {"dog", 100}; // order of args matter
    printf("animal_b name: %s\n", animal_b.name);
    //3; by using the dot operator
    struct animal animal_c = {.age=15, .name="human"}; //notice how order does not matter
    printf("animal_c name: %s\n", animal_c.name);

    // passing structs to functions:
    // two ways to do this:
    // you can pass the struct or a pointer to the struct
    // There are basically two cases when you’d want to pass a pointer to the struct:

    // You need the function to be able to make changes to the struct that was passed in, and have those changes show in the caller.
    // The struct is somewhat large and it’s more expensive to copy that onto the stack than it is to just copy a pointer71.
    struct car saturn = {.speed=175, .name="Saturn SL/2"};

    // Pass a pointer to this struct car, along with a new,
    // more realistic, price:
    set_price(&saturn, 799.99);

    printf("Price: %f\n", saturn.price);

    return 0;
}