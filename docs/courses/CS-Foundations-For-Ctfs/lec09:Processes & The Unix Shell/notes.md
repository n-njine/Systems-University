The heap and format strings taught you to corrupt memory. Now you’ll learn to **control the system itself**. This week we dive into the Unix process model, file descriptors, pipes, and signals. Then you’ll pour everything into building a working Unix shell from scratch. Mastering this layer unlocks privilege escalation, command injection, and the deep understanding of how a compromised binary interacts with the kernel. This is the bridge between binary exploitation and full system control.

# 🔥 WEEK 9: SUPER INTENSIVE DEEP DIVE  
## *"Processes & The Unix Shell"*

---

## ⏳ Session Overview (8‑Hour Immersive Block)

| Time (total) | Section |
|--------------|---------|
| 0:00 – 1:30 | Unix Processes: `fork`, `exec`, `wait` |
| 1:30 – 3:00 | File Descriptors & Redirection |
| 3:00 – 4:00 | Pipes & Inter‑Process Communication |
| 4:00 – 5:00 | Signal Handling |
| 5:00 – 7:00 | Mini‑Shell Project: Tokenizer & Execution |
| 7:00 – 8:00 | Problem Set Kickoff & Reflection |

---

## 🧠 PART 1: UNIX PROCESSES (1.5 HOURS)

Everything you run on Linux is a process. Exploits spawn reverse shells, escalate privileges, and interact with the kernel through the **process model**. You must understand `fork`/`exec`/`wait` like a second language.

### 1.1 Process Creation with `fork()`
- `pid_t fork(void)` creates a child process that is an exact copy of the parent (code, stack, file descriptors).
- Returns **0** to child, **child’s PID** to parent, or **‑1** on error.
- After fork, both run concurrently. Whichever gets scheduled first is non‑deterministic.

```c
#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        printf("I am child, PID = %d\n", getpid());
    } else {
        printf("I am parent, child PID = %d, my PID = %d\n", pid, getpid());
    }
    return 0;
}
```

**Key observation:** Both print from the same program memory—yet they execute different branches. This is the foundation of how a shell runs external commands.

### 1.2 `execve()` – Replace the Process Image
`int execve(const char *pathname, char *const argv[], char *const envp[])` loads a new program into the current process memory. It does **not** create a new process; the old one is replaced.

Typical pattern in a shell: **fork then exec**—child calls `exec`, parent waits.

```c
char *args[] = {"/bin/ls", "-l", NULL};
execve("/bin/ls", args, NULL);
perror("execve failed"); // only reached if error
```

**Exercise:** Write a program that forks a child, child does `execve("/bin/ls", ...)`, parent prints "Waiting" then calls `wait`.

### 1.3 `wait()` and Zombie/Oprhan Processes
- `pid_t wait(int *wstatus)` blocks until a child terminates. Stores exit status.
- If parent doesn’t `wait`, the child becomes a **zombie** (defunct) until parent terminates (then reparented to init).
- If parent exits before child, the child becomes an **orphan**, adopted by init (PID 1).

```c
int status;
pid_t wpid = wait(&status);
if (WIFEXITED(status))
    printf("Child exited with %d\n", WEXITSTATUS(status));
else if (WIFSIGNALED(status))
    printf("Child killed by signal %d\n", WTERMSIG(status));
```
These macros from `<sys/wait.h>` decode the status.

**CTF connection:** In security challenges, you may encounter SUID binaries that fork and execute without properly dropping privileges. Or race conditions where a child can modify state before the parent checks. Zombie processes can be used for resource exhaustion.

### 1.4 Process IDs and Parent Relationships
- `getpid()`, `getppid()`.
- Understanding process tree is important in forensic analysis of malware.

**Code‑along:** Write a loop that forks multiple children and waits for all, printing when each terminates.

---

## 📁 PART 2: FILE DESCRIPTORS & REDIRECTION (1.5 HOURS)

Everything is a file. Your shell must be able to redirect input/output of commands to files. This is done by manipulating **file descriptors** before `exec`.

