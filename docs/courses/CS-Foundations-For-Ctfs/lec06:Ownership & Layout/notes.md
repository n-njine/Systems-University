The past five weeks have given you a programmer’s eyes. This week, you gain an **exploit developer’s eyes**. Pointers to pointers, structs, bitwise operations, macros, and Makefiles—these are the tools that make you capable of reading real source code, understanding complex memory layouts, and beginning to see how a malicious input can turn data into control. Every section in this deep dive is chosen to appear in real-world CTF challenges: heap metadata, packet headers, linked lists, XOR decryption, and manual memory crafting.

# 🔥 WEEK 6: INTENSIVE DEEP DIVE  
## *“Ownership & Layout”*

---

## ⏳ Session Overview (8‑Hour Immersive Block)

| Time (total) | Section |
|--------------|---------|
| 0:00 – 1:30 | Pointer to Pointers & Multi‑dimensional Arrays |
| 1:30 – 2:30 | `struct` and `typedef` |
| 2:30 – 4:00 | Bitwise Operations |
| 4:00 – 5:00 | Preprocessor & Macros |
| 5:00 – 5:30 | Error Handling & `errno` |
| 5:30 – 6:00 | Makefile Basics |
| 6:00 – 7:00 | Mini‑Project: Hash Table Phonebook |
| 7:00 – 8:00 | Problem Set Kickoff & Reflection |

---

## 1. POINTER TO POINTERS & MULTI‑DIMENSIONAL ARRAYS (1.5 HOURS)

You’ve mastered single pointers. Now we add a level of indirection—essential for dynamically allocated 2D structures, modifying pointers themselves in functions, and understanding complex data types in disassembly.

### 1.1 Pointer to Pointer (`int **`)
A `T **` holds the address of a pointer that holds the address of a `T`. This allows us to modify a pointer passed to a function, and to build dynamic 2D arrays.

```c
int x = 10;
int *p = &x;
int **pp = &p;

printf("%d\n", **pp);  // 10
```

**Call-by-reference for pointers:** Imagine a function that allocates memory and wants to give back the new pointer. Without `**`, you can only modify the pointed-to value, not the pointer itself.

```c
void allocate_array(int **arr, int n) {
    *arr = (int*)malloc(n * sizeof(int));
}

int main() {
    int *data = NULL;
    allocate_array(&data, 5);
    // now data points to the allocated block
    free(data);
}
```

**Exercise:** Write `void append_node(Node **head, int value)` that appends a node to a linked list, correctly updating the head if the list was empty. (You did something similar in Week 5, but here’s the explicit double‑pointer version.)

### 1.2 Multi‑dimensional Arrays
C offers two forms: statically declared 2D arrays and dynamically allocated pointer‑to‑pointer tables. They are **not** the same in memory.

**Static 2D array:** Contiguous block of memory, row‑major order.
```c
int mat[3][4];  // 3 rows, 4 cols, 12 ints contiguous
```
`mat[i][j]` is accessed as `*(*(mat + i) + j)`. The compiler knows the row width (4 ints), so `mat+i` jumps `i*4*sizeof(int)` bytes.

**Dynamic 2D array using pointer‑to‑pointer:** You allocate an array of pointers, each pointing to a row (potentially separate allocations).
```c
int **mat2 = (int**)malloc(rows * sizeof(int*));
for (int i = 0; i < rows; i++)
    mat2[i] = (int*)malloc(cols * sizeof(int));
```
Now `mat2[i][j]` works syntactically, but memory is **not** contiguous—each row can be anywhere. This matters for cache behavior and for exploits: a buffer overflow in one row won’t necessarily corrupt the next row unless the allocator places them contiguously (they probably won’t be).

**Single‑block dynamic 2D:** For cache efficiency and easier overflows, you can allocate a flat `int *block = malloc(rows*cols*sizeof(int))` and index with `block[i*cols + j]`. Many CTF binary challenges use this pattern.

**Exercise:** Implement a function `int **create_matrix(int rows, int cols)` that returns a pointer‑to‑pointer matrix, and another `void free_matrix(int **mat, int rows)` that frees all rows and then the row pointers. Check with Valgrind.

**CTF relevance:** In a heap exploit, understanding whether a target uses a contiguous block or pointer‑to‑pointer affects how you can corrupt metadata or adjacent objects. A flat array overflow can corrupt the next object; a row‑pointer table overflow might overwrite a pointer that later gets dereferenced (arbitrary write).

