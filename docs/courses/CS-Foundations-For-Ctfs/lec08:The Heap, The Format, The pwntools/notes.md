We’ve hijacked `rip` and popped our first shell. Now we go deeper into the memory corruption underworld: the **heap**, format strings, integer bugs, and automated exploitation. This week is about turning your fragile “works in GDB” overflow into a reliable, weaponized exploit, and about learning to see vulnerabilities in every line of C.

# 🔥 WEEK 8: SUPER INTENSIVE DEEP DIVE  
## *“The Heap, The Format, The pwntools”*

---

## ⏳ Session Overview (8‑Hour Immersive Block)

| Time (total) | Section |
|--------------|---------|
| 0:00 – 1:30 | Heap Management & Chunk Anatomy |
| 1:30 – 3:00 | Format String Vulnerabilities |
| 3:00 – 4:00 | Integer Overflows & Other C Pitfalls |
| 4:00 – 5:30 | Intro to Ghidra & Solving a Crackme |
| 5:30 – 7:00 | pwntools Deep Dive: ROP & Automation |
| 7:00 – 7:30 | CTF Strategy & Live Mini‑Challenge |
| 7:30 – 8:00 | Problem Set Kickoff & Reflection |

---

## 🧠 PART 1: HEAP MANAGEMENT & CHUNK ANATOMY (1.5 HOURS)

The stack was your first target. Now meet the **heap**: a more flexible, more complex region of memory where `malloc`/`free` play. Understanding the heap is essential because modern exploits (use‑after‑free, heap spraying, House of Force) rely on corrupting its metadata.

### 1.1 The Heap vs. Stack
- **Stack:** LIFO, fast, limited size, automatically managed, data tied to function calls. Overwrite return address.
- **Heap:** large pool, allocation survives function returns, manual management, vulnerable to use‑after‑free, double‑free, arbitrary write via corrupted pointers.

### 1.2 How `malloc` Works (Glide’s Eye View)
In most Unix systems, `malloc` is provided by **ptmalloc2** (glibc’s allocator). It manages memory in **chunks**. A chunk is a contiguous block of memory with a header (metadata) preceding the user data pointer.

When you call `malloc(24)`, you get a pointer to a region with at least 24 bytes, but just before that pointer (8 or 16 bytes back) lies a control structure:

```
chunk -> +-------------------+  (prev_size: size of previous chunk if free)
         |   size | flags    |  (size of chunk, 3 bits at bottom: PREV_INUSE, IS_MMAPPED, NON_MAIN_ARENA)
         +-------------------+
user -> |   user data ...    |
         +-------------------+
```

The `size` field includes the header size and is aligned to 16 bytes (on 64‑bit). If the previous chunk is in use, its `prev_size` is available for data (that’s a space optimization).

**Key flags:** If `PREV_INUSE` bit is 0, the previous chunk is free, and we can use its `prev_size` to find the beginning of the previous chunk to unlink.

### 1.3 Free Chunks and Bins
When you `free(p)`, the allocator doesn’t just mark it free; it coalesces with adjacent free chunks to avoid fragmentation. Free chunks are stored in **bins** (doubly‑linked lists for various sizes). A free chunk has `fd` (forward pointer) and `bk` (backward pointer) stored inside the old user data area:

```
free chunk:
  prev_size
  size
  fd (pointer to next free chunk in bin)
  bk (pointer to previous free chunk)
  (unused space)
```

**Unlink attack (classic):** If you can overwrite the `fd` and `bk` of a free chunk, then when the allocator removes it from the bin, it writes `fd->bk = bk` and `bk->fd = fd`, which can turn into an **arbitrary write**. This was exploited before glibc hardened unlinking with integrity checks (check `fd->bk == p && bk->fd == p`). Still, understanding the concept is foundational.

### 1.4 Hands‑On: Writing a Naive Allocator
To truly internalize heap mechanics, we’ll write a simple bump‑allocator or explicit free‑list allocator in 20 minutes.

**Simple bump allocator:**
```c
#define HEAP_SIZE 1024
static char heap[HEAP_SIZE];
static char *free_ptr = heap;

void *simple_malloc(size_t size) {
    if (free_ptr + size > heap + HEAP_SIZE) return NULL;
    void *p = free_ptr;
    free_ptr += size;
    return p;
}
void simple_free(void *p) { /* nope */ } // trivial, but shows limitation
```
**Explicit free‑list:** maintains a linked list of free blocks. Each free block stores size, next pointer. When freeing, coalesce adjacent if possible. Write basic `my_malloc`/`my_free` and test it with a sequence of allocations and frees.

