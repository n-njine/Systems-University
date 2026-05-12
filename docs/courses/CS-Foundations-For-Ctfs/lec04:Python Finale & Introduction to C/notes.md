We’re closing the Python chapter with a bang: advanced features, network programming, and a full‑blown multi‑client chat server. Then we switch gears—hard—and dive into C, the language that will bring you closer to the machine. By the end of this week, you’ll be writing TCP servers, crafting raw HTTP requests, and writing compilable C programs. The bridge between high‑level scripting and bare‑metal exploitation begins here.

# 🔥 WEEK 4: INTENSIVE DEEP DIVE
## *“Python Finale & Introduction to C”*

---

## ⏳ Session Overview (8‑Hour Immersive Block)

| Time (total) | Section |
|--------------|---------|
| 0:00 – 1:00 | Advanced Python: Lambdas, Args, Functional Tools |
| 1:00 – 2:30 | Networking with Sockets |
| 2:30 – 4:30 | Final Python Project: Multi‑Client Chat Server |
| 4:30 – 6:30 | Introduction to C: The Language |
| 6:30 – 7:30 | Functions & Scope in C |
| 7:30 – 8:00 | Problem Set Kickoff & Reflection |

---

## 🐍 PART 1: ADVANCED PYTHON FEATURES (1 HOUR)

You’ve mastered functions, loops, and OOP. Now let’s explore the Python features that make your code concise, expressive, and CTF‑ready. These are the tools you’ll use to write one‑liner decoders, fuzzers, and protocol parsers.

### 1.1 Lambda Functions
A lambda is an anonymous function defined in a single expression. No `def`, no name.

```python
add = lambda x, y: x + y
print(add(3, 5))   # 8
```

Useful when you need a simple function for a short time, like a key for sorting:
```python
pairs = [(1, 'b'), (2, 'a'), (3, 'c')]
pairs.sort(key=lambda p: p[1])   # sort by second element
```

**CTF connection:** In exploit scripts, you often need a quick converter (e.g., pack integer to little‑endian bytes). While `struct.pack` exists, lambdas let you write ad‑hoc helpers without cluttering the namespace.

**Exercise:** Write a lambda that takes a hex string and returns its decimal value. (Hint: `lambda s: int(s, 16)`).

### 1.2 `map`, `filter`, `reduce`
These functional tools process iterables with functions.

- **`map(func, iterable)`:** apply func to every item, returns an iterator.
  ```python
  nums = [1, 2, 3, 4]
  squared = list(map(lambda x: x**2, nums))   # [1,4,9,16]
  ```
- **`filter(func, iterable)`:** keep items where func returns True.
  ```python
  evens = list(filter(lambda x: x % 2 == 0, nums))  # [2,4]
  ```
- **`reduce(func, iterable)`:** cumulatively apply func, returning a single value (from `functools`).
  ```python
  from functools import reduce
  product = reduce(lambda a, b: a * b, nums)   # 24
  ```

**Pythonic alternative:** list comprehensions are often more readable. Compare:
```python
# map+filter:
list(map(lambda x: x**2, filter(lambda x: x%2==0, range(10))))
# vs list comprehension:
[x**2 for x in range(10) if x % 2 == 0]
```
In CTF scripting, use what’s clearest; but knowing both ensures you can read other people’s code.

### 1.3 `*args` and `**kwargs`
Functions can accept arbitrary numbers of arguments.

- **`*args`** gathers extra positional arguments into a tuple.
- **`**kwargs`** gathers extra keyword arguments into a dictionary.

```python
def build_url(base, *paths, **params):
    url = base.rstrip('/') + '/' + '/'.join(paths)
    if params:
        query = '&'.join(f"{k}={v}" for k,v in params.items())
        url += '?' + query
    return url

print(build_url("http://ctf.local", "api", "v1", "flag", user="admin", token="secret"))
# http://ctf.local/api/v1/flag?user=admin&token=secret
```

