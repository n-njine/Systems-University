We’re building on the rock-solid Week 1. You’re comfortable in the CLI, you can think in Python, and you’ve already cracked Caesar ciphers and bandit levels. Now it’s time to make your code **efficient**, **correct**, and **unstoppable**.

This week is about transforming you from someone who writes scripts that *happen to work* into someone who **thinks like a computer scientist**. You’ll learn to measure time, master recursion, debug fearlessly, and harness randomness. These are the skills that separate a script kiddie from a CTF competitor who can reverse a binary’s custom search algorithm, brute‑force a password space intelligently, or fuzz a program with statistical precision.

# 🔥 WEEK 2: INTENSIVE DEEP DIVE
## *“Efficiency & Correctness”*

---

## ⏳ Session Overview (8‑Hour Immersive Block)

| Time (total) | Section |
|--------------|---------|
| 0:00 – 1:30 | Big‑O Notation & Complexity |
| 1:30 – 2:30 | Sorting Algorithms |
| 2:30 – 3:30 | Testing & Debugging |
| 3:30 – 4:30 | File I/O & Modules |
| 4:30 – 6:00 | Recursion (Deep Dive) |
| 6:00 – 7:00 | Monte Carlo Simulation |
| 7:00 – 8:00 | Problem Set Kickoff & Reflection |

---

## ⏱️ PART 1: BIG‑O NOTATION & COMPLEXITY (1.5 HOURS)

### 1.1 Why Does Efficiency Matter for CTFs?
In a CTF, you might need to brute‑force a 6‑character password. That’s trivial (a few seconds). But what if the key space is all possible 20‑byte sequences? An O(2ⁿ) algorithm will run until the heat death of the universe. Understanding complexity lets you know **instantly** whether an approach is feasible, and it helps you spot the hidden shortcut the challenge author left.

### 1.2 Counting Operations
Start with linear search. Given a list of `n` numbers, find `target`.

```python
def linear_search(arr, target):
    for i in range(len(arr)):
        if arr[i] == target:
            return i
    return -1
```

**Trace:** In the worst case, how many comparisons does it make? Exactly `n`. We say it’s **O(n)**: the time grows linearly with the input size.

**Now binary search** (requires sorted list):
```python
def binary_search(arr, target):
    low, high = 0, len(arr)-1
    while low <= high:
        mid = (low + high) // 2
        if arr[mid] == target:
            return mid
        elif arr[mid] < target:
            low = mid + 1
        else:
            high = mid - 1
    return -1
```

**Trace worst case:** Each iteration halves the search space. For `n` elements, maximum iterations ≈ log₂(n). So **O(log n)**.

**Code‑along:** Create a list of 1,000,000 ordered integers. Use `time.time()` to measure how long each search takes for the worst case (target not in list). Compare:
```python
import time, random
n = 10**6
data = list(range(n))
target = -1  # not present

start = time.time()
linear_search(data, target)
print("Linear:", time.time() - start)

start = time.time()
binary_search(data, target)
print("Binary:", time.time() - start)
```
You should see a massive difference. This is the power of algorithmic choice.

### 1.3 Formalizing Big‑O
- **Big‑O (O)** describes an upper bound on growth.
- **Common complexities:** O(1), O(log n), O(n), O(n log n), O(n²), O(2ⁿ).
- **Rules of thumb:**
  - A single loop over `n` items → O(n).
  - Nested loop over `n` items → O(n²).
  - Loop that repeatedly divides problem size → O(log n).
  - Combinations multiply: two sequential O(n) loops → O(n + n) = O(n) (drop constants).

**Exercise:** Determine the complexity of:
```python
def mystery(n):
    count = 0
    for i in range(n):
        j = 1
        while j < n:
            j *= 2
            count += 1
    return count
```
(Answer: O(n log n) – outer loop n times, inner loop log₂(n) times.)

**CTF Preview:** In binary exploitation, you’ll often need to find the exact offset to overwrite the return address. If the offset is unknown, you can binary search the payload size: halfway crash? move down. No crash? move up. This is exactly O(log n) cookie‑cutting, and it saves immense time over brute‑forcing.

### 1.4 Measuring Growth Empirically
Write a small harness that times your functions for increasing `n` and plots a simple text graph (or if you want, install matplotlib and plot). This builds intuition for what O(n²) *feels* like when your program suddenly slows to a crawl.

