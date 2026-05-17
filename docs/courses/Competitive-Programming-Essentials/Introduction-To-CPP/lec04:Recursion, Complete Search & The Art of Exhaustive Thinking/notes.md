## Lesson 4 — Recursion, Complete Search & The Art of Exhaustive Thinking

Welcome back. By now you can write loops in your sleep, you know which STL container to reach for, and you can look at a problem’s constraints and instantly estimate whether your idea will run in time. These are the mechanical skills of a competitive programmer. But they are not yet algorithmic thought.

Today we cross the threshold. You will learn to think in **recursion** — to solve a problem by pretending you already have a solution to a smaller version of itself. You will learn **complete search** — the art of systematically exploring every candidate answer. And you will learn **backtracking** — how to prune that search so you don’t wait until the heat death of the universe.

This session is long because it rewires your brain. Do not rush. Type every code, draw the recursion trees, and argue with yourself until the logic clicks. The reward is immense: a huge class of problems will suddenly become tractable, and you will have your first real taste of algorithmic power.

---

### Part 1 — Recursion Under the Microscope

You saw recursion in Week 1: factorial, Fibonacci. Now we make it instinct.

#### 1.1 The Three Laws of Recursion
1. **Base case** — the simplest possible input, for which the answer is known without any recursive calls.
2. **Recursive step** — the function calls itself on a strictly smaller / simpler input.
3. **Convergence** — each recursive call must move toward the base case; otherwise you recurse forever and crash.

If you keep these three laws in mind, you can design any recursive function.

#### 1.2 Thinking Recursively — The Leap of Faith
When writing a recursive function, assume the recursive call works correctly. Don’t try to “unroll” the whole chain in your head. Focus on how to combine the sub‑problem’s answer into the current answer.

**Example: sum of array elements recursively**
```cpp
int sumArray(const vector<int>& v, int i) {
    if (i == v.size()) return 0;           // base case: no elements
    return v[i] + sumArray(v, i + 1);      // current + sum of rest
}
```
Call: `sumArray(arr, 0)`. At each step, we believe `sumArray(v, i+1)` correctly sums the rest. Then we add `v[i]`.

#### 1.3 Recursion vs. Iteration
Everything you can write recursively can be written iteratively (and vice versa). But many problems — trees, graphs, backtracking — are naturally expressed with recursion, and the code is often shorter and clearer. Use recursion when it simplifies your thinking; use iteration when recursion depth might blow the stack (roughly > 10⁵ calls).

#### 1.4 Visualising the Call Stack
Draw a tree of calls. For `sumArray({3,1,2}, 0)`:
```
sumArray(0) -> 3 + sumArray(1)
                  sumArray(1) -> 1 + sumArray(2)
                                     sumArray(2) -> 2 + sumArray(3)
                                                        sumArray(3) -> 0
```
The answers flow back up. This is exactly how the computer’s call stack works. I will ask you to draw such trees for every new recursive function until your brain does it automatically.

---

### Part 2 — Complete Search: The Brute‑Force Philosophy

**Complete search** (also called **brute force** or **exhaustive search**) means generating every possible candidate solution and checking if it satisfies the problem constraints. It is the most straightforward algorithm family and, surprisingly, often the intended solution when input sizes are small.

The key is to systematically generate all candidates without missing any and without duplicates.

#### 2.1 Subsets — The Power Set
Given a set of `n` elements, generate all `2ⁿ` subsets.

**Approach 1: Recursive inclusion/exclusion**
For each element, we have two choices: include it or exclude it.
```cpp
void generateSubsets(int i, const vector<int>& nums, vector<int>& cur, vector<vector<int>>& all) {
    if (i == nums.size()) {
        all.push_back(cur);
        return;
    }
    // exclude
    generateSubsets(i+1, nums, cur, all);
    // include
    cur.push_back(nums[i]);
    generateSubsets(i+1, nums, cur, all);
    cur.pop_back();          // backtrack
}
```
We start with `i=0`, empty `cur`. At leaf, add `cur` to `all`. This visits exactly `2ⁿ` leaves.