### 1.3 Pointers to Arrays
You can declare a pointer to an entire array (fixed size):
```c
int arr[3][4];
int (*p)[4] = arr;   // p points to a row of 4 ints
p++;                  // advances by one whole row (4 ints)
```
This is less common but appears in some legacy code and CTF reversing. Don't confuse `int *p[4]` (array of pointers) with `int (*p)[4]` (pointer to array).

**Exercise:** Read a 2D static array using a pointer to array, and print elements using pointer arithmetic.

---

## 2. `STRUCT` AND `TYPEDEF` (1 HOUR)

Structures let you group variables of different types into a single, meaningful unit. They are the foundation of objects in C, network packet headers, and heap chunk metadata.

### 2.1 Defining and Using Structs
```c
struct Student {
    char name[50];
    int age;
    float gpa;
};

struct Student s1;
s1.age = 21;
strcpy(s1.name, "Alice");

// Or initialise:
struct Student s2 = {"Bob", 22, 3.5};
```

Memory layout: fields are placed sequentially, but the compiler may add **padding** between fields to satisfy alignment requirements (e.g., an `int` often at a 4‑byte aligned address). The size of a struct may be larger than the sum of its members.

**Check alignment:**
```c
printf("Size: %zu\n", sizeof(struct Student));  // likely 60 or 64
```

**Why padding matters:** When you craft an exploit payload that overwrites a struct, you must account for padding bytes—otherwise your controlled data might land in the gaps and not where you think.

### 2.2 Pointer to Struct
```c
struct Student *sp = &s1;
sp->age = 21;   // equivalent to (*sp).age
```
The arrow operator `->` dereferences a pointer and accesses a member.

**Exercise:** Write a function `void print_student(const struct Student *s)` that prints all fields.

### 2.3 Linked List of Structs (Revisited)
Your `Node` from Week 5 is a struct. Now embed arbitrary data:
```c
typedef struct Node {
    int id;
    char data[32];
    struct Node *next;
} Node;
```

**Exercise:** Write a function `Node *find_by_id(Node *head, int id)` that traverses and returns the node with matching id.

### 2.4 `typedef` – Type Aliases
```c
typedef unsigned long size_t;
typedef struct Student Student;  // now we can just write Student s;
```
`typedef` doesn’t create a new type, just an alias. Use it to reduce verbosity and improve readability.

**Example:** `typedef int (*cmp_fn)(const void*, const void*);` defines `cmp_fn` as a pointer to a comparison function. This is exactly how `qsort`'s callback works.

**CTF connection:** In reverse engineering, you'll see structures all the time. Recognizing them in Ghidra or IDA (e.g., a struct with `char`, `int`, `pointer` fields) helps you understand program state. Many exploit primitives involve overwriting struct fields (e.g., a function pointer in a struct, or a `size` field before a buffer to cause a larger overflow).

---

## 3. BITWISE OPERATIONS (1.5 HOURS)

Bitwise operators are your Swiss army knife for cryptography, binary data manipulation, flag checks, and shellcode encoding. Every CTF player breathes hex and bitops.

### 3.1 Operators & Precedence
- `&` (AND), `|` (OR), `^` (XOR), `~` (NOT)
- `<<` (left shift), `>>` (right shift)

**Note:** `^` XOR is not exponentiation—it’s exclusive OR. `&` and `|` are bitwise, not logical (which are `&&` and `||`).

**Exercise:** Without compiling, compute: `5 & 3`, `5 | 3`, `5 ^ 3`, `~5` (two’s complement 32-bit), `5 << 1`, `5 >> 1`.

### 3.2 Bit Masking and Setting/Clearing/Toggling Bits
```c
unsigned int flags = 0;
flags |= (1 << 3);      // set bit 3
flags &= ~(1 << 2);     // clear bit 2
flags ^= (1 << 1);      // toggle bit 1
int is_set = (flags & (1 << 3)) != 0;  // test bit 3
```

**Use case:** A CTF protocol might use a byte where each bit represents a flag (e.g., is_encrypted, is_compressed). You’ll parse these with masks.

### 3.3 Printing Binary Representation
Write a function `print_binary(unsigned int n)` that prints the 32-bit binary representation of `n`. This will be your debugging friend.

```c
void print_binary(unsigned int n) {
    for (int i = 31; i >= 0; i--) {
        putchar((n & (1 << i)) ? '1' : '0');
        if (i % 8 == 0) putchar(' ');
    }
    putchar('\n');
}
```