---

## 📊 PART 2: SORTING ALGORITHMS (1 HOUR)

Sorting is a fundamental operation, and understanding its cost is critical for everything from database queries to manipulating payloads in a specific order.

### 2.1 Bubble Sort (the pedagogical bad boy)
```python
def bubble_sort(arr):
    n = len(arr)
    for i in range(n):
        for j in range(0, n-i-1):
            if arr[j] > arr[j+1]:
                arr[j], arr[j+1] = arr[j+1], arr[j]
```
**Complexity:** Outer loop → n, inner loop → up to n → O(n²). Count comparisons: about n(n‑1)/2.

**Interactive task:** Implement it and test on `[5,2,9,1,5,6]`. Print the list after each pass. Notice how the largest element “bubbles” up.

### 2.2 Selection Sort
```python
def selection_sort(arr):
    for i in range(len(arr)):
        min_idx = i
        for j in range(i+1, len(arr)):
            if arr[j] < arr[min_idx]:
                min_idx = j
        arr[i], arr[min_idx] = arr[min_idx], arr[i]
```
Also O(n²). Compare performance with bubble sort on a list of 10,000 random numbers.

### 2.3 Python’s Built‑in Sort (Timsort)
`arr.sort()` is O(n log n) and stable. It’s a hybrid merge‑insertion sort that exploits runs of already‑sorted data. You’ll rarely write your own sorting algorithm for production, but **understanding what’s under the hood lets you reason about worst‑case inputs**. For example, some naive quicksort implementations degrade to O(n²) if given sorted data. In a CTF, if you see a server that sorts user‑provided data and becomes slow, you might craft a pathological input to cause a denial‑of‑service (DoS).

**Exercise:** Write a function that generates the worst‑case input for selection sort (descending order). Confirm by counting comparisons.

---

## 🧪 PART 3: TESTING & DEBUGGING (1 HOUR)

Code that passes a couple of happy‑path tests is **not** correct code. You need to think adversarially about your own functions—exactly like an attacker.

### 3.1 Assertions as Contracts
Put `assert` statements at the start and end of functions to guarantee pre‑ and post‑conditions.
```python
def median(nums):
    """Return median of a non-empty list of numbers."""
    assert len(nums) > 0, "Empty list not allowed"
    sorted_nums = sorted(nums)
    n = len(sorted_nums)
    if n % 2 == 1:
        return sorted_nums[n // 2]
    else:
        return (sorted_nums[n//2 - 1] + sorted_nums[n//2]) / 2

# Test cases
assert median([5]) == 5
assert median([1,3]) == 2
assert median([1,2,3]) == 2
```
If any assert fails, you get an immediate, clear error instead of a silent wrong answer.

**Why CTF?** When writing an exploit, you’ll assert that the target program’s output contains a certain pattern to know your shellcode executed. Assertions are your automated sanity checks.

### 3.2 The Debugger: `pdb`
Python’s built‑in debugger is your time‑travel microscope.
1. Insert `breakpoint()` anywhere in your code (Python 3.7+).
2. Run the script. It will pause at that line and give you a `(Pdb)` prompt.
3. Commands:
   - `n` (next) – execute current line and stop at the next line in the same function.
   - `s` (step) – step into any function call.
   - `c` (continue) – run until next breakpoint or end.
   - `p variable` – print value.
   - `l` – list surrounding code.
   - `q` – quit.

**Debug this buggy function:**
```python
def average(lst):
    total = 0
    for i in range(len(lst) + 1):  # bug: off-by-one
        total += lst[i]
    return total / len(lst)

print(average([10,20,30]))
```
It will raise an `IndexError`. Add `breakpoint()` before the loop, step through, and watch `i` exceed the last index. Fix it. Now use `pdb` to set a conditional breakpoint: `b 4, i == len(lst)` to catch exactly when it goes out of bounds.

**In‑session drill:** Write a function `binary_to_decimal(bin_str)` that converts a binary string to integer (e.g., "1010" → 10). Include assertions that the string contains only 0s and 1s. Then deliberately introduce a bug (like ignoring the last bit) and use `pdb` to locate it.

