We’ve programmed and solved problems. Now we lift our code to the next level: **abstraction**. You’ll learn to model complex systems with classes, make your code resilient with exception handling, and start building the reusable toolkit that will follow you into every CTF. This week is about transforming your scripts into **professional-grade components**.

# 🔥 WEEK 3: INTENSIVE DEEP DIVE  
## *“Abstraction & Robustness”*

---

## ⏳ Session Overview (8‑Hour Immersive Block)

| Time (total) | Section |
|--------------|---------|
| 0:00 – 2:00 | Classes & Objects |
| 2:00 – 3:00 | Magic Methods & Operator Overloading |
| 3:00 – 4:00 | Exception Handling |
| 4:00 – 5:00 | Generators & Iterators |
| 5:00 – 5:30 | Modules & Packages |
| 5:30 – 6:30 | Mini‑Project: Building a CTF Utility Library |
| 6:30 – 7:30 | Problem Set Kick‑off & Hands‑On Start |
| 7:30 – 8:00 | Reflection & Wrap‑up |

---

## 🧬 PART 1: CLASSES & OBJECTS (2 HOURS)

Object‑oriented programming (OOP) isn’t just a paradigm—it’s a way to **model the adversary’s world**. In CTFs, you’ll interact with packets, cryptographic primitives, file formats, and memory structures. Classes let you wrap these into neat, reusable objects.

### 1.1 Why OOP? The Stateful Toolbox
Procedural code (what you’ve written so far) is a list of instructions. OOP bundles **state** (attributes) and **behavior** (methods) into a single entity. This mimics how real systems work: a network connection has an IP address, a port, and methods like `send()`, `recv()`.  

**Analogy:** You don’t want to write `send_packet(ip, port, data)` every time; you want a `Connection` object that remembers `ip` and `port`, and you just call `conn.send(data)`.

### 1.2 Defining Your First Class
Open a new file, type along:
```python
class Pokemon:
    """A wild Pokémon appeared!"""
    
    def __init__(self, name, hp, move):
        self.name = name          # instance variable
        self.hp = hp
        self.move = move
        self.fainted = False

    def attack(self, target):
        print(f"{self.name} used {self.move} on {target.name}!")
        target.hp -= 20
        if target.hp <= 0:
            target.fainted = True
            print(f"{target.name} fainted!")

    def heal(self):
        self.hp += 10
        print(f"{self.name} healed to {self.hp} HP.")
```

**Key points:**
- `__init__` is the **constructor**, called when you create `poke = Pokemon("Pikachu", 100, "Thunderbolt")`. `self` refers to the specific instance.
- All instance variables are prefixed with `self.` – they belong to *that* object.
- Every method must have `self` as the first parameter (it’s passed automatically).

**Exercise:** Create two Pokémon: `pika = Pokemon("Pikachu", 80, "Quick Attack")` and `char = Pokemon("Charmander", 90, "Ember")`. Make them attack each other until one faints. Use a `while` loop.

### 1.3 Inheritance: Building Specialization
In CTFs, you might have different kinds of exploits (stack overflow, format string, heap spray) that share common preparation steps. Inheritance lets you define a **base** exploit class and extend it.

```python
class WaterPokemon(Pokemon):
    def __init__(self, name, hp, move, water_resist=5):
        super().__init__(name, hp, move)   # call parent constructor
        self.water_resist = water_resist

    def swim(self):
        print(f"{self.name} is swimming!")

# Overriding a method
class FirePokemon(Pokemon):
    def heal(self):   # override parent's heal
        self.hp += 15  # fire types heal more
        print(f"{self.name} warmed up to {self.hp} HP!")
```

- `super().__init__()` calls the parent’s `__init__`. Without it, `name`, `hp`, `move` won’t be set.
- You can add new methods (`swim`) or change existing ones (`heal`). This is **polymorphism**—a `FirePokemon` behaves like a `Pokemon` but with different healing.