**Exercise:** Write a function `log_event(message, **details)` that prints a timestamp, the message, and any extra key‑value pairs formatted nicely. Use `datetime.datetime.now()`.

### 1.4 Unpacking and Argument Forwarding
You can unpack sequences/dicts into function arguments:
```python
def send_packet(dst, src, data):
    print(f"Sending from {src} to {dst}: {data}")

args = ("10.0.0.2", "10.0.0.1", b"\xde\xad\xbe\xef")
send_packet(*args)
```
This is crucial when writing wrappers that forward parameters to lower‑level libraries.

### 1.5 Decorators (Quick Preview)
A decorator is a function that wraps another function to modify its behavior. You’ll use them heavily later for timing, logging, and resource management. For now, just understand the syntax:
```python
def debug(func):
    def wrapper(*args, **kwargs):
        print(f"Calling {func.__name__} with args={args}, kwargs={kwargs}")
        result = func(*args, **kwargs)
        print(f"Returned {result}")
        return result
    return wrapper

@debug
def add(a, b):
    return a + b
```
You don’t need to write complex decorators yet, but be aware they exist.

**Wrap‑up:** These features will appear in the problem set and later in your exploit toolkits. Spend the next 5 minutes rewriting one of your previous functions (like Caesar cipher) using `map` and a lambda, just to feel the shift.

---

## 🌐 PART 2: NETWORKING WITH SOCKETS (1.5 HOURS)

Networking is the lifeblood of CTFs. You’ll connect to remote services, send crafted packets, and intercept traffic. Today we build the foundational socket programming skills.

### 2.1 The TCP Handshake (Review)
Before coding, recall: a TCP client creates a socket, calls `connect()` to the server (IP:port), then `send()` and `recv()` data. The server calls `bind()`, `listen()`, `accept()`, and then communicates.

### 2.2 A Simple TCP Client
Write a Python script to connect to a web server and manually send an HTTP GET request.

```python
import socket

HOST = 'example.com'  # change to a known server or localhost
PORT = 80

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    request = "GET / HTTP/1.0\r\nHost: example.com\r\n\r\n"
    s.sendall(request.encode())
    response = b''
    while True:
        chunk = s.recv(4096)
        if not chunk:
            break
        response += chunk
print(response.decode())
```

**Run this.** Inspect the raw HTTP headers. This is how your browser communicates. Later, you’ll use this to talk to CTF flag servers.

**Exercise:** Modify the client to send a request to `httpbin.org` port 80, path `/get?flag=test`. Observe the JSON response. Parse it to extract the `flag` parameter.

### 2.3 Building a Port Scanner
Write a script that scans ports 1–1024 on `127.0.0.1` to find open ones. Use a for loop, try to connect on each port, and if successful, print the port number. Catch connection refused errors with `socket.error`.

```python
import socket

def scan_port(host, port):
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.settimeout(0.5)   # don't wait too long
            s.connect((host, port))
            return True
    except:
        return False

host = '127.0.0.1'
for port in range(1, 1025):
    if scan_port(host, port):
        print(f"Port {port} OPEN")
```

**CTF context:** In network reconnaissance, you’ll need to identify open services quickly. This scanner is slow (iterative), but you’ll later add threading or `select` to make it faster.

**Enhancement (time permitting):** Use the `socket.setdefaulttimeout()` to avoid setting per socket.

### 2.4 Non‑Blocking I/O with `select` (Intro)
Blocking `recv()` can stall your program if the server doesn’t respond. `select.select()` lets you monitor multiple sockets and wait until one is ready to read or write.

```python
import select, socket

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.setblocking(False)
try:
    sock.connect(('example.com', 80))
except BlockingIOError:
    pass  # connection in progress

while True:
    ready, _, _ = select.select([sock], [], [], 2.0)
    if ready:
        # can send or recv now
        break
```
You’ll use this heavily in the chat server. For now, understand that `select` prevents busy‑waiting and allows handling multiple clients.