**Approach 2: Bitmask iteration** (often faster, no recursion)
Represent each subset as an integer mask from `0` to `2ⁿ - 1`. If the `j`‑th bit is set, include element `j`.
```cpp
vector<vector<int>> subsets;
for (int mask = 0; mask < (1<<n); ++mask) {
    vector<int> cur;
    for (int j = 0; j < n; ++j)
        if (mask & (1<<j))
            cur.push_back(arr[j]);
    subsets.push_back(cur);
}
```
This is O(n·2ⁿ), fine for n ≤ 20.

**When is it used?** Problems that ask for the subset with some property (e.g., minimum difference, sum to target). We will see “Apple Division” as a classic example.

#### 2.2 Permutations
Generate all `n!` orderings of an array.

**Recursive with backtracking:**
```cpp
void permute(vector<int>& a, int l, int r, vector<vector<int>>& res) {
    if (l == r) {
        res.push_back(a);
        return;
    }
    for (int i = l; i <= r; ++i) {
        swap(a[l], a[i]);
        permute(a, l+1, r, res);
        swap(a[l], a[i]);          // backtrack
    }
}
```
This swaps each element into position `l`, recurses, and swaps back. Time: O(n! × n) for copying.

**Using STL `next_permutation`:** If the array is sorted, `next_permutation` generates all permutations in lexicographic order until it returns false. Much simpler.
```cpp
vector<int> a = {1,2,3};
sort(a.begin(), a.end());
do {
    // process permutation a
} while (next_permutation(a.begin(), a.end()));
```
This is your go‑to for most permutation problems.

**When to use:** problems like “Creating Strings” (generate all unique permutations of a string), or tasks that require trying every ordering.

#### 2.3 Combinations
If we need to choose `k` items from `n`, there are C(n,k) possibilities. You can generate them via recursion similar to subsets, but prune branches that can’t reach `k` items. Or use bitmasks and count bits.

---

### Part 3 — Backtracking: Search with Pruning

Backtracking is complete search that abandons a partial solution as soon as it can determine it’s invalid. This pruning often makes an exponential search feasible for moderate N.

#### 3.1 The N‑Queens Problem
Place N queens on an N×N chessboard so that no two attack each other. N ≤ 8 for brute force, but with pruning we can go higher.

**Naive approach:** generate all ways to place N queens on N rows (one per row), column permutations: N! ≈ 40,320 for N=8 — fine.

**Smart backtracking:** Place queens row by row. In each row, try each column if it doesn’t conflict with already placed queens (check columns and diagonals). If conflict, skip.
```cpp
int n;
vector<int> col; // col[r] = column of queen in row r

bool isValid(int r, int c) {
    for (int i = 0; i < r; ++i) {
        if (col[i] == c || abs(i - r) == abs(col[i] - c))
            return false; // same column or same diagonal
    }
    return true;
}

int solve(int r) {
    if (r == n) return 1; // all queens placed
    int cnt = 0;
    for (int c = 0; c < n; ++c) {
        if (isValid(r, c)) {
            col[r] = c;
            cnt += solve(r+1);
            // no explicit pop_back needed because col[r] will be overwritten
        }
    }
    return cnt;
}
```
This counts all solutions. For CSES “Chessboard and Queens” you’ll need to handle blocked cells and place exactly 8 queens.

#### 3.2 Sudoku Solver (Concept)
You can use backtracking to solve Sudoku: pick an empty cell, try digits 1‑9 that don’t violate row/col/box constraints, recurse. If a dead end, backtrack. This exemplifies constraint satisfaction.

#### 3.3 Pruning Techniques
- **Ordering heuristics:** try more constrained choices first (e.g., in N‑Queens, order columns by fewest conflicts) — advanced.
- **Memoization within backtracking:** sometimes overlapping subproblems occur (we’ll see this later in DP on subsets).