**In‑class mini‑project:** Create a class `NetworkDevice` with `ip`, `mac`, and method `send(data)`. Then derive `Router` and `Switch`. Each overrides `send()` to add routing logic (print something like “Forwarding packet via route table”). Use `super()` to keep the base behavior.

### 1.4 The `__str__` and `__repr__` Methods
When you `print(pika)`, you get something ugly like `<__main__.Pokemon object at 0x7f...>`. Define how it looks:

```python
class Pokemon:
    def __str__(self):
        return f"Pokemon({self.name}, HP={self.hp})"
    def __repr__(self):
        return f"Pokemon(name='{self.name}', hp={self.hp})"
```
`__repr__` is for debugging (should ideally recreate the object), `__str__` for user-friendly display. Implement both.

**CTF connection:** When you dump an object in a debugger or log, `repr` helps you see its exact state. Missing `__repr__` is like trying to reverse engineer a binary without symbols.

### 1.5 Encapsulation: Protecting Attributes (Pythonic Style)
Python doesn’t have true `private`, but convention: prefix with `_` for internal use, `__` for name mangling.
```python
self._secret_key = 0xdeadbeef   # "please don't touch"
self.__really_hidden = 42       # becomes _Pokemon__really_hidden
```
We rarely need heavy encapsulation, but in exploit development, you’ll want to hide sensitive data (e.g., raw shellcode) from accidental modification.

**Exercise:** Add a `_status` attribute to Pokémon that can be “normal”, “poisoned”, “paralyzed”. Write a method `apply_status(effect)` that changes it and adjusts behavior in `attack()` (e.g., poisoned loses 5 HP each turn). Encapsulate so that status is only changed via the method.

---

## ✨ PART 2: MAGIC METHODS & OPERATOR OVERLOADING (1 HOUR)

Magic methods (dunder methods) let your objects react to Python operators like `+`, `==`, and `len()`. This makes your classes intuitive and powerful.

### 2.1 The Vector2D Example
Create a math vector class:
```python
class Vector2D:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __add__(self, other):
        return Vector2D(self.x + other.x, self.y + other.y)

    def __mul__(self, scalar):
        return Vector2D(self.x * scalar, self.y * scalar)

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y

    def __abs__(self):
        return (self.x**2 + self.y**2) ** 0.5

    def __repr__(self):
        return f"Vector2D({self.x}, {self.y})"
```
Now you can write: `v1 + v2`, `v1 * 3`, `abs(v1)`, `v1 == v2`.

**CTF relevance:** When implementing cryptographic group operations (e.g., elliptic curves), you’ll override `__add__` and `__mul__` to work with points. This makes the code readable and less error-prone.

### 2.2 Indexing and Slicing with `__getitem__`
Create a `ByteBuffer` that wraps a `bytes` object:
```python
class ByteBuffer:
    def __init__(self, data):
        self.data = data

    def __getitem__(self, index):
        if isinstance(index, slice):
            return ByteBuffer(self.data[index])
        return self.data[index]

    def __len__(self):
        return len(self.data)

    def __repr__(self):
        return f"ByteBuffer({self.data.hex()})"
```
Now `buf[5]` returns an integer (like bytes normally), `buf[2:6]` returns a new `ByteBuffer`. This is how you’ll later handle packet payloads elegantly.

**Exercise:** Implement `__setitem__` to allow mutation for a mutable buffer (use `bytearray` inside). Then add a `find(needle)` method that uses the buffer indexing.

### 2.3 Context Managers with `__enter__` and `__exit__`
The `with open(...) as f:` is a context manager. You can build your own:
```python
class ExploitEnv:
    def __init__(self, target):
        self.target = target

    def __enter__(self):
        print(f"[*] Connecting to {self.target}")
        self.conn = create_connection(self.target)  # hypothetical
        return self.conn

    def __exit__(self, exc_type, exc_val, exc_tb):
        print("[*] Cleaning up connection")
        self.conn.close()
```
Use: `with ExploitEnv("127.0.0.1") as conn: conn.send(payload)`. It guarantees cleanup even if the exploit crashes.