---

## 💬 PART 3: FINAL PYTHON PROJECT – MULTI‑CLIENT CHAT SERVER (2 HOURS)

This is the capstone of your Python journey. You will build a **multi‑client TCP chat server** using `select`. This project combines sockets, concurrency, OOP, error handling, and protocol design—all skills you need for CTF attack automation.

### 3.1 Requirements
- The server listens on a host:port and accepts multiple clients simultaneously.
- Each client sends messages (text) terminated by `\n`.
- The server broadcasts received messages to all connected clients, prefixed with a client identifier (e.g., `[Client1] Hello`).
- Support **private messages**: a client can send `/msg <id> <message>` and only that client receives it.
- Support a `/list` command to see all client IDs.
- Clients can gracefully disconnect; server removes them and notifies others.

### 3.2 Architecture
We’ll create a `ChatServer` class that:
- Holds a `socket` (listening) and a dictionary `{socket: client_info}`.
- Uses `select.select()` to monitor the listening socket and all client sockets for readability.
- When the listening socket is ready, accept a new client and add it.
- When a client socket is ready, read data, parse commands, and handle.

### 3.3 Implementation (Build Along)
**Step 1 – Skeleton & Server Setup**
```python
import select
import socket

class ChatServer:
    def __init__(self, host='127.0.0.1', port=9999):
        self.server_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.server_sock.bind((host, port))
        self.server_sock.listen(5)
        self.server_sock.setblocking(False)
        self.clients = {}   # socket -> {'id': int, 'addr': str, 'buffer': bytes}
        self.next_id = 1

    def run(self):
        print(f"[*] Chat server started on {self.server_sock.getsockname()}")
        inputs = [self.server_sock]
        while True:
            readable, _, exceptional = select.select(inputs, [], inputs)
            for s in readable:
                if s is self.server_sock:
                    self._accept_client()
                else:
                    self._handle_client(s)
            for s in exceptional:
                self._remove_client(s)
```

**Step 2 – Accepting Clients**
```python
    def _accept_client(self):
        client_sock, addr = self.server_sock.accept()
        client_sock.setblocking(False)
        self.clients[client_sock] = {
            'id': self.next_id,
            'addr': addr,
            'buffer': b''
        }
        self.next_id += 1
        print(f"[+] New connection from {addr} assigned ID {self.clients[client_sock]['id']}")
        self._broadcast(f"[Server] Client {self.clients[client_sock]['id']} joined.\n", exclude=client_sock)
```

**Step 3 – Handling a Client (Reading & Processing)**
```python
    def _handle_client(self, client_sock):
        try:
            data = client_sock.recv(4096)
            if not data:
                self._remove_client(client_sock)
                return
            client_info = self.clients[client_sock]
            client_info['buffer'] += data
            while b'\n' in client_info['buffer']:
                msg_bytes, client_info['buffer'] = client_info['buffer'].split(b'\n', 1)
                msg = msg_bytes.decode().strip()
                if msg:
                    self._process_message(client_sock, msg)
        except ConnectionResetError:
            self._remove_client(client_sock)

    def _process_message(self, sender_sock, msg):
        sender_id = self.clients[sender_sock]['id']
        if msg.startswith('/'):
            self._handle_command(sender_sock, msg)
        else:
            self._broadcast(f"[{sender_id}] {msg}\n", exclude=None)
```