### 2.1 The FD Table
Each process has a table of file descriptors. 0 = stdin, 1 = stdout, 2 = stderr. When you `open()`, the kernel returns the lowest unused FD.

### 2.2 System Calls: `open()`, `close()`, `read()`, `write()`
```c
int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
if (fd < 0) { perror("open"); exit(1); }
write(fd, "hello\n", 6);
close(fd);
```

### 2.3 `dup2()` for Redirection
`dup2(oldfd, newfd)` makes `newfd` point to the same file structure as `oldfd`, closing `newfd` first if open. Then we can redirect stdout to a file:

```c
int fd = open("output.txt", O_WRONLY|O_CREAT|O_TRUNC, 0644);
dup2(fd, STDOUT_FILENO);
close(fd);  // fd no longer needed
execlp("ls", "ls", NULL);  // output goes to file
```
In the child process, after `fork`, we redirect then `exec`.

**Exercise:** Write a program that implements `> file` redirection: takes filename as argument, opens it, `dup2` to stdout, then executes `echo hello`.

### 2.4 Input Redirection (`<`)
Similar, but `dup2(fd, STDIN_FILENO)` before executing command.

### 2.5 Here‑doc and Pipes (Preview)
Even advanced shell features are built on `pipe()` and descriptor manipulation.

**CTF connection:** Many SUID binaries open files with elevated privileges. If they don’t properly close descriptors or if an attacker can manipulate the fd table (e.g., through `close + dup2` tricks), they can read/write sensitive files. This is the basis of **file descriptor attacks** and **privilege escalation**.

**Mini‑project:** In the session, start a `redir.c` that parses a simple command line and performs redirection.

---

## 🔗 PART 3: PIPES & INTER‑PROCESS COMMUNICATION (1 HOUR)

Pipes connect the stdout of one process to the stdin of another, the heart of Unix pipelines.

### 3.1 `pipe(int fd[2])`
- Creates a unidirectional byte stream. `fd[0]` is read end, `fd[1]` is write end.
- Normally used between parent and child, or between two children.

### 3.2 Implementing `ls | wc -l`
The shell must:
1. Create a pipe.
2. Fork first child: redirect stdout to pipe write end, close read end, exec `ls`.
3. Fork second child: redirect stdin to pipe read end, close write end, exec `wc -l`.
4. Parent closes both ends and waits for both children.

```c
int pipefd[2];
pipe(pipefd);          // create pipe

pid_t pid1 = fork();
if (pid1 == 0) {
    // child1: ls
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[0]); close(pipefd[1]);
    execlp("ls", "ls", NULL);
    perror("exec ls"); exit(1);
}

pid_t pid2 = fork();
if (pid2 == 0) {
    // child2: wc -l
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]); close(pipefd[1]);
    execlp("wc", "wc", "-l", NULL);
    perror("exec wc"); exit(1);
}

close(pipefd[0]); close(pipefd[1]);
wait(NULL); wait(NULL);
```

**Key pitfall:** Always close unused pipe ends in each process, otherwise reader will hang waiting for end‑of‑file.

**Exercise:** Write a program that pipes `ps aux | grep root`.

### 3.3 Multiple Pipes (Future)
Generalization for longer pipelines requires careful descriptor management; we’ll implement in the shell project.

**CTF connection:** In a jail escape or restricted shell, you might need to create a pipe between two commands to bypass restrictions. Also, understanding how data flows through descriptors helps in crafting command injection payloads.

---

## ⚡ PART 4: SIGNAL HANDLING (1 HOUR)

Signals are software interrupts. Your shell must handle `SIGINT` (Ctrl+C) without dying, and manage foreground/background job control.

### 4.1 Common Signals
- `SIGINT` (2): Interrupt from keyboard.
- `SIGCHLD` (17): Child status change.
- `SIGTERM` (15): Termination request.
- `SIGKILL` (9): Cannot be caught/ignored.

### 4.2 Signal Handlers using `sigaction()`
Better than `signal()` due to portability and control.

