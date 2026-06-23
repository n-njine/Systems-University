Absolutely. We’re going to take Week 1 of your Phase 1 bedrock and turn it into a living, breathing laboratory. This isn’t a list of tasks; it’s a **deep, layered experience**. By the end of this week, you won’t just *know* Python basics—you’ll be able to **think** in scripts, navigate a terminal like it’s your home, and start seeing the faint outlines of how programs can be tricked.

Let’s dive in.

---

# 🔥 WEEK 1: INTENSIVE DEEP DIVE  
## *“Your Mind as an Interpreter”*

*Everything you build this week forms the cognitive muscle memory you’ll later use to write exploits, parse network traffic, and automate CTF workflows. Approach it with curiosity, not haste.*

---

## ⏳ Session Overview (8-Hour Immersive Block)

You’ll do this in one intense day (or split into two 4‑hour sessions, but aim for immersion). The following is a **minute-by-minute guide** to exactly what to learn, code, and ponder.

| Time (total) | Section |
|--------------|---------|
| 0:00 – 1:00 | Setup & CLI Survival |
| 1:00 – 2:00 | Python as a Calculator & Types |
| 2:00 – 3:30 | Branching & Loops |
| 3:30 – 5:00 | Functions & Scope |
| 5:00 – 6:00 | Basic Data Structures |
| 6:00 – 7:00 | Reading Code & Tracing |
| 7:00 – 8:00 | Problem Set Kickoff & Reflection |

---

## 🧠 PART 1: SETUP & ENVIRONMENT (1 HOUR)

### 1.1 The Goal
You need a **Linux command line** as your primary interface for the entire journey. No IDEs that hide the build process; no GUI file managers that obscure permissions. The command line is where you will live, debug, and eventually exploit binaries.

### 1.2 Setup Steps (Complete Before the Session)
- **Install a Linux environment:**
  - **Option A (Recommended for Windows users)**: WSL2 with Ubuntu 22.04. Follow Microsoft’s guide to install, then ensure `wsl --set-version Ubuntu 2` (for full system call compatibility).
  - **Option B**: VirtualBox + Ubuntu 22.04 desktop ISO (if you want a full GUI, but you’ll work mostly in terminal).
  - **Option C**: Native Linux (dual boot).

- **Open a terminal** and update packages:
  ```bash
  sudo apt update && sudo apt upgrade -y
  sudo apt install python3 python3-pip gcc git curl wget vim build-essential -y
  ```
  (You can use nano instead of vim if you prefer, but learn one terminal editor well.)

- **Verify installations:**
  ```bash
  python3 --version   # should be 3.10+
  gcc --version       # should be 11+
  ```

### 1.3 The CLI Survival Kit (40 minutes)
The terminal is not just a place to run commands; it’s an **interactive environment** with its own language. Master these concepts today:

#### 1.3.1 File System Navigation
```
pwd                 # Where am I?
ls                  # List files
ls -la              # List all (including hidden) with details
cd ~                # Go home
cd /                # Go to root
cd -                # Go to previous directory
.                   # Current directory
..                  # Parent directory
```

#### 1.3.2 File & Directory Manipulation
```
mkdir mydir         # Create directory
touch file.txt      # Create empty file
cp source dest      # Copy
mv old new          # Move / rename
rm file             # Remove (careful, no trash!)
rm -r folder        # Remove directory recursively
```

#### 1.3.3 Viewing & Searching
```
cat file            # Print entire file
less file           # Scroll (q to quit)
head -n 5 file      # First 5 lines
tail -n 5 file      # Last 5 lines
grep "pattern" file # Find lines containing pattern
find . -name "*.py" # Find all Python files under current dir
```

#### 1.3.4 The Power Tools
- **Redirection & pipes:**
  ```
  echo "hello" > file.txt    # Overwrite
  echo "world" >> file.txt   # Append
  cat file.txt | grep hello  # Pipe output
  ```
- **Command substitution & expansion:**
  ```
  echo "Today is $(date)"
  echo file_{01..10}.txt     # Brace expansion
  ```
- **Wildcards:** `*` (any), `?` (single char), `[abc]` (set).

#### 1.3.5 Getting Help
```
man ls              # Manual page (press q to exit)
ls --help           # Quick help
type cd             # Shows that cd is a shell builtin
```

