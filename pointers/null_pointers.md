## What a null pointer is

A **null pointer** is a pointer value that means:

> “This pointer points to no valid object or function.”

It is the special “points to nothing” pointer value in C.

Example:

```c
int *p = NULL;
```

This means `p` is not currently pointing at an `int`.

---

## `NULL` does not mean zero memory address in a simple hardware sense

You often write null pointers with `NULL`, and historically people say “it’s address 0”, but the more correct C view is:

* `NULL` is a **null pointer constant**
* when assigned to a pointer, it becomes the pointer’s **null value**
* it means “no valid target”

So mentally:

* `NULL` = “points nowhere”
* not “safe dummy object”
* not “empty memory”

---

## Why null pointers exist

They are useful for saying:

* “not initialized yet”
* “lookup failed”
* “optional pointer absent”
* “end of a linked list”
* “malloc failed”
* “no callback provided”

---

## Basic usage

### Initialize a pointer safely

```c
int *p = NULL;
```

Better than leaving it uninitialized:

```c
int *p;   // garbage value, dangerous
```

An uninitialized pointer points to some unknown junk address.

A null pointer clearly points nowhere.

---

## Checking for null

```c
if (p == NULL) {
    printf("p points to nothing\n");
}
```

Or more idiomatically:

```c
if (p) {
    printf("not null\n");
}

if (!p) {
    printf("is null\n");
}
```

Because in C:

* null pointer counts as false
* non-null pointer counts as true

---

## Dereferencing a null pointer

This is bad:

```c
int *p = NULL;
*p = 42;   // BAD
```

Why? Because dereferencing means:

> “Go to the place this pointer points to, and read/write there.”

But null means it points nowhere valid.

This causes **undefined behavior**. In practice, it often crashes with a segmentation fault, but C itself just says the behavior is undefined.

Same problem here:

```c
int *p = NULL;
printf("%d\n", *p);   // BAD
```

---

## Very important distinction

These are different:

```c
int *p = NULL;
```

vs

```c
int x = 0;
int *p = &x;
```

In the first:

* `p` points nowhere

In the second:

* `p` points to a real integer
* that integer happens to contain value `0`

So:

* **null pointer** ≠ pointer to zero
* **null pointer** ≠ integer zero stored somewhere

---

## `NULL` vs `0`

In C, this is allowed:

```c
int *p = 0;
```

because `0` can act as a null pointer constant.

And `NULL` is usually defined by the headers as something like:

```c
#define NULL ((void *)0)
```

or sometimes just:

```c
#define NULL 0
```

So these often behave similarly in pointer contexts:

```c
int *p1 = NULL;
int *p2 = 0;
```

But style-wise, use `NULL` because it makes your intent clearer:

```c
int *p = NULL;
```

---

## Null pointer vs uninitialized pointer

This is one of the biggest beginner gotchas.

### Null pointer

```c
int *p = NULL;
```

* known state
* safe to compare
* unsafe to dereference

### Uninitialized pointer

```c
int *p;
```

* unknown garbage state
* unsafe even to reason about as a real pointer target
* dereferencing is garbage/UB

So if you do not have a valid address yet, initialize to `NULL`.

---

## Common pattern with functions

### Returning null on failure

```c
int *find_value(...);
```

Maybe:

```c
int *find_value(...) {
    if (not_found) {
        return NULL;
    }
    return &some_int;
}
```

Caller:

```c
int *p = find_value(...);
if (p != NULL) {
    printf("%d\n", *p);
}
```

Null is often used to mean “no result”.

---

## `malloc()` and null pointers

Very important.

```c
int *p = malloc(sizeof(int));
```

If allocation fails, `malloc` returns `NULL`.

So you should check:

```c
int *p = malloc(sizeof(int));
if (p == NULL) {
    // allocation failed
}
```

Then only use it if non-null:

```c
if (p != NULL) {
    *p = 42;
}
```

---

## `free(NULL)` is safe

This is a nice special rule:

```c
free(NULL);
```

This does **nothing**, and it is safe.

So code like this is fine:

```c
if (p != NULL) {
    free(p);
}
```

but also this is fine:

```c
free(p);
```

even if `p` is `NULL`.

---

## Why people set pointers to `NULL` after `free`

Example:

```c
int *p = malloc(sizeof(int));
free(p);
p = NULL;
```

Why do this?

Because after `free(p)`, the pointer still holds the old address, but that address is no longer valid to use. That pointer is now a **dangling pointer**.