**Exercise:** Implement `my_free` that marks the block free and inserts into a linked list (unsorted). Then simulate a use‑after‑free: allocate a block, free it, but still have a pointer. Allocate again, and you may reuse the same memory; the old pointer still points there. Print its content before and after new allocation—corrupted.

This is exactly how use‑after‑free works.

### 1.5 Heap Overflow Demo
Compile a program with two adjacent `malloc`s:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char *a = (char*)malloc(16);
    char *b = (char*)malloc(16);
    strcpy(b, "SECRET");
    printf("Before: b = %s\n", b);

    // overflow 'a'
    memset(a, 'A', 40);  // writes into b's chunk
    printf("After:  b = %s\n", b);
    free(a); free(b);
    return 0;
}
```
Run: `b` might become `AAAAA...`. In more sophisticated attacks, you overwrite the chunk header of the next chunk (`size` field) to trick `free` into performing an unlink, leading to arbitrary write.

**Why this matters:** Many CTF heap challenges involve corrupting the `size` field to overlap chunks, then reading/writing sensitive data.

---

## 🔓 PART 2: FORMAT STRING VULNERABILITIES (1.5 HOURS)

The format string bug is one of the most powerful primitives: it can **read arbitrary memory** and **write arbitrary values** to arbitrary addresses, all with a single `printf(user_input)`.

### 2.1 How It Works
When `printf` is called with a format string like `"%d %s"`, it expects arguments on the stack corresponding to each format specifier. If the user supplies the format string (e.g., `printf(input)` instead of `printf("%s", input)`), and the input contains `%x` or `%n`, they can dump stack values or write to addresses.

Let’s craft a vulnerable program:
```c
#include <stdio.h>
int main() {
    char buf[100];
    fgets(buf, sizeof(buf), stdin);
    printf(buf);   // BUG!
    return 0;
}
```

### 2.2 Leaking Stack Data
Input: `"%p %p %p %p"` will print hex values from the stack. This can leak libc addresses, canaries, or return addresses, bypassing ASLR.

**In‑session:** Run the program and input `"%p"` repeatedly. Count how many `%p`s it takes to start seeing the contents of `buf` itself. Then you can read arbitrary strings by pointing to an address placed on the stack.

Exercise: Put an address (e.g., the GOT entry for `exit`) into the beginning of your buffer, then use `%s` at an appropriate offset to print that address’s content as a string. This is a **format string read**.

### 2.3 Writing to Memory with `%n`
`%n` writes the number of characters printed so far to an integer pointer from the stack. If we can control that pointer, we can write arbitrary values.

**Experiment:**
```python
payload = p32(target_addr) + b"%x%x%x%n"  # simple overwrite
```
We want to write a specific value (e.g., the address of `win` function to GOT entry). Fine‑tune the count with precision specifiers: `"%<num>c%<offset>$n"`.

### 2.4 Exercise: Leak libc address
Use format string to leak a libc address from the GOT (e.g., `printf`’s GOT entry contains actual printf address after resolution). Then calculate `system` and overwrite a function pointer with it. This is a classic attack.

We’ll do a guided example with a toy binary.

**Step‑by‑step:**
1. Compile with `-no-pie -fno-stack-protector -m32` (or 64-bit).
2. Find address of `printf`@GOT via `objdump -R ./fmt`.
3. Determine offset to our input on the stack (using `%p`s).
4. Construct payload that writes the address of `win` to the GOT of `exit` (or `puts`). We’ll use pwntools’ `fmtstr_payload` later.

**Time‑saving:** pwntools provides `fmtstr_payload(offset, {addr: value})` that calculates all the %c and %n for you. We’ll use it later in pwntools section.

---

## ⚠️ PART 3: INTEGER OVERFLOWS & OTHER C PITFALLS (1 HOUR)

Integer vulnerabilities are often overlooked but can lead to buffer overflows when a naive size check is bypassed.

### 3.1 Integer Overflow
```c
int n;
scanf("%d", &n);
char *buf = (char*)malloc(n + 1);  // if n is 0x7FFFFFFF, n+1 wraps to negative/small positive
```
In unsigned arithmetic, `UINT_MAX + 1 == 0`. If `n` is 0xFFFFFFFF (uint32_t), `n + 1` overflows to 0, `malloc(0)` returns a minimal chunk (or NULL), and then a `read` with size `n` overflows.

**Demo:**
```c
unsigned short len = 65535;
if (len < 256) {
    // assume safe
    char buf[256];
    read(fd, buf, len);  // len is truncated to 65535 & 0xFFFF? Actually read uses size_t, but len is 16-bit, passed as int -> large value
}
```
Compile with `-Wall` to see warnings.

### 3.2 Signed/Unsigned Mismatch
```c
int len = read(fd, &n, sizeof(int));  // len is -1 on error, but if stored in unsigned, becomes huge
if (len < 256) {
    buf[len] = 'a';  // off-by-one or worse
}
```

### 3.3 Off‑by‑One and Loop Conditions
Classic `<=` vs `<` in loops, leading to a single byte zero overflow (null‑byte poisoning). In heap, a null byte overwriting the `PREV_INUSE` bit of the next chunk can lead to chunk confusion (see “House of Einherjar”).

**Exercise:** Write a function that copies a string into a buffer with `strncpy(dest, src, n)` but forgets to null‑terminate when src length equals n, leading to info leak. (Already discussed, but now actively think exploitation.)

---

## 🔍 PART 4: INTRO TO GHIDRA & SOLVING A CRACKME (1.5 HOURS)

Reverse engineering is your scalpel. Ghidra (NSA’s open‑source reverse engineering framework) turns binaries back into readable C, drastically speeding up analysis.

### 4.1 Ghidra Setup & First Look
- Download and install Ghidra from [ghidra-sre.org](https://ghidra-sre.org/).
- Create a project, import a simple binary (e.g., the vulnerable program from Week 7).
- Double‑click to analyze with default options.

### 4.2 The Interface
- **Program Tree:** list of functions.
- **Listing:** disassembly and decompiled C.
- **Decompiler window:** Shows C code generated from assembly. It’s not perfect, but incredibly useful.
- **Symbol Tree:** exports, imports, labels.

**Walkthrough:** Open `main`. Ghidra will show something like:
```c
undefined8 main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: ...");
        return 1;
    }
    vulnerable(argv[1]);
    return 0;
}
```
Click on `vulnerable` to see its decompilation: `strcpy((char *)&local_18, param_1);` where `local_18` is a 24‑byte buffer. That’s your overflow.

### 4.3 Solving a Crackme
I’ll provide a simple crackme (we can create one). Write a C program that asks for a password, compares the input to a hardcoded string using a simple transformation (e.g., XOR with 0xAA). Compile without symbols: `gcc -s -o crackme crackme.c`. Use Ghidra to understand the logic and extract the password.

**Live demo:**
```c
// crackme.c
#include <stdio.h>
#include <string.h>
int main() {
    char pass[32];
    printf("Password: ");
    fgets(pass, sizeof(pass), stdin);
    pass[strcspn(pass, "\n")] = 0;
    char secret[32];
    for (int i = 0; i < strlen(pass); i++) {
        secret[i] = pass[i] ^ 0xAA;
    }
    secret[strlen(pass)] = '\0';
    if (strcmp(secret, "\xeb\xf2\xf7\xe6\xd8") == 0) { // XORed "hello"
        printf("Correct!\n");
    } else {
        printf("Wrong.\n");
    }
}
```
In Ghidra, find the `strcmp` call; one argument is a constant string (the XORed value). Then figure out the XOR, reverse it, and get the password. This is a typical easy RE challenge.

**Why Ghidra?** It reveals hidden constants, control flow, and even handles function IDA-like FLIRT signatures. In CTFs, you’ll use it to analyze malware, find backdoors, and understand custom protocols.

---

## 🛠️ PART 5: PWNDEEP DIVE: ROP & AUTOMATION (1.5 HOURS)

`pwntools` is your exploit Swiss Army knife. We’ve used basic send/recv. Now we master **ROP** and `fmtstr` helpers.

### 5.1 ROP Basics
When NX (non‑executable stack) is enabled, you can’t just execute shellcode on the stack. Instead, you chain together existing code snippets (“gadgets”) that end with `ret` to achieve arbitrary execution (Return‑Oriented Programming).

A typical ROP chain for x86‑64 calls `execve("/bin/sh", 0, 0)` by setting `rax=59` (syscall number), `rdi=pointer to "/bin/sh"`, etc. This is done by popping values into registers from the stack.

**pwntools makes it easy:**
```python
from pwn import *
elf = ELF('./vuln')
rop = ROP(elf)
# Find gadgets
pop_rdi = rop.find_gadget(['pop rdi', 'ret'])[0]
pop_rsi = rop.find_gadget(['pop rsi', 'ret'])[0]  # maybe rsi, rdx
# etc.
# Or use rop.rdi.address, etc. But often you can just call syscall if you have control of rax.
# However, easier is to leak libc and call system("/bin/sh")
```

**Exercise:** Given a simple overflow with NX enabled, use `rop` to call `printf` to leak a libc address, then call `system`. This is ret2libc. We’ll do a guided example with a provided binary.

**In‑session task:** We’ll use the same vulnerable program but compiled with `-no-pie -fno-stack-protector -z noexecstack`. Without shellcode, we need to call `system("/bin/sh")`. Find `system` in libc (we’ll assume libc given). Use pwntools to build a ROP chain: `pop rdi; ret` gadget, string address, `system`. Then exploit. This might be a bit advanced for one hour, so we’ll provide step‑by‑step instructions.

### 5.2 Automating with `fmtstr_payload`
```python
payload = fmtstr_payload(offset, {write_addr: value})
```
This computes the minimal format string to write `value` to `write_addr` given the offset where the format string input appears on the stack. We’ll use it on the format string binary from Part 2 to overwrite a GOT entry.

**Hands‑on:** Complete the format string exploit you started earlier using pwntools. Get a shell.

### 5.3 Template and Debugging
Integrate `context.log_level = 'debug'` to see sent/received bytes. Use `gdb.attach()` within pwntools to attach GDB to the process at a certain point for debugging.

**Create a robust exploit template for future use.**

---

## 🎯 PART 6: CTF STRATEGY & LIVE MINI‑CHALLENGE (1 HOUR)

### 6.1 Strategy
- **Recon:** `checksec` (from pwntools) tells you protections (PIE, RELRO, canary, NX). Use `file`, `strings`.
- **Vulnerability identification:** look for `gets`, `sprintf`, `strcpy`, format string.
- **Gather offsets:** cyclic pattern.
- **Build exploit:** start with crash, get control of `rip`, then decide payload (shellcode, ROP, format string write).
- **Remote adjustments:** may need to brute force ASLR base if no leak, use libc database.

### 6.2 Live Mini‑Challenge
Provide a simple binary (maybe from pwnable.kr "bof") and solve it together. For instance, the "bof" challenge from pwnable.kr: it’s a stack overflow where you need to overwrite a variable to pass a check. We’ll walk through downloading, analyzing, exploiting with pwntools.

**Task:** Fetch `bof` from pwnable.kr (if they have access), or use a similar local binary. Exploit it in 20 minutes, covering all steps.

---

## 📝 WEEK 8 PROBLEM SET (Daily 2‑4 Hours)

### PS8‑1: CS:APP Chapter 9 (Virtual Memory)
Read Sections 9.9 – 9.11 on Dynamic Memory Allocation. Understand implicit free lists, coalescing, and the basics of a simple allocator. This will deepen your heap knowledge.

### PS8‑2: Write Your Own Simple Allocator
Implement a **first‑fit free‑list allocator** with coalescing in C. Use sized headers and a global free list. Write test programs that allocate, free, and check for block reuse. Then, simulate a heap overflow that corrupts the `size` of a neighboring chunk and show how a subsequent `free` could be tricked into writing to an arbitrary location (basic unlink). Document the steps. This is a mini‑research project.

### PS8‑3: pwnable.kr – fd, collision, bof
Complete these three challenges on [pwnable.kr](http://pwnable.kr). Use `ssh` or the provided download. For each, write a full exploit script in Python with pwntools, and a detailed write‑up explaining the vulnerability and how you exploited it.

### PS8‑4: Crackmes.one – Easy RE Challenges
Go to [crackmes.one](https://crackmes.one/) and solve 2 easy crackmes. Use Ghidra to reverse engineer them. Write short solutions.

### PS8‑5: picoCTF – Pwn Category
Solve as many easy/medium pwn challenges as possible from picoGym (e.g., “buffer overflow 0”, “buffer overflow 1”, “format string 0”, “format string 1”). If you already did some, pick new ones. Aim for at least 5.

### PS8‑6: OverTheWire Narnia Levels 0 – 2
Continue your wargame journey. Narnia is classic buffer overflows with various twists. Level 0 is a simple environment variable overflow. Level 1 is a classic shellcode injection. Level 2 introduces format strings. Use your new pwntools skills.

### PS8‑7: Reflection & Mind Map
Create a mind map of vulnerability classes you’ve learned so far: stack overflow, format string, integer overflow, heap overflow, use‑after‑free. For each, note the cause, the impact (control flow, info leak, write), and a key defense.

---

## 🔥 DEEP DIVE WRAP‑UP

You’ve graduated from “I can write C” to “I can break C.” You now wield the heap, format strings, and reverse engineering like a professional. pwntools is your mechanized infantry. With the bomb lab behind you and the Narnia ahead, you’re building a formidable exploit arsenal.

This week’s projects will solidify these concepts under your fingers. The heap allocator you write will teach you more about corruption than any lecture. The crackmes will train your Ghidra‑fu. And the pwnable.kr trio will be your first real-world CTF victories.

**Fire up Ghidra. Crack the crackme. Then `nc pwnable.kr 9000` and break the BOF. You’re not just a programmer anymore—you’re a bender of execution.**