---

### Part 4 — Classic Complete Search Problems and How to Approach Them

Let’s apply these ideas to well‑known problems from CSES and USACO.

#### 4.1 Apple Division (CSES 1623)
Given `n` apples with weights `p_i`, split into two groups such that the difference of total weights is minimized. n ≤ 20.

**Solution:** We can’t do O(2ⁿ) if we check all subsets? 2²⁰ = 1,048,576, totally feasible. Just iterate all masks from 0 to 2ⁿ‑1, compute sum of group A (bits set) and group B (rest), track minimum `abs(total - 2*A_sum)`.

**Bitmask solution:**
```cpp
int n;
cin >> n;
vector<long long> p(n);
long long total = 0;
for (int i=0;i<n;++i) {
    cin >> p[i];
    total += p[i];
}
long long best = LLONG_MAX;
for (int mask = 0; mask < (1<<n); ++mask) {
    long long sumA = 0;
    for (int i = 0; i < n; ++i)
        if (mask & (1<<i)) sumA += p[i];
    long long diff = abs(total - 2*sumA);
    best = min(best, diff);
}
cout << best << endl;
```
Complexity O(n·2ⁿ) ≈ 20×10⁶ ≈ 2×10⁷ operations — fast enough.

#### 4.2 Creating Strings (CSES 1622)
Generate all distinct permutations of a given string (length ≤ 8). Use `next_permutation` on a sorted string, put results into a `set` or `vector`, sort and unique. Since length small, O(n! log n) fine.

**Code:**
```cpp
string s; cin >> s;
sort(s.begin(), s.end());
vector<string> ans;
do {
    ans.push_back(s);
} while (next_permutation(s.begin(), s.end()));
cout << ans.size() << endl;
for (auto &str : ans) cout << str << endl;
```

#### 4.3 Chessboard and Queens (CSES 1624)
Place 8 queens on an 8×8 board with some blocked squares. This is exactly 8‑queens with additional constraint that queen cannot be placed on a `'*'`. Standard backtracking with `isValid` adapted.

**Implementation:**
```cpp
string board[8];
int ans = 0;
// column tracking as before, but now check board[i][c] == '.'
// col array of size 8 (since always 8 queens)
void search(int r) {
    if (r == 8) { ans++; return; }
    for (int c = 0; c < 8; ++c) {
        if (board[r][c] == '.' && isValid(r, c)) {
            col[r] = c;
            search(r+1);
        }
    }
}
```
(Define `col` globally and `isValid` checks previous rows only.)

#### 4.4 USACO Training – Section 1.2
The USACO Training Gateway (train.usaco.org) provides a progressive set of problems. At this stage, you should attempt:

- **Your Ride Is Here** (ride) — simple string manipulation.
- **Greedy Gift Givers** (gift1) — map of strings, careful simulation.
- **Friday the Thirteenth** (friday) — date calculation, loops.
- **Broken Necklace** (beads) — more complex, uses circular array and DP/complete search.

These problems force you to write careful, bug‑free code. Do them. They are timeless.

---

### Part 5 — Advanced Recursion Patterns

#### 5.1 Generating Combinations (Choosing k from n)
Use recursion that builds a combination of size `k`. At each step, decide the next element to include, ensuring we never go backwards to avoid duplicates.
```cpp
void comb(int start, int k, vector<int>& cur, const vector<int>& arr, vector<vector<int>>& res) {
    if (cur.size() == k) {
        res.push_back(cur);
        return;
    }
    for (int i = start; i < arr.size(); ++i) {
        cur.push_back(arr[i]);
        comb(i+1, k, cur, arr, res);
        cur.pop_back();
    }
}
```
This is efficient and doesn't generate unwanted subsets.

#### 5.2 Flood Fill / DFS on a Grid (Intro)
Though we'll cover graphs later, recursion on grids is natural. Example: count connected components of `'.'` in a grid (like counting rooms). Recursive DFS:
```cpp
int n, m;
vector<string> grid;
vector<vector<bool>> vis;
int dx[] = {0,0,1,-1};
int dy[] = {1,-1,0,0};

void dfs(int x, int y) {
    vis[x][y] = true;
    for (int d=0; d<4; d++) {
        int nx = x+dx[d], ny = y+dy[d];
        if (nx>=0 && nx<n && ny>=0 && ny<m && !vis[nx][ny] && grid[nx][ny]=='.')
            dfs(nx, ny);
    }
}
```
This is a complete search over reachable cells. It’s the same recursive principle.

---

### Part 6 — Practical Wisdom: When to Use Complete Search

- **Check constraints first:** n ≤ 20 → subset/permutation, n ≤ 10⁶ → must be O(n log n).  
- **Look for hidden small parameters:** Sometimes the problem says N ≤ 100 but M ≤ 8 — the search space is small in one dimension; you can enumerate that dimension exhaustively.  
- **Don't over‑optimise:** If constraints allow O(2ⁿ) and your idea is correct, just code it. Simple is bug‑free.  
- **USACO Bronze** problems are almost entirely complete search and simulation. Mastering exhaustive generation will clear the entire Bronze division.

---

### Part 7 — The Recursion Stack & Common Pitfalls

- **Stack overflow:** Recursion depth > ~10⁵ may crash. For deep trees/graphs, consider iterative DFS or increasing stack size (`-Wl,-stack,16777216` on Windows). In CP, most judges give 256MB, but deep recursion can still overflow the stack segment; be mindful.  
- **Exponential growth:** `2ⁿ` for n=30 is 10⁹ — too large. Always calculate.  
- **Modifying global state:** In backtracking, always undo changes (pop element, restore visited flag) before returning.  
- **Copying vectors unnecessarily:** Pass by reference to avoid O(n) overhead per call.

---

### Part 8 — Your Problem Set for Week 4

Solve these in order, writing your own code from scratch. After each, I want you to manually draw the recursion tree for at least one test case (small one). This is not optional; it builds your mental simulator.

1. **Weird Algorithm** (CSES 1068) — (if not done) Use while loop; now also write a recursive version that prints the sequence.  
2. **Apple Division** (CSES 1623) — bitmask or recursion.  
3. **Creating Strings** (CSES 1622) — `next_permutation`.  
4. **Chessboard and Queens** (CSES 1624) — backtracking with N‑Queens.  
5. **Subset Sum** — given array, is there a subset with sum = X? Write both recursive and bitmask. (No official CSES, but practice).  
6. **USACO Training: Your Ride Is Here (ride)** — solve and submit.  
7. **USACO Training: Greedy Gift Givers (gift1)** — use map.  
8. **USACO Training: Friday the Thirteenth (friday)** — simulate.  
9. **CSES 1069 Repetitions** — not recursion, but a breather.  
10. **CF 550A Two Substrings** — find if “AB” and “BA” appear disjointly. Use complete search over positions? Actually it's greedy/DP, but think about substring search. Try a brute force over all possible split points.

Additionally, I challenge you to write a **sudoku solver** using backtracking. Find any 9x9 puzzle online (or generate one) and write a program that solves it. This will test your mastery of pruning and state management.

---

### Part 9 — A Glimpse Ahead

Next week we will connect recursion to **greedy algorithms** and **binary search**, completing the foundational problem‑solving patterns. You are now equipped to handle any problem that demands exploring a solution space. The boundary between “I have no idea” and “I’ll just try everything” is the line you just crossed.

Now, open your editor and start with Apple Division. Remember the three laws. When you get stuck, draw the tree. When you get WA, stress‑test against a brute‑force you know is correct. And when you succeed, take a moment to appreciate that your brain can now orchestrate an exponential explosion of possibilities and tame it into an elegant program.

Onward.