**Step 4 – Commands: /msg and /list**
```python
    def _handle_command(self, sender_sock, msg):
        parts = msg.split(maxsplit=2)
        cmd = parts[0].lower()
        if cmd == '/msg':
            if len(parts) < 3:
                self._send_to(sender_sock, "Usage: /msg <id> <message>\n")
                return
            try:
                target_id = int(parts[1])
            except ValueError:
                self._send_to(sender_sock, "Invalid client ID.\n")
                return
            message = parts[2]
            target_sock = self._find_client_by_id(target_id)
            if target_sock:
                sender_id = self.clients[sender_sock]['id']
                self._send_to(target_sock, f"[{sender_id} -> You] {message}\n")
            else:
                self._send_to(sender_sock, f"No client with ID {target_id}\n")
        elif cmd == '/list':
            ids = [str(info['id']) for info in self.clients.values()]
            self._send_to(sender_sock, "Connected: " + ", ".join(ids) + "\n")
        else:
            self._send_to(sender_sock, "Unknown command.\n")
```

**Step 5 – Helper Methods**
```python
    def _send_to(self, sock, text):
        try:
            sock.sendall(text.encode())
        except:
            self._remove_client(sock)

    def _broadcast(self, text, exclude=None):
        for sock in self.clients:
            if sock != exclude:
                self._send_to(sock, text)

    def _find_client_by_id(self, cid):
        for sock, info in self.clients.items():
            if info['id'] == cid:
                return sock
        return None

    def _remove_client(self, sock):
        if sock in self.clients:
            cid = self.clients[sock]['id']
            print(f"[-] Client {cid} disconnected.")
            del self.clients[sock]
            sock.close()
            self._broadcast(f"[Server] Client {cid} left.\n")
```

**Step 6 – Test It!**
Run the server: `python3 chat_server.py`. Open two more terminals and connect using `nc 127.0.0.1 9999`. Chat, try `/list`, `/msg 2 hello`, and watch the magic.

**Exercise:** Add a `/nick <name>` command that lets a client set a display name. The broadcast then uses the nickname instead of the numeric ID. This will teach you to update client state.

**Why this project matters:** You now understand a concurrent network server. When you exploit a service later, you’ll know exactly how the server might be handling input, buffering, and broadcasting—knowledge that reveals vulnerabilities like race conditions, buffer misuse, or command injection.

**Congratulations** — you’re no longer just a scripter, you’re a **network programmer**. You could replace this chat backend with a CTF bot that accepts commands from multiple handlers.

---

## 🇨 PART 4: INTRODUCTION TO C – THE LANGUAGE (2 HOURS)

Now we leave Python’s warm embrace and step into the world of manual memory management, close‑to‑the‑metal execution, and the language that runs the world’s operating systems and exploitable binaries. C is **not** forgiving, but it’s honest. Every byte matters.

### 4.1 Why C for CTFs?
- The vast majority of binary exploitation targets are compiled C/C++ programs.
- You must **think like the compiler** to understand stack frames, buffer layouts, and memory corruption.
- Writing shellcode often requires C‑level knowledge of system calls and memory.
- Reversing binaries is much easier when you can mentally translate assembly back to C.

### 4.2 Compilation Process (Code to Executable)
1. **Preprocessor:** handles `#include`, `#define` macros.
2. **Compiler:** translates C to assembly (`gcc -S`).
3. **Assembler:** converts assembly to object code (`gcc -c`).
4. **Linker:** combines objects and libraries into an executable.

**Let’s watch it happen:**
```bash
echo '#include <stdio.h>
int main() { printf("Hello, CTF!\n"); return 0; }' > hello.c
gcc -E hello.c -o hello.i   # preprocessed
gcc -S hello.i -o hello.s   # assembly
gcc -c hello.s -o hello.o   # object
gcc hello.o -o hello        # executable
./hello
```
Use `cat hello.s` to see the assembly. You’ll see `main:` with function prologue, a `call printf`, etc. We’ll dissect this in Phase 2; for now, just appreciate the pipeline.

### 4.3 Basic Syntax and Types
```c
#include <stdio.h>

int main(void) {
    int age = 25;
    float pi = 3.14159;
    char grade = 'A';
    printf("I am %d years old, pi is %f, grade %c\n", age, pi, grade);
    return 0;
}
```
Key differences from Python:
- Semicolons end statements.
- Variable types must be declared (`int x;`).
- Curly braces `{}` define blocks, not indentation (indentation is for readability).
- `printf` format specifiers: `%d` (int), `%f` (float), `%c` (char), `%s` (string), `%x` (hex).