---

## 🚨 PART 3: EXCEPTION HANDLING (1 HOUR)

Your programs will face the unexpected. Exception handling is how you **control chaos** and keep your exploit running.

### 3.1 The `try/except` Mechanic
```python
try:
    with open('target_config.txt') as f:
        config = f.read()
except FileNotFoundError as e:
    print(f"Config file missing: {e}")
    config = 'default'
except PermissionError:
    print("No permission; running as root?")
    raise  # re-raise if can't handle
```
- Catch **specific** exceptions; never use bare `except:` unless you know what you’re doing (it hides keyboard interrupts and bugs).
- `as e` captures the exception object for debugging.

### 3.2 `else` and `finally`
```python
try:
    sock = socket.socket(...)
    sock.connect((ip, port))
except socket.error:
    print("Connection failed")
else:
    sock.send(payload)   # only if no exception
finally:
    sock.close()         # always runs, even if exception above
```
`finally` is gold for resource cleanup. In an exploit, you don’t want to leave a half‑open connection that triggers an intrusion detection system.

### 3.3 Raising Exceptions
```python
def check_flags(flag_str):
    if not flag_str.startswith("CTF{"):
        raise ValueError(f"Invalid flag format: {flag_str}")
```
Use `raise` to signal errors at the right level. Custom exceptions:
```python
class ExploitFailedError(Exception):
    pass
```

### 3.4 Robust Input Handling
Write a function that asks for an integer, retrying on invalid input:
```python
def get_int(prompt):
    while True:
        try:
            return int(input(prompt))
        except ValueError:
            print("That's not a valid integer, try again.")
```
Apply this to your previous age‑calculator or guessing game.

**CTF scenario:** Interacting with a remote service, a read might time out (`timeout`), return garbage, or the connection might drop. You’ll wrap all socket operations in retry loops with exception handling. We’ll practice that now.

**Drill:** Write a mock function `recv_until_pattern(sock, pattern)` that reads bytes in a loop and catches `socket.timeout`, raising a custom `NoResponseError`.

---

## 🔁 PART 4: GENERATORS & ITERATORS (1 HOUR)

Generators produce values on the fly, saving memory. You’ll use them to iterate over huge dictionaries, brute‑force keys, or lazily parse network streams.

### 4.1 Iterator Protocol
An **iterable** is anything with `__iter__()` (like lists). An **iterator** has `__next__()` which raises `StopIteration` when exhausted. A `for` loop works on iterators.

```python
class Countdown:
    def __init__(self, start):
        self.current = start
    def __iter__(self):
        return self
    def __next__(self):
        if self.current <= 0:
            raise StopIteration
        self.current -= 1
        return self.current + 1

for i in Countdown(5):
    print(i)   # 5,4,3,2,1
```
This is verbose; generators simplify it.

### 4.2 `yield` and Generator Functions
```python
def countdown(n):
    while n > 0:
        yield n
        n -= 1
```
Calling `countdown(5)` returns a **generator object**. Each `next()` runs until the next `yield`, and the function’s state (local variables) is frozen.

```python
gen = countdown(3)
print(next(gen))   # 3
print(next(gen))   # 2
```
When it returns (no more yields), `StopIteration` is raised.

**Exercise:** Write a generator `read_chunks(file_path, chunk_size=1024)` that yields chunks of a file without loading it entirely into memory. Use it to process a large file (e.g., your system’s `/var/log/syslog`).

### 4.3 Generator Expressions
Like list comprehensions but lazy: `(x**2 for x in range(10**6))`. Use when you just need to iterate once.