#### 🧪 In-Session Exercise (Must Do)
Create the directory structure you’ll use this week:
```bash
mkdir -p ~/ctf_bedrock/week1
cd ~/ctf_bedrock/week1
```
Now, **in one or two lines**, create 10 empty files named `file_01.txt` through `file_10.txt` using a single command. Do *not* manually type each name. Hint: Use brace expansion or a loop.

**Expected solution with brace expansion:**
```bash
touch file_{01..10}.txt
```
Check with `ls`. Now remove them with `rm file_*`. Try the loop method as an alternative:
```bash
for i in $(seq -w 1 10); do touch "file_${i}.txt"; done
```
Understand why `seq -w` pads with zeros.

### 1.4 Quick Text Editing (if time)
Open `vimtutor` (just spend 10 minutes) or use `nano` to create a simple text file. You’ll be writing Python scripts from the terminal, so be comfortable.

*Take a 5-minute break; stand up, stretch.*

---

## 🧠 PART 2: PYTHON AS A CALCULATOR & TYPES (1 HOUR)

### 2.1 Why Python?
In CTFs, Python is your swiss army knife. You’ll use it to prototype exploit logic, manipulate byte strings, communicate over sockets, and fuzz protocols. We begin with the absolute core.

### 2.2 The Python Interpreter
Open a terminal and type `python3`. You’re now in a **REPL** (Read-Eval-Print Loop). Every line is evaluated immediately. Use this as a scratchpad.

### 2.3 Atoms: Integers, Floats, Strings, Booleans
```python
>>> 5 + 3
8
>>> type(5)
<class 'int'>
>>> type(3.14)
<class 'float'>
>>> type('hello')
<class 'str'>
>>> type(True)
<class 'bool'>
```

**Crucial:** Everything in Python is an **object** with a type. The type determines what operations you can perform. This is static in terms of the object, but variables don’t have fixed types—they’re just names bound to objects (dynamic binding). If you later come from C, you’ll appreciate the difference.

### 2.4 Operators & Precedence
- Arithmetic: `+`, `-`, `*`, `/` (float division), `//` (floor division), `%` (modulo), `**` (exponent).
- Comparison: `==`, `!=`, `<`, `>`, `<=`, `>=`.
- Logical: `and`, `or`, `not`.
- **Precedence matters:** `3 + 5 * 2` is `13`, not `16`. Use parentheses to be explicit.

**CTF Connection:** Understanding modulo and floor division is essential for crypto challenges (e.g., modular arithmetic, chunking data).

### 2.5 Strings: Immutable Sequences
```python
s = "CTF{hello}"
len(s)          # 10
s[0]            # 'C'
s[4]            # '{'
s[-1]           # '}'
s[0:3]          # 'CTF' (slice from 0 to 2)
s[4:]           # '{hello}'
s[::-1]         # Reverse string
```
Strings are **immutable** – you cannot do `s[0] = 'X'`. You must create a new string. This is because strings in Python (and many languages) are implemented as immutable arrays to allow hashing and interning.

**Why this matters:** In binary exploitation, you’ll be injecting bytes into buffers. Python’s `bytes` type is similar to strings but for binary data. You’ll need to construct payloads carefully.

**Interactive Exercise:**
1. Store `"flag{example}"` in a variable. Extract just `"example"` without the braces using slicing.
2. Write an expression that reverses a string without a loop (hint: slice with step -1).

### 2.6 Input and Output
```python
name = input("What is your name? ")   # Always returns a string
print(f"Hello, {name}!")              # f-string (Python 3.6+)
```
**CTF scripts** often read from stdin or sockets. The `input()` function is a simple bridge, but later you’ll use `sys.stdin.read()` for bulk.

### 2.7 Type Conversions and Casting
```python
int("42")       # 42
float("3.14")   # 3.14
str(100)        # "100"
```
Danger: `int("abc")` raises a `ValueError`. Learn to handle this with `try/except` later.

**Code-Along Project (part of this hour):** Write a script in a file (`age_calc.py`) that asks the user for their birth year, calculates their age in days (approximate: 365.25 days per year), and prints it with an f-string. Run it from the terminal with `python3 age_calc.py`.

**Key takeaway:** Python dynamically infers types; it’s your responsibility to ensure variables hold what you think they do.

---

## 🔁 PART 3: BRANCHING & LOOPS (1.5 HOURS)

### 3.1 Conditional Logic
```python
x = 10
if x > 5:
    print("Big")
elif x == 5:
    print("Equal")
else:
    print("Small")
```
Indentation is **mandatory** and defines blocks. 4 spaces is the standard.

