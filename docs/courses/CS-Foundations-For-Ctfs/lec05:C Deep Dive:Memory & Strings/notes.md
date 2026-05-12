We’re peeling back the layers. Python gave you comfort; C will give you **power**. This week, you’ll touch the machine’s raw memory — pointers, arrays, the stack, and the heap. These aren’t abstract ideas; they’re the exact arena where buffer overflows, use‑after‑frees, and format string attacks happen. By the end, you’ll not only write C that works, you’ll understand *how* it works down to the bytes, and you’ll start using GDB to see what the CPU sees.

# 🔥 WEEK 5: INTENSIVE DEEP DIVE  
## *“C Deep Dive: Memory & Strings”*

---

## ⏳ Session Overview (8‑Hour Immersive Block)

| Time (total) | Section |
|--------------|---------|
| 0:00 – 1:30 | Arrays, Strings, and the Null Terminator |
| 1:30 – 3:30 | Pointers — The Core |
| 3:30 – 5:00 | Dynamic Memory Allocation |
| 5:00 – 6:00 | The Compilation Process |
| 6:00 – 7:00 | GDB Hands‑On: Seeing Memory |
| 7:00 – 8:00 | Problem Set Kickoff & Reflection |

---

## 🧱 PART 1: ARRAYS, STRINGS, AND THE NULL TERMINATOR (1.5 HOURS)

C has no native “string” type. Strings are just arrays of `char` with a null byte (`'\0'`) marking the end. This is the cause of a thousand exploits. Grasp this thoroughly.

### 1.1 Arrays: Contiguous Blocks of Memory
```c
int numbers[5] = {10, 20, 30, 40, 50};
```
- Memory: 5 integers laid out side‑by‑side. The array name `numbers` is essentially a pointer to the first element.
- Access: `numbers[2]` is `30`. It’s syntactic sugar for `*(numbers + 2)`.
- Arrays are **fixed size**; you can’t resize them. The compiler allocates space on the stack (or in data segment).

**Create and compile:**
```c
#include <stdio.h>
int main() {
    int a[5] = {1,2,3,4,5};
    printf("Size of a: %zu bytes\n", sizeof(a));   // 5 * 4 = 20 (if int is 4)
    for (int i = 0; i < 5; i++)
        printf("a[%d] at %p: %d\n", i, &a[i], a[i]);
    return 0;
}
```
Observe addresses: each `int` is 4 bytes apart. The stack grows, but within the array, addresses increase.

### 1.2 Characters and Strings
```c
char s[] = "hello"; // equivalent to {'h','e','l','l','o','\0'}
```
- `sizeof(s)` is 6, not 5.
- The null terminator is **mandatory** for functions like `printf("%s")`, `strlen()`, `strcpy()`. If missing, they’ll read/write past your buffer until they hit a zero. That’s undefined behavior and the basis of information leaks/overflows.

**The Danger Family:**
- `strcpy(dest, src)` — copies until it sees a null, regardless of `dest` size.
- `strcat(dest, src)` — appends, same problem.
- `gets(buffer)` — reads until newline, zero size check (removed from C11, but still in old code).
- `scanf("%s", buf)` — same.

**Always use bounded versions:** `strncpy`, `snprintf`, `fgets`, but even those have pitfalls (e.g., `strncpy` might not null‑terminate if source is too long).

### 1.3 K&R Chapter 1: Strings by Hand
Re‑implement standard string functions to truly know them. Write your own:

```c
// mystrlen: count until '\0'
size_t mystrlen(const char *s) {
    size_t len = 0;
    while (*s != '\0') {
        len++;
        s++;
    }
    return len;
}

// mystrcpy: copy src to dest, including '\0', return dest
char *mystrcpy(char *dest, const char *src) {
    char *original = dest;
    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return original;
}
```

**Code‑along exercise:** Write `mystrcmp` that compares two strings lexicographically, returning negative, zero, or positive. Test it with various strings.

**Why this matters:** In CTFs, you might need to locate where a binary uses a custom string function. Understanding the byte‑level loops makes you faster at reversing.

### 1.4 Array Overflow Demonstration
Compile with `-fno-stack-protector` to disable canaries for this demo (don’t do this in production).