### 4.4 Infinite Generators
```python
def primes():
    """Yield prime numbers indefinitely (naive implementation)."""
    num = 2
    while True:
        if all(num % i != 0 for i in range(2, int(num**0.5)+1)):
            yield num
        num += 1
```
CTF use: brute‑forcing a hash with a dictionary that’s too large to store—generate candidates on the fly and test.

### 4.5 Sending Values and `yield from`
Simple today: just master `yield`. `yield from` delegates to another generator, useful for flattening nested structures.

**Mini‑task:** Write a generator `bruteforce_wordlist(alphabet, max_length)` that yields all strings up to `max_length` from the given alphabet (e.g., `'a','b'`). Use recursion and `yield from`. This is a direct brute‑force engine.

---

## 📦 PART 5: MODULES & PACKAGES (0.5 HOUR)

You’ve written `.py` files and imported them. Now organise your growing toolkit.

### 5.1 Python Packages
A package is a directory with an `__init__.py` file (can be empty). Structure:
```
ctf_toolkit/
    __init__.py
    utils.py
    net.py
    crypto.py
```
Now `from ctf_toolkit import net` works. `__init__.py` can import specific classes to make the package present a clean interface (e.g., `from .net import Connection`).

**Create your package now:**
```bash
mkdir -p ~/ctf_bedrock/week3/ctf_tools
touch ~/ctf_bedrock/week3/ctf_tools/__init__.py
```
Add a `conversions.py` module inside.

### 5.2 Virtual Environments
Keep dependencies isolated.  
```bash
python3 -m venv ctf_venv
source ctf_venv/bin/activate
pip install pwntools   # our future exploit library
```
Deactivate with `deactivate`. All your CTF work will live inside such environments.

**Why?** Different CTF tools require different library versions. Isolation prevents dependency hell.

---

## 🧰 PART 6: MINI‑PROJECT – CTF UTILITY LIBRARY (1 HOUR)

Now you’ll bootstrap the library that will grow with you. This is **your own pwntools‑lite**.

### 6.1 conversions.py
```python
import base64

def hex_to_bytes(hex_str: str) -> bytes:
    return bytes.fromhex(hex_str)

def bytes_to_hex(b: bytes) -> str:
    return b.hex()

def base64_to_bytes(b64_str: str) -> bytes:
    return base64.b64decode(b64_str)

def bytes_to_base64(b: bytes) -> str:
    return base64.b64encode(b).decode()

def xor_bytes(a: bytes, b: bytes) -> bytes:
    return bytes(x ^ y for x, y in zip(a, b))
```
**Test them:**
```python
assert hex_to_bytes("deadbeef") == b'\xde\xad\xbe\xef'
assert xor_bytes(b'abc', b'xyz') == bytes([...])  # compute manually
```
### 6.2 net.py
Implement a simple `Netcat` class:
```python
import socket

class Netcat:
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def connect(self):
        self.sock.connect((self.host, self.port))

    def send(self, data: bytes):
        self.sock.sendall(data)

    def recv(self, size=4096) -> bytes:
        return self.sock.recv(size)

    def recv_until(self, pattern: bytes) -> bytes:
        buf = b''
        while True:
            chunk = self.sock.recv(1)
            if not chunk:
                raise ConnectionError("Connection closed")
            buf += chunk
            if pattern in buf:
                break
        return buf

    def close(self):
        self.sock.close()
```

**Usage test:** Connect to `httpbin.org` port 80, send `b"GET / HTTP/1.0\r\n\r\n"`, receive response. (You can do this in a try block with exception handling.)

### 6.3 crypto.py
- Add the Caesar cipher functions from last week, but refactor them into a class `CaesarCipher` with `encrypt`, `decrypt`, `crack` methods.
- Include the `crack` function using a wordlist. Make the wordlist path configurable.

**Time‑saver:** You can copy over your existing code and wrap it in a class.