**Exercise:** Run `print_binary(0xDEADBEEF)`. Output should be `11011110 10101101 10111110 11101111`.

### 3.4 Endianness: Little vs Big
x86 is little‑endian: least significant byte stored at lowest address. A 32‑bit value `0xAABBCCDD` in memory: `DD, CC, BB, AA`. This is crucial when you inject addresses into a payload.

**Demo:**
```c
unsigned int val = 0xAABBCCDD;
unsigned char *p = (unsigned char*)&val;
for (int i = 0; i < 4; i++)
    printf("%02x ", p[i]);   // output: dd cc bb aa
```

**CTF relevance:** When crafting a ROP chain, you’ll pack addresses using `p64(addr)` which produces little‑endian bytes. Endianness errors cause exploits to fail silently.

### 3.5 XOR “Encryption”
XOR is reversible: `(A ^ K) ^ K = A`. Use this to write a simple XOR cipher:

```c
void xor_cipher(char *data, size_t len, char key) {
    for (size_t i = 0; i < len; i++) {
        data[i] ^= key;
    }
}
```

**Exercise:** Encrypt a string with key `0x2A`, then decrypt it back. Print intermediate hex.

**CTF connection:** Many crypto and reversing challenges involve XORing with a single byte or a repeating key. Your bitwise skills will let you write solvers quickly.

### 3.6 Bitfields in Structs
C allows specifying exact bit widths:
```c
struct Packet {
    unsigned int version : 4;
    unsigned int type    : 4;
    unsigned int length  : 16;
};
```
The layout is compiler‑dependent but common in protocol implementations. Exploits may overwrite such bitfields to bypass checks.

---

## 4. PREPROCESSOR & MACROS (1 HOUR)

The preprocessor is a text‑manipulation engine. It can create flexible code, but also obfuscate and introduce subtle bugs. Understanding macros helps you read real‑world codebases and CTF challenge source.

### 4.1 Object‑like Macros
```c
#define MAX_SIZE 256
```
`MAX_SIZE` is replaced literally by `256` before compilation. No type safety, just substitution.

### 4.2 Function‑like Macros and Pitfalls
```c
#define SQUARE(x) x * x
// Problem: SQUARE(1+2) -> 1+2*1+2 = 5, not 9.
// Fix: #define SQUARE(x) ((x) * (x))
```
**Always parenthesize both arguments and the whole expression.**

**Common macro traps:**
- Multiple evaluation: `#define MAX(a,b) ((a)>(b)?(a):(b))` and then `MAX(i++, j++)` increments twice.
- No type safety.
- Use `do { ... } while(0)` for multi‑statement macros to make them safe with semicolons.

**Debugging macros:** Use `gcc -E` to see the preprocessed output and check expansions.

### 4.3 Conditional Compilation
```c
#ifdef DEBUG
    printf("Debug: x = %d\n", x);
#endif

#if defined(__linux__)
    // Linux-specific
#elif defined(_WIN32)
    // Windows
#endif
```
This is how portable code and debug prints are controlled. In CTF binary challenges, the challenge might parse flags differently based on compile‑time defines.

### 4.4 `assert` and Debugging Macros
`assert(condition)` from `<assert.h>` is a macro that terminates the program if the condition is false, and is stripped when `NDEBUG` is defined.

**Build your own DEBUG macro:**
```c
#ifdef DEBUG
#define debug_printf(fmt, ...) fprintf(stderr, "[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define debug_printf(fmt, ...)
#endif
```
Use it: `debug_printf("x=%d\n", x);`. This prints file and line only in debug mode.

**Exercise:** Add `debug_printf` to your linked list functions and test with `-DDEBUG` flag.

### 4.5 Common Predefined Macros
- `__FILE__`, `__LINE__`, `__func__` – useful for logging.
- `__DATE__`, `__TIME__` – not often used, but appear.

**CTF connection:** In reversing, you might see strings left from debug macros, giving away source file paths or function names. Exploit developers use macros to quickly switch between local testing and remote attack configurations.

---

## 5. ERROR HANDLING & `ERRNO` (0.5 HOUR)

C programs report errors via return values and a global variable `errno`. Not checking them leads to crashes and exploitable states; checking them is the difference between a robust tool and a fragile one.

### 5.1 Return Codes and `errno`
Many standard functions return `-1` or `NULL` on error and set `errno` (from `<errno.h>`) to a code like `EACCES`, `ENOMEM`, `EINVAL`.

