## Lesson 6 — Binary Search: The Most Powerful Four‑Letter Word in CP

If last week’s tools were swords, this week’s is a scalpel. With binary search you will cut through problems that initially look impossible. You will learn to search not only over sorted arrays, but over *all possible answers*, narrowing down a universe of trillions of candidates to a single correct result in a few dozen steps.

This is the week when many beginners transform into confident problem‑solvers. I will walk you through every variant, every trap, every mental model. By the end you will be able to write a bug‑free binary search without thinking, and you will have developed a “binary‑search reflex” that lights up whenever you see a problem asking for “minimum maximum” or “maximum minimum”.

Open your editor. I am going to give you templates, and you are going to type them, test them, and then use them on real problems. Do not move on until you can write each template from memory.

---

### Part 1 — The Core Idea: Halving the Search Space

You have a sorted array `A` of length `n`. You want to find whether a value `x` exists, and if so, where. The naive approach scans left to right in O(n). Binary search cuts the search space in half at each step, giving O(log n). For n = 10⁹, that’s 30 steps instead of 10⁹ — the difference between an instant answer and a time‑limit exceeded.

But binary search is far more general than arrays. It works on any **monotonic predicate**: a function `bool P(i)` that is false for some prefix and true for the remaining suffix (or vice versa). If you can formulate your problem as finding the first `i` where `P(i)` is true, binary search will find it in O(log n) evaluations of `P`.

---

### Part 2 — Binary Search on a Sorted Array

#### 2.1 `lower_bound` and `upper_bound` — Use the STL
The standard library gives you these ready‑made. I demand that you learn them by heart, but you must also know how to write your own because sometimes you need a custom condition.

```cpp
vector<int> v = {1, 2, 2, 3, 4, 4, 4, 5};

auto it1 = lower_bound(v.begin(), v.end(), 3);   // first element >= 3  -> points to 3
auto it2 = lower_bound(v.begin(), v.end(), 4);   // first element >= 4  -> first 4
auto it3 = upper_bound(v.begin(), v.end(), 4);   // first element > 4   -> 5
auto it4 = lower_bound(v.begin(), v.end(), 6);   // returns v.end() if not found

int idx = it1 - v.begin();                       // index
```
Time complexity: O(log n) for random‑access iterators (vector). For `set` and `map`, use their member functions `.lower_bound()` and `.upper_bound()` — these are O(log n) whereas `std::lower_bound` on a set is O(n).

#### 2.2 Writing Your Own `lower_bound` — Two Templates
You need a reusable, bug‑free template for binary search on an array. There are several correct patterns; I will give you two that are impossible to mess up once you understand the invariant.

**Template A: `while (lo < hi)` — answer converges to single index**
Use this when you want to find the first index where a condition holds.
```
// F F F T T T   <- find first T
lo = 0, hi = n;  // hi is exclusive, we can never evaluate hi itself
while (lo < hi) {
    int mid = lo + (hi - lo) / 2;   // bias left
    if (condition(mid))             // condition true
        hi = mid;                   // keep mid as possible answer
    else
        lo = mid + 1;              // discard mid
}
// lo is the answer (first index where condition is true), or lo == n if all false
```

**Template B: `while (lo <= hi)` — explicit answer variable**
This is more natural when searching for an exact value.
```
lo = 0, hi = n-1;
int ans = -1;
while (lo <= hi) {
    int mid = lo + (hi - lo) / 2;
    if (a[mid] == target) {
        ans = mid;
        break; // or hi = mid-1 for first occurrence, lo = mid+1 for last
    } else if (a[mid] < target)
        lo = mid + 1;
    else
        hi = mid - 1;
}
```

For finding the **first** occurrence of a value, we can modify the condition to continue searching left when we find a match:
```
int first = -1;
while (lo <= hi) {
    int mid = lo + (hi - lo) / 2;
    if (a[mid] >= target) {
        if (a[mid] == target) first = mid;
        hi = mid - 1;   // search left
    } else {
        lo = mid + 1;
    }
}
```

**Crucial detail:** Never use `mid = (lo + hi) / 2` when `lo+hi` can overflow. Use `lo + (hi - lo) / 2` or `mid = ((unsigned int)lo + (unsigned int)hi) >> 1` (advanced). I will use the safe form always.

#### 2.3 Practice: Find the First Element Greater Than X
Write your own implementation of `upper_bound` using Template A:
```cpp
int upperBound(const vector<int>& a, int x) {
    int lo = 0, hi = a.size();
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (a[mid] > x)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;  // could be a.size() if all elements <= x
}
```
Test it immediately against STL results on several arrays.

---

### Part 3 — Binary Search on the Answer: The Real Magic

This is the skill that separates competitors from casual coders. Many problems can be rephrased as:

> *Find the smallest X such that some property P(X) holds, where P is monotonic (if P(x) is true, then P(y) is true for all y > x).*

or the reversed variant:
> *Find the largest X such that P(X) holds, where P is monotonic decreasing.*

You then perform binary search over the possible answer range, and for each candidate answer you check feasibility in O(n) or O(n log n). The overall complexity becomes O(log(range) × cost of check). Since range is often up to 10⁹ or 10¹⁸, log(range) ≈ 30‑60, which is tiny.

#### 3.1 Classic Example: Minimum Maximum Distance (Aggressive Cows / Factory Machines)
**Problem:** You have `n` stalls at positions `x[]`, and `c` cows. Place the cows such that the minimum distance between any two cows is as large as possible. Output that distance.

**Monotonic property:** For a given distance `D`, can we place `c` cows with at least `D` distance apart? If we can do it for `D`, we can certainly do it for any smaller `D`. So the predicate `can(D)` is true for small D, and false for large D. We want the **largest** D where `can(D)` is true.

**Feasibility check (greedy):** Sort positions. Place first cow at the first stall. Then iterate, placing the next cow at the first stall that is at least `D` away from the last placed cow. If we manage to place `c` cows, return true.

```cpp
bool can(vector<long long>& pos, long long dist, int cows) {
    int placed = 1;
    long long last_pos = pos[0];
    for (int i = 1; i < pos.size(); ++i) {
        if (pos[i] - last_pos >= dist) {
            placed++;
            last_pos = pos[i];
            if (placed == cows) return true;
        }
    }
    return placed >= cows;
}
```

**Binary search:**
```cpp
sort(pos.begin(), pos.end());
long long lo = 1, hi = pos.back() - pos.front();
long long ans = 0;
while (lo <= hi) {
    long long mid = lo + (hi - lo) / 2;
    if (can(pos, mid, c)) {
        ans = mid;
        lo = mid + 1;      // try for larger distance
    } else {
        hi = mid - 1;
    }
}
cout << ans << endl;
```
This is the “maximize the minimum” pattern. It appears constantly: CSES 1085 (Array Division), 2420 (Palindrome Reorder? no, let’s pick 1085 indeed), 1631 (Reading Books?), etc.

#### 3.2 The Opposite: Minimize the Maximum (Array Division)
**CSES 1085: Array Division**
Split an array into `k` contiguous subarrays such that the maximum sum among them is minimized.

**Monotonic predicate:** `can(max_sum)` — can we split into ≤ `k` subarrays each with sum ≤ `max_sum`? If yes, then we can also do it for any larger max_sum. So we want the **smallest** `max_sum` where `can` is true.

**Feasibility check:** Greedily add elements to current subarray; if adding the next element exceeds `max_sum`, start a new subarray. Count subarrays; if ≤ `k`, return true.
```cpp
bool can(vector<long long>& a, long long max_sum, int k) {
    int subarrays = 1;
    long long cur = 0;
    for (long long x : a) {
        if (x > max_sum) return false; // single element exceeds limit
        if (cur + x > max_sum) {
            subarrays++;
            cur = x;
        } else {
            cur += x;
        }
    }
    return subarrays <= k;
}
```

**Binary search:** `lo` = maximum element (since any valid subarray must at least contain the max element), `hi` = sum of all elements.
```cpp
long long lo = *max_element(a.begin(), a.end());
long long hi = accumulate(a.begin(), a.end(), 0LL);
long long ans = hi;
while (lo <= hi) {
    long long mid = lo + (hi - lo) / 2;
    if (can(a, mid, k)) {
        ans = mid;
        hi = mid - 1;     // try for smaller max sum
    } else {
        lo = mid + 1;
    }
}
cout << ans << endl;
```

#### 3.3 Recognizing the Binary Search on Answer Pattern
Ask yourself: “Can I test in O(n) or O(n log n) whether a given value X is feasible?” If yes, and the problem asks for an extreme value (minimum possible maximum, maximum possible minimum, earliest time, etc.), binary search over X is likely the solution.

Examples of predicate types:
- `can(time)` — can all tasks be completed by time `T`? (Parallel scheduling)
- `can(cost)` — can we achieve the goal with cost ≤ `C`?
- `can(d)` — is there a subarray/product with property at least `d`?

---

### Part 4 — Binary Search on Continuous Functions (Floating Point)

Sometimes you need to find a real root or maximize/minimize a function with real values. The same idea works: `while (hi - lo > eps)` where `eps` is your desired precision (e.g., 1e-9). But be careful:
```cpp
double lo = 0, hi = 1e9;
for (int iter = 0; iter < 80; ++iter) {   // safer than while loop with epsilon
    double mid = lo + (hi - lo) / 2;
    if (can(mid))
        lo = mid;   // or hi = mid depending on problem
    else
        hi = mid;
}
// answer is lo
```
Typically 60-100 iterations give enough precision for doubles.

Example: Square root of X without using sqrt(). Use binary search on [0, max(1.0, X)].

---

### Part 5 — Binary Search on a Hidden Array (Interactive Problems)

Occasionally judges ask you to find a hidden value with queries. You binary search by asking “is it less than or equal to mid?” But we can cover interactive later. For now, focus on offline.

---

### Part 6 — Common Pitfalls and How to Avoid Them Forever

1. **Infinite loop:** This happens when `mid` is rounded down and the update `lo = mid` does not shrink the interval. That’s why in Template A I use `lo = mid+1` when condition false. For maximizing problem, you might have `if (can(mid)) { ans = mid; lo = mid+1; } else hi = mid-1;`. Never do `lo = mid;` without a `+1` unless you also adjust the `mid` calculation to round up.
2. **Off-by-one in answer boundaries:** Initialize `ans` to a known impossible value (e.g., for minimization, `ans = hi`; for maximization, `ans = lo`). Adjust based on the problem.
3. **Overflow in `(lo+hi)`:** Always compute `mid = lo + (hi - lo) / 2`. Even when `lo` and `hi` are `long long`, this is safe.
4. **Wrong predicate direction:** Clearly decide whether you want the first true or last true, and set the `if` branches accordingly. Write a comment:
   ```
   // F F F T T T   find first T
   ```
5. **Using `std::lower_bound` on `set`:** Use the member function, not the generic one.
6. **Not considering that the answer might not exist:** Always check edge cases (e.g., no feasible answer). If `can(hi)` is false, the answer might not exist — handle appropriately.

---

### Part 7 — Worked Examples: Step‑by‑Step

#### 7.1 Factory Machines (CSES 1620)
There are `n` machines, each produces a product in `t[i]` time. Goal: produce `k` products. Find the minimum time needed. Machines work in parallel.

**Monotonic property:** Given time `T`, total products made = sum of `T / t[i]`. Can we reach `k`? If yes, we can try smaller `T`; if no, we need more time. We seek the smallest `T` such that total ≥ `k`.

**Check function:**
```cpp
bool can(vector<long long>& t, long long T, long long k) {
    long long total = 0;
    for (long long x : t) {
        total += T / x;
        if (total >= k) return true;
    }
    return false;
}
```

**Binary search:** lo = 0, hi = 1e18 (or the time if only one fastest machine). Answer = hi, then minimize.
```cpp
long long lo = 0, hi = 1e18;
long long ans = hi;
while (lo <= hi) {
    long long mid = lo + (hi - lo) / 2;
    if (can(t, mid, k)) {
        ans = mid;
        hi = mid - 1;
    } else {
        lo = mid + 1;
    }
}
cout << ans << endl;
```

#### 7.2 Maximum Median (CF 1201C or CSES 1092? Actually CSES 1092 is Two Knights; I'll use CSES 1086?)
Let's use a classic: Given an array, you can perform `k` operations of incrementing an element by 1. Maximize the median after operations. (CF 1201C)

**Strategy:** Sort array. Binary search on the target median `M`. Check if we can make at least `n/2 + 1` elements from the middle onwards at least `M` using ≤ `k` increments. For sorted array, we can greedily add needed increments for each element starting from the middle index.

