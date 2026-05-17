## Lesson 3 — The Complexity Compass & The Art of Debugging

Last week you learned to wield the STL. This week I will teach you to *think* before you type, and to *heal* your own code. No amount of clever algorithms will save you if you habitually submit solutions that run forever or crash on hidden test cases. This lesson will turn you into a programmer who can look at a problem’s constraints and instantly know what is possible, and who can track down a bug without random guessing.

We are going to do three things today:
1. Master Big‑O notation and the “time‑limit clock”.
2. Build a systematic debugging workflow.
3. Write a stress‑testing harness that will make you invincible against Wrong Answer.

Open your editor. You will be running code, breaking it, and fixing it.

---

### Part 1 — The Clock Is Ticking: Complexity and Time Limits

#### 1.1 What Is Big‑O?
Big‑O notation is how we describe how the runtime of an algorithm grows as the input size grows. It ignores constants and looks at the dominating term.

| Code pattern                                          | Complexity      | When N=10⁵ (rough ops) |
|-------------------------------------------------------|-----------------|-------------------------|
| A single loop over N                                  | O(N)            | 10⁵                    |
| Loop inside loop (both N)                             | O(N²)           | 10¹⁰ → too slow!       |
| Sorting                                              | O(N log N)      | ~1.7×10⁶               |
| Binary search over N                                  | O(log N)        | ~17                    |
| Generating all subsets                                | O(2ᴺ)           | impossible for N>20    |
| All permutations                                      | O(N!)           | impossible for N>11    |

A typical CPU can do about **10⁸ simple operations per second**. Most online judges give you 1–2 seconds. So the **hard upper bound** is roughly 10⁸ operations for C++. If your algorithm will perform many more than that, it will TLE (Time Limit Exceeded).

#### 1.2 Translating Constraints to Algorithms
When you read a problem, look at the maximum value of N. Here’s the cheat sheet:

- **N ≤ 10⁶ (or 10⁷)** → O(N) or O(N log N) is required. A simple loop is fine, sorting is fine.
- **N ≤ 10⁵** → O(N log N) is very safe. O(N√N) might pass if constant is tiny, but avoid O(N²).
- **N ≤ 5000** → O(N²) is okay (25 million operations). O(N² log N) borderline.
- **N ≤ 500** → O(N³) might pass (125 million ops, risky but possible with simple inner code).
- **N ≤ 20–25** → O(2ᴺ) subset generation works.
- **N ≤ 10** → O(N!) permutations.

**Example:** If a problem says *n ≤ 200 000*, you are allowed a single loop over the array, maybe sorting, maybe a map/set insertion, but **not** a loop within a loop over all pairs. That immediately tells you: you need O(n log n) or O(n) with some clever data structure, not brute‑force.

**Exercise:** For each of these constraints, quickly decide if O(N²) is safe:
1. N ≤ 1000 → yes (1 million ops)  
2. N ≤ 10⁵ → no (10 billion)  
3. N ≤ 3000 → yes (9 million, safe)  

#### 1.3 The Constant Factor Trap
O(N log N) with a heavy constant (like `map` instead of `unordered_map`, or frequent `push_back` without `reserve`) can still TLE. In C++:
- `endl` is slower than `'\n'` because it flushes the output buffer every time. Use `'\n'` and only add `endl` or `flush` when you absolutely need the output to appear immediately (almost never in CP).
- `cin`/`cout` without `ios_base::sync_with_stdio(false); cin.tie(nullptr);` can be 10× slower.
- Modulo operations `%` are relatively slow; don’t put them inside the innermost loop if you can accumulate and modulo once.
- Recursion depth can cause stack overflow. Default stack size is about 8 MB, enough for ~10⁵ recursion depth. If you need deeper, use an explicit stack or iteration.

#### 1.4 Complexity Nailing Practice
Let’s analyze some snippets. **Don’t just guess – compute the number of operations for N=10⁵.**

**Snippet A:**
```cpp
for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
        // simple operation
    }
}
```
O(N²) → 10¹⁰ operations → TLE.

**Snippet B:**
```cpp
for (int i = 0; i < n; i++) {
    for (int j = 0; j < i; j++) {
        // ...
    }
}
```
The inner loop runs 0+1+2+...+(N‑1) = N(N‑1)/2 ≈ 5×10⁹ for N=10⁵. Still O(N²), still TLE.

**Snippet C:**
```cpp
for (int i = 0; i < n; i += 2) { ... }
```
O(N), fine.

**Snippet D:**
```cpp
for (int i = 1; i <= n; i *= 2) { ... }
```
O(log N), extremely fast.

**Snippet E (common with `string`):**
```cpp
string s = "";
for (int i = 0; i < n; i++) s += 'a';
```
Looks O(N), but `+=` on a string is amortized O(1). It’s fine. However:
```cpp
string s = "";
for (int i = 0; i < n; i++) s = s + 'a';
```
This creates a new temporary string every time, O(N²). That’s a disaster. Use `+=` or `push_back`.

**Snippet F:**
```cpp
vector<int> v;
for (int i = 0; i < n; ++i) v.insert(v.begin(), i);
```
Each `insert` at the beginning shifts all existing elements, total O(N²). Instead, use `push_back` and `reverse`, or a `deque`.

Now you can read a problem’s constraints and predict the necessary complexity. But you also need to be able to *detect* when your code is slower than you think.

---

### Part 2 — The Debugging Mindset

Debugging is not a punishment; it’s a skill. The best programmers spend as much time testing and fixing as they do writing. You will learn a systematic method so that when you get WA (Wrong Answer) or TLE, you are not lost.

#### 2.1 The First Line of Defense: Print Debugging
The simplest tool: `cout`. When logic fails, print the values of key variables at critical points. Use a distinctive format and remove them before submission (or wrap them in `#ifdef` flags).

```cpp
#define DEBUG true
#if DEBUG
#define dbg(x) cerr << #x << " = " << x << endl
#else
#define dbg(x)
#endif
```

`cerr` prints to standard error, which you can see in the judge’s log on some platforms, but safer to use `cout` during local testing. I often just use:

```cpp
cout << "i: " << i << " arr[i]: " << arr[i] << '\n';
```

#### 2.2 `assert` – Your Guard Dog
`assert(condition);` crashes the program with a message if the condition is false. Use it to check your invariants.

```cpp
int main() {
    int n;
    cin >> n;
    assert(n >= 1 && n <= 100000);    // sanity check constraints
    vector<int> v(n);
    // ...
}
```

During testing, if something impossible happens, `assert` will catch it immediately. **Always compile with assertions enabled.** (They are on by default; `-DNDEBUG` disables them.)

#### 2.3 Small Test Cases & Hand‑Tracing
If your program fails on a large input but passes tiny ones, create the smallest failing case manually. Reduce the problem size until you can walk through your algorithm with pencil and paper. This often reveals off‑by‑one errors or misunderstood statements.

#### 2.4 Brute‑Force and Stress Testing – The Ultimate Weapon
For many problems, you can write a simple, obviously correct but slow solution (e.g., complete search) for small N. Then run it against thousands of random test cases, comparing its output to your fast solution’s output. If they differ, you have a test case to debug.

I will now teach you to build a **stress testing script** in C++. This is the single most powerful habit you can adopt.

---

### Part 3 — Building a Stress Tester

#### 3.1 Architecture
We’ll write three functions:
1. `brute(vector<int>& arr)` — the naive O(N²) correct solution.
2. `solve(vector<int>& arr)` — your optimized solution.
3. `main()` — generates random arrays, feeds them to both, and compares.

#### 3.2 Example Problem: “Count Inversions”
*Given an array, count pairs (i,j) with i<j and A[i] > A[j].*