```c
#include <errno.h>
#include <string.h>
FILE *f = fopen("nonexistent.txt", "r");
if (f == NULL) {
    fprintf(stderr, "Error opening file: %s\n", strerror(errno));
    // or perror("fopen");
    exit(EXIT_FAILURE);
}
```
`perror()` prints a description of `errno` to stderr. `strerror()` returns the string.

### 5.2 Safe Coding Practices
- Always check `malloc` return for `NULL`.
- Check `fopen`, `fread`, `fwrite` returns.
- Use `feof()` and `ferror()` on streams.

**Exercise:** Write a robust file‑read function `char *read_file(const char *path)` that returns the content as a dynamically allocated string, or `NULL` on any error, with detailed error output.

**CTF exploitation angle:** A program that doesn’t check `malloc` failure might still use the returned `NULL` pointer, leading to a null pointer dereference (DoS) or, in rare cases, writable NULL page mapping. In some CTFs, you might need to manipulate `errno`‑based paths to cause intentionally faulty behavior.

---

## 6. MAKEFILE BASICS (0.5 HOUR)

As your projects grow, typing long `gcc` commands is error‑prone. A `Makefile` automates compilation and can teach you about build systems used in real CTF problem sources.

### 6.1 Simple Makefile Structure
```make
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -g

all: my_program

my_program: main.c utils.c utils.h
	$(CC) $(CFLAGS) -o my_program main.c utils.c

clean:
	rm -f my_program *.o
```
- `all` is the default target (first in file).
- Dependencies: `my_program` depends on source files; if they change, it recompiles.
- **Tab** character must precede commands (not spaces).

### 6.2 Object Files & Separate Compilation
```make
OBJS = main.o utils.o

my_program: $(OBJS)
	$(CC) $(CFLAGS) -o my_program $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
```
- `$<` is the first prerequisite (the .c file), `$@` is the target (the .o file).
- Now only changed files are recompiled.

### 6.3 Phony Targets and Variables
- `.PHONY: all clean` prevents conflicts with files named `all` or `clean`.
- Use `CFLAGS`, `LDFLAGS` for linking flags.

**Exercise:** Write a Makefile for your linked list project (multiple .c files: `main.c`, `list.c`, `list.h`). Build with `make`, run with `./list`, clean with `make clean`. Then add a `debug` target that sets `CFLAGS += -DDEBUG -O0`.

**CTF context:** Many CTF challenges provide a `Makefile`; understanding it helps you compile with correct flags (e.g., `-no-pie`, `-fno-stack-protector`). You’ll also write Makefiles for your exploit tools.

---

## 📞 MINI‑PROJECT: HASH TABLE PHONEBOOK (IN‑SESSION, 1 HOUR)

This project cements structs, pointers, dynamic allocation, and introduces a fundamental data structure used everywhere.

### 7.1 Design
- Hash table: array of 101 buckets (prime for less collisions).
- Each bucket is a pointer to a linked list of entries.
- Entry struct: `char name[50]; char phone[20]; struct Entry *next;`.
- Hash function: sum of ASCII values of characters modulo 101.
- Operations: `add`, `find`, `delete`, `list_all`.

### 7.2 Implementation (Code Along)
```c
#define TABLE_SIZE 101

typedef struct Entry {
    char name[50];
    char phone[20];
    struct Entry *next;
} Entry;

typedef struct Phonebook {
    Entry *buckets[TABLE_SIZE];
} Phonebook;

unsigned int hash(const char *name) {
    unsigned int h = 0;
    while (*name) {
        h = h * 31 + *name;  // djb2 simplified
        name++;
    }
    return h % TABLE_SIZE;
}

void pb_init(Phonebook *pb) {
    for (int i = 0; i < TABLE_SIZE; i++)
        pb->buckets[i] = NULL;
}

int pb_add(Phonebook *pb, const char *name, const char *phone) {
    unsigned int idx = hash(name);
    Entry *e = (Entry*)malloc(sizeof(Entry));
    if (!e) return -1;
    strncpy(e->name, name, sizeof(e->name)-1);
    strncpy(e->phone, phone, sizeof(e->phone)-1);
    e->name[sizeof(e->name)-1] = '\0';
    e->phone[sizeof(e->phone)-1] = '\0';
    e->next = pb->buckets[idx];
    pb->buckets[idx] = e;
    return 0;
}

char *pb_find(Phonebook *pb, const char *name) {
    unsigned int idx = hash(name);
    for (Entry *e = pb->buckets[idx]; e; e = e->next) {
        if (strcmp(e->name, name) == 0)
            return e->phone;
    }
    return NULL;
}

void pb_free(Phonebook *pb) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Entry *e = pb->buckets[i];
        while (e) {
            Entry *next = e->next;
            free(e);
            e = next;
        }
        pb->buckets[i] = NULL;
    }
}
```

