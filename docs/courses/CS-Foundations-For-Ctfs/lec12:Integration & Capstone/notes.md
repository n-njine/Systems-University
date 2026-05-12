You’ve built a Python toolkit, a C shell, a network stack, and exploit primitives. Now it’s time to fuse everything into one cohesive, unstoppable skillset. This week is your **capstone**: you’ll simulate a full CTF — attacking your own vulnerable services, defending them, and analyzing forensic artifacts. You’ll unlock the basics of memory forensics and file carving, refresh essential cryptography, and polish your shell into a work of art. By the end, you won’t just be ready for Phase 2; you’ll be a competitor.

# 🔥 WEEK 12: SUPER INTENSIVE DEEP DIVE  
## *“Integration & Capstone”*

---

## ⏳ Session Overview (8‑Hour Immersive Block)

| Time (total) | Section |
|--------------|---------|
| 0:00 – 3:00 | Mini‑CTF Simulation: Attack & Defend |
| 3:00 – 4:00 | Forensics: File Carving & Memory Analysis |
| 4:00 – 5:00 | Cryptography Refresher & Real‑World Crypto |
| 5:00 – 6:30 | Shell Final Polish: Readline, History, Completion |
| 6:30 – 7:00 | Git Cleanup, Documentation & Repo Release |
| 7:00 – 7:30 | Phase 2 Roadmap & Next Steps |
| 7:30 – 8:00 | Final Reflection & Retrospective |

---

## 🛡️ PART 1: MINI‑CTF SIMULATION – ATTACK & DEFEND (3 HOURS)

This is the capstone event of Phase 1. You will set up **three custom vulnerable services** on your local VM, exploit them, capture flags, and then patch the vulnerabilities. You’ll practice the complete attack‑defense cycle.

### 1.1 Setup the Battlefield
Create a directory `~/mini_ctf` with subdirectories for each challenge. Also create a `flags/` directory to store the flag files that your services will read.

```bash
mkdir -p ~/mini_ctf/{stack_chall,format_chall,heap_chall,exploits,flags}
echo "CTF{st4ck_0v3rfl0w_m4ster}" > ~/mini_ctf/flags/stack_flag.txt
echo "CTF{f0rm4t_str1ng_l34k}" > ~/mini_ctf/flags/format_flag.txt
echo "CTF{h34p_c0rrupt10n_w1n}" > ~/mini_ctf/flags/heap_flag.txt
```

### 1.2 Challenge 1 – Stack Buffer Overflow (Vulnerable Service)
Write a simple TCP server in C that reads a message and echoes it back — but using `gets()` instead of `fgets()`.

**Vulnerable code skeleton:**
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

void handle_client(int clientfd) {
    char buffer[64];
    char flag_buf[128];
    FILE *f = fopen("flags/stack_flag.txt", "r");
    if (f) {
        fgets(flag_buf, sizeof(flag_buf), f);
        fclose(f);
    }
    // Read client data — VULNERABLE
    write(clientfd, "Enter name: ", 12);
    gets(buffer);  // deliberate overflow
    write(clientfd, "Hello, ", 7);
    write(clientfd, buffer, strlen(buffer));
    write(clientfd, "\n", 1);
    close(clientfd);
}