```c
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void sigint_handler(int sig) {
    write(STDOUT_FILENO, "\nCaught SIGINT!\n", 16);
}

int main() {
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    printf("Press Ctrl+C...\n");
    while (1) pause();  // wait for signal
    return 0;
}
```
Important: only use **async‑signal‑safe** functions in a handler (e.g., `write`, not `printf`). In our shell, the handler might just set a flag.

### 4.3 Ignoring and Blocking Signals
- `signal(SIGINT, SIG_IGN)` prevents Ctrl+C from killing a process.
- Blocking signals with `sigprocmask()` can protect critical sections.

**Shell design:** The shell itself should ignore `SIGINT`, but foreground child processes should receive it normally. The shell can manage this by setting signal dispositions after fork/exec.

### 4.4 Zombie Reaping with `SIGCHLD`
Instead of blocking `wait`, a shell can set a `SIGCHLD` handler that reaps children in the background. This is advanced but powerful.

**CTF connection:** Signal handling can be buggy. Race conditions between a signal and main code can cause double‑free, use‑after‑free, or bypass of security checks. Also, some exploits use `SIGABRT` to trigger a core dump with sensitive data.

---

## 🖥️ PART 5: MINI‑SHELL PROJECT – ARCHITECTURE & EXECUTION (2 HOURS)

Now you build a minimal but functional Unix shell. This is the capstone of the session, combining everything you’ve learned.

### 5.1 Core Loop
```c
while (1) {
    print_prompt();
    char *line = read_input();
    if (line == NULL) break;  // EOF
    if (strlen(line) == 0) continue;
    execute(line);
    free(line);
}
```

### 5.2 Reading Input
Use `getline()` (GNU) or `fgets`. `getline` dynamically allocates, so you must free.

```c
char *line = NULL;
size_t len = 0;
ssize_t nread = getline(&line, &len, stdin);
if (nread == -1) exit(0);
line[strcspn(line, "\n")] = 0;  // strip newline
```

### 5.3 Tokenizing the Command Line
We need to split by whitespace, respecting quotes (basic), and extract redirections `>`, `<`, and pipe `|`.

Simplest version: use `strtok()` or custom parser.

```c
char **parse(char *line, int *argc) {
    char **args = malloc(64 * sizeof(char*));
    char *token = strtok(line, " ");
    int i = 0;
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
    *argc = i;
    return args;
}
```
But we need to detect special symbols: `<`, `>`, `|`. So we’ll keep them as separate tokens.

### 5.4 Builtins: `cd`, `exit`, `help`
- `cd` must change the shell’s own working directory via `chdir(path)`.
- `exit` terminates the shell.
- Implement using `strcmp`.

```c
if (strcmp(args[0], "cd") == 0) {
    if (args[1] == NULL) fprintf(stderr, "expected argument to cd\n");
    else if (chdir(args[1]) != 0) perror("cd");
    continue;
}
```

### 5.5 External Command Execution
For simple commands (no pipes), fork and exec.

```c
pid_t pid = fork();
if (pid == 0) {
    execvp(args[0], args);   // search PATH
    perror("execvp");
    exit(EXIT_FAILURE);
} else if (pid > 0) {
    wait(NULL);
} else {
    perror("fork");
}
```
`execvp` uses `PATH` automatically, so users don’t need full paths.

### 5.6 Redirection Implementation
Scan tokens for `<` and `>`. Save the filename. Remove those tokens from args before passing to exec. Then in child, before `exec`:
```c
if (input_file) { 
    int fd = open(input_file, O_RDONLY);
    dup2(fd, STDIN_FILENO); close(fd);
}
if (output_file) {
    int fd = open(output_file, O_WRONLY|O_CREAT|O_TRUNC, 0644);
    dup2(fd, STDOUT_FILENO); close(fd);
}
```

**Exercise:** Implement a simple `>` redirection in your shell during the session.

### 5.7 Pipes for a Single Pipe
Detect `|` token. Split the command into left and right parts. Create pipe, fork left, fork right, as shown earlier. Must carefully close fds.