### 3.3 Testing Philosophy
Write a separate `test_my_module.py` that imports your functions and runs a series of tests. This habit pays off immensely when your exploit script relies on helper functions that must not fail.

---

## 📁 PART 4: FILE I/O & MODULES (1 HOUR)

Real CTF data comes in files: packet captures, firmware dumps, encoded strings, logs. You need to read and write files effortlessly.

### 4.1 Reading and Writing Text Files
```python
# Reading whole file
with open('data.txt', 'r') as f:
    content = f.read()

# Reading line by line
with open('log.txt', 'r') as f:
    for line in f:
        print(line.strip())

# Writing
with open('output.txt', 'w') as f:
    f.write("Hello, CTF!\n")
```
The `with` statement ensures the file is closed automatically, even if an error occurs.

### 4.2 Parsing CSV Files
Given a file `grades.csv`:
```
name,score
alice,88
bob,92
charlie,85
```
Write a script that reads it and prints the name of the student with the highest score. Do it **first** using the `csv` module, and **then** manually using `split(',')` to understand the low‑level parsing.

**Manual approach:**
```python
with open('grades.csv') as f:
    header = f.readline()  # skip
    best_name, best_score = None, -1
    for line in f:
        name, score_str = line.strip().split(',')
        score = int(score_str)
        if score > best_score:
            best_score, best_name = score, name
    print(best_name, best_score)
```

**Using `csv` module:**
```python
import csv
with open('grades.csv') as f:
    reader = csv.DictReader(f)
    max_row = max(reader, key=lambda row: int(row['score']))
    print(max_row['name'], max_row['score'])
```

**CTF connection:** Many web challenges return data as CSV or JSON. You’ll often need to extract the flag from a large dump. Efficiency in parsing matters—`DictReader` is elegant but might be slower than manual splitting for huge files.

### 4.3 File‑Based Micro‑Project: Log Analyzer
Create a file `access.log` with fake Apache‑style entries (just copy‑paste a few lines from the internet). Write a script that counts how many times each IP address appears. Use a dictionary. This is a tiny forensics warm‑up.

---

## 🌳 PART 5: RECURSION DEEP DIVE (1.5 HOURS)

Recursion is a mindset shift. It’s also the foundation of many exploit techniques (return‑oriented programming essentially constructs a recursive chain on the stack). Embrace it.

### 5.1 The Anatomy of Recursion
Every recursive function must have:
1. **Base case(s):** the simplest input that stops the recursion.
2. **Recursive step:** breaks the problem into a smaller instance of the same problem and calls itself.

### 5.2 Classic Examples (Code Along)
**Factorial:**
```python
def fact(n):
    if n == 0:          # base case
        return 1
    return n * fact(n-1) # recursive step
```
Draw the recursion tree for `fact(4)`. Each call creates a new stack frame. When `n` hits 0, the frames unwind, multiplying.

**Fibonacci (Naive):**
```python
def fib(n):
    if n <= 1:
        return n
    return fib(n-1) + fib(n-2)
```
This is beautiful but **horrifically inefficient** O(2ⁿ). Compute `fib(35)` and watch it crawl.

**Discussion:** Why so slow? Because it recomputes the same values countless times. This is a perfect intro to **memoization** (caching). You can add a dictionary to store results. We’ll revisit when we cover dynamic programming.

### 5.3 Tower of Hanoi (Conceptual & Implementation)
You have three pegs and `n` disks of different sizes. Move all disks from peg A to peg C, never placing a larger disk on a smaller one.
```python
def hanoi(n, source, target, auxiliary):
    if n == 1:
        print(f"Move disk 1 from {source} to {target}")
        return
    hanoi(n-1, source, auxiliary, target)
    print(f"Move disk {n} from {source} to {target}")
    hanoi(n-1, auxiliary, target, source)

hanoi(3, 'A', 'C', 'B')
```
**Trace this on paper.** Understand that the function calls itself to move `n-1` disks out of the way. The number of moves is 2ⁿ - 1, exponential. This is why brute‑forcing some problems is impossible.

### 5.4 Recursion on Data Structures
**Sum of a list recursively:**
```python
def rec_sum(lst):
    if not lst:
        return 0
    return lst[0] + rec_sum(lst[1:])
```
This is elegant but creates many list slices, which is O(n²) in total! A better version uses an index parameter. Understand the trade‑off.