```c
#include <stdio.h>
#include <string.h>
int main() {
    char buffer[8];
    char secret = 'A';
    printf("Enter string: ");
    gets(buffer);   // danger!
    printf("You entered: %s\n", buffer);
    printf("secret = %c (0x%x)\n", secret, secret);
    return 0;
}
```
Compile: `gcc -fno-stack-protector -o overflow overflow.c` (ignore warning about `gets`). Run and input `AAAAAAAA` (8 A’s). Then try `AAAAAAAAAA` (10 A’s). You might corrupt `secret`. Use GDB later to see exactly what happened.

**CTF connection:** This is the simplest stack buffer overflow. You’ll soon overwrite the saved return address, not just a local variable.

---

## 🎯 PART 2: POINTERS — THE CORE (2 HOURS)

Pointers are the soul of C. They represent memory addresses. Learn them now, or forever be blind to exploitation.

### 2.1 Definition and Operators
- `&x` → “address of x”
- `*ptr` → “value at address held by ptr”
- `int *p;` → p is a pointer to an int

```c
int x = 42;
int *p = &x;
printf("x=%d, &x=%p, p=%p, *p=%d\n", x, (void*)&x, (void*)p, *p);
// *p = 43;  would change x to 43
```

**Mental model:** Draw boxes for variables. For pointers, the box contains an arrow to another box.

### 2.2 Pointer Arithmetic
Adding 1 to a pointer moves it by the size of the pointed‑to type, not by one byte.

```c
int arr[3] = {100,200,300};
int *p = arr;       // points to arr[0]
printf("%d\n", *p); // 100
p++;                // now points to arr[1]
printf("%d\n", *p); // 200
printf("%d\n", *(arr + 2)); // 300, arr[2]
```

**Byte‑level view:** `arr` is at address `A`. `arr+1` is at `A+4` (if int is 4 bytes). `char *cp = (char*)arr; cp+1` is at `A+1`.

**Exercise:** Use pointer arithmetic to sum an array: `for (p=arr; p < arr+3; p++) sum += *p;`. This is equivalent to indexing.

### 2.3 Pointers and Functions: Call by Reference
C passes everything by value. To change a variable in the caller, you pass its address.

```c
void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int main() {
    int x=3, y=5;
    swap(&x, &y);
    printf("x=%d, y=%d\n", x, y);  // 5,3
}
```
Draw the memory: `&x` is passed, so inside `swap`, `a` points to `x`. The dereference `*a` modifies `x`.

**Why CTF:** Exploit functions take addresses of buffers or gadgets. The `pwntools` `p64()` function packs addresses. This is fundamental.

### 2.4 The Relationship Between Arrays and Pointers
The name of an array decays to a pointer to its first element in most contexts. But there’s a difference: `sizeof(array)` gives total bytes, `sizeof(pointer)` gives pointer size.

```c
int arr[10];
int *p = arr;
// arr = p;  // ERROR, array name is not an l-value
```
When passing an array to a function, you’re really passing a pointer. So `void func(int a[])` is equivalent to `void func(int *a)`.

**Exercise:** Write a function `void reverse(int *arr, int len)` that reverses an array in place using two pointers (one from start, one from end). Use only pointer arithmetic.

### 2.5 `void *` and Casting
`void *ptr;` is a generic pointer that can hold any address. You can’t dereference it directly; you must cast to a specific type first. This is the basis of `malloc` returning `void *`.

### 2.6 Function Pointers (Briefly)
Store an address of a function: `int (*fptr)(int, int) = &add;`. Call via `(*fptr)(2,3)`. Function pointers in structs are virtual method tables under the hood. Overwriting one is a classic exploitation technique.

---

## 🧠 PART 3: DYNAMIC MEMORY ALLOCATION (1.5 HOURS)

So far, arrays have fixed size known at compile time. The heap gives you runtime‑size memory that lives until you free it. Mismanaging the heap is the root of use‑after‑free, double‑free, and heap overflow vulnerabilities.

### 3.1 `malloc` and `free`
```c
#include <stdlib.h>
int *p = (int*)malloc(sizeof(int));  // allocate one int
if (p == NULL) { /* handle error */ }
*p = 42;
free(p);
// p is now a dangling pointer; don't dereference it.
```

