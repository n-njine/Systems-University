# Phase 1, Week 1: Comprehensive Examination

**Course:** Advanced Computer Systems & Software Vulnerability Analysis

**Topic:** Machine Architecture, Register Topology, and Information Representation

**Format:** Open-Book, Analytical Theory & Machine-State Verification

---

### ⚠️ Instructions

* Show every intermediate logical step, mathematical derivation, or memory state change.
* Hexadecimal values must be prefixed with `0x`. Binary values must be clearly grouped in 4-bit nibbles (e.g., `0000 0000`).
* Approaching these questions requires thinking like an optimizing compiler and an execution engine simultaneously.

---

## Part A: Mathematical Foundations & Integer Subversion

### Question 1: Two's Complement Range Analysis (15 Marks)

An embedded flight control surface positioning system reads raw inputs from a sensor using a specialized 12-bit signed integer representation at the hardware layer.

1. Derive the maximum positive decimal value and the minimum negative decimal value that can be natively represented within this 12-bit storage space. Outline the bitwise assignments for each boundaries.
2. Provide the exact 3-nibble hexadecimal representation of both boundaries as they would appear when packed into a standard memory word layout.
3. Suppose a logical glitch in the software forces a bitwise inversion (NOT operation) on the minimum negative value. Calculate the resulting decimal value if the system continues to interpret the modified pattern as a 12-bit signed integer.

### Question 2: The Malicious Frame Dimension (20 Marks)

A highly privileged system service written in C parses untrusted network frames. The frame structure begins with an unsigned 16-bit integer length header fields (`uint16_t frame_len`). The program dynamically calculates the actual buffer allocation size using the following allocation expression:

```c
uint16_t allocation_size = (frame_len * 4) + 32;
char *buffer = (char *)malloc(allocation_size);

```

1. Identify the minimum decimal value of `frame_len` required to cause a strict integer wrap-around (overflow) back to an `allocation_size` of exactly **`0`**. Show your algebraic proof.
2. An attacker crafts a payload where `frame_len` is set to exactly `16,383` (decimal). Calculate the resulting value of `allocation_size` in hexadecimal and decimal.
3. If the operating system’s underlying memory allocator (`malloc`) executes with the calculated `allocation_size` from step 2, evaluate the critical discrepancy that arises between the physical heap allocation size and a processing loop that copies data based on the original `frame_len`. What specific memory exploit vector is created here?

---

## Part B: Hardware State & Assembly Execution Tracing

### Question 3: Processor Flag State Space (25 Marks)

An execution unit on an x86 processor initializes its lower 8-bit accumulator register `AL` to a state of `0x00`. The CPU status flags are initially clear: `CF = 0` (Carry), `ZF = 0` (Zero), `SF = 0` (Sign), `OF = 0` (Overflow).

The processor executes the following sequential assembly instruction block:

```assembly
mov al, 0x80
sub al, 0x01

```

Trace the exact cycle-by-cycle evolution of the machine state by completing the architectural execution grid below. For each step, provide the bit pattern, hex representation, and the status of the flags register.

| Instruction Step | Target Register | Register Binary State | Register Hex State | `CF` | `ZF` | `SF` | `OF` |
| --- | --- | --- | --- | --- | --- | --- | --- |
| **Initial State** | `AL` | `0000 0000` | `0x00` | 0 | 0 | 0 | 0 |
| `mov al, 0x80` | `AL` |  |  |  |  |  |  |
| `sub al, 0x01` | `AL` |  |  |  |  |  |  |

*Note: For the `sub` instruction, remember that subtracting $1$ is mathematically equivalent to adding the two's complement representation of $-1$ (`0xFF`).*

**Analytical Supplement:**

1. Based on your completed execution grid, explain why the Sign Flag (`SF`) and the Overflow Flag (`OF`) flipped to their respective states after the subtraction. What did the hardware assume occurred versus what actually occurred mathematically?

---

## Part C: Memory Layout Boundaries & Structural Geometry

### Question 4: Endianness Transformation Mapping (20 Marks)

A target computer system runs an application that receives a 32-bit signature value over a raw network socket. The network protocol transmits data using standard **Network Byte Order (Big-Endian)**. The memory snapshot of the incoming stream on the physical wire arrives in this exact linear sequence of bytes:

$$\text{Stream Memory Offset:} \quad [0\text{x00}]=\text{0x7F} \quad [0\text{x01}]=\text{0x45} \quad [0\text{x02}]=\text{0x4C} \quad [0\text{x03}]=\text{0x46}$$

1. Write out the 32-bit hexadecimal double-word signature value as interpreted by a CPU running on a **Big-Endian** architecture.
2. Write out the exact layout of these bytes as they will be written into a local data structure inside the RAM of a target server utilizing a standard Intel x86-64 **Little-Endian** architecture. Fill in the specific byte contents for each address index:
* Base Memory Pointer (`Base + 0`): `0x__`
* Base Memory Pointer (`Base + 1`): `0x__`
* Base Memory Pointer (`Base + 2`): `0x__`
* Base Memory Pointer (`Base + 3`): `0x__`


3. If a debugger inspects the main CPU tracking register (`EAX`) immediately after the processor loads this signature value from local RAM on the Little-Endian system (`mov eax, dword ptr [base_ptr]`), what literal hexadecimal value will be displayed in the register panel?

---

## Part D: Synthesis & Code Review

### Question 5: Vulnerability Identification (20 Marks)

Analyze the following source code snippet extracted from an application handling untrusted digital media rendering.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int process_user_payload(char *untrusted_bytes, short user_specified_count) {
    // Note: 'short' is a 16-bit signed integer on this platform.
    char processing_buffer[256]; 

    if (user_specified_count > 256) {
        printf("Error: Input count exceeds static stack buffer allocation boundary!\n");
        return -1;
    }

    // High-performance block memory copy
    memcpy(processing_buffer, untrusted_bytes, user_specified_count);
    return 0;
}

```

1. Isolate the architectural flaw hidden within this function validation mechanism.
2. Describe an input vector (specifically, the exact value of `user_specified_count`) that completely neutralizes the security check `if (user_specified_count > 256)`.
3. Track the execution path of your malicious input value through the code. Explain exactly what happens when it is passed as the third parameter to `memcpy()`. (Hint: Check the definition of the size parameter in standard `memcpy`).

---

---

### 💡 Submission Guidance

Take your time working through each proof on paper or in your study workspace. When you are ready, post your complete solutions or request clarification on specific questions.

We will verify every answer down to the single-bit level before unlocking the next stage: **Phase 1, Lecture 2 (The Architecture of Execution: Assembly Mnemonics, Instruction Decoding, and GDB Diagnostics).**
