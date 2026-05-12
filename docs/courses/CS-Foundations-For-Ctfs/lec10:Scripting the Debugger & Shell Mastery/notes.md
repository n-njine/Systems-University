Your shell runs commands. Now it’s time to make it a **real shell**—with job control, environment management, history, and the polish of a professional tool. Then we’ll turn the tables: instead of just **using** a debugger, you’ll learn to **script GDB with Python**, automating the most tedious parts of exploit development. By the end of this week, you’ll have a fully functional custom shell that can serve as your post‑exploitation agent, and the ability to bend GDB to your will with one‑liners.

# 🔥 WEEK 10: SUPER INTENSIVE DEEP DIVE  
## *“Scripting the Debugger & Shell Mastery”*

---

## ⏳ Session Overview (8‑Hour Immersive Block)

| Time (total) | Section |
|--------------|---------|
| 0:00 – 1:30 | Shell Enhancement I: Environment, PATH, Builtins |
| 1:30 – 2:30 | Shell Enhancement II: Background Jobs & `&` |
| 2:30 – 3:00 | Job Control & Reaping Zombies (SIGCHLD) |
| 3:00 – 3:30 | Environment & Configuration Files |
| 3:30 – 5:00 | GDB Scripting with Python: Automating Analysis |
| 5:00 – 6:30 | Reverse Engineering Deep Dive: Keygen Challenge |
| 6:30 – 7:00 | Git Basics & Project Management |
| 7:00 – 8:00 | Problem Set Kickoff, Live Debugging & Reflection |

---

## 🧠 PART 1: SHELL ENHANCEMENT I – ENVIRONMENT, PATH, BUILTINS (1.5 HOURS)

Your shell can execute commands and do simple pipes. Now we add the features that make it feel like a true Unix shell.

### 1.1 Environment Variables
Each process inherits an **environment** — an array of `"KEY=VALUE"` strings. The shell must manage its own environment and pass it to children via `execve()` (or use `execvpe`).

- Read the current environment with the global `extern char **environ`.
- Functions: `getenv()`, `setenv()`, `unsetenv()`, `putenv()`.

**Add `export` and `unset` builtins:**
```c
if (strcmp(args[0], "export") == 0) {
    if (args[1]) {
        char *eq = strchr(args[1], '=');
        if (eq) {
            *eq = '\0';
            setenv(args[1], eq+1, 1);
        } else {
            // export existing variable? Just ignore or set to empty
            setenv(args[1], "", 1);
        }
    } else {
        // print all env vars
        for (char **env = environ; *env; env++)
            printf("%s\n", *env);
    }
    return 0;
}
```

**CTF connection:** Many privilege‑escalation exploits manipulate `PATH` or `LD_PRELOAD` to trick SUID binaries into loading malicious libraries. Your shell can be used to craft these environments precisely.

### 1.2 Custom `PATH` Resolution
Your shell should find executables by searching the directories listed in `PATH`. Write a function `resolve_path(cmd)` that returns a full path if found, or NULL.

```c
char *path_search(const char *cmd) {
    if (strchr(cmd, '/')) { // already path
        if (access(cmd, X_OK) == 0) return strdup(cmd);
        return NULL;
    }
    char *path = getenv("PATH");
    if (!path) return NULL;
    char *dup = strdup(path);
    char *dir = strtok(dup, ":");
    while (dir) {
        char full[1024];
        snprintf(full, sizeof(full), "%s/%s", dir, cmd);
        if (access(full, X_OK) == 0) {
            free(dup);
            return strdup(full);
        }
        dir = strtok(NULL, ":");
    }
    free(dup);
    return NULL;
}
```
Then use `execve(resolved_path, args, environ)` instead of `execvp`. This gives you control over exactly what gets executed—crucial for security.

### 1.3 Tilde Expansion and `~`
Before parsing, expand `~` to the home directory (`getenv("HOME")`). `~user` to that user’s home (`getpwnam`).

### 1.4 Builtins: `history`
Keep a list of previously entered commands. You can store them in a fixed‑size ring buffer or linked list. Add a `history` command to print them. For arrow‑key support, we’ll eventually link with `libreadline`; for now, a simple file `~/.mysh_history` will do.

