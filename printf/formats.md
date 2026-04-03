# `printf` in C

`printf` prints formatted text to standard output.

```c
#include <stdio.h>

int main(void) {
    int age = 21;
    printf("I am %d years old\n", age);
}
```

## 1. Basic idea

`printf` takes:

* a **format string**
* then extra values to plug into it

```c
printf("Age: %d\n", 21);
```

`%d` is a **format specifier**. It tells `printf` what type of value to expect and how to print it.

---

## 2. Common format specifiers

### Integers

```c
int x = 42;
printf("%d\n", x);   // decimal integer
printf("%i\n", x);   // basically same as %d in printf
```

### Unsigned integers

```c
unsigned int u = 42;
printf("%u\n", u);
```

### Floating point

```c
float f = 3.5f;
double d = 9.876;

printf("%f\n", f);   // float is promoted to double
printf("%f\n", d);
```

`%f` prints 6 digits after the decimal by default.

### Character

```c
char c = 'A';
printf("%c\n", c);
```

### String

```c
char *s = "hello";
printf("%s\n", s);
```

### Pointer / address

```c
int x = 5;
printf("%p\n", (void *)&x);
```

Use `%p` for addresses.

### Hex / octal

```c
int x = 255;
printf("%x\n", x);   // ff
printf("%X\n", x);   // FF
printf("%o\n", x);   // 377
```

---

## 3. Very common specifiers cheat sheet

```c
%d   int
%i   int
%u   unsigned int
%f   double
%c   char
%s   char *
%p   pointer
%x   hexadecimal
%X   hexadecimal uppercase
%o   octal
%%   print a literal percent sign
```

Example:

```c
printf("Score: 95%%\n");
```

prints:

```c
Score: 95%
```

---

## 4. Escape sequences inside the string

These are not format specifiers, but special characters in the string itself:

```c
\n   newline
\t   tab
\"   double quote
\\   backslash
```

Example:

```c
printf("Hello\nWorld\n");
printf("Name:\tAlice\n");
printf("\"quoted\"\n");
```

---

## 5. Matching types matters

This is super important:

The format specifier must match the value type.

Correct:

```c
int x = 10;
double y = 3.14;

printf("%d\n", x);
printf("%f\n", y);
```

Wrong:

```c
printf("%d\n", y);   // WRONG
```

That is **undefined behavior**.

So yes, C has casting, but `printf` does **not** automatically reinterpret types safely for you.

If you want to print a `double` as an integer:

```c
printf("%d\n", (int)y);
```

Now the type matches `%d`.

---

## 6. Why `%f` works for `float`

You might expect `float` to need a different specifier, but in `printf`:

* `float` is automatically promoted to `double`

So this is fine:

```c
float f = 2.5f;
printf("%f\n", f);
```

---

## 7. Printing multiple values

```c
int age = 21;
char grade = 'A';
double gpa = 3.85;

printf("Age: %d, Grade: %c, GPA: %f\n", age, grade, gpa);
```

The values are matched to specifiers from left to right.

---

## 8. Width and precision

### Width

```c
printf("%5d\n", 42);
```

Prints the number in a field at least 5 characters wide.

### Precision for floats

```c
printf("%.2f\n", 3.14159);
```

prints:

```c
3.14
```

### Width + precision

```c
printf("%8.2f\n", 3.14159);
```

This gives width 8, precision 2.

---

## 9. A few useful variants

### Left align

```c
printf("%-5d!\n", 42);
```

### Zero padding

```c
printf("%05d\n", 42);
```

prints:

```c
00042
```

---

## 10. `printf` does not know array lengths

For strings, `%s` keeps printing characters until it hits the null terminator `'\0'`.

```c
char s[] = "hello";
printf("%s\n", s);
```

So a C string must be properly null-terminated.

---

## 11. `printf` examples you’ll use a lot

```c
printf("x = %d\n", x);
printf("pi = %.2f\n", pi);
printf("letter = %c\n", ch);
printf("name = %s\n", name);
printf("addr = %p\n", (void *)&x);
```

---

## 12. `printf` vs `scanf`

Easy way to remember:

* `printf` = **print out**
* `scanf` = **scan input in**

---

## 13. Best beginner rule

Memorize these first:

* `%d` → `int`
* `%f` → `double`
* `%c` → `char`
* `%s` → `char *`
* `%p` → address
* `%%` → literal `%`

And always make the type match.

---

## Final mini example

```c
#include <stdio.h>

int main(void) {
    int age = 21;
    double height = 1.75;
    char grade = 'A';
    char *name = "Ercole";

    printf("Name: %s\n", name);
    printf("Age: %d\n", age);
    printf("Height: %.2f m\n", height);
    printf("Grade: %c\n", grade);
    printf("Age completion: 100%%\n");

    return 0;
}
```