### 6.4 __init__.py
Make the package clean:
```python
# ctf_tools/__init__.py
from .conversions import hex_to_bytes, bytes_to_hex, xor_bytes
from .net import Netcat
from .crypto import CaesarCipher
```
Now your main script can do `from ctf_tools import Netcat` and it feels professional.

**Reflect:** You just built a foundation for all future CTF automation. Next week you’ll add socket send‑and‑receive with timeout, error handling, and pattern matching.

---

## 📝 WEEK 3 PROBLEM SET (Daily 2‑4 Hours)

### PS3‑1: MIT 6.0001 Problem Set 2 – Hangman
Complete the full **Hangman** game from MIT OCW 6.0001. The problem set asks you to implement:
- The `is_word_guessed` function
- `get_guessed_word`
- `get_available_letters`
- The main game loop with hints and scoring.
You must **use dictionaries** and **string slicing**, but the key is that you should structure it using a class `Hangman` that holds the secret word, guessed letters, and available letters. This forces you to apply OOP to a familiar problem.

### PS3‑2: Cryptopals Set 1 – Challenges 1 & 2
Go to [Cryptopals Crypto Challenges](https://cryptopals.com/). Solve:
- **Challenge 1: Convert hex to base64.** Use your `ctf_tools.conversions` module, but implement the core manually (no just `base64.b64encode`). Process bytes: 3 bytes → 4 base64 characters. Write the logic yourself. This deepens your understanding of encoding.
- **Challenge 2: Fixed XOR.** Write a function that takes two equal‑length buffers and produces their XOR combination. You already have `xor_bytes`; now use it to solve the challenge (the given strings are hex). This is your first taste of cryptopals.

### PS3‑3: OverTheWire Bandit Levels 9‑12
- **Level 9:** Use `strings` and `grep` to find a password in a binary file.
- **Level 10:** `base64` decoding.
- **Level 11:** `tr` for ROT13 translation.
- **Level 12:** A multi‑step decompression challenge (gzip, bzip2, tar). This teaches you to follow the trail, much like extracting stages from a shellcode loader.

**Document each** in your `bandit_notes.md`, explaining the tools used and the clue that led you.

### PS3‑4: Project – Command‑Line Address Book
Build an address book that persists data to a JSON file. Create a class `AddressBook` with methods:
- `add_contact(name, phone, email)`
- `delete_contact(name)`
- `search(name)` (fuzzy search: returns all contacts where name contains the query)
- `list_all()`
- `save(filename)` and `load(filename)` using `json` module.

The CLI should present a menu: `1. Add`, `2. Search`, etc., using a loop. Use exception handling to catch file not found on load, and invalid input. **All code in a class.** This is a real‑world OOP application.

### PS3‑5: Write‑Up: Your CTF Toolkit Documentation
Create a `README.md` inside your `ctf_tools/` directory. Describe each module, its purpose, and how to use it, with example code. Write it as if you’re onboarding a teammate. This documentation habit will make you a better CTF collaborator.

### PS3‑6: Week 3 Reflection
Create `week3_reflection.md`:
1. How did OOP change the way you structure your code? Compare your Hangman from this week to a script you might have written before.
2. What was the most useful magic method you implemented? Why?
3. Describe a bug you caught with exception handling that would have crashed your program.
4. How does it feel to have your own CTF library? What will you add next?

---

## 🔥 DEEP DIVE WRAP‑UP

You now see code as **architecture**, not just instructions. You’re building systems that are reusable, robust, and ready for the unpredictability of CTF environments. The `try/except` blocks will save you in the middle of an exploit; the `Netcat` class will be your gateway to remote targets; and the generator‑based brute‑forcers will chew through password lists without memory strain.

This week’s work transforms you from a lone scripter into a **tool‑building operator**. In Week 4, we’ll finish the Python portion with networking and an introduction to C—bridging the gap to the machine.

Open your terminal, activate your virtual environment, and start building that `AddressBook` class. **Own this week.**