**Boolean expressions:** `True`, `False`, and truthiness. The following are *false* in a boolean context: `0`, `0.0`, `""`, `None`, `[]`, `{}`, `False`. Everything else is true.
```python
if []:
    print("Never printed")
```

**Logical operators:** `and`, `or`, `not`. They short-circuit:
```python
x = 0
if x != 0 and 10/x > 2:   # if x is 0, second part never evaluated -> no error
    print("Safe")
```

### 3.2 Loops

**`while` loop:** Executes while a condition is true.
```python
count = 0
while count < 5:
    print(count)
    count += 1
```
**Infinite loop trap:** Forgetting to update the condition.

**`for` loop:** Iterates over a sequence.
```python
for i in range(5):   # 0,1,2,3,4
    print(i)

for char in "flag":
    print(char)
```
`range(start, stop, step)` is a generator-like object that produces integers on demand.

**`break` and `continue`:**
- `break` exits the loop entirely.
- `continue` skips to the next iteration.

### 3.3 Mini-Project: Number Guessing Game (write in file)
Write a script that:
- Generates a random number between 1 and 100 (`import random; random.randint(1,100)`).
- Repeatedly asks the user to guess, giving hints "Too high"/"Too low" until correct.
- Counts attempts.

This combines `while`, `if/elif`, `input()`, type conversion, and randomness. Write it now, debug any errors.

### 3.4 FizzBuzz – The Classic Loop Exercise
Print numbers 1 to 100 inclusive, but:
- For multiples of 3, print `"Fizz"` instead of the number.
- For multiples of 5, print `"Buzz"`.
- For multiples of both 3 and 5, print `"FizzBuzz"`.

**Solution to type along with me:**
```python
for i in range(1, 101):
    if i % 15 == 0:
        print("FizzBuzz")
    elif i % 3 == 0:
        print("Fizz")
    elif i % 5 == 0:
        print("Buzz")
    else:
        print(i)
```
Now, can you refactor it to use string concatenation or a conditional expression? (Try later.)

**Why this matters for CTFs:** Loop control and conditionals are the basis of every brute-force password cracker, every fuzzing loop, every parser you’ll ever write.

---

## 🏗️ PART 4: FUNCTIONS & SCOPE (1.5 HOURS)

### 4.1 Defining Functions
```python
def greet(name):
    """Return a greeting string."""
    return f"Hello, {name}!"
```
- **Parameters** are local variables assigned the arguments passed.
- **`return`** sends a value back to the caller; if missing, function returns `None`.
- **Docstrings** (`"""..."""`) are documentation; they become the function’s `__doc__` attribute.

**Exercise:** Write a function `absolute(x)` that returns the absolute value without using `abs()`.

### 4.2 Scope Rules
Variables defined inside a function are **local** to that function. Variables defined outside are **global**. A function can *read* globals, but to modify them you must use `global` (discouraged). Better: return new values.

```python
x = 10
def change():
    global x
    x = 5
```
*Avoid this pattern unless absolutely necessary.* Instead, pass values and return results. It makes code testable and predictable.

### 4.3 Modular Programming: Build Your Own Library
Create a new file `utilities.py` in your week1 folder. In it, write:
```python
def gcd(a, b):
    """Euclidean algorithm: greatest common divisor."""
    while b != 0:
        a, b = b, a % b
    return a

def lcm(a, b):
    """Least common multiple."""
    return a * b // gcd(a, b)

def is_prime(n):
    """Return True if n is prime."""
    if n < 2:
        return False
    for i in range(2, int(n**0.5) + 1):
        if n % i == 0:
            return False
    return True
```
Now open a new file `main.py`:
```python
from utilities import gcd, is_prime

print(gcd(48, 18))   # 6
print(is_prime(17))  # True
```
Run `python3 main.py`. You’ve just created a module. This is how you’ll build your CTF toolkit: a `crypto_utils.py`, `net_utils.py`, etc.

### 4.4 Recursion (Briefly)
A function can call itself. The classic starter:
```python
def factorial(n):
    if n == 0:
        return 1
    else:
        return n * factorial(n-1)
```
**Tracing recursion** on paper is invaluable. Draw a tree for `factorial(4)`. Understand that each call has its own stack frame. Later, in Phase 2, you’ll see this exact mechanism exploited with buffer overflows.

### 4.5 Getting Help and Using `help()`
In the interpreter, `import utilities` then `help(utilities.gcd)`. You’ll see the docstring. Good documentation habits from day one.