**Exercise:** Add `save_history()` and `load_history()` calls.

---

## ⚙️ PART 2: SHELL ENHANCEMENT II – BACKGROUND JOBS & `&` (1 HOUR)

Real shells allow commands to run in the background while you continue working. This is what `&` signifies.

### 2.1 Detecting `&`
If the last token of the command line is `&`, we remove it and set a flag `background = 1`.

### 2.2 Running in Background
In the parent process, if `background == 1`, do **not** `wait()` for the child. Instead, print the child’s PID and add it to a **job list**.

```c
typedef struct Job {
    pid_t pid;
    char *command;
    struct Job *next;
} Job;
Job *jobs = NULL;
```

When we spawn a background child:
```c
pid_t pid = fork();
if (pid == 0) {
    // child setup and exec
} else {
    if (background) {
        add_job(&jobs, pid, line); // copy line
        printf("[%d] %d\n", job_id, pid);
        // do NOT wait
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
}
```

### 2.3 Listing Jobs: `jobs` Builtin
Iterate the list and print their PIDs, status, and command.

### 2.4 Bringing Jobs to Foreground: `fg`
`fg %d` needs to:
- Find the job by id/pid.
- Use `tcsetpgrp()` to give it terminal control (advanced); for now, just `waitpid(pid, &status, 0)` to wait for it.

**Exercise:** Implement `fg` by waiting for the specified PID.

**CTF connection:** In a reverse shell scenario, you might need to run a long‑running scan or brute‑forcer in the background while keeping an interactive session alive. Your shell’s job control gives you that.

---

## 🧟 PART 3: JOB CONTROL & REAPING ZOMBIES (SIGCHLD) (0.5 HOUR)

When a background child finishes, it becomes a zombie until the parent `wait()`s. We can avoid zombies by handling `SIGCHLD` asynchronously.

### 3.1 SIGCHLD Handler
```c
void sigchld_handler(int sig) {
    int saved_errno = errno;
    pid_t pid;
    int status;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        // find job in list and remove/update
        remove_job(pid);
    }
    errno = saved_errno;
}
```
Set this handler early in `main` (shell). This reaps children automatically, so no zombies accumulate.

### 3.2 Updating the Job Table
In the handler, mark the job as done, and optionally print a message `[1] Done    ls -lR`.

**Caution:** Signal handlers must be minimal and async‑safe. We can’t call `printf` safely, so we just set a flag and let the main loop print messages.

**Exercise:** Integrate this into your shell and test with short background commands.

---

## 🏠 PART 4: ENVIRONMENT & CONFIGURATION FILES (0.5 HOUR)

Every time a shell starts, it reads scripts like `.bashrc`. We’ll mimic that.

### 4.1 Reading a Startup File
On startup, attempt to open `~/.mysh_profile`. Read it line‑by‑line and execute each as a normal command (but without prompting). This allows users to set aliases, export variables, etc.

```c
void source_file(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return;
    char *line = NULL;
    size_t len = 0;
    while (getline(&line, &len, f) != -1) {
        execute(line); // reuse your main command executor
    }
    free(line);
    fclose(f);
}
```

**Exercise:** Add a `source` builtin so users can reload config.

### 4.2 Saving History on Exit
Use `atexit()` or explicit save when `exit` is called.

---

## 🐍 PART 5: GDB SCRIPTING WITH PYTHON – AUTOMATING ANALYSIS (1.5 HOURS)

GDB’s Python API lets you write custom commands that introspect the inferior’s memory and registers, automate breakpoint handling, and build powerful reverse‑engineering assistants.

### 5.1 Enabling Python in GDB
Most modern GDBs have Python. Check with `gdb --configuration`. Create a file `mygdb.py` and source it: `source mygdb.py` in GDB, or put commands in `~/.gdbinit`.

