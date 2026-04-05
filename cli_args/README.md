```c
int main(int argc, char *argv[])
```

This is the usual form of `main` for receiving command-line arguments in C.

## What each part means

### `int`

This is the **return type** of `main`.

It means `main` returns an integer to the operating system when the program finishes.

Usually:

```c
return 0;
```

means success.

Nonzero often means some kind of error.

---

### `main`

This is the program’s entry function. Execution starts here.

---

### `(int argc, char *argv[])`

These are the parameters passed to your program by the OS.

## `argc`

`argc` means **argument count**.

It tells you how many command-line arguments were passed.

Example:

```bash
./prog hello 123
```

Then:

* `argc == 3`

because the arguments are:

* `argv[0]` → `"./prog"`
* `argv[1]` → `"hello"`
* `argv[2]` → `"123"`

So the program name itself counts as one argument.

---

## `argv`

`argv` means **argument vector**.

It stores the command-line arguments as strings.

Each argument is a C string, so each one is a `char *`.

That is why the declaration is:

```c
char *argv[]
```

meaning:

* `argv` is an array
* of pointers
* to `char`

So each `argv[i]` is a `char *`, pointing to the first character of a null-terminated string.

Example:

```c
argv[1]   // "hello"
argv[1][0] // 'h'
```

---

# Precedence: how `char *argv[]` is interpreted

In C declarations, `[]` binds tighter than `*`.

So:

```c
char *argv[]
```

is interpreted as:

```c
argv is an array of pointers to char
```

Not:

```c
pointer to array of char
```

If you wanted “pointer to array of char”, you’d need parentheses:

```c
char (*argv)[]
```

That is a different type.

---

# Why `char *argv[]` and `char **argv` are equivalent here

In a function parameter list, array parameters **decay** to pointers.

So inside a function declaration:

```c
int main(int argc, char *argv[])
```

is equivalent to:

```c
int main(int argc, char **argv)
```

because arrays in parameter lists are treated as pointers to their first element.

Since `argv` is “array of `char *`”, it decays to “pointer to `char *`”, which is:

```c
char **
```

So these mean the same thing in `main`:

```c
int main(int argc, char *argv[])
int main(int argc, char **argv)
```

---

# Mental model

Think of it like this:

* `argc` = how many strings you got
* `argv` = list of those strings

So if the user runs:

```bash
./prog cat dog
```

then memory conceptually looks like:

```c
argc = 3

argv[0] --> "./prog"
argv[1] --> "cat"
argv[2] --> "dog"
```

and often:

```c
argv[3] == NULL
```

though the main thing you usually rely on is `argc`.

---

# Example program

```c
#include <stdio.h>

int main(int argc, char *argv[]) {
    printf("argc = %d\n", argc);

    for (int i = 0; i < argc; i++) {
        printf("argv[%d] = %s\n", i, argv[i]);
    }

    return 0;
}
```

If run as:

```bash
./prog hello world
```

output might be:

```text
argc = 3
argv[0] = ./prog
argv[1] = hello
argv[2] = world
```

---

# Type breakdown carefully

For:

```c
char *argv[]
```

* `argv` is the variable name
* `argv[]` means “array of”
* `*` means “pointer to”
* `char` is the base type

So:

```c
argv = array of pointer to char
```

Then:

* `argv` is effectively `char **` inside the function
* `argv[i]` is `char *`
* `argv[i][j]` is `char`

Example:

```c
argv[1]      // second argument, type char *
argv[1][0]   // first character of second argument, type char
```

---

# Why `char *` for a string?

Because in C, a string is just a sequence of chars ending in `'\0'`.

So a string is represented by a pointer to its first character:

```c
char *s = "hello";
```

Similarly, each command-line argument is given as a `char *`.

---

# Common beginner confusion

## 1. Is `argv` really an array?

In the function parameter declaration, it behaves like a pointer.

So although written as:

```c
char *argv[]
```

inside the function it is effectively:

```c
char **argv
```

## 2. Is each argument one char?

No. Each `argv[i]` is a whole string.

So:

```c
argv[1]
```

might be `"hello"`.

And:

```c
argv[1][0]
```

is just `'h'`.

## 3. Why is `argv[0]` the program name?

By convention, the system passes the invoked program name/path as the first argument.

---

# One-line summary

```c
int main(int argc, char *argv[])
```

means:

> `main` returns an `int`, receives the number of command-line arguments in `argc`, and receives the arguments themselves in `argv`, where `argv` is an array of C strings.

---

# Super short type summary

```c
argc        // int
argv        // char **   (effectively inside main)
argv[i]     // char *
argv[i][j]  // char
```

If you want, I can also draw the actual memory layout of `argc`, `argv`, and the strings in a very visual way.


## Additional info on precedence
It means when C parses a declaration, `[]` attaches to the name before `*` does.

So in:

```c
char *argv[]
```

C sees:

1. `argv[]` first
   → `argv` is an **array**
2. then `*`
   → the array contains **pointers**
3. base type is `char`
   → array of pointers to `char`

So:

```c
char *argv[]
```

means:

> `argv` is an array of `char *`

not:

> `argv` is a pointer to an array of `char`

That second meaning would need parentheses:

```c
char (*argv)[]
```

because parentheses force `*argv` to be grouped first.

A good way to read declarations is to start at the variable name and move outward.

For `char *argv[]`:

* start at `argv`
* look right: `[]` → array
* look left: `*` → of pointers
* base type: `char`

So: **array of pointers to char**

Tiny comparison:

```c
char *argv[];    // array of pointers to char
char (*argv)[];  // pointer to array of char
char **argv;     // pointer to pointer to char
```

And in a function parameter list, this:

```c
int main(int argc, char *argv[])
```

is treated the same as:

```c
int main(int argc, char **argv)
```

because array parameters decay to pointers.

The key idea is: **`[]` wins over `*` unless parentheses change the grouping.**