---

## 🧺 PART 5: BASIC DATA STRUCTURES (1 HOUR)

### 5.1 Lists – Mutable Sequences
```python
fruits = ["apple", "banana", "cherry"]
fruits.append("orange")
fruits[1] = "blueberry"    # mutation
fruits.pop()               # removes last
fruits.sort()
```
Slicing works like strings: `fruits[0:2]`.

**List comprehensions** are Pythonic:
```python
squares = [x**2 for x in range(10)]   # [0,1,4,9,...]
```
They’re loops in disguise, but faster and more readable once you get the hang.

### 5.2 Tuples – Immutable Sequences
```python
point = (3, 4)
x, y = point   # unpacking
```
Tuples are used where you want to guarantee data doesn’t change (e.g., coordinates, function returns).

### 5.3 Dictionaries – Key-Value Maps
```python
scores = {"alice": 10, "bob": 7}
scores["charlie"] = 9
print(scores.keys(), scores.values(), scores.items())
```
**Why this matters:** Dictionaries are how you’ll count frequencies (crypto attacks), store configuration, and parse JSON APIs. They’re hash maps under the hood, giving O(1) average lookup.

**In-class exercise: Character Frequency Counter**
Write a script that takes a string (e.g., `"hello world"`) and prints how many times each character appears. Ignore spaces. Use a dict.

**Reference:**
```python
text = "hello world"
freq = {}
for ch in text:
    if ch != ' ':
        freq[ch] = freq.get(ch, 0) + 1
print(freq)
```
You can also use `collections.Counter`, but build it manually first to understand the logic.

**CTF foreshadowing:** A simple substitution cipher cracker begins with frequency analysis. You just wrote the engine.

---

## 🔍 PART 6: READING CODE & TRACING (1 HOUR)

### 6.1 The Skill of “Being the Interpreter”
Take a short, nested-loop snippet and trace it manually. Write a table of variable values as you step through.

**Example:**
```python
def mystery(n):
    a = 0
    b = 1
    for i in range(n):
        a, b = b, a + b
    return a
```
Trace for `n=4`:
| i | a | b |
|---|---|---|
| start | 0 | 1 |
| 0 | 1 | 1 |
| 1 | 1 | 2 |
| 2 | 2 | 3 |
| 3 | 3 | 5 |
| return a = 3 |

This is Fibonacci. Now, try a harder one: nested loops.
```python
def grid(n):
    for i in range(n):
        row = []
        for j in range(n):
            row.append(i * j)
        print(row)
```
Trace for `n=3`. Predict the printed output before running. Then run and verify.

### 6.2 Watch a Lecture Segment
Watch MIT 6.0001 Lecture 2 (Branching, Iteration) from around 20:00 to 45:00 where Prof. Grimson discusses iteration and program flow. Notice how he uses a **state machine** diagram to explain loops. This is how you’ll later model TCP connections.

### 6.3 Common Mistakes & Debugging
- **IndentationError:** Mixing tabs and spaces. Set your editor to insert spaces when tab is pressed.
- **NameError:** Using a variable before assignment.
- **SyntaxError:** Missing colon after `if`/`for`/`def`.
- **TypeError:** Trying to add string and integer. Use `str()` or `int()`.

**Reflect:** When you hit an error (you will), read the traceback from the **bottom up**—the last line tells you what went wrong, the lines above tell you where.

---

## 📝 WEEK 1 PROBLEM SET (Daily Workload, 2-4h/day)

Now that the intensive session has given you the concepts, it’s time to **deepen through practice**. The following tasks are to be completed by the end of the week.

### PS1-1: MIT 6.0001 Finger Exercises (From Guttag Ch 2)
Complete **all** “finger exercises” in Chapter 2 of *Introduction to Computation and Programming Using Python* (3rd Ed.). These are small but crucial. Solutions are not provided here, but you should attempt each before looking online (many students post them). Focus especially on:
- The `for` and `while` loop exercises.
- The exhaustive enumeration (cube root) exercise. It’s your first algorithm!