Setting it to `NULL` helps avoid accidental reuse:

```c
if (p) { ... }
```

Now `p` clearly means “don’t use me”.

---

## Dangling pointer vs null pointer

### Null pointer

```c
int *p = NULL;
```

Points nowhere.

### Dangling pointer

```c
int *p = malloc(sizeof(int));
free(p);
// p is now dangling
```

`p` is non-null, but the memory it points to is no longer valid.

This is more dangerous because it can look valid.

---

## Null pointer in linked lists

Very common use.

```c
struct Node {
    int value;
    struct Node *next;
};
```

End of list:

```c
node->next = NULL;
```

That means:

> there is no next node

Traversal:

```c
while (curr != NULL) {
    printf("%d\n", curr->value);
    curr = curr->next;
}
```

Null marks the end.

---

## Passing null to functions

Sometimes a function allows null:

```c
print_name(NULL);
```

But only if the function was designed to handle it.

Example safe style:

```c
void print_num(int *p) {
    if (p == NULL) {
        printf("no number\n");
        return;
    }
    printf("%d\n", *p);
}
```

If the function does not check for null and you pass null anyway, dereference crashes/UB.

So always know the contract.

---

## Null pointer for strings

In C, strings are `char *` to null-terminated character arrays.

Be careful:

```c
char *s = NULL;
```

means:

* `s` points to no string at all

But:

```c
char s[] = "";
```

means:

* there is a real string
* it is empty
* first character is `'\0'`

These are not the same.

### Null string pointer

```c
char *s = NULL;
```

### Empty string

```c
char *s = "";
```

or

```c
char s[] = "";
```

Null means “no valid string object”. Empty string means “valid string of length 0”.

---

## `'\0'` is not the same as `NULL`

Another classic confusion.

### `NULL`

Used for pointers.

### `'\0'`

A character with numeric value 0, used to terminate C strings.

Example:

```c
char *s = "hi";
```

Memory is:

```c
'h'  'i'  '\0'
```

That final `'\0'` is a character byte, not a null pointer.

So:

* `NULL` is about pointers
* `'\0'` is about characters/string termination

---

## Can function pointers be null?

Yes.

```c
void (*f)(void) = NULL;
```

This means no function is assigned yet.

Check before calling:

```c
if (f != NULL) {
    f();
}
```

Calling a null function pointer is bad.

---

## Common mistakes

### 1. Dereferencing before checking

```c
if (*p == 5)   // BAD if p might be NULL
```

Need:

```c
if (p != NULL && *p == 5)
```

---

### 2. Thinking non-null means valid

Not always.

```c
int *p = malloc(sizeof(int));
free(p);
if (p != NULL) {
    *p = 5;   // still BAD, dangling pointer
}
```

Non-null only means “not null”, not “safe”.

---

### 3. Using `NULL` with non-pointers

This is wrong style:

```c
int x = NULL;   // don't do this
```

Use `0` for integers, `NULL` for pointers.

---

### 4. Confusing null pointer with empty string

```c
char *s = NULL;   // no valid string
char *t = "";     // valid empty string
```

---

## Good habits

### Initialize pointers

```c
int *p = NULL;
```

### Check results from `malloc`

```c
int *p = malloc(sizeof(int));
if (p == NULL) {
    // handle failure
}
```

### Check before dereferencing if null is possible

```c
if (p != NULL) {
    use(*p);
}
```

### After `free`, set to `NULL` if pointer stays around

```c
free(p);
p = NULL;
```

---

## Tiny mental model

Think of pointer states like this:

* **valid pointer** → points to a real usable object
* **null pointer** → intentionally points to nothing
* **dangling pointer** → points to somewhere that used to be valid but no longer is
* **uninitialized pointer** → random garbage, unknown

Only the first one is safe to dereference.

---

## Mini example tying it together

```c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int *p = NULL;

    if (p == NULL) {
        printf("p is null\n");
    }

    p = malloc(sizeof(int));
    if (p == NULL) {
        printf("malloc failed\n");
        return 1;
    }

    *p = 42;
    printf("%d\n", *p);

    free(p);
    p = NULL;

    if (p == NULL) {
        printf("p reset to null after free\n");
    }

    return 0;
}
```

---

## One-sentence takeaway

A **null pointer** in C is a special pointer value meaning **“this pointer currently points to no valid object or function”**, and it is safe to compare but **never safe to dereference**.