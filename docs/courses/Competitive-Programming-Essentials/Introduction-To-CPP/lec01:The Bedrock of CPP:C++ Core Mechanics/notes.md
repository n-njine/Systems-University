## Lesson 1 — The Bedrock of Competitive Programming: C++ Core Mechanics

Welcome. You are now sitting with me at the workbench of algorithms, and before we build anything clever we must learn to hold the hammer. This session will be long and demanding, but I promise: by the end you will understand exactly what every line of a simple C++ program does, why it works, and how to write your own without hesitation.

Do not simply read. Open your editor, type every piece of code I show you, run it, break it, fix it. That is the only way your fingers will learn what your brain thinks it already knows.

---

### 1. Getting Set Up
You need two things:
- A **compiler** (g++ on Linux/macOS, MinGW or Visual Studio on Windows).  
- A **text editor** or IDE (VS Code, Sublime, CLion — whatever feels fast).  

I will assume you can compile and run a `.cpp` file from the terminal. If you haven’t done that yet, stop and make sure the following compiles and prints `Hello, ICPC!`:

```cpp
#include <iostream>

int main() {
    std::cout << "Hello, ICPC!" << std::endl;
    return 0;
}
```

Compile with:  
`g++ -std=c++17 -o hello hello.cpp`  
Run with:  
`./hello`

If that works, we are ready.

---

### 2. The Skeleton of Every C++ Program
A competitive programming solution is always a single `.cpp` file with this structure:

```cpp
#include <bits/stdc++.h>       // includes almost every standard library
using namespace std;           // so we can write cout instead of std::cout

int main() {
    // your code here
    return 0;
}
```

- `#include <bits/stdc++.h>` is a non‑standard header that pulls in every STL file. It saves typing, but it only works with g++. Use it for CP.
- `using namespace std;` tells the compiler to look inside the `std` namespace for `cout`, `cin`, `vector`, etc. In large projects this is dangerous; here it saves precious time.
- `int main()` is the entry point. The operating system calls it.
- `return 0;` tells the system “everything finished correctly”. You can omit it in `main` (default is 0), but I’ll keep it explicit.

From now on, every example assumes the first three lines are there. I will show only the inside of `main`.

---

### 3. Variables and Primitive Types
A variable is a named box that holds a value. C++ is *statically typed* — you must declare the type.

**Common types you must memorise:**

| Type       | Typical size | Range (approx)                         | Usage                    |
|------------|--------------|----------------------------------------|--------------------------|
| `int`      | 4 bytes      | -2×10⁹ … 2×10⁹                        | loop counters, most work |
| `long long`| 8 bytes      | -9×10¹⁸ … 9×10¹⁸                     | large integers           |
| `float`    | 4 bytes      | ~7 decimal digits                      | avoid, use double        |
| `double`   | 8 bytes      | ~15 decimal digits                     | real numbers             |
| `char`     | 1 byte       | -128 … 127 or 0 … 255 (ASCII)         | single characters        |
| `bool`     | 1 byte       | `true` / `false`                       | flags                    |

Always use `int` unless you need a bigger range (e.g., factorials, prefix sums of large arrays) — then `long long`. Watch out: `int` may be only 2 bytes on ancient systems, but we assume modern 64‑bit.

**Example:**
```cpp
int a = 10;
long long big = 10000000000LL;   // LL suffix forces long long literal
double pi = 3.1415926535;
char letter = 'A';
bool ok = true;
```

Arithmetic works as expected: `+`, `-`, `*`, `/` (integer division truncates), `%` (modulo, only for integers).  
Try this:
```cpp
int x = 7, y = 2;
cout << x / y << endl;   // prints 3
cout << x % y << endl;   // prints 1
```

The compiler follows normal precedence; use parentheses to avoid doubt. `++` and `--` increment/decrement by 1.

---

### 4. Input and Output (cin / cout)
To read data and print results, use `cin` and `cout`. They work with the extraction `>>` and insertion `<<` operators.

**Reading integers:**
```cpp
int n;
cin >> n;
cout << "You typed " << n << endl;
```

You can chain multiple readings:
```cpp
int a, b;
cin >> a >> b;
cout << a + b << endl;
```

`cin` skips whitespace (spaces, newlines). So if the input is:
```
3 7
```
`a` gets 3 and `b` gets 7 regardless of whether they are on the same line.

**Reading a whole line:**
```cpp
string name;
getline(cin, name);
cout << "Hello, " << name << endl;
```

**The most common beginner trap:** mixing `cin >>` and `getline`. After `cin >>` a number, the newline is left in the buffer. If you then call `getline`, it immediately reads that empty newline. Fix: add `cin.ignore();` after `cin >>` when you plan to use `getline` next.

```cpp
int age;
cin >> age;
cin.ignore();               // discard the newline
string name;
getline(cin, name);
```

**Fast I/O (you will need this later):**
Add these two lines right at the start of `main`:
```cpp
ios_base::sync_with_stdio(false);
cin.tie(nullptr);
```
They turn off synchronisation with C’s stdio and untie `cin` from `cout`, making I/O several times faster. For now, you can live without them, but it’s good practice to include them early.

---

### 5. Control Flow – Making Decisions
`if`, `else if`, `else`:
```cpp
int score;
cin >> score;
if (score >= 90)
    cout << "A" << endl;
else if (score >= 80)
    cout << "B" << endl;
else
    cout << "Below B" << endl;
```

Curly braces `{ }` are mandatory if you have more than one statement in a branch. I always use them even for single statements — it prevents bugs.

**Comparison operators:** `==`, `!=`, `<`, `>`, `<=`, `>=`.  
**Logical operators:** `&&` (and), `||` (or), `!` (not).

---

### 6. Loops – Repeating Work
`for` loop for a known number of iterations:
```cpp
for (int i = 0; i < 5; ++i) {
    cout << i << " ";
}
// Output: 0 1 2 3 4
```

The three parts: initialisation; condition (run while true); update (run after each iteration).

`while` loop when the number of iterations is unknown:
```cpp
int n;
cin >> n;
while (n != 1) {
    // do something
    if (n % 2 == 0)
        n = n / 2;
    else
        n = 3 * n + 1;
    cout << n << " ";
}
```
(That’s the core of the Collatz / “Weird Algorithm” problem.)

`do-while` is rarely used; it executes at least once and checks the condition at the end.

**Loop control:** `break` exits the loop immediately, `continue` jumps to the next iteration.

---

### 7. Functions – Organising Your Thoughts
A function is a reusable block of code. It has a return type, a name, and parameters.

```cpp
int square(int x) {
    return x * x;
}

int main() {
    cout << square(5) << endl;   // prints 25
    return 0;
}
```

**Pass by value vs. pass by reference**  
By default, parameters are passed *by value* — the function gets a copy. Changes inside do not affect the original.

```cpp
void addOne(int x) {
    x = x + 1;     // only the local copy changes
}
```

If you want the function to modify the original variable, pass *by reference* using `&`:

```cpp
void addOneRef(int &x) {
    x = x + 1;     // modifies the variable that was passed
}
```

In competitive programming you will pass large data structures (vectors, strings) by reference to avoid expensive copies, even if you don’t intend to modify them. Use `const` reference for that:

```cpp
void print(const string &s) {
    cout << s << endl;
}
```

---

### 8. Recursion – Functions Calling Themselves
Recursion is a function that calls itself. It must have a **base case** to stop.

**Factorial:**
```cpp
int factorial(int n) {
    if (n == 0) return 1;             // base case
    return n * factorial(n - 1);      // recursive case
}
```

Work through an example on paper: `factorial(3)` expands to `3 * factorial(2)` → `3 * 2 * factorial(1)` → `3 * 2 * 1 * factorial(0)` → `3*2*1*1 = 6`.

**Fibonacci (warning: slow for large n):**
```cpp
int fib(int n) {
    if (n <= 1) return n;
    return fib(n-1) + fib(n-2);
}
```

Recursion is elegant but can cause stack overflow if the depth is too large (typical limit ~10⁵ calls). We will later learn to avoid that with iteration or DP.

---

### 9. Arrays – Storing a Sequence
An array is a fixed‑size contiguous block of memory. You declare it with a constant size:

```cpp
int arr[100];        // array of 100 ints, indices 0..99
```

**Filling and printing:**
```cpp
int n;
cin >> n;            // assuming n <= 100
for (int i = 0; i < n; ++i)
    cin >> arr[i];

for (int i = 0; i < n; ++i)
    cout << arr[i] << " ";
```

**Important:** C++ arrays don’t know their own size. You must keep track of it. Also, accessing `arr[100]` is an out‑of‑bounds error that may crash your program silently — be careful.

---

### 10. Strings – Text Is Essential
We use `std::string` almost exclusively. It handles memory automatically and offers many useful methods.