### PS1-2: CodingBat Python Warm-up
Go to [codingbat.com/python](https://codingbat.com/python) and complete:
- All problems in **Logic‑1**
- All problems in **List‑1**
- All problems in **String‑1**
These are quick puzzles that solidify syntax. No install required, just code in the browser. Write them in your own editor as well.

### PS1-3: Project – Caesar Cipher & Brute-forcer
Create a module `caesar.py` with these functions:

1. **`encrypt(plaintext: str, shift: int) -> str`**  
   - For each letter (ignore non-letters), shift forward by `shift` within the alphabet, preserving case.  
   - ‘A’ shifted by 3 becomes ‘D’, ‘z’ shifted by 1 wraps to ‘a’.  
2. **`decrypt(ciphertext: str, shift: int) -> str`**  
   - Reverses the shift.
3. **`crack(ciphertext: str) -> str`**  
   - Try all 26 shifts. For each decrypted text, check if it contains common English words (e.g., “the”, “flag”, “and”). Use a simple wordlist.  
   - Return the most likely plaintext.

**Hints:**
- Use `ord('a')` (97) and `chr()` to convert between letters and numbers.
- Handle case with `if char.isupper()`.
- For cracking, you can download a wordlist from the internet (`wget https://raw.githubusercontent.com/dwyl/english-words/master/words.txt`), load it into a set for fast lookup. But for a small test, you can hardcode a list like `["the", "flag", "and", "for", "CTF"]`.

**Test your cipher:** Encrypt “This is a test message for CTF” with shift 7, then crack it without knowing the shift. This is your first toy cryptanalysis!

**Why this project matters:** You just implemented the core of many easy CTF crypto challenges. Understanding character shifting and brute-force will help with XOR and ROT13 later.

### PS1-4: OverTheWire Bandit – Levels 0 to 4
Go to [OverTheWire Bandit](https://overthewire.org/wargames/bandit/). Follow the instructions to connect via SSH (`ssh bandit0@bandit.labs.overthewire.org -p 2220`).

**Your mission for each level:** After capturing the password, write a **one-paragraph write-up** in a file `bandit_notes.md` explaining:
- What concept or skill you used (e.g., `ls -a` to find hidden files, `cat` with special filename).
- Any errors encountered and how you solved them.

**Guided walkthrough spirit (no spoilers):**
- **Level 0:** Read the page carefully to log in. The password is right there.
- **Level 0→1:** You’re in a server. Use `ls` to see a file. Read it. It might be tricky if the file name starts with a dash; try `cat ./-` or `cat -- -` or just look at the hint.
- **Level 1→2:** Find a hidden directory, find a file with spaces in its name. Use quotes or escapes.
- **Level 2→3:** Find a file with a dash in the name. Use a different approach.
- **Level 3→4:** Hidden files in a directory; look for a clue in the file.

**For each, after you get the password, exit (`logout` or `Ctrl+D`), then connect to the next level using the new password.**

**Why Bandit?** It’s a low-pressure introduction to the mindset of “find the hidden thing”. You’ll practice the terminal skills from Part 1 in a gamified way.

### PS1-5: Mind Map – Python Object Types
Get a piece of paper or use a digital tool (draw.io, Miro). Create a mind map with the central node “Python Data Model”. Branches:
- **Immutable:** `int`, `float`, `str`, `tuple`
- **Mutable:** `list`, `dict`, `set`
Under each, note a few key methods or behaviors (e.g., `list.append`, `str.upper`). Draw connections: `str` slicing, `dict` key lookup.
**Deep thought question:** Why would you use a tuple instead of a list? (Because you want to ensure it doesn’t change, and it can be used as a dict key. In CTFs, you’ll use tuples to represent fixed immutable ID sequences.)

### PS1-6: Reflection Log
Create a file `week1_reflection.md`. Write:
1. What concept was hardest for you today, and why?
2. One thing you built that you’re proud of.
3. How does the CLI feel now compared to when you started?
4. One question you have about Python or the shell.

Revisit this at the end of the week. It’s your learning journal.

---

## 🔥 WEEK 1 DEEP DIVE WRAP-UP

You’re not meant to just “complete” these tasks; you’re meant to **live inside them**. Type every line of code yourself. Break things on purpose: try to divide by zero, access an index out of range, misspell a variable—then read the error and fix it.

By the end of this week, you’ll have:
- A working Python environment and terminal fluency.
- The ability to write scripts with loops, conditionals, functions, and basic data structures.
- A cipher cracking tool.
- Access to the Bandit server and the habit of documenting your steps.
- A notes system that will become your second brain.

Every skill you practiced here will be used next week when we move into algorithmic thinking and testing, and they will echo all the way into Phase 2’s binary exploitation. Now, close this document (just for now), open your terminal, and **type** your first command into Week 1’s battlefield.

**Let’s begin.**