**Key points:**
- `malloc(size)` returns a `void*`; cast it.
- It does **not** zero the memory; contents are uninitialized.
- `free(ptr)` returns memory to the allocator; `ptr` still holds the old address (dangerous).
- **Always check return value** for NULL.
- **Never** use after free, double free, or free an unallocated pointer.

### 3.2 Allocating Arrays on the Heap
```c
int n = 10;
int *arr = (int*)malloc(n * sizeof(int));
if (arr) {
    for (int i = 0; i < n; i++)
        arr[i] = i;
    free(arr);
}
```
`malloc` knows how many bytes you allocated; the allocator stores metadata before the returned block (size, flags). Overwriting this metadata leads to heap exploits.

### 3.3 `calloc` and `realloc`
- `calloc(n, size)` allocates `n*size` bytes and **zeros** them.
- `realloc(ptr, new_size)` resizes an existing allocation, possibly moving it. Returns new pointer.

**Exercise:** Write a function `int *read_numbers(int count)` that allocates space for `count` ints, reads them from user (using `scanf` within a loop), and returns the array. Call it and print the numbers, then free.

**Watch for memory leaks:** Use `valgrind --leak-check=full ./program` to check for leaks. We’ll use Valgrind later.

### 3.4 Building a Singly Linked List (Start It)
Practice dynamic structures:
```c
typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node *create_node(int data) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    if (new_node) {
        new_node->data = data;
        new_node->next = NULL;
    }
    return new_node;
}

void append(Node **head, int data) {
    Node *new_node = create_node(data);
    if (*head == NULL) {
        *head = new_node;
    } else {
        Node *cur = *head;
        while (cur->next) cur = cur->next;
        cur->next = new_node;
    }
}

void print_list(Node *head) {
    while (head) {
        printf("%d -> ", head->data);
        head = head->next;
    }
    printf("NULL\n");
}
```

**Code‑along:** Write a `free_list(Node **head)` function that frees all nodes, and sets the head to NULL.

**Why this matters:** In CTFs, you might need to understand a binary that uses a linked list. Use‑after‑free on a freed node (which still points to other nodes) can leak or corrupt memory.

### 3.5 Heap Overflow Demo
Two adjacent allocations: overflow the first to corrupt the second's data or metadata.

```c
char *a = (char*)malloc(8);
char *b = (char*)malloc(8);
// Fill b with something
strcpy(b, "secret");
printf("Before: b = %s\n", b);
// Overflow a
strcpy(a, "AAAAAAAAAAAAAAAA"); // 16 A's, spills into b's memory (due to heap layout)
printf("After: b = %s\n", b);  // likely corrupted
```
Run and see the behavior. Heap exploitation builds on exactly this.

---

## ⚙️ PART 4: THE COMPILATION PROCESS IN DEPTH (1 HOUR)

Understanding the pipeline from C source to executable demystifies how vulnerabilities survive optimization and how shellcode interacts with the binary.

### 4.1 The Four Stages
1. **Preprocessing:** `gcc -E file.c -o file.i`
   - Replaces `#include` with file contents, expands macros (`#define`), removes comments.
   - Example: see how `#define MAX 100` becomes literal 100 everywhere.
2. **Compilation:** `gcc -S file.i -o file.s`
   - Translates C to assembly for the target architecture.
   - You’ll see `movl`, `call`, stack manipulation.
3. **Assembly:** `gcc -c file.s -o file.o`
   - Converts assembly to machine code (object file), but addresses are not yet resolved.
4. **Linking:** `gcc file.o -o executable`
   - Resolves symbols (e.g., `printf` from libc), combines object files and libraries.

### 4.2 Examining the Output
Take a simple `int add(int a, int b) { return a+b; }` and go through all stages.
- The `.s` file will show `add` label, parameter passing in registers, and `ret`.
- The object file can be examined with `objdump -d file.o` to see raw instructions.
- The final executable includes startup code.

### 4.3 Linking and the Global Offset Table (GOT) / Procedure Linkage Table (PLT)
When you call `printf`, your code actually jumps to a PLT stub, which jumps to the GOT entry that holds the real `printf` address (resolved by the dynamic linker). This indirection is what makes **GOT overwrites** possible: if you can write to the GOT entry for `free`, you can redirect execution to arbitrary code.