### 5.5 Maze Solving with Depth‑First Search (The Big Project)
This is the centerpiece of today’s recursion. You’ll write a solver that takes a 2D grid of 0s (path) and 1s (walls) and finds the exit from a start point.

**Grid representation:**
```python
maze = [
    [0, 1, 0, 0, 0],
    [0, 0, 0, 1, 0],
    [1, 1, 0, 1, 0],
    [0, 0, 0, 0, 0],
    [0, 1, 1, 1, 0]
]
start = (0,0); exit = (4,4)
```

**DFS algorithm:**
- Mark the current cell as visited (e.g., set to 2).
- If current is exit → success.
- For each neighbor (up, down, left, right):
  - If within bounds, is a path (0), and not visited → recursively try that direction.
  - If recursion returns true, propagate true upward.
- If all directions fail, backtrack (unmark cell, though not strictly necessary with marker) and return false.

**Write `solve_maze(maze, x, y, exit_x, exit_y, path)`.** The function prints the coordinates of the path when found, or "No path".

This is your first **state‑space search** —the same algorithm used by vulnerability scanners to map reachable code paths. In a CTF, you might use DFS to find a sequence of moves in a lock‑picking challenge.

---

## 🎲 PART 6: MONTE CARLO SIMULATION (1 HOUR)

Randomness isn’t just for games; it’s for estimating probabilities where analytical math is hard, for fuzzing, and for breaking crypto that uses bad randomness.

### 6.1 The `random` Module
```python
import random
random.random()       # float in [0.0, 1.0)
random.randint(1,6)   # dice
random.choice(['a','b','c'])
random.shuffle(lst)
```

**Seeding for reproducibility:** `random.seed(42)` makes the sequence deterministic, useful for debugging.

### 6.2 Estimating π by Throwing Darts
Imagine a unit circle inscribed in a unit square. The area of the circle is π/4. If you throw darts uniformly at the square, the proportion that land in the circle approximates π/4.

```python
import random

def estimate_pi(n):
    inside = 0
    for _ in range(n):
        x, y = random.random(), random.random()
        if x*x + y*y <= 1.0:
            inside += 1
    return 4 * inside / n

print(estimate_pi(100000))
```

Run with increasing `n`. The error decreases as 1/√n. This is your first Monte Carlo method.

### 6.3 Simulating a Gambling Game
Consider a simple game: flip a fair coin. Heads you win \$1, tails you lose \$1. Start with \$10, what’s the probability you go broke within 100 bets? Solve analytically? Hard. Monte Carlo: easy.

```python
def sim_broke(start=10, max_bets=100, trials=10000):
    broke = 0
    for _ in range(trials):
        money = start
        for _ in range(max_bets):
            if money == 0:
                broke += 1
                break
            if random.random() < 0.5:
                money += 1
            else:
                money -= 1
    return broke / trials

print(sim_broke())
```

This gives you a feel for the “gambler’s ruin” problem.

**CTF connection:** Fuzzers are a kind of Monte Carlo simulation—they randomly (or semi‑randomly) generate inputs and monitor for crashes. If you can model a program’s behavior, you can estimate how many fuzz iterations are needed to trigger a rare bug. Later you’ll build a mutation fuzzer; the same sampling mindset applies.

### 6.4 Plotting Convergence (Optional, but rewarding)
If you have matplotlib (`pip install matplotlib`), plot the running average of π estimate vs trial number. You’ll see it converge. This teaches you to visualize data, critical for analyzing side‑channel leakage (e.g., timing distributions).

---

## 📝 WEEK 2 PROBLEM SET (Daily 2‑4 Hours)

### PS2‑1: MIT 6.0001 Problem Set 1 – Credit Card Debt
Complete all three parts of PS1 from MIT OCW (available on MIT OpenCourseWare):
- **Part A:** Calculating the minimum fixed monthly payment to pay off debt in a year by brute force (loop increments by \$10).
- **Part B:** Same, but using bisection search (binary search) to find the exact payment to the cent. This is the **key learning** —you’ll implement a binary search over a continuous range with condition derived from a simulation loop.
- **Part C:** Apply it to a different case.

This problem bridges loops, conditionals, and binary search. It’s a classic.

