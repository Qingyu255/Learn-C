- `malloc` allocates memory on the heap and returns a pointer; unchanged until `free` —  which takes a single argument—a pointer to the memory block you want to deallocate.

**complete CS210 malloc/free crash course:**.

I’ll build it from first principles, then show the patterns, bugs, and when to use it. This is exactly the part of C where the language starts feeling powerful and dangerous at the same time. The lectures frame heap memory, `malloc`, `free`, memory leaks, and use-after-free as core topics, and then connect them to structs, trees, and dynamic arrays.  

# 1. Why `malloc` even exists

In C, not all memory is the same.

## Stack memory

This is where local variables usually live.

```c
void f(void) {
    int x = 5;
}
```

`x` exists only while `f()` is running.
Once `f()` returns, `x` is gone.

## Heap memory

This is memory you ask for manually while the program is running.

That is what `malloc` gives you.

You use heap memory when:

* the size is only known at runtime
* the data must live longer than the current function call
* you need dynamic data structures like linked lists, trees, graphs, dynamic arrays, buffers

So the high-level idea is:

* **stack** = automatic, temporary
* **heap** = manual, flexible

---

# 2. What `malloc` does

Prototype:

```c
void *malloc(size_t size);
```

Meaning:

* ask for `size` bytes on the heap
* if successful, return the address of the start of that block
* if it fails, return `NULL`

The lectures say exactly this: `malloc` returns a pointer to a memory block of at least the requested size, and returns `NULL` on failure. 

---

# 3. What `free` does

Prototype:

```c
void free(void *p);
```

Meaning:

* tell the allocator: “I’m done with this heap block”
* that memory can now be reused later

Important:

* `free()` frees the **heap block**
* it does **not** erase your pointer variable
* your pointer still contains the old address unless you change it yourself

The lectures explicitly warn about this in the use-after-free discussion. 

---

# 4. Your first mental model

Imagine:

```c
int *p = malloc(sizeof(int));
```

This means:

* `p` is a pointer variable, usually on the stack
* `malloc(sizeof(int))` creates a heap block big enough for one `int`
* `p` stores the address of that heap block

Picture:

```text
stack:   p  ----->   heap block (4 bytes for int)
```

Then:

```c
*p = 42;
```

stores `42` into the heap block.

Then:

```c
free(p);
```

returns that heap block to the allocator.

But `p` still holds the old address. That is why we often write:

```c
free(p);
p = NULL;
```

---

# 5. The simplest full example

```c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int *p = malloc(sizeof(int));

    if (p == NULL) {
        printf("malloc failed\n");
        return 1;
    }

    *p = 42;
    printf("%d\n", *p);

    free(p);
    p = NULL;

    return 0;
}
```

## Step by step

### Step 1

```c
int *p = malloc(sizeof(int));
```

Allocate enough bytes for one `int`.

### Step 2

```c
if (p == NULL)
```

Check failure.

### Step 3

```c
*p = 42;
```

Write into the allocated memory.

### Step 4

```c
printf("%d\n", *p);
```

Read from it.

### Step 5

```c
free(p);
```

Release the heap block.

### Step 6

```c
p = NULL;
```

Avoid accidental dangling-pointer use.

---

# 6. Why `sizeof(...)` matters

You should almost never hardcode byte counts like this:

```c
int *p = malloc(4);
```

That is fragile.

Instead do:

```c
int *p = malloc(sizeof(int));
```

because:

* `sizeof(int)` matches the platform
* it is clearer
* it scales properly for different types

Even better style:

```c
int *p = malloc(sizeof(*p));
```

Why nice?
Because `*p` is an `int`, so `sizeof(*p)` automatically means the right thing.

Similarly:

```c
struct person *sp = malloc(sizeof(*sp));
```

---

# 7. `malloc` for arrays

Very common use:

```c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n = 5;
    int *arr = malloc(sizeof(int) * n);

    if (arr == NULL) {
        return 1;
    }

    for (int i = 0; i < n; i++) {
        arr[i] = i * 10;
    }

    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    free(arr);
    arr = NULL;
    return 0;
}
```

## Why this is useful

Because now the array size can depend on runtime:

```c
int n;
scanf("%d", &n);
int *arr = malloc(sizeof(int) * n);
```

That is one of the biggest real reasons developers use `malloc`.

The lectures explicitly connect `malloc` to dynamically allocated arrays. 

---

# 8. `malloc` for strings

```c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    char *s = malloc(6 * sizeof(char));

    if (s == NULL) {
        return 1;
    }

    s[0] = 'H';
    s[1] = 'e';
    s[2] = 'l';
    s[3] = 'l';
    s[4] = 'o';
    s[5] = '\0';

    printf("%s\n", s);

    free(s);
    s = NULL;
    return 0;
}
```