**Code‑along:** Write a C program that prints the sizes of various types using `sizeof`:
```c
#include <stdio.h>
int main() {
    printf("Size of int: %zu bytes\n", sizeof(int));
    printf("Size of char: %zu\n", sizeof(char));
    printf("Size of float: %zu\n", sizeof(float));
    printf("Size of double: %zu\n", sizeof(double));
    printf("Size of pointer: %zu\n", sizeof(int*));
    return 0;
}
```
Compile and run. On a 64‑bit system, `int` is 4 bytes, pointer is 8 bytes. This size matters for buffer overflows: you’ll overwrite exactly these many bytes.

### 4.4 Control Flow (Loops & Conditionals)
Exactly like you’d expect, but with typed conditions.

**Fahrenheit‑Celsius Table:**
```c
#include <stdio.h>
int main() {
    int fahr;
    for (fahr = 0; fahr <= 300; fahr += 20) {
        printf("%3d F = %6.1f C\n", fahr, (5.0/9.0)*(fahr-32));
    }
    return 0;
}
```
Note: `5.0/9.0` ensures floating‑point division; `5/9` would be integer 0.

**Exercise:** Write a C program that calculates the sum of numbers from 1 to N, where N is hardcoded. Then modify to read N from the user (we’ll get to input shortly).

### 4.5 The Perils of `scanf` and Why We Use `fgets`
In C, reading user input is dangerous. Consider:
```c
char name[20];
scanf("%s", name);  // if user types more than 19 chars -> overflow!
```
`scanf` doesn’t know the buffer size. Instead, use:
```c
char name[20];
fgets(name, sizeof(name), stdin);
```
`fgets` reads at most `sizeof(name)-1` characters and null‑terminates. Always use it for strings.

**Why this is critical:** Buffer overflows are the heart of binary exploitation. You will learn to overflow buffers with `scanf`/`gets` and exploit them. Understanding safe input is the defender’s view; today we’re the defender. Later we become the attacker.

### 4.6 Command‑Line Arguments
```c
int main(int argc, char *argv[]) {
    // argc = argument count, argv = array of strings
    if (argc < 2) {
        printf("Usage: %s <name>\n", argv[0]);
        return 1;
    }
    printf("Hello, %s!\n", argv[1]);
    return 0;
}
```
`argv[0]` is the program name. This is how you’ll pass exploit parameters later.

**Code‑along:** Write a program `calc.c` that takes two integers as command‑line arguments, converts them with `atoi()` (from `<stdlib.h>`), and prints their sum. Handle missing arguments gracefully.

Now you’ve written your first useful C programs. The syntax will start to feel natural.

---

## ⚙️ PART 5: FUNCTIONS & SCOPE IN C (1 HOUR)

### 5.1 Function Definitions & Prototypes
In C, you can’t call a function before it’s declared. So either define it before `main`, or provide a **prototype**:

```c
#include <stdio.h>

double power(double base, int exp); // prototype

int main() {
    printf("%f\n", power(2.0, 3));
    return 0;
}

double power(double base, int exp) {
    double result = 1.0;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}
```
**Call by value:** arguments are copied; changes inside the function do not affect the original variables. To modify a caller’s variable, you must pass a pointer (next week!).

### 5.2 Local vs. Global Variables
```c
int global = 100;   // visible everywhere in this file (bad practice)

void test() {
    int local = 42;
    printf("%d\n", local);
    printf("%d\n", global);  // accessible
}

int main() {
    test();
    // printf("%d", local);  ERROR, local not in scope
    return 0;
}
```
Keep variables as local as possible. Global state leads to confusion and security bugs.

### 5.3 The Stack (Conceptual)
Every function call creates a **stack frame** containing:
- Return address (where to go back)
- Saved frame pointer
- Local variables
- Possible alignment padding