### PS2‑2: HackerRank Python Practice
Go to [HackerRank Python](https://www.hackerrank.com/domains/python) and solve at least **10 problems** from the “Basic Data Types” and “Strings” sections. Some recommended ones:
- Lists: “List Comprehensions”, “Find the Runner‑Up Score!”, “Nested Lists”
- Strings: “sWAP cASE”, “String Split and Join”, “Mutations”, “Find a string”
- Additionally, solve “Introduction to Sets” and “Symmetric Difference” to get a first taste of sets.

These enforce Python fluency with data structures.

### PS2‑3: Project – Secure Password Generator & ZIP Cracker
1. **Generator:** Write a script `passgen.py` that generates a random password of specified length, ensuring it includes at least one lowercase, one uppercase, one digit, and one special character. It should be cryptographically random (use `random.SystemRandom` or `secrets` module, but for now `random` is fine). It takes length as command‑line argument.
2. **ZIP Cracker:** Create a password‑protected ZIP file containing a flag file (e.g., `flag.txt` with content `CTF{brut3_f0rc3_m4ster}`). Use the `zipfile` module. Write a script `zip_crack.py` that:
   - Takes a ZIP filename and a dictionary file path.
   - Opens the ZIP and attempts to extract `flag.txt` using each password in the dictionary.
   - If successful, prints the password and the flag.
   - **Bonus:** If dictionary fails, implement a brute‑force over all 4‑letter lowercase passwords (aaaa‑zzzz) and measure the time. This shows how small the space is for short passwords.
You are allowed to use the `zipfile` and `itertools` modules.

**CTF scenario:** Many challenges involve cracking a ZIP with a known small keyspace. This project makes you ready.

### PS2‑4: OverTheWire Bandit Levels 5‑8
Continue the Bandit wargame. For each level, after finding the password, update your `bandit_notes.md` with:
- The command(s) used.
- The vulnerability or misconfiguration exploited (e.g., file permissions, SUID bit, grep‑able word).
- Any pitfalls you encountered.

Levels 5‑8 introduce `file`, `find`, `grep`, and `/etc/passwd`‑style data. You’ll get comfortable with file system forensics.

### PS2‑5: Read Guttag Chapters 10‑11
- **Chapter 10 (Some Simple Algorithms):** Focus on search algorithms (binary search), hashing, and maybe sorting. You’ve implemented them, now read the formal treatment.
- **Chapter 11 (Plotting and More about Classes):** Skim plotting, but pay attention to the class section: inheritance, `__str__`, etc. We’ll dive into OOP next week, so this is a primer.

### PS2‑6: Write a “Binary Search Explained” Blog Post
Write a short, self‑contained explanation of binary search (the algorithm and its O(log n) complexity) as if teaching a peer. Include:
- The prerequisites (sorted array).
- Step‑by‑step example with a small array.
- A code snippet in Python.
- Why it’s faster than linear search, with a graph of runtime growth (even if hand‑drawn, insert a photo).
- A CTF‑related application: how you might use bisection to find a buffer overflow offset.

This solidifies your understanding and builds your communication skills—vital for writing clear CTF write‑ups.

### PS2‑7: Week 2 Reflection Log
Create `week2_reflection.md` and answer:
1. What was the hardest concept this week (Big‑O, recursion, or debugging?) and how did you overcome it?
2. Describe a moment when you used `pdb` to fix a bug that you couldn’t spot just by reading code.
3. How has your thinking about efficiency changed? Give an example where you chose a faster approach after analyzing complexity.
4. What was your proudest achievement from the problem set?

---

## 🔥 DEEP DIVE WRAP‑UP

This week, you’re no longer just a coder; you’re an **algorithmic thinker**. You’ve learned to:
- Measure and predict your code’s performance.
- Make algorithms work for you, not against you.
- Debug with surgical precision.
- Wield recursion to solve mazes and crack passwords.
- Use randomness to estimate the impossible.

Carry these skills forward. Next week, you’ll build real‑world‑ready object‑oriented systems and start laying the groundwork for your CTF tool library. The maze solver’s DFS will evolve into graph‑based fuzzers; the Monte Carlo simulation will become your side‑channel analysis; and the sorting knowledge will help you spot algorithmic vulnerabilities.

Close this document, open your terminal, and start writing the `estimate_pi` function. **Make it work, then make it fast.**

Let’s conquer Week 2.