Need 6 because:

* 5 letters in `"Hello"`
* plus `'\0'`

The string lectures emphasize that C strings are arrays of chars terminated by a null byte.  

---

# 9. `malloc` for structs

Another major use.

```c
#include <stdio.h>
#include <stdlib.h>

struct person {
    int age;
    int grad_year;
    double height;
};

int main(void) {
    struct person *p = malloc(sizeof(struct person));

    if (p == NULL) {
        return 1;
    }

    p->age = 20;
    p->grad_year = 2027;
    p->height = 1.75;

    printf("%d %d %.2f\n", p->age, p->grad_year, p->height);

    free(p);
    p = NULL;
    return 0;
}
```

This is how heap-allocated nodes for trees and linked lists are built.

The lectures move directly from structs to pointers-to-structs and then heap-allocated structures.  

---

# 10. Returning heap memory from a function

This is one of the most important cases.

## Good

```c
#include <stdlib.h>

int *make_number(void) {
    int *p = malloc(sizeof(int));
    if (p == NULL) {
        return NULL;
    }
    *p = 99;
    return p;
}
```

This is okay because the heap block still exists after the function returns.

## Bad

```c
int *bad(void) {
    int x = 99;
    return &x;
}
```

Why bad?
Because `x` is a local variable on the stack. It disappears when the function returns.

The lectures explicitly warn never to return a pointer to a local variable. 

So one major reason to use `malloc` is:

> “I need memory that survives after the function returns.”

---

# 11. The allocator view: what `malloc` is doing under the hood

Conceptually, the heap is a big pool of memory.

The lectures explain that the allocator:

* requests large chunks from the OS
* tracks which blocks are free vs in use
* responds to `malloc()` quickly
* marks blocks free again when `free()` is called. 

So think:

```text
heap memory pool
[ used ][ free ][ used ][ free ][ free ]
```

`malloc` finds a suitable free chunk.
`free` marks a chunk reusable.

This is why:

* forgetting `free` leaks memory
* use-after-free is dangerous
* fragmentation can happen

---

# 12. What happens after `free(p)`?

This is the part people often misunderstand.

Suppose:

```c
int *p = malloc(sizeof(int));
*p = 42;
free(p);
```

After `free(p)`:

* the heap block is no longer yours
* the memory may later be reused
* but `p` still contains the old address

So `p` becomes a **dangling pointer**.

It is dangling because it points to memory that used to be valid for you, but no longer is.

That is why this is bad:

```c
printf("%d\n", *p);   // BAD after free
```

It might:

* seem to work
* print old garbage
* crash
* corrupt something later

The slides call this out as a serious pitfall and recommend nulling the pointer after free. 

---

# 13. The three classic bugs

## Bug 1: memory leak

```c
int *p = malloc(sizeof(int) * 100);
/* use p */
return 0;   // forgot free(p)
```

You allocated memory and never released it.

A memory leak means heap memory becomes unreachable or is never freed when no longer needed. The lectures define this explicitly. 

## Bug 2: use after free

```c
int *p = malloc(sizeof(int));
free(p);
*p = 5;   // BAD
```

You are using memory after giving it back.

## Bug 3: double free

```c
int *p = malloc(sizeof(int));
free(p);
free(p);   // BAD
```

You freed the same block twice.

---

# 14. Subtle leak: losing the only pointer

This one is really important.

```c
int *p = malloc(sizeof(int) * 10);
int *q = malloc(sizeof(int) * 10);

p = q;
```

Why bad?

Because the original block that `p` pointed to is now lost forever.
You have no pointer to it anymore, so you cannot free it.

The lectures use exactly this kind of example as a problematic treatment of memory.  

---

# 15. Good `malloc/free` habits

## Habit 1: always check `malloc`

```c
int *p = malloc(sizeof(*p));
if (p == NULL) {
    return 1;
}
```

## Habit 2: free what you allocate

If you call `malloc`, there should usually be a matching `free` later.

## Habit 3: set pointer to `NULL` after free

```c
free(p);
p = NULL;
```

## Habit 4: do not overwrite the only pointer before freeing

Bad:

```c
p = malloc(...);
p = malloc(...);   // lost first block
```

## Habit 5: do not return stack addresses when you meant long-lived memory

---

# 16. `calloc` and `realloc`

The later lecture also mentions these. 

## `calloc`

```c
int *arr = calloc(5, sizeof(int));
```

Like `malloc`, but zero-initializes the block.

So this:

```c
for (int i = 0; i < 5; i++) {
    printf("%d ", arr[i]);
}
```

prints zeros.

## `realloc`

```c
int *bigger = realloc(arr, sizeof(int) * 20);
```

This resizes a previously allocated block.

Important:

* the returned pointer may be the same address
* or a new address
* so always assign carefully

Safer pattern:

```c
int *tmp = realloc(arr, sizeof(int) * 20);
if (tmp != NULL) {
    arr = tmp;
}
```

---

# 17. Real examples of when developers use `malloc`

Developers use it when they need **dynamic size** or **dynamic lifetime**.

## Example: user-sized array

```c
int n;
scanf("%d", &n);
int *scores = malloc(sizeof(int) * n);
```

## Example: linked list node

```c
struct node {
    int value;
    struct node *next;
};

struct node *n = malloc(sizeof(*n));
```

## Example: tree node

```c
struct tree_node {
    int data;
    struct tree_node *left;
    struct tree_node *right;
};

struct tree_node *root = malloc(sizeof(*root));
```

## Example: buffer

```c
char *buf = malloc(1024);
```

## Example: data returned from function

```c
char *make_message(void) {
    char *s = malloc(100);
    return s;
}
```

The final C lectures explicitly connect heap allocation to trees and other dynamic structures. 

---

# 18. When developers do not use `malloc`

Do **not** use it when simple stack storage is enough.

Instead of:

```c
int *p = malloc(sizeof(int));
*p = 7;
```

just do:

```c
int x = 7;
```

Instead of:

```c
int *arr = malloc(sizeof(int) * 10);
```

if size is fixed and local lifetime is enough, just do:

```c
int arr[10];
```

So `malloc` is not “better.”
It is just for cases where stack memory is the wrong tool.

---

# 19. Common CS210-style comparisons

## Stack variable

```c
int x = 5;
```

* automatic lifetime
* disappears when scope ends
* no `free`

## Heap variable

```c
int *p = malloc(sizeof(int));
*p = 5;
free(p);
```

* manual lifetime
* must call `free`
* pointer on stack, pointee on heap

---

# 20. A full worked example with function boundaries

```c
#include <stdio.h>
#include <stdlib.h>

int *make_array(int n) {
    int *arr = malloc(sizeof(int) * n);
    if (arr == NULL) {
        return NULL;
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
        printf("allocation failed\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");

    free(a);
    a = NULL;

    return 0;
}
```

Why this is good:

* runtime-sized array
* survives return from `make_array`
* checked for failure
* freed at the end

---

# 21. What not to do cheat sheet

## Wrong: no check

```c
int *p = malloc(sizeof(int));
*p = 5;
```

## Better

```c
int *p = malloc(sizeof(int));
if (p == NULL) return 1;
*p = 5;
```

---

## Wrong: use after free

```c
free(p);
printf("%d\n", *p);
```

## Better

```c
free(p);
p = NULL;
```

---

## Wrong: leak

```c
int *p = malloc(...);
p = malloc(...);
```

## Better

```c
int *p = malloc(...);
free(p);
p = malloc(...);
```

---

## Wrong: return pointer to local

```c
int *f(void) {
    int x = 5;
    return &x;
}
```

## Better

```c
int *f(void) {
    int *p = malloc(sizeof(int));
    if (p != NULL) *p = 5;
    return p;
}
```

---

# 22. Best one-line template to memorize

```c
Type *p = malloc(sizeof(*p) * count);
if (p == NULL) {
    /* handle failure */
}

/* use p */

free(p);
p = NULL;
```

Examples:

```c
int *arr = malloc(sizeof(*arr) * n);
struct node *node = malloc(sizeof(*node));
char *buf = malloc(sizeof(*buf) * 100);
```

---

# 23. Final intuition

`malloc` is how you say:

> “Please give me some memory on the heap that I can keep using until I explicitly give it back.”

`free` is how you say:

> “I’m done with that memory now; it can be reused.”

If you forget `free`, you leak memory.
If you use the pointer after `free`, you have a dangling pointer.
If you free twice, you corrupt allocator logic.

---

# 24. Mini summary card

* `malloc(n)` → allocate `n` bytes on heap
* returns pointer to start of block
* returns `NULL` on failure
* use `sizeof(...)`
* `free(p)` releases the block
* `free` does **not** change `p`
* after `free`, `p` is dangling
* good habit: `p = NULL`
* use `malloc` for dynamic size or dynamic lifetime
* common uses: arrays, strings, structs, trees, linked lists, buffers

If you want, next I can give you a **malloc/free bug drill** with 10 tiny snippets where you tell me whether each one is safe, leaking, dangling, or double-free.