**Declaration and input:**
```cpp
string s;
cin >> s;               // reads one word (stops at whitespace)
```

**Read a whole line:**
```cpp
string line;
getline(cin, line);
```

**Common operations:**
```cpp
string s = "hello";
string t = "world";

string combined = s + " " + t;    // "hello world"
int len = s.length();             // 5
char c = s[1];                    // 'e'
s[0] = 'H';                       // now "Hello"

if (s == t) { ... }               // comparison works!

string sub = s.substr(1, 2);      // from index 1, length 2 -> "el"
size_t pos = s.find("ll");         // returns 2 (first position)
```

**C‑strings (historical context):**  
You might see `char s[100];` and functions like `strcpy`, `strcmp`, `scanf("%s", s)`. They are null‑terminated arrays of characters. In CP you almost never need them; `std::string` is safer and faster to code. But just know they exist.

---

### 11. Stringstream – Parsing Like a Pro
When you have a line containing several numbers separated by spaces, `stringstream` is your friend.

```cpp
#include <sstream>
...
string line = "3 14 15 9";
stringstream ss(line);
int x;
while (ss >> x) {
    cout << x * 2 << " ";
}
// Output: 6 28 30 18
```

It treats the string exactly like `cin` does with standard input. This is invaluable when input format is messy.

**Converting a string to an integer manually:**
```cpp
string num = "1234";
int val = stoi(num);       // stoi, stol, stoll, stod exist
```

---

### 12. Putting It All Together – A Mini Problem
Let’s write a program that reads an integer `n`, then reads `n` words and prints the longest one. If several have the same length, print the first one.

```cpp
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cin >> n;
    string best = "";
    for (int i = 0; i < n; ++i) {
        string current;
        cin >> current;
        if (current.length() > best.length())
            best = current;
    }
    cout << best << endl;
    return 0;
}
```

Experiment: add a loop that also prints the length. Test with your own input.

---

### 13. Your First Real Training Problems
Now you have all the tools needed to solve real CP problems. **Do not skip this part.** Set up accounts on [Codeforces](https://codeforces.com) and [CSES](https://cses.fi) if you haven’t already. Then solve these problems **today**, in this order:

1. **Weird Algorithm** (CSES 1068)  
   Input `n`, simulate the Collatz sequence. Print the numbers separated by space.  
   *Hint:* Use a `while (n != 1)` loop, `long long` for safety (n can be up to 10⁶ but the sequence can exceed int range).

2. **Missing Number** (CSES 1083)  
   You are given all numbers from 1 to `n` except one. Read them and find the missing one.  
   *Hint:* Compute the expected sum `n*(n+1)/2` and subtract the sum you read. No array needed.

3. **Repetitions** (CSES 1069)  
   Read a DNA string and find the longest run of the same character.  
   *Hint:* Loop through the string, keep a counter, reset when character changes.

4. **Watermelon** (CF 4A)  
   Input a weight `w`. Output “YES” if it can be divided into two even parts (each > 0), else “NO”.

5. **Way Too Long Words** (CF 71A)  
   Read `n`, then for each string: if length ≤ 10 print it, otherwise print first char, length‑2, last char.

6. **Team** (CF 231A)  
   For each of `n` problems, read three integers (0/1). Count how many problems have at least two ones.

Write your solutions, compile, and test locally before submitting. If you get stuck, read the problem statement again, trace your logic on paper, and only then look at my hints. The struggle is where the learning happens.

---

### Summary – What You Now Own
After this one session, you have:
- The boilerplate of every C++ file.
- Command of variables, operators, and I/O.
- Decision‑making with `if`.
- Repetition with `for` and `while`.
- Functions, including by reference.
- Recursion (factorial, Fibonacci).
- Arrays and `std::string` with essential methods.
- `stringstream` for parsing.
- A set of solved CP problems under your belt.

This may feel like a lot, but you have now planted the seeds. Every future lesson will build directly on these roots, and you will never again be confused by a basic loop or an `if` statement.

**Before the next session, do this:**
- Re‑solve the six problems without looking at your old code.
- Write a small program that reads a full name (first and last), then prints initials (e.g., “John Doe” → “J.D.”) using `getline` and `stringstream`.
- Experiment with recursion: write a function that prints the numbers from n down to 1 using recursion, then prints them back up.

In Week 2 we will dive into the STL — vectors, sets, and maps that will make you 10 times faster. You are ready. See you then, and keep coding.