We’ll revisit this, but plant the seed now.

### 4.4 Compiler Explorer (Hands-on)
Go to [godbolt.org](https://godbolt.org/) and paste a simple C function. See the assembly output for different optimization levels (`-O0`, `-O2`). Notice how `-O0` keeps stack variables in memory; `-O2` might optimize them to registers only. This difference affects exploit reliability.

**Exercise:** Write a function with a buffer and a call to `gets`. Compile with `-O0` vs `-O2` and observe how the stack layout changes.

---

## 🐞 PART 5: GDB HANDS-ON — SEEING MEMORY (1 HOUR)

GDB is your X‑ray machine. You’ll use it relentlessly for reverse engineering and exploit development.

### 5.1 Setup and Commands
Install GDB with GEF (GDB Enhanced Features) or Pwndbg for exploit‑friendly output. For now, plain GDB is enough, but we’ll install peda/GEF to get a stack view.

```bash
# Install GEF (recommended)
wget -q -O- https://github.com/hugsy/gef/raw/master/scripts/gef.sh | sh
```

Launch GDB on a binary compiled `-g` (debug symbols):
```
gdb ./overflow
```

**Essential commands:**
- `break main` (or `b *main+42`) — set breakpoint
- `run` (or `r < input.txt`) — start with optional input redirection
- `next` (n) — step over line
- `step` (s) — step into function
- `continue` (c) — continue until next breakpoint
- `print x` (p) — print variable
- `info registers` — show register values
- `x/10x $rsp` — examine 10 word‑sized hex values at stack pointer
- `x/s address` — print as string
- `disassemble main` — show assembly of function
- `bt` — backtrace (stack frames)

### 5.2 Tracing the Overflow Demo
Recompile the overflow program with `-g -fno-stack-protector -no-pie`.
```bash
gcc -g -fno-stack-protector -no-pie -o overflow overflow.c
gdb ./overflow
```
Inside GDB:
- `b main`, `run`, then `next` past `gets` but before `printf`.
- Enter a long input like `AAAAAAAAAAAAAAAA`.
- Examine memory: `x/20x $rsp`. Locate the buffer (8 bytes) and find where `secret` is. Notice the overwrite.
- If you entered enough A’s, the return address (saved RIP) might also be clobbered, causing segfault. Look at `info registers` after `ret`.

**Draw the stack:**
```
Higher addresses
+----------------+
| return address |  <-- overwritten if too long
| saved rbp      |
| secret (1 byte)|
| buffer[7]      |
| ...            |
| buffer[0]      |
+----------------+ <- rsp (lower)
```

**Take a screenshot or notes.** This mental picture is the blueprint for exploitation.

### 5.3 Inspecting a Function’s Prologue
Disassemble `main`: `disass main`. See:
```
push   rbp
mov    rbp, rsp
sub    rsp, 0x20   ; allocate space for locals
...
leave
ret
```
Understand: `sub rsp, N` makes space for local variables. The buffer is somewhere in that space.

**Exercise:** Find the exact offset from buffer start to the saved return address using `cyclic` later (in pwntools). For now, just note that the buffer is 8 bytes, but the compiler might add alignment padding.

### 5.4 Using GDB to Solve a Simple Crackme
Create a tiny password checker:
```c
#include <stdio.h>
#include <string.h>
int main() {
    char pass[20];
    printf("Password: ");
    fgets(pass, sizeof(pass), stdin);
    pass[strcspn(pass, "\n")] = 0; // remove newline
    if (strcmp(pass, "supersecret") == 0) {
        printf("Granted!\n");
    } else {
        printf("Denied!\n");
    }
    return 0;
}
```
In GDB, set a breakpoint at `strcmp`, run, and inspect registers: `x/s $rdi` and `x/s $rsi` to see both strings. You’ve just reverse‑engineered the password without reading the source.

**CTF skill:** You’ll do this on binaries where you don’t have source. GDB + strings = many easy reversing challenges solved.

---

## 📝 WEEK 5 PROBLEM SET (Daily 2‑4 Hours)

### PS5‑1: K&R Exercises (Chapter 1 finish, start 5)
- Finish Chapter 1: exercises 1‑16 to 1‑24 if not done. Focus especially on 1‑18 (remove trailing blanks), 1‑19 (reverse string), 1‑20 (detab), and 1‑22 (fold long lines). These force you to handle character arrays and logic.
- Start Chapter 5 (Pointers and Arrays): read the chapter up to 5.4. Do exercises 5‑1 to 5‑10. Exercise 5‑5 (simplified versions of strncpy, strncat, strncmp) is excellent practice.

For all exercises, compile with `gcc -Wall -Wextra -pedantic -o prog prog.c` and ensure zero warnings.

### PS5‑2: CS50 Problem Set 1 – Mario and Cash/Credit (C)
If you haven’t already, complete the following from CS50 Week 1 (C):
- **Mario (less comfortable)**: Print a right‑aligned pyramid of hashes based on user input (1-8). Emphasize loop logic.
- **Cash**: Greedy algorithm for minimum coins. Good logic practice.
- If you feel comfortable, try **Credit** (Luhn's Algorithm) to validate credit card numbers. This involves grabbing digits and conditionals.

These might feel easy compared to K&R, but they build fluency.

### PS5‑3: Valgrind Mastery
Install valgrind (`sudo apt install valgrind`). Run **every C program you wrote this week** under Valgrind:
```bash
valgrind --leak-check=full --show-leak-kinds=all ./prog
```
Ensure you understand each error type:
- "Invalid write/read" – buffer overflow/heap overflow.
- "Use of uninitialised value".
- "definitely lost" – memory leak.
- "Conditional jump depends on uninitialised value".

Fix any memory errors in your linked list code. Being Valgrind‑clean is a discipline.

### PS5‑4: Project – Singly Linked List in C
Complete the linked list implementation from the session. Add the following functions (no peeking, implement from scratch):
- `int length(Node *head)` – returns number of nodes.
- `int get(Node *head, int index)` – returns data at index, -1 if out of bounds.
- `void insert_at(Node **head, int index, int data)` – inserts at position.
- `void delete_at(Node **head, int index)` – removes node and frees it.
- `void reverse(Node **head)` – reverses list in place (iteratively; for an extra challenge, recursively).
- `int detect_cycle(Node *head)` – returns 1 if there is a cycle (Floyd’s tortoise and hare).
Test thoroughly, run under Valgrind, ensure no leaks even after errors.

This project forces you to understand double pointers, dynamic allocation, and edge cases—exactly what you face in heap exploits.

### PS5‑5: OverTheWire Bandit Levels 16‑20
Complete and document. These levels introduce:
- **Level 16‑17:** Use `nmap` to scan for an open port within a range, then connect with SSL.
- **Level 17‑18:** SSH key comparison, `diff`.
- **Level 18‑19:** `.bashrc` trick to prevent logout, or running a command with `ssh -T`.
- **Level 19‑20:** Setuid binary, understanding effective user ID.

These increase your command‑line sophistication.

### PS5‑6: Read “Smashing the Stack for Fun and Profit”
Read the first 3‑4 pages of Aleph One’s paper (up to the section where the exploit is constructed). Don’t worry if the assembly details aren’t fully clear yet; we’ll revisit. Focus on:
- The stack diagram (buffer, saved FP, return address).
- The idea that overwriting the return address lets you jump to arbitrary code.
- How the exploit injects shellcode into the buffer.

After reading, write a short paragraph explaining in your own words why a buffer overflow can allow code execution.

### PS5‑7: Week 5 Reflection
In `week5_reflection.md`:
1. What did you see in GDB that surprised you about memory layout?
2. Describe a pointer‑related bug you encountered and how you fixed it.
3. What was the hardest part of implementing the linked list?
4. How has your mental model of “variables” changed now that you understand pointers and stack frames?

---

## 🔥 DEEP DIVE WRAP‑UP

You are now operating at the level where exploits are born. You can allocate memory, manipulate pointers, and peer inside a running program with GDB. The mysterious “stack buffer overflow” is no longer an abstract phrase—you’ve seen it corrupt a variable, maybe even the return address. Next week, we’ll formalize memory structures: structs, bitwise operations, and more on the heap. Soon you’ll build your own `malloc`, then exploit it.

Keep your terminal open. Create `buffer.c`. Overflow it. **Break things on purpose, then understand why they broke.** That’s the path.