The brute‑force solution is O(N²): check all pairs.
The fast solution uses a Fenwick tree or merge sort (O(N log N)) but for this demo we might implement merge sort.

Let’s write the stress test for a simpler problem: “Find maximum subarray sum” (Kadane vs O(N²) brute).

**Brute:**
```cpp
int brute(vector<int>& a) {
    int n = a.size();
    int best = a[0];
    for (int i = 0; i < n; i++) {
        int sum = 0;
        for (int j = i; j < n; j++) {
            sum += a[j];
            best = max(best, sum);
        }
    }
    return best;
}
```

**Fast (Kadane):**
```cpp
int solve(vector<int>& a) {
    int best = a[0], cur = a[0];
    for (int i = 1; i < a.size(); i++) {
        cur = max(a[i], cur + a[i]);
        best = max(best, cur);
    }
    return best;
}
```

**Stress test main:**
```cpp
int main() {
    srand(time(0));
    for (int test = 0; test < 10000; test++) {
        int n = rand() % 10 + 1;          // small size
        vector<int> a(n);
        for (int i = 0; i < n; i++)
            a[i] = rand() % 20 - 10;      // random between -10 and 9
        int res1 = brute(a);
        int res2 = solve(a);
        if (res1 != res2) {
            cout << "Mismatch on test " << test << endl;
            cout << "Array: ";
            for (int x : a) cout << x << ' ';
            cout << "\nBrute: " << res1 << " Fast: " << res2 << endl;
            return 1;
        }
    }
    cout << "All tests passed!" << endl;
    return 0;
}
```

Run this. If it prints “All tests passed!”, your fast solution is probably correct. If there is a mismatch, you have a concrete small case to debug.

**Make it a template** you can adapt for any problem. Replace `brute` and `solve` with the relevant functions, adjust constraints and input generation.

#### 3.3 How to Use This in Real Contests
During a contest, after you write your optimized solution, quickly code a brute‑force for the same problem (if you can think of one that works for small inputs). Run a few hundred random tests. This catches logic errors before you submit. For example, you can stress‑test your greedy decision, your DP, your graph algorithm against BFS brute.

---

### Part 4 — Debugging a Real Mess: Live Walkthrough

Let’s debug a deliberately broken code together. I’ll give you a snippet that attempts to solve “Missing Number” (CSES) but fails. Your job is to fix it using the tools we just learned.

```cpp
int main() {
    long long n;
    cin >> n;
    long long sum = 0;
    for (int i = 0; i < n - 1; i++) {
        int x;
        cin >> x;
        sum += x;
    }
    long long expected = (n * (n + 1)) / 2;
    cout << expected - sum << endl;
    return 0;
}
```

Looks fine? Try with n=1, input nothing (since 0 numbers are given). The loop doesn’t run, sum=0, expected = 1*2/2 = 1, output 1. Correct? But wait, the problem says n ≥ 2. So n=1 isn’t tested. But there is a subtle bug: when n is large, `n * (n + 1)` can overflow a 64‑bit integer? n ≤ 2·10⁵, n*(n+1) ≈ 4·10¹⁰, fits in `long long`. So what’s wrong? The sum of inputs is also within range. Actually the code is correct for the problem. Let’s introduce a real bug: forget that the input numbers are not necessarily distinct? No, the problem guarantees distinct. I want to show a case where output is large and wrong: use `int` for sum and expected. Change `sum` and `expected` to `int`. Then overflow kills you. That’s a classic.

Now let’s walk through debugging with print statements. Add `cout << "sum: " << sum << " expected: " << expected << endl;` and you’ll see negative numbers. The fix: use `long long`.

Now, suppose we have a wrong solution for “Concert Tickets” (CSES 1091) that uses a vector, sorts it, and for each customer iterates from the end and takes the first ticket ≤ bid, then erases it. This is O(N·M) and TLEs. Your debugger won’t show anything except “TLE”. How to find it? Time complexity analysis: N, M up to 2·10⁵, N·M = 4·10¹⁰ → must be faster. Use `multiset` with `upper_bound`.