### 5.2 Writing a Simple GDB Command in Python
```python
import gdb

class PrintStackFrame(gdb.Command):
    """Print the stack frame around RSP for a given size."""
    def __init__(self):
        super().__init__("stack_frame", gdb.COMMAND_USER)

    def invoke(self, arg, from_tty):
        size = 64
        try:
            size = int(arg)
        except:
            pass
        rsp = int(gdb.parse_and_eval("$rsp"))
        rip = int(gdb.parse_and_eval("$rip"))
        memory = gdb.selected_inferior().read_memory(rsp, size)
        print(f"   RSP = {rsp:#018x}  RIP = {rip:#018x}")
        for i in range(0, size, 8):
            qword = int.from_bytes(memory[i:i+8], 'little')
            print(f"   [{rsp+i:#018x}] {qword:#018x}")
        # Mark if RIP is inside
PrintStackFrame()
```
**Usage:** `stack_frame 80` prints the top of stack.

**Exercise:** Write a GDB command `pattern_offset` that automates `cyclic`: given a crash value (like in `$rip`), runs `cyclic -l` in Python and prints offset.

### 5.3 Hooking Breakpoints
```python
class BreakpointLogger(gdb.Breakpoint):
    def stop(self):
        print(f"Hit breakpoint at {self.location}")
        # Print registers
        for reg in ['rax','rbx','rcx','rdx','rsi','rdi']:
            val = gdb.parse_and_eval(f"${reg}")
            print(f"   {reg}: {val}")
        return False  # don't stop, continue (or True to stop)
```
Use it to trace function calls without stopping.

### 5.4 Automating Exploit Offset Discovery
With pwntools, you can run GDB headless and parse output. But a simple GDB script can do:
- Load binary.
- Set breakpoint at `ret` of vulnerable function.
- Run with cyclic pattern.
- Read `$rip` and compute offset.
- Print result.

**In‑session:** Write a GDB script that sets a breakpoint on `vulnerable+offset`, runs with cyclic, catches the segfault, reads `$rsp` or `$rip`, and prints the offset. This becomes your “find_offset.py” GDB helper.

### 5.5 Dumping Heap Chunks
Create a command that reads the `main_arena` from libc and walks the bins, printing chunk sizes and flags. This is advanced but I’ll provide a skeleton.

**CTF connection:** In complex heap exploits, you often need to inspect bin state. GDB scripting automates that, turning hours of manual `x/64gx` into a single command.

---

## 🔍 PART 6: REVERSE ENGINEERING DEEP DIVE – KEYGEN CHALLENGE (1.5 HOURS)

We’ll use Ghidra and Python to crack a simple licence‑key verifier and write a keygen.

### 6.1 The Challenge Binary
I’ll describe a binary we can create (or use a public one). The program takes a name and a serial, then checks if the serial matches a custom algorithm based on the name.

Example algorithm: serial = name[0]*2 + name[1]*3 + ... XOR 0x2A. We’ll implement it in C, compile without symbols, and use Ghidra to reverse.

**Live walkthrough:**
1. Load into Ghidra, find `main`, look for `scanf` or `fgets` calls.
2. Track the input into a function that performs the check.
3. The function loops over the name string, accumulates a sum, XORs a constant, and compares to user serial.
4. Identify the constant and the operation. Re‑implement the algorithm in Python to generate valid serials for any name.

**CTF connection:** This mirrors many CTF “keygen” or “reverse” challenges where you must produce a valid input based on reversing a binary.

### 6.2 Writing a Keygen in Python
```python
def keygen(name):
    s = 0
    for i, ch in enumerate(name):
        s += ord(ch) * (i + 2)
    serial = s ^ 0x2A
    return serial
```

**Exercise:** After reversing, write the keygen, test it against the real binary, and capture the flag string if one is printed on success.

### 6.3 Using Unicorn for Emulation (Teaser)
Unicorn is a CPU emulator that can run code snippets. Not today, but mention how you can emulate the check function to brute‑force quickly.

---

## 🧰 PART 7: GIT BASICS & PROJECT MANAGEMENT (0.5 HOUR)

Your shell and tools deserve version control.

### 7.1 Setting Up a Repository
```bash
cd ~/myshell
git init
git config user.name "Your Name"
git config user.email "you@example.com"
```

