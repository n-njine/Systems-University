## Lesson 9 — The Art of Remembering: Dynamic Programming I

You have trained your mind to search, to greedily choose, to halve the unknown with binary search. But there is a class of problems where brute force would revisit the same state millions of times. Dynamic Programming (DP) is how you eliminate that waste. It is not a single algorithm; it is a way of thinking: **solve a problem by combining solutions to overlapping subproblems**, and **store those solutions so you never compute them twice**.

This week you will cross the most important threshold in competitive programming. Many quit before DP; you will break through. By the end of this session you will be able to:

- Recognise a DP problem by its *optimal substructure* and *overlapping subproblems*.
- Write a top‑down recursive solution with memoization.
- Transform it into a bottom‑up iterative solution.
- Solve coin change, knapsack, and longest increasing subsequence in your sleep.

Open your editor. Every example must be typed, run, and tested. I will walk with you every step of the way.

---

### Part 1 — What Is Dynamic Programming?

#### 1.1 The Fibonacci Lesson
Computing the 40th Fibonacci number recursively:
```cpp
int fib(int n) {
    if (n <= 1) return n;
    return fib(n-1) + fib(n-2);
}
```
This takes *minutes*. Why? Because `fib(38)` is calculated twice, `fib(37)` three times, and the total number of calls is exponential.

But notice: `fib(5)` needs `fib(4)` and `fib(3)`, `fib(4)` needs `fib(3)` and `fib(2)`, etc. The **subproblems overlap**. If we store the result of `fib(i)` the first time we compute it, we cut the work to O(n). This is the heart of DP.

#### 1.2 The Two Properties
For DP to apply, your problem must have:
- **Optimal substructure:** An optimal solution can be built from optimal solutions of its subproblems.
- **Overlapping subproblems:** The recursive tree contains the same subproblems many times.

If a problem has optimal substructure but no overlapping subproblems, you just use divide‑and‑conquer (like merge sort). If it has overlapping subproblems but no optimal substructure, you cannot combine solutions. DP is for when both hold.

---

### Part 2 — Top‑Down DP: Recursion with Memory (Memoization)

#### 2.1 Memoized Fibonacci
```cpp
vector<long long> memo(1000, -1);

long long fib(int n) {
    if (n <= 1) return n;
    if (memo[n] != -1) return memo[n];
    return memo[n] = fib(n-1) + fib(n-2);
}
```
Now `fib(40)` is instantaneous. The `memo` array acts as a cache. This is **top‑down DP**: you start from the original problem and recurse down to base cases, storing results on the way back up.

#### 2.2 General Pattern for Top‑Down
```cpp
int dp(state) {
    if (base case) return base_value;
    if (memo[state] already computed) return memo[state];
    int ans = initial_value;
    for (each choice) {
        ans = best(ans, dp(smaller_state) + cost);
    }
    return memo[state] = ans;
}
```

**Example: Minimum Steps to 1**  
Problem: Given n, you can subtract 1, divide by 2 (if even), divide by 3 (if divisible). Find minimum steps to reach 1.
```cpp
int minSteps(int n) {
    if (n == 1) return 0;
    if (memo[n] != -1) return memo[n];
    int res = 1 + minSteps(n-1);
    if (n % 2 == 0) res = min(res, 1 + minSteps(n/2));
    if (n % 3 == 0) res = min(res, 1 + minSteps(n/3));
    return memo[n] = res;
}
```
Time complexity: O(n) states, each O(1) transitions.

---

### Part 3 — Bottom‑Up DP: Building the Table

Top‑down is intuitive, but it uses recursion stack space and has overhead. Bottom‑up fills a table iteratively, from base cases to final answer. It often runs faster and avoids recursion limits.

#### 3.1 Fibonacci Bottom‑Up
```cpp
vector<long long> dp(n+1);
dp[0] = 0; dp[1] = 1;
for (int i = 2; i <= n; ++i)
    dp[i] = dp[i-1] + dp[i-2];
```
You can even space‑optimise to O(1) by keeping only the last two values, but for now focus on clarity.

#### 3.2 Coin Change: Minimum Number of Coins
Given coin denominations `coins[]` and target amount `x`. Find the minimum number of coins needed to make `x` (each coin unlimited). If impossible, return -1.

**State definition:** `dp[i]` = minimum coins needed to make amount `i`.  
**Transitions:** For each coin `c`, if `i >= c`, `dp[i] = min(dp[i], dp[i-c] + 1)`.  
**Base case:** `dp[0] = 0`, others initially INF (large value).

```cpp
int minCoins(vector<int>& coins, int x) {
    const int INF = 1e9;
    vector<int> dp(x+1, INF);
    dp[0] = 0;
    for (int i = 1; i <= x; ++i) {
        for (int c : coins) {
            if (i >= c)
                dp[i] = min(dp[i], dp[i-c] + 1);
        }
    }
    return dp[x] == INF ? -1 : dp[x];
}
```
Complexity O(x * number of coins). This is CSES 1634: Minimizing Coins.