We’ll draw this next week; for now, just know that local variables are placed on the stack, and a function that declares a buffer like `char buf[64]` puts it on the stack. If you overflow `buf`, you can overwrite the return address. This is the fundamental attack we’ll study.

**Mental model:** start imagining the stack as an array of values, growing downward. When you write past the end of `buf`, you’re changing the return address. GDB will make this visible.

### 5.4 The `static` Keyword
Inside a function, `static int count = 0;` persists its value across calls. Not critical now, but you’ll see it in some CTF challenges.

---

## 📝 WEEK 4 PROBLEM SET (Daily 2‑4 Hours)

### PS4‑1: Complete the Chat Server Enhancements
- Add the `/nick` command as described. Store nicknames in the client info dict.
- Add a `/exit` command that gracefully disconnects the client.
- (Bonus) Implement a file‑transfer feature: `/send <id> <filepath>` that reads a local file and sends it to the target client, who receives it as a new file. This simulates an exfiltration tool.

### PS4‑2: Cryptopals Set 1 – Challenge 3 (Single‑byte XOR cipher)
Solve **Challenge 3** from Cryptopals. The hex‑encoded string has been XORed against a single character. Find the key, decrypt, and print the message. You’ll implement a frequency scoring function that uses English letter frequencies. This problem combines hex decoding, XOR, and statistical analysis—all Python. Use your `ctf_tools` conversions.

### PS4‑3: K&R C Exercises – Chapters 1
Complete the following exercises from *The C Programming Language* (2nd Ed.):
- 1‑1, 1‑2, 1‑3, 1‑4, 1‑5 (introductory)
- 1‑6, 1‑7 (EOF)
- 1‑8, 1‑9, 1‑10 (character counting, word replacement)
- 1‑11 to 1‑15 (word counting, arrays, functions)
At minimum, do 1‑1 through 1‑10, and 1‑15 (temperature conversion using function). Type every program, compile with `gcc -Wall -Wextra -pedantic -o prog prog.c`, fix all warnings, and test.

### PS4‑4: OverTheWire Bandit Levels 13‑15
- **Level 13:** SSH private key login.
- **Level 14:** Send password to a service on a specific port using `nc`.
- **Level 15:** SSL/TLS connection using `openssl s_client`.
Document each in your notes.

### PS4‑5: picoCTF Python & General Skills
Go to [picoGym](https://play.picoctf.org/practice) and solve **5 challenges** under categories:
- “General Skills” (e.g., “Obedient Cat”, “Python Wrangling”)
- “Cryptography” (any easy ones like “Mod 26”, “Transformation”)
Apply your Python skills to solve them, and write short write‑ups.

### PS4‑6: Reflection & C Mindset Shift
Create `week4_reflection.md`:
1. How does thinking in C (static types, manual memory) differ from Python? What felt most alien?
2. Describe a mistake you made in C (e.g., missing `&` in `scanf` or forgetting a semicolon) and how you fixed it.
3. What did the chat server project teach you about concurrency and protocol design?
4. Look ahead: what do you think “overflowing a buffer” means now, having seen fixed‑size arrays in C?

---

## 🔥 DEEP DIVE WRAP‑UP

This week you crossed a major threshold: from the comfort of Python into the raw truth of C. You built a concurrent server that could serve as the backbone of a CTF command‑and‑control system, and you wrote your first C programs that will soon be exploited. You’ve seen both sides: the high‑level logic and the low‑level memory layout.

Next week, we’ll dive deeper into C’s memory model: pointers, arrays, dynamic allocation, and the stack frame diagrams that will unlock binary exploitation. You’ll start using GDB to step through your C programs and witness the stack in action.

**Start your terminal. Activate your environment. Finish that chat server, then open `vim hello.c` and compile it with `gcc -Wall -o hello hello.c`. The machine is waiting.**