But sometimes you get WA, not TLE. You need a small test case where your set logic fails. Stress test against a brute that tries all possible allocations. That’s the mindset.

---

### Part 5 — Practical Exercises: Building Complexity Intuition

I will give you code snippets. Your task: determine the time complexity, then estimate if it will pass for N=10⁵, and if not, propose a fix.

**Exercise 1:**
```cpp
string s;
for (int i = 0; i < n; i++) {
    s += 'a';
    reverse(s.begin(), s.end());
}
```
Complexity: reverse takes O(|s|) each time, loop runs n times, total O(n²). TLE. Fix: remove the reverse (or use deque).

**Exercise 2:**
```cpp
set<int> s;
for (int i = 0; i < n; i++) s.insert(i);
for (int i = 0; i < m; i++) {
    s.erase(i);
}
```
O((n+m) log n), fine.

**Exercise 3:**
```cpp
vector<int> v;
for (int i = 0; i < n; i++) v.push_back(i);
for (int i = 0; i < m; i++) {
    v.erase(v.begin());
}
```
`v.erase(begin)` shifts all remaining elements each time, O(m·n). Fix: use `deque` or just don’t erase from front.

**Exercise 4:**
```cpp
int sum = 0;
for (int i = 0; i < n; i++) {
    for (int j = 1; j <= i; j *= 2) {
        sum++;
    }
}
```
Inner loop runs log(i) times, total O(n log n). Safe.

Now you can predict performance.

---

### Part 6 — Problem Set: Learn by Debugging Real Submissions

You will now take code that almost works and fix it. I’ve prepared three “broken” solutions. For each, first **identify the bug without running** (by thinking about complexity or edge cases), then write the correct version.

#### Problem 1: CF 479A – Expression (modified)
Given three numbers a,b,c (1≤a,b,c≤10), insert + or * to maximize result. Constraints tiny. A beginner writes:
```cpp
int main() {
    int a,b,c;
    cin>>a>>b>>c;
    cout << max(a+b*c, a*b+c) << endl; // only two options? wrong
}
```
Bug: Not all possibilities (a+b+c, a*b*c, (a+b)*c, a*(b+c)). But the problem’s original is to maximize with any placement, including parentheses. The given code misses some. Fix by checking all 6 combinations (only three numbers, so it’s easy). Use brute force: store all possible values in an array and take max. Complexity O(1). This teaches you to never guess the combination; always brute force when small.

#### Problem 2: CF 579A – Raising Bacteria
A beginner writes:
```cpp
int main() {
    int x; cin >> x;
    int ans = 0;
    while (x > 0) {
        if (x % 2 == 1) ans++;
        x /= 2;
    }
    cout << ans << endl;
}
```
This is actually correct (counts set bits). But many try to simulate: subtract the largest power of two each time, which is equivalent. However, if someone wrote a simulation with a loop finding powers each time, it could be O(log² x). Not a bug, but I want to illustrate that sometimes the correct answer is a one‑liner. I’ll instead provide a TLE‑prone simulation and ask them to optimize it to O(log x). For this exercise, I’ll design a broken simulation:

```cpp
int main() {
    int x; cin >> x;
    int ans = 0;
    while (x > 0) {
        int p = 1;
        while (p*2 <= x) p *= 2; // finds highest power
        x -= p;
        ans++;
    }
    cout << ans << endl;
}
```
This is O(number of bits * log x)? Actually inner while runs log x times each outer loop, and outer loop runs ans times (number of set bits), which is at most log x. So O(log² x), still fast. Not good for TLE demo. Let’s choose a different problem: “313A – Ilya and Bank Account” is trivial. I want a problem where a nested loop causes TLE. The plan had “Raising Bacteria” but it’s bit manipulation. I’ll pick a self‑contained example: given an array of n up to 10⁵, find pairs with sum zero using nested loops O(n²) and fix with map O(n). I’ll provide that as a broken code.