int main() {
    int servfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {.sin_family=AF_INET, .sin_port=htons(8888), .sin_addr.s_addr=INADDR_ANY};
    bind(servfd, (struct sockaddr*)&addr, sizeof(addr));
    listen(servfd, 5);
    while (1) {
        int clientfd = accept(servfd, NULL, NULL);
        if (fork() == 0) {
            close(servfd);
            handle_client(clientfd);
            exit(0);
        }
        close(clientfd);
    }
}
```
Compile without protections: `gcc -fno-stack-protector -z execstack -no-pie -o stack_svc stack_svc.c`

**Your mission:** Write a pwntools exploit that overflows the buffer, overwrites the return address, and executes shellcode to read and send back `stack_flag.txt`. Then patch the source to use `fgets` and recompile.

### 1.3 Challenge 2 – Format String Vulnerability
Create a similar service that uses `printf(user_input)` directly.

**Vulnerable code:** `recv(clientfd, buffer, sizeof(buffer)-1, 0); printf(buffer);`

Put a secret variable on the stack containing a pointer to the flag. The flag itself is loaded into a global buffer.

**Your mission:** Use format string `%x` to leak the secret pointer, then use `%s` to read the flag at that address. Alternatively, use `%n` to overwrite a function pointer (like the return address or a GOT entry) to redirect execution to a `print_flag` function that you add. Write the exploit. Then patch to `printf("%s", buffer)`.

### 1.4 Challenge 3 – Heap Overflow / Use‑After‑Free
Create a service that manages a simple note‑taking system:
- `CREATE <size> <data>` — allocates memory.
- `READ <id>` — prints note.
- `DELETE <id>` — frees note (but doesn't remove the pointer — UAF).
- `EDIT <id> <data>` — overwrites note with same size.

The flag is read into a note at startup and hidden.

**Your mission:** Exploit the use‑after‑free to read or overwrite an existing note and leak the flag. Then patch to NULL the pointer after free.

### 1.5 Defense Phase
After exploiting, patch all three sources, recompile, and verify the exploits no longer work. Write a short report on each vulnerability, its impact, and the mitigation. This report is your **first security advisory**.

**CTF connection:** In attack‑defense CTFs, you need to patch your own services while exploiting others. Speed matters. This simulation teaches the flow.

---

## 🔍 PART 2: FORENSICS – FILE CARVING & MEMORY ANALYSIS (1 HOUR)

Forensics challenges are common in CTFs. You’ll learn to recover deleted files from disk images and extract secrets from memory dumps.

### 2.1 File Carving Basics
- **`strings`**: extract ASCII strings from any binary.
- **`binwalk`**: find embedded files inside firmware or disk images.
- **`foremost`/`scalpel`**: carve files based on headers/footers (e.g., JPEG, PNG).

**Hands‑on:** Download a simple CTF forensics challenge (e.g., “shark on wire” or a known dump) or create your own:
```bash
dd if=/dev/zero of=test.img bs=1M count=10
mkfs.ext4 test.img
mkdir mnt && sudo mount test.img mnt
echo "CTF{f1le_c4rv1ng_r0cks}" > mnt/secret.txt
sudo umount mnt
# Now delete the file by overwriting the inode (complicated) or just provide the image
```
**Exercise:** Use `strings` and `grep` to find the flag. Then use `foremost` to recover the file.

### 2.2 Memory Dump Analysis (Volatility Framework)
Install Volatility3 (`pip install volatility3`). Download a sample memory dump (e.g., from a CTF like “memlapse” or publicly available test dumps).

Commands to know:
- `vol -f mem.raw windows.info` or `linux.info`
- `vol -f mem.raw linux.pslist` — list processes.
- `vol -f mem.raw linux.filescan` — find open file handles.
- `vol -f mem.raw linux.dumpfile --pid <PID> --virtaddr <addr>` — dump a file from memory.

**Exercise:** In a Linux memory dump, find the bash process, dump its command history, and extract a flag that was passed as an environment variable.

**CTF connection:** Memory forensics is often the only way to retrieve encryption keys, injected shellcode, or hidden malware from a live system.

### 2.3 Network Traffic Forensics
Revisit the pcap from Week 11. Use Wireshark to follow TCP streams, extract objects (HTTP files), and identify suspicious patterns. Use `tcpflow` or `foremost` on the pcap.

**Hands‑on:** Given a pcap, extract a ZIP file transferred over HTTP, then crack its password using the dictionary from Phase 1.

---

## 🔐 PART 3: CRYPTOGRAPHY REFRESHER & REAL‑WORLD CRYPTO (1 HOUR)

You’ve done Caesar and XOR. Now we touch **modern symmetric encryption modes** and **padding oracle attacks** — a classic CTF crypto vulnerability.

### 3.1 XOR and One‑Time Pads
Quick recall: XOR with a repeating key is breakable via frequency analysis (Vigenère on bytes). Write a Python function `break_repeating_key_xor(ciphertext)` using Hamming distance to guess key size, then single‑byte XOR on each transposed block.

### 3.2 AES Modes – ECB vs CBC
- **ECB (Electronic Codebook):** identical plaintext blocks produce identical ciphertext blocks. You can detect patterns (e.g., Tux penguin image encrypted with ECB still shows outlines). In CTFs, ECB allows block‑swapping and byte‑by‑te‑byte decryption through oracle attacks.
- **CBC (Cipher Block Chaining):** more secure, but vulnerable to **padding oracle** if error messages differ.

### 3.3 Padding Oracle Attack (Concept)
When a server decrypts CBC‑encrypted data and returns a padding error versus a generic error, you can decrypt the whole ciphertext without the key. This is a classic crypto challenge.

**Hands‑on:** Use CryptoHack’s “Padding Oracle” challenge or set up a local server with Python’s `cryptography` library. Write a script that performs the attack byte by byte. We’ll do a simplified walkthrough:

1. Given an IV and ciphertext, you control a modified ciphertext sent to the server.
2. The server decrypts and checks PKCS#7 padding; if invalid, it tells you “bad padding”.
3. You brute‑force the last byte of the intermediate state until padding becomes valid (0x01), then move backwards.

**Exercise:** Solve the “Padding Oracle” challenge from CryptoHack.

### 3.4 RSA Basics (Optional Glimpse)
- Public key (n,e), private key (d). Encryption: `c = m^e mod n`.
- Common CTF pitfalls: small e (3) with short messages, common modulus, Wiener’s attack on small d, etc.
- We won’t dive deep, but introduce the concepts for Phase 2 crypto specialisation.

**CTF connection:** Crypto challenges range from simple rot13 to complex lattice attacks. This refresher puts you in the game.

---

## 💻 PART 4: SHELL FINAL POLISH – READLINE, HISTORY, COMPLETION (1.5 HOURS)

Your shell is functional. Now we make it a **pleasure to use**, adding GNU Readline support for line editing, history, and tab completion.

### 4.1 Integrating Readline
Replace your `fgets`/`getline` with `readline`:
```c
#include <readline/readline.h>
#include <readline/history.h>