**Check:**
```cpp
bool can(vector<long long>& a, long long mid, long long k, int n) {
    long long ops = 0;
    for (int i = n/2; i < n; ++i) {
        if (a[i] < mid) ops += mid - a[i];
        if (ops > k) return false;
    }
    return ops <= k;
}
```
Binary search `lo = a[n/2]`, `hi = a[n/2] + k` (since max we can raise the median). Find max `mid` where `can` true.

#### 7.3 Other Classics
- **Sagheer and Nubian Market (CF 812C):** For a given `k`, compute cost as `a[i] + i*k` and pick `k` cheapest. Binary search on `k`.
- **Pie or Cake (CF 1059C):** Not binary search; but many problems with “minimum maximum” are binary search.

---

### Part 8 — Deriving Your Own Binary Search Intuition

Whenever you see:
- “minimize the maximum …”
- “maximize the minimum …”
- “find the earliest time when …”
- “split the array such that the largest sum is as small as possible”
- “can we achieve at least X …” with monotonic behavior

You should immediately think: “I can binary search the answer and write a check function.”

The check function is often greedy, DP, or simulation. Sometimes the check itself is non‑trivial, but as long as it runs in polynomial time, the whole solution is efficient.

---

### Part 9 — Problem Set for Week 6

This is the most critical problem set so far. Do not skip any. The first four will teach you the mechanics; the rest will stretch your recognition.

1. **CSES 1620 Factory Machines** – binary search on time, sum of quotients.
2. **CSES 1085 Array Division** – minimize maximum subarray sum.
3. **CF 812C Sagheer and Nubian Market** – binary search on number of items, sort with modified cost.
4. **CF 1201C Maximum Median** – maximize the median.
5. **CF 1118B Yet Another Array Partitioning Task** – actually not binary search (it's greedy with sort), but you can practice idea: might be DP. (Skip if too hard, but I'll include typical BS: CF 1352C K-th Not Divisible by n, it's pure math but binary search works).
   Instead, do **CF 1352C K‑th Not Divisible by n** — find the k‑th positive integer not divisible by n. Binary search on X, number of not divisible up to X is X - X/n.
6. **CSES 2420 Palindrome Reorder?** Actually that's string. Let's pick **CF 371C Hamburgers** (binary search on number of hamburgers). Classic.
7. **CSES 1074 Stick Lengths** – not BS but median; but you can binary search on the target length? Overkill. Instead **CSES 1621 Distinct Numbers** (already done), we need more. **CF 1352C** already, so **CF 660C Hard Process** — longest subarray with at most k zeros. This is two pointers/sliding window, not BS, but you can binary search on length. So binary search on answer: for a given length L, check if there is a subarray of length L with ≤ k zeros. O(n log n). Good practice.
8. **USACO Training 1.4: Milk Pails**? Not exactly. I'd rather assign **USACO 2016 US Open Bronze: Diamond Collector** (binary search after sort). The problem: given N numbers, find max number of elements you can pick such that max-min ≤ K. After sorting, for each i, find farthest j with a[j]-a[i] ≤ K using binary search or two pointers. Practice binary search.

**Challenge:** Write a generic binary search template function `binarySearch(lo, hi, predicate)` that returns the first index where predicate is true, and test it on multiple scenarios. This will forever be your go‑to.

---

### Part 10 — The Binary Search Reflex

From now on, whenever you read a problem and think “I need to find the smallest X such that…”, do not waste time on clever mathematical formulas. Ask: is the condition monotonic? If yes, write a `can(X)` function and binary search over X. Even if there is a direct formula, binary search will often get you an accepted solution faster and with fewer bugs.

I want you to practice this so much that you dream about `lo` and `hi`. The next time you see a problem with “minimum maximum”, your fingers should type the binary search skeleton before your brain fully engages. That is the level of automaticity we are building.

---

### Part 11 — Looking Ahead

Next week, we will combine everything you’ve learned—greedy, two pointers, prefix sums, binary search—to solve problems that require multiple techniques in sequence. We will also begin your journey into number theory and modulo arithmetic, which will open up a new universe of problems. But none of that will stick unless binary search is now part of your DNA.

So, go. Open CSES 1620, write the `can` function, and binary search the answer. Do not stop until you get it accepted. Then do the same for the other problems. I will be here, ready to push you further, when you return for Week 7. Now, code.