### 5.8 Signal Handling in the Shell
- Shell ignores `SIGINT`, but child processes should have default (Ctrl+C kills child).
- We can achieve this by resetting signal dispositions in child before exec (or rely on default after fork).

```c
signal(SIGINT, SIG_IGN);   // shell ignores
// In child, before exec:
signal(SIGINT, SIG_DFL);
```

### 5.9 Testing & Iteration
In the remaining time, build step by step. Start with a working `execute` that handles simple commands, then add builtins, then redirection, then pipes. Test after each addition.

**CTF connection:** This shell becomes your post‑exploitation tool. You can customize it to automatically exfiltrate flags, avoid detection, or execute multi‑stage payloads. Understanding how a shell works internally also helps you break out of restricted shells (jail escapes) and command injection.

---

## 📝 WEEK 9 PROBLEM SET (Daily 2‑4 Hours)

### PS9‑1: Complete the Mini Shell
Add these features:
- **Redirection** `<`, `>`, `>>` (append).
- **Single pipe** `|`. Bonus: support multiple pipes.
- **Background execution** (`&`): If command ends with `&`, don’t `wait` in parent; print child PID.
- **`jobs` builtin**: track background processes and list them.
- **Signal handling**: Ensure Ctrl+C kills only the foreground child, not the shell. Shell should reclaim children periodically (or via `SIGCHLD`).

### PS9‑2: MIT 6.828 xv6 Book – Chapter 0 (Operating System Interfaces)
Read Chapter 0 of the xv6 commentary (available on [pdos.csail.mit.edu](https://pdos.csail.mit.edu/6.828/)). It explains processes, system calls, I/O, and file descriptors exactly matching this week’s material. Absorb it.

### PS9‑3: OverTheWire Bandit 25‑27
Continue the Bandit journey. Document each. Level 26 is a tricky “setuid” and restricted shell; your new knowledge should help.

### PS9‑4: Write a Simple `cronjob` Exploit (Simulation)
Create a SUID binary that uses `system()` to run a cleanup script with a path to a file. Then exploit it by manipulating `PATH` or using symlinks to escalate to root. This brings together `fork`/`exec`, privileges, and command injection.

### PS9‑5: picoCTF – General Skills & Forensics
Spend some time on picoGym expanding your breadth: solve 5 challenges in “General Skills” or “Forensics” (e.g., “droids0”, “whitePages”). Keep your Python sharp.

### PS9‑6: Write a shellcode that uses `execve`
Using assembly (manual or via pwntools), write a minimal shellcode that calls `execve("/bin/sh", NULL, NULL)` without any null bytes. Explain each instruction. This will connect your assembly knowledge (Week 7) with the `exec` system call.

**Tip:** Syscall number for `execve` on x86‑64 is 59 (`0x3b`). Use `pwntools` `shellcraft.execve("/bin/sh")` to generate, then disassemble and study.

### PS9‑7: Week 9 Reflection
In `week9_reflection.md`:
1. What was the trickiest part of building the shell? (Pipes? Signal handling? Tokenization?)
2. How does understanding `fork/exec` change your view of what happens when you run a program?
3. Describe a security vulnerability that could arise from incorrectly managing file descriptors or signals in a SUID binary.
4. How do you think the shell will evolve into your own CTF command‑and‑control tool?

---

## 🔥 DEEP DIVE WRAP‑UP

You now own the process. You can spawn children, wire them together with pipes, redirect their I/O, and respond to signals. You’ve built a shell from scratch—one of the most empowering projects of any CS curriculum. This isn’t just a toy; it’s a blueprint for how every command injection exploit works, how post‑exploitation agents run, and how privilege escalation baits are set.

Next week, we’ll enhance the shell with background jobs, environment variables, and deeper signal handling, then move into the OS engineering mindset (xv6). The path from C to OS to kernel exploitation is now clear.

**Open your editor. Write the main loop. Run `./myshell` and type `ls`. The terminal is yours.**