char *line = readline("mysh $ ");
if (!line) break; // EOF
if (*line) {
    add_history(line);  // automatically saved/loaded if you want
}
execute(line);
free(line);
```
Compile with `-lreadline`.

**Benefits:**
- Line editing (left/right, delete).
- Persistent history if you set `history_file` and call `read_history`/`write_history`.
- Tab completion via custom completion function.

### 4.2 Custom Tab Completion
Write a completion generator that suggests builtin names and filenames. This uses the GNU Readline callback mechanism.

Simplified:
```c
char *completion_generator(const char *text, int state) {
    static int i, len;
    static char **list;
    if (!state) {
        i = 0;
        len = strlen(text);
        // Build list of possible completions (builtins + binaries in PATH)
        list = build_completion_list(text);
    }
    while (list[i]) {
        if (strncmp(list[i], text, len) == 0)
            return strdup(list[i]);
        i++;
    }
    return NULL; // no more matches
}
```
Register it with `rl_attempted_completion_function = custom_completion;`.

**Exercise:** Implement completion for filenames only, using `opendir`/`readdir`. Then add builtin completion.

### 4.3 Environment and Prompt
Make the prompt dynamic: `[user@host cwd]$`. Use `getenv`, `getcwd`, `gethostname`.

### 4.4 Startup Script and Aliases
Your `~/.mysh_profile` can now define functions and aliases. Implement simple alias substitution: `alias ll='ls -l'`.

**Exercise:** Add a builtin `alias` that manages a dictionary of aliases and expands them before parsing.

**CTF connection:** This fully‑featured shell becomes your post‑exploitation agent. You can add builtins like `download`, `upload`, `exec` for easy remote control.

---

## 📦 PART 5: GIT CLEANUP, DOCUMENTATION & REPO RELEASE (0.5 HOUR)

You’ve built a mountain of code. Now package it for the world (or your future self).

### 5.1 Repository Structure
```
~/myshell/          – your Unix shell
~/ctf_tools/        – Python library
~/mini_ctf/         – challenges and exploits
~/writeups/         – weekly reflections
```

### 5.2 Writing a Good README
Each project gets a `README.md`:
- What it does.
- How to build and run.
- Examples.
- License (MIT).

### 5.3 Creating a Release
Commit all final changes, tag a version (`v1.0`), and push to GitHub. This builds a public portfolio.

### 5.4 Write‑Up Collection
Collect your weekly reflection files and problem set solutions into a single document. This is your personal CTF learning journal.

---

## 🗺️ PART 6: PHASE 2 ROADMAP & NEXT STEPS (0.5 HOUR)

You’ve completed Phase 1 — the bedrock. Over the 12 weeks, you’ve gone from “Hello World” to building a shell, writing exploits, reversing binaries, and crafting network packets. This is equivalent to the first 1–2 years of a top CS degree focused on security.

### 6.1 What Awaits in Phase 2
- **CMU 15‑213 (ICS) full labs:** Attack Lab (deeper ROP), Malloc Lab (implement a real allocator, exploit it), Proxy Lab.
- **MIT 6.828 (xv6):** Implement system calls, copy‑on‑write fork, file systems, network drivers. Exploit your kernel modules.
- **Stanford CS144:** Build a complete TCP stack in C++. Real understanding of sequence numbers, retransmission, congestion control.
- **Applied Security Courses:** Stanford CS155, MIT 6.858, CMU 18‑330 — deep dives into web, network, system, and advanced binary exploitation.
- **Specialization:** Choose your track — pwn, web, RE, crypto.

### 6.2 Immediate Next Steps
- Set up the CMU ICS environment (download the labs from CS:APP website).
- Start reading CS:APP Ch 3 thoroughly — you already have the assembly background.
- Begin the Bomb Lab Phase 2 if you haven’t. Use your improved GDB scripting.

**You are ready.** The skills you’ve built aren’t fragile; they’re hammered into muscle memory through hundreds of hours of deliberate practice.

---

## 📝 WEEK 12 PROBLEM SET (Daily 2‑4 Hours)

### PS12‑1: Complete the Mini‑CTF
- Exploit all three challenges (stack, format, heap) and capture the flags.
- Write the patches and verify them.
- Write a **security advisory** report (2‑3 pages) describing the vulnerabilities, exploitation steps, and patches. Include code snippets.

### PS12‑2: Forensics Deep Dive
- Solve the “Shark on Wire” series (1 and 2) from picoCTF if you haven’t.
- Download a memory dump from a public CTF (e.g., “memlapse” from picoCTF) and use Volatility to find the flag.
- Create your own forensics challenge: hide a flag in a PNG file using `echo "CTF{stego}" >> image.png`, then recover it with `strings`.

### PS12‑3: Crypto Challenge – Padding Oracle
- Complete the CryptoHack “Padding Oracle” challenge (under Symmetric Ciphers).
- Write your own padding oracle server in Python (using Flask or just TCP) and attack it.

### PS12‑4: Shell Final Touches
- Integrate Readline, tab completion, history file, and dynamic prompt.
- Add `alias` and `unalias` builtins.
- Write a `~/.mysh_profile` that sets up useful aliases.
- Push your final shell to GitHub with a polished README.

### PS12‑5: pwnable.kr “random”, “input2”, “leg”
Continue the pwnable.kr journey. “random” is a classic, “input2” deepens the multi‑vector input challenge, “leg” involves ARM assembly. Use everything you’ve learned.

### PS12‑6: Start CS:APP Chapter 3 Deep Read
Spend 2 hours reading CS:APP Chapter 3 (Machine‑Level Representation) up to 3.7. You’ll find it much easier now with your assembly and GDB background.

### PS12‑7: Final Retrospective
Write a comprehensive `week12_final_reflection.md` covering:
1. What are the top 5 skills you gained over these 12 weeks?
2. Which week was the hardest? Why?
3. Describe a bug you spent hours on and how you solved it.
4. How has your mental model of computers changed?
5. What are you most excited to learn in Phase 2?
6. Advice for your past self starting Week 1.

---

## 🏁 THE END OF THE BEGINNING

You’ve completed an extraordinary journey. Twelve weeks ago, you were wondering how to set up a CLI. Now you write shellcode, reverse engineer binaries, forge packets, and automate exploits. You’ve built a custom shell, a CTF toolkit, and a full capstone competition. You’ve absorbed the curriculum of the world’s best universities and made it your own.

Phase 2 will take you even deeper — into kernel internals, advanced ROP, hardware attacks, and competitive CTF dominance. But for now, breathe. Take a snapshot of your VM. Save everything. You’ve earned a moment of pride.

The machine will be waiting when you’re ready. **Go break something—and then fix it.**