**Note:** The order of loops matters if you want permutations vs combinations (we'll see that later for counting). For minimization of coins (unlimited), the above is fine.

---

### Part 4 — Knapsack: Choosing Items with a Weight Limit

#### 4.1 0/1 Knapsack
Given `n` items, each with weight `w[i]` and value `v[i]`. You have a knapsack of capacity `W`. Choose a subset of items such that total weight ≤ W and total value is maximized. Each item can be used at most once.

**State:** `dp[i][j]` = maximum value using first `i` items with capacity `j`.  
**Transition:** For the `i`‑th item (1‑indexed), you either skip it: `dp[i][j] = dp[i-1][j]`, or take it (if `j >= w[i]`): `dp[i][j] = max(dp[i][j], dp[i-1][j - w[i]] + v[i])`.

**Bottom‑up (2D table):**
```cpp
vector<vector<int>> dp(n+1, vector<int>(W+1, 0));
for (int i = 1; i <= n; ++i) {
    for (int j = 0; j <= W; ++j) {
        dp[i][j] = dp[i-1][j];
        if (j >= w[i])
            dp[i][j] = max(dp[i][j], dp[i-1][j - w[i]] + v[i]);
    }
}
cout << dp[n][W];
```

**Space optimisation to 1D:** Notice `dp[i][*]` only depends on `dp[i-1][*]`. We can use a single array and iterate `j` from `W` down to `w[i]`:
```cpp
vector<int> dp(W+1, 0);
for (int i = 1; i <= n; ++i)
    for (int j = W; j >= w[i]; --j)
        dp[j] = max(dp[j], dp[j - w[i]] + v[i]);
```
This is O(nW) time, O(W) space. Classic CSES 1158: Book Shop.

#### 4.2 Unbounded Knapsack
Each item can be used any number of times. Transition: `dp[j] = max(dp[j], dp[j - w[i]] + v[i])` with `j` iterating forward, because you can reuse the same item. Change the inner loop direction: `for (int j = w[i]; j <= W; ++j)`.

---

### Part 5 — Longest Increasing Subsequence (LIS)

Given array `a` of length `n`, find the length of the longest strictly increasing subsequence.

#### 5.1 O(n²) DP
Define `dp[i]` = length of LIS ending at index `i`. For each `i`, look back to all `j < i` where `a[j] < a[i]` and update `dp[i] = max(dp[i], dp[j] + 1)`. Answer = `max(dp[i])`.

```cpp
vector<int> dp(n, 1);
for (int i = 0; i < n; ++i)
    for (int j = 0; j < i; ++j)
        if (a[j] < a[i])
            dp[i] = max(dp[i], dp[j] + 1);
int ans = *max_element(dp.begin(), dp.end());
```
This is fine for n ≤ 5000. CSES 1145: Increasing Subsequence.

#### 5.2 O(n log n) with Patience Sorting
Maintain a tail array `tails`, where `tails[i]` is the smallest possible tail of an increasing subsequence of length `i+1`. Process elements one by one. For each `x`, find the first `tails[i] >= x` (using `lower_bound`) and replace it with `x`. If `x` is larger than all tails, append it. The length of `tails` at the end is the LIS length.

```cpp
vector<int> tails;
for (int x : a) {
    auto it = lower_bound(tails.begin(), tails.end(), x);
    if (it == tails.end())
        tails.push_back(x);
    else
        *it = x;
}
cout << tails.size();
```
This is O(n log n) and works for strictly increasing. For non‑decreasing, use `upper_bound`. This is essential for large n.

---

### Part 6 — DP on Grids and Paths

#### 6.1 Counting Paths
Problem: In an `n x m` grid, move only right and down from (1,1) to (n,m). Some cells are blocked. Count ways mod M.

Define `dp[i][j]` = ways to reach (i,j). `dp[i][j] = (dp[i-1][j] + dp[i][j-1]) % M` if cell is free. Base: `dp[1][1] = 1`. Iterate i from 1..n, j from 1..m.

```cpp
vector<vector<int>> dp(n+1, vector<int>(m+1, 0));
dp[1][1] = 1;
for (int i = 1; i <= n; ++i)
    for (int j = 1; j <= m; ++j) {
        if (grid[i][j] == '#') { dp[i][j] = 0; continue; }
        if (i > 1) dp[i][j] = (dp[i][j] + dp[i-1][j]) % MOD;
        if (j > 1) dp[i][j] = (dp[i][j] + dp[i][j-1]) % MOD;
    }
```
CSES 1638: Grid Paths.

#### 6.2 Maximum Sum Path (Gold Grid)
Similar, but store the maximum sum instead of count.

---

### Part 7 — How to Identify a DP Problem

Ask yourself:
- Am I asked for an **optimal value** (minimum, maximum) or a **count**?
- Can I express the answer in terms of smaller inputs of the same type? (e.g., f(n) = something + f(n-1)).
- Does a brute‑force recursion tree contain repeated states?
- Are the input parameters small enough to allow O(N²) or O(NW)? Look for constraints: N ≤ 10⁵ suggests O(N log N) LIS or O(N) DP; N ≤ 5000 suggests O(N²); small W ≤ 10⁵ suggests knapsack.

If you see words like *subsequence*, *subset*, *partition*, *ways*, *minimum number of operations*, think DP.

---

### Part 8 — DP Pitfalls and Debugging

- **Wrong base cases:** Always manually trace the smallest inputs.
- **Off‑by‑one in indices:** If you use 1‑based indexing for items/weights, be consistent.
- **Memory limit exceeded:** If the DP table is too large (e.g., 10⁶ x 500), try to optimise space (1D for knapsack, rolling arrays).
- **Using INF:** Set INF to something like 1e18 for `long long`, and check against it.
- **Recursion depth:** For top‑down, if recursion depth > 10⁵, it may stack overflow. Use bottom‑up or increase stack size.
- **Memoization with `vector`:** Use -1 for uncomputed values; for states that can legitimately be 0, use a separate `visited` bool.

---

### Part 9 — Combined Problem Walkthroughs

#### 9.1 CSES 1633 – Dice Combinations
Count ways to roll dice to sum `n`. `dp[i]` = sum of `dp[i-j]` for j=1..6. Base `dp[0] = 1`. Mod 1e9+7.
```cpp
vector<int> dp(n+1, 0);
dp[0] = 1;
for (int i = 1; i <= n; ++i)
    for (int j = 1; j <= 6 && i-j >= 0; ++j)
        dp[i] = (dp[i] + dp[i-j]) % MOD;
cout << dp[n];
```

#### 9.2 CSES 1635 – Coin Combinations I (Ordered)
Count number of ways to form sum `x` using coins, where different orderings count as different ways (permutations). `dp[i] = sum(dp[i - c])` for all coins `c`. This is exactly like dice combinations. Loop `i` outer, inner coins.

#### 9.3 CSES 1636 – Coin Combinations II (Unordered)
Count ways disregarding order (combinations). For unordered, we must avoid counting permutations multiple times. The trick: loop over coins first, then for each coin, loop over amounts from `c` to `x` (forward). This ensures each coin is considered only once and avoids reordering.
```cpp
vector<int> dp(x+1, 0);
dp[0] = 1;
for (int c : coins)
    for (int i = c; i <= x; ++i)
        dp[i] = (dp[i] + dp[i-c]) % MOD;
```
This is classic.

#### 9.4 CSES 1158 – Book Shop (0/1 Knapsack)
Use 1D DP as shown.

#### 9.5 CF 189A – Cut Ribbon
Given a ribbon length `n`, and pieces `a,b,c`. Maximize number of pieces (unlimited cuts). Equivalent to unbounded knapsack counting with max pieces. `dp[i] = max(dp[i], dp[i-cut] + 1)`. Initialize dp with -INF.

#### 9.6 CF 327A – Flipping Game
Given binary array, choose one subarray to flip bits, maximize final number of 1s. DP solution: let dp[i][0] = max 1s without flip up to i, dp[i][1] = max 1s having started a flip, dp[i][2] = max 1s after finishing flip. Or transform to maximum subarray sum (count 1s as -1 and 0s as 1). But DP is instructive.

---

### Part 10 — Your Assignment for Week 9

This problem set is non‑negotiable. DP cannot be learned by reading; you must fail, debug, and finally see the pattern.

1. **CSES 1633 Dice Combinations**
2. **CSES 1634 Minimizing Coins**
3. **CSES 1635 Coin Combinations I**
4. **CSES 1636 Coin Combinations II**
5. **CSES 1637 Removing Digits** (min steps by subtracting a digit)
6. **CSES 1638 Grid Paths** (count paths with obstacles)
7. **CSES 1158 Book Shop** (0/1 knapsack)
8. **CSES 1145 Increasing Subsequence** (first O(n²), then O(n log n))
9. **CF 189A Cut Ribbon**
10. **CF 327A Flipping Game**

**Additional Challenge:**
- Implement both top‑down and bottom‑up for Minimizing Coins and verify they give identical results on random tests. Stress test with small amounts and brute force recursion.

For each DP problem, follow this routine:
- Write the state definition on paper.
- Write the recurrence.
- Determine base cases.
- Choose iteration order.
- Code.
- Test on sample, then small random cases against a brute‑force.

---

### Part 11 — The DP Mindset

From this moment, whenever you encounter a problem that asks for “minimum/maximum/count” and has a recursive structure, you will instinctively ask: *What is the state? What are the transitions?* You will draw the dependency graph, decide whether to memoize or tabulate, and then write the loop.

DP will appear again and again: on trees, on graphs, on strings, on subsets. Today you laid the foundation. In Week 10, we will advance to 2D DP, string DP (edit distance), and begin our exploration of **Graph Traversal** — where many of these DP patterns recur on DAGs.

You have now entered the inner circle. Own it.

Now go. Open CSES 1633, code it, and don’t stop until you see “Accepted”. I will be here when you are ready for Week 10.