### 7.2 Basic Workflow
```
git add myshell.c Makefile
git commit -m "Initial commit of my shell"
git log
git diff
```

### 7.3 Branching and Merging
```bash
git checkout -b feature-pipes
# ... make changes
git commit -a -m "Implement multi-pipe support"
git checkout master
git merge feature-pipes
```

### 7.4 Remote Repositories
Create a GitHub repo, push your code:
```bash
git remote add origin https://github.com/username/myshell.git
git push -u origin master
```

**Why this matters:** Your exploits and tools will be stored, versioned, and shareable. Collaborating on CTFs requires Git fluency.

---

## 📝 WEEK 10 PROBLEM SET (Daily 2‑4 Hours)

### PS10‑1: Finish Your Shell with All Features
- **Multi‑pipe** support (e.g., `ls | grep foo | wc -l`). Build an n‑stage pipeline with careful descriptor handling (using arrays of pipe fds).
- **Background execution** `&` with `jobs` and `fg` builtins.
- **Signal handling**: Proper `SIGINT` behavior (shell ignores, child gets default). `SIGCHLD` reaper updates job list and prints notifications.
- **`history`** and `!` (repeat last command, e.g., `!ls`).
- **Redirection** `>`, `<`, `>>`, and `>&` (redirect stderr).
- **File reader loop** (`source` builtin) and startup config.
- **Bonus:** Filename wildcard expansion (`*`) using `glob()`.

### PS10‑2: GDB Automation Scripts
Create a Python GDB script `auto_offset.py` that:
- Takes a vulnerable binary and input causing crash (from argv).
- Automatically finds the offset to return address using the cyclic method.
- Prints the offset and exits.
Test it against the overflow program from Week 7.

Create another GDB command `heap_walk` that, given an address (or using the current arena), prints a summary of the first few free chunks in the small/large bins (using glibc symbols). This deepens your understanding of heap internals.

### PS10‑3: pwnable.kr – “input”
This challenge requires you to send input through multiple channels (argv, stdin, file descriptor, environment, etc.) to pass a series of checks. Use your shell skills and pwntools. Write a detailed write‑up, explaining each stage and the solution script.

### PS10‑4: Reverse Engineering Keygen (Challenge)
I’ll provide a binary (or you can download an easy crackme from crackmes.one). Write a keygen in Python after reversing with Ghidra. Provide a full explanation of the algorithm and your keygen code.

Example: https://crackmes.one/crackme/... (choose one with a serial check).

### PS10‑5: OverTheWire Leviathan Levels 0‑2
The Leviathan wargame begins with a setuid binary and a home directory with restricted permissions. Use your skills to find the password for the next level. Document each step.

### PS10‑6: Read “Advanced Programming in the UNIX Environment” (Stevens)
Read Chapter 9 (Process Relationships) and Chapter 10 (Signals) in *APUE*. This will fill in gaps on process groups, sessions, and terminal handling. Use the knowledge to improve your shell’s job control.

### PS10‑7: Reflection
In `week10_reflection.md`:
1. What was the most satisfying feature you added to your shell? Why?
2. How has GDB scripting changed your debugging/exploit workflow? Give a concrete example.
3. Describe the hardest part of the keygen challenge (e.g., understanding the algorithm, handling signed/unsigned, etc.).
4. Now that you have a fully functional shell, how might you modify it for post‑exploitation tasks (e.g., automatic flag exfiltration)?

---

## 🔥 DEEP DIVE WRAP‑UP

You now possess a **personalized Unix shell** that can run pipelines, manage jobs, and read your startup scripts. It’s the foundation of your future post‑exploitation agents. And you’ve unlocked the power of **GDB Python scripting**, turning the debugger from a manual tool into an automated exploit‑assist robot. You can reverse any binary, write keygens, and handle processes like a kernel hacker.

Next week, we begin **networking**—sockets, packet crafting, and your first network‑based CTF. Then we’ll dive into the OS itself, building your own kernel modules and exploring privilege escalation at the system‑call level.

**Type `make` in your shell directory. Watch it build. Then run `./myshell` and feel the power. The machine is yours.**