So I’ll craft a fictional problem: “Count pairs (i,j) with i<j such that A[i] + A[j] == 0.” Brute force O(n²) will TLE for n=10⁵. The learner must replace with an unordered_map frequency counter, counting `-A[i]`. I’ll present the brute code.

#### Problem 3: TLE code for Two Sum (from CSES 1640, but we need indices not count). I’ll use the earlier CSES “Sum of Two Values” where you must output indices. The brute checks all pairs O(n²) and TLEs. They must implement the map method. I’ll show a broken version: using map but incorrectly storing indices, e.g., not checking i != j. Or a naive O(n²) that they must transform.

I’ll present a fully brute‑force solution and ask them to speed it up.

**Exercise 2 (TLE transformation):**
```cpp
int main() {
    int n, x;
    cin >> n >> x;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            if (a[i] + a[j] == x) {
                cout << i+1 << " " << j+1 << endl;
                return 0;
            }
        }
    }
    cout << "IMPOSSIBLE" << endl;
}
```
Constraints: n ≤ 2·10⁵. This will TLE. They need to rewrite using `unordered_map<int, int>` storing value → index as they scan. That’s the assignment.

**Exercise 3: Stress test your solution.** After fixing the above, write a brute‑force version (the nested loops) that works for n ≤ 100, and a random test generator that compares your fast solution with the brute. Run it for 1000 tests. This solidifies the habit.

---

### Part 7 — Your Assignment for the Week

1. **Solve these problems with a self‑imposed “complexity analysis first” rule.** Before writing code, write down the max input size, the required complexity, and your algorithm’s complexity on a piece of paper. Then code.

   - **CSES 1640: Sum of Two Values** (done in exercise, but now formal submission)
   - **CSES 1091: Concert Tickets** (use multiset, ensure O((n+m) log n))
   - **CF 466C: Number of Ways** (prefix sums, O(n))
   - **CF 230B: T‑primes** (precompute primes, O(√max) + O(n))
   - **CF 977C: Less or Equal** (sort, O(n log n))
   
2. **TLE‑to‑AC transformation:** I’ll give you a slow code that solves “Maximum Subarray Sum” in O(n²) (two nested loops). Your job: replace it with Kadane’s algorithm O(n). Then write a stress test that compares both on random arrays of size up to 100.

3. **Debugging drill:** I intentionally wrote a buggy solution for “Concert Tickets” using `set` and `upper_bound` but forgetting to check the case where `upper_bound` returns `begin()` (meaning no ticket ≤ bid). Find the bug, fix it, and stress test against a brute that scans all tickets each query (slow but correct for small n,m).

4. **Complexity quiz:** For each of the following problem constraints, write down the maximum acceptable big‑O:
   - n ≤ 10⁷, one second: O(n) only.
   - n ≤ 2·10⁵: O(n log n) or O(n).
   - n ≤ 10³: O(n²) fine.
   - n ≤ 16: O(2ⁿ) subset.
   - n ≤ 10⁹ (only one number): O(1) or O(log n).

---

### Part 8 — What You Carry Forward

You now possess the coder’s equivalent of X‑ray vision. When you see a problem, you will immediately scan the constraints, calculate the maximum number of allowed operations, and decide what algorithm class is viable. You will no longer blindly submit O(N²) solutions when N is 200,000. And when you do get a wrong answer, you will not stare at the screen; you will build a tiny test case, compare against a brute force, and isolate the bug scientifically.

This is a superpower that separates amateurs from competitors. Use it relentlessly.

**Before Week 4, complete the assignments. The stress‑testing script must become a permanent tool in your toolbox.** In our next session, we will dive into recursion, complete search, and backtracking — the first truly algorithmic weapon that opens the door to a huge class of problems.

Now go. Open your editor, type the stress tester template, and run it on the exercises. I will be here when you’re ready for the next step.