**Exercise:** Implement `pb_delete(Phonebook *pb, const char *name)` that removes and frees the entry, handling the case where it's the first node in the bucket (need to update the bucket pointer, using a double pointer is elegant).

**Why this matters:** In CTF, you may encounter custom hash tables in challenges (e.g., a cache, a login database). Understanding this implementation lets you reason about bugs like hash collision DoS, use-after-free in deletion, or dangling pointers.

---

## 📝 WEEK 6 PROBLEM SET (Daily 2‑4 Hours)

### PS6‑1: K&R Exercises Chapter 6
Complete exercises 6‑1 to 6‑5 (or at least 6‑1 through 6‑4).
- 6‑1: Enhanced version of `getword` to handle underscores, string constants, comments.
- 6‑2: Read a C source file and group variable names by first 6 characters (use your hash table or a linked list).
- 6‑3: Cross‑ref program – print all words along with line numbers where they appear (a simple index).
- 6‑4: Word frequency counting, print sorted by frequency (requires sorting a struct).
- 6‑5: Undefine a macro (implement a symbol table).  
These exercises directly practice structs, pointers, and complex data structures.

### PS6‑2: CS50 Week 2 Problem Set – Readability and Caesar
If not yet done, complete “Readability” (Coleman‑Liau index) and “Caesar” (implementation of Caesar cipher in C). This solidifies C string handling and command‑line arguments. Use only C standard library; no external crypto.

### PS6‑3: Project – Hash Table Phonebook (Full)
Finish the phonebook started in session. Add:
- `pb_delete` function.
- Main menu loop (add, find, delete, list all, exit).
- Persistence: load/save phonebook to a text file (one entry per line: `name:phone`).
- Support command‑line arguments: `./phonebook add "John" "555-1212"`, `./phonebook find "John"`, etc.
- Ensure zero memory leaks (Valgrind).

This is a medium‑size C project that reinforces everything from Weeks 5‑6.

### PS6‑4: picoCTF Reverse Engineering Intro
Log into picoGym and solve at least 3 challenges under “Reverse Engineering” (e.g., “keygenme‑py”, “Transformation”, “Mod 26”, “ARMssembly 0” if you’re adventurous). For each, write a short solution summary. Use the skills of pointer reasoning and bitwise operations where applicable.

### PS6‑5: OverTheWire Bandit 21‑24
Complete and document. These levels deal with `cron`, shell variables, and a SUID bit usage that writes to a file. Pay special attention to how a timed job can be exploited.

### PS6‑6: Read “Smashing the Stack” (Finish the Paper)
Finish Aleph One’s paper. Now you have enough background to understand the assembly snippets. After reading, attempt to explain in your own words the steps of the exploit: determine offset, overwrite return address to point into the buffer, place shellcode, land with NOP sled. This prepares you for Week 7’s Bomb Lab.

### PS6‑7: Week 6 Reflection
In `week6_reflection.md`:
1. What was the most challenging part of implementing the hash table? (Edge cases, double pointers?)
2. How do bitwise operations change the way you think about data?
3. Describe a situation in a CTF where knowing struct padding could help build an exploit payload.
4. How does the Makefile improve your development workflow?

---

## 🔥 DEEP DIVE WRAP‑UP

This week you’ve added the final bricks to your C foundation. You can manipulate pointers at multiple levels, design complex data structures, manipulate bits like a pro, and compile efficiently. These skills aren’t just academic—they appear constantly in CTF challenges: a binary may use a custom hash table for user sessions (vulnerable to corruption), network packets have bit‑level flags, and shellcode is encoded with XOR.

Next week, you’ll cross the threshold into assembly language and start the legendary CMU Bomb Lab. Everything you’ve learned about pointers and memory will become visual and immediate when you see the stack frame in GDB and realize: you control the return address.

**Build that phonebook. Run `gcc -Wall -g -o phonebook phonebook.c`. Fight the segfaults. You’re a memory‑owner now.**
