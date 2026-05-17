## Lesson 5 — The Strategist’s Arsenal: Greed, Two Pointers, and Prefix Sums

You have learned to brute‑force a solution when the search space is small, and to recurse when the problem is self‑similar. Now we add three powerful weapons that will let you solve problems in O(n log n) or O(n) that initially look like they need exponential time. These are not mere tricks; they are modes of thought. By the end of this session you will be able to:

- Recognise a greedy choice and prove it works.
- Apply the two‑pointer sliding window to problems on subarrays and sorted sequences.
- Use prefix sums to answer range queries in O(1) and to split an array by value.
- Sort anything with a custom comparator to set up a greedy or two‑pointer algorithm.

Open your editor. Everything I show you must be typed and run. No passive reading.

---

### Part 1 — Greedy Algorithms: The Art of Local Optimality

#### 1.1 What Is a Greedy Algorithm?
A greedy algorithm builds up a solution piece by piece, always choosing the next piece that looks best **at the moment**, without ever reconsidering earlier decisions. You hope that a sequence of locally optimal choices leads to a globally optimal solution.

This does not always work. When it does, the problem has a special property: the **greedy‑choice property** — a globally optimal solution can be reached by making a locally optimal choice at each step. Additionally, the problem must have **optimal substructure** — an optimal solution contains optimal solutions to subproblems.

#### 1.2 A Classic: Activity Selection / Movie Festival
*Problem:* You have `n` activities with start and end times. You can only do one activity at a time, and you want to attend as many as possible. (CSES 1629: Movie Festival)

**Greedy strategy:** Always pick the activity that **finishes earliest** among the remaining ones, provided it starts after the last chosen one finishes.

**Why does this work?** Intuitive proof by exchange argument: Suppose an optimal schedule does not include the earliest‑finishing activity. Replace the first activity in the optimal schedule with the earliest‑finishing one; it will finish no later, so no conflicts are introduced, and the count stays optimal. Inductively, we can build an optimal schedule by always choosing the earliest finish time.

**Algorithm:**
1. Sort activities by ending time (ascending).
2. Keep `last_end = -∞`, `count = 0`.
3. For each activity (start, end) in sorted order:
   - If `start >= last_end`, choose it: `count++`, `last_end = end`.

**Implementation:**
```cpp
int n;
cin >> n;
vector<pair<int, int>> movies(n);
for (int i = 0; i < n; ++i)
    cin >> movies[i].first >> movies[i].second; // first = start, second = end
sort(movies.begin(), movies.end(),
     [](const pair<int,int>& a, const pair<int,int>& b) { return a.second < b.second; });
int cnt = 0, last = 0; // movies start at 0 or later; use -1e9 for safety
for (auto &m : movies) {
    if (m.first >= last) {
        cnt++;
        last = m.second;
    }
}
cout << cnt << endl;
```
Time O(n log n) dominated by sorting.

**Draw the proof:** Take any optimal schedule. Find the first activity. If it's not the one with the earliest finish time among all activities, swap it. Convince yourself this never makes the schedule invalid or shorter.

#### 1.3 Coin Change (Canonical Coin Systems)
*Problem:* Given coin denominations `{1, 2, 5}` (or typical currencies), make change for amount `X` using the fewest coins.

**Greedy approach:** Take as many of the largest coin as possible, then the next, etc. This works for many standard coin systems but **not for all**. For example, coins `{1, 3, 4}` and amount 6: greedy takes 4+1+1 = 3 coins, but optimal is 3+3 = 2. So always verify that the coin system is canonical before using greedy. In CP, if the problem says “standard Euro coins”, greedy works; if custom, use DP (next week).

**Lesson:** Greedy is only safe when you can prove it, or when the problem constraints are so small that you can brute force and check.

#### 1.4 Fractional Knapsack
You can take fractions of items. Greedy by value/weight ratio works perfectly. But 0/1 knapsack (you can’t take fractions) fails greedy (needs DP). So again, watch the problem statement.

#### 1.5 General Greedy Template
```cpp
// 1. Sort candidates according to some key
// 2. Initialize answer structure (counter, set, etc.)
// 3. Iterate through sorted candidates, maintaining a state (last_end, current load, etc.)
//    If candidate is feasible according to current state, add it and update state.
```
Greedy often needs a **proof** or at least a strong intuition. In contests, if you can't prove but small constraints allow, you could write a brute force to verify greedy for random inputs (stress test). Over time, you'll build a catalogue of known greedy patterns.

---

### Part 2 — Two Pointers & Sliding Window: Moving a Window in Linear Time

Two pointers is a technique where you maintain two indices (often called `l` and `r`) that traverse the array monotonically. This turns a potential O(n²) loop into O(n). It is used for:
- Finding a subarray with a given sum (all elements positive).
- Longest subarray with at most K distinct elements.
- Two‑sum on a sorted array.
- Partition problems.

#### 2.1 The Basic Sliding Window for Positive Arrays
**Problem:** Given an array of positive integers and a target `S`, find the length of the longest contiguous subarray with sum ≤ S.

If all numbers are positive, the sum increases as we expand `r` and decreases as we contract `l`. We can slide:
```cpp
int l = 0, curSum = 0, best = 0;
for (int r = 0; r < n; ++r) {
    curSum += a[r];
    while (curSum > S) {
        curSum -= a[l];
        l++;
    }
    best = max(best, r - l + 1);
}
```
Both pointers move right at most `n` times, so O(n). Works only because `a[i] >= 0`. If negative numbers are allowed, this fails.

**Example:** "Books" (CF 279B) — you have `n` books, each takes `t_i` minutes, you have `T` free minutes. Find maximum number of books you can read consecutively. (All `t_i` positive). Exactly the template.

```cpp
int n, T;
cin >> n >> T;
vector<int> a(n);
for (int &x : a) cin >> x;
int l = 0, sum = 0, ans = 0;
for (int r = 0; r < n; ++r) {
    sum += a[r];
    while (sum > T) sum -= a[l++];
    ans = max(ans, r - l + 1);
}
cout << ans << endl;
```

#### 2.2 Two Pointers on Sorted Arrays
Often you sort, then use two pointers to find pairs.

**Example:** CSES "Sum of Two Values" (we solved with map), but if we only needed to answer existence and the array can be sorted, we can use two pointers:
```cpp
sort(a.begin(), a.end());
int l = 0, r = n-1;
while (l < r) {
    int s = a[l] + a[r];
    if (s == x) { cout << a[l] << " " << a[r]; return 0; }
    else if (s < x) l++;
    else r--;
}
```
O(n log n) due to sorting, but if the problem requires indices, you need to store original positions. Still, the technique is O(n).

#### 2.3 Sliding Window with Conditions on Characters
**Problem:** "Smallest window that contains all characters of a pattern" — but in CP a simpler one: "Find the length of the smallest substring that contains at least one of each of the letters 'a', 'b', 'c'." (Ternary String, CF 1354B). Use two pointers and a frequency array.

**Template:**
```cpp
int l = 0, ans = n+1;
int cnt[4] = {0}; // we'll use 1-indexed for 'a','b','c'
for (int r = 0; r < n; ++r) {
    cnt[s[r]-'a'+1]++;
    while (cnt[1] && cnt[2] && cnt[3]) {
        ans = min(ans, r-l+1);
        cnt[s[l]-'a'+1]--;
        l++;
    }
}
cout << (ans > n ? 0 : ans) << endl;
```

#### 2.4 Common Pitfalls
- Two pointers only works if the condition is monotonic (e.g., expanding the window never makes a valid condition become invalid without a chance to fix by shrinking). For "sum ≤ S" with positive numbers, it's monotonic; with negatives, it's not.  
- Sliding window can also be used with a `map` to handle "at most K distinct elements" — we'll see that later.

---

### Part 3 — Prefix Sums: O(1) Range Queries & Array Partitions

The prefix sum array `pref[i]` stores the sum of `a[0..i-1]` (or `a[0..i]`, whichever convention you prefer). I use `pref[i] = sum of a[0] to a[i-1]`, so `pref[0] = 0`. Then sum of subarray `[l, r]` inclusive is `pref[r+1] - pref[l]`. This makes range sum queries O(1) after O(n) preprocessing.

#### 3.1 Construction and Usage
```cpp
vector<long long> pref(n+1, 0);
for (int i = 0; i < n; ++i)
    pref[i+1] = pref[i] + a[i];
// sum of a[l..r] = pref[r+1] - pref[l];
```

**CSES Static Range Sum** (1646) is trivial with this.

#### 3.2 Difference Array for Range Updates (Point Query)
If you need to add `x` to all elements in `[l, r]` many times and then query a single element, use a difference array `d` of size `n+1`. For each update `(l, r, x)`, do `d[l] += x; d[r+1] -= x;`. After all updates, the actual array `a[i]` is the prefix sum of `d` up to `i`. This is O(1) per update, O(n) final.

```cpp
vector<long long> d(n+2, 0); // extra for r+1
// add x to [l,r]
d[l] += x;
d[r+1] -= x;
// reconstruct a:
long long cur = 0;
for (int i = 0; i < n; ++i) {
    cur += d[i];
    a[i] += cur; // or assign
}
```

#### 3.3 Number of Ways to Split an Array (CF 466C)
*Given array, count ways to split into three contiguous parts with equal sum.*  
Idea: Total sum S must be divisible by 3. Compute prefix sums. Let target = S/3. Iterate from left to right, counting positions `i` (from 0 to n-3) where `pref[i] == target` (end of first part). At the same time, maintain count of valid second cuts. The standard solution:
```cpp
long long S = accumulate(a.begin(), a.end(), 0LL);
if (S % 3 != 0) { cout << 0; return 0; }
long long target = S/3;
vector<long long> pref(n, 0);
partial_sum(a.begin(), a.end(), pref.begin()); // pref[i] sum of a[0..i]
long long ans = 0;
int cntFirst = 0;
for (int i = 0; i < n-1; ++i) {
    // if prefix up to i equals 2*target, then it can serve as the end of second part, so add cntFirst
    if (pref[i] == 2*target) ans += cntFirst;
    // if prefix equals target, then it's a valid first part end, increment cntFirst
    if (pref[i] == target) cntFirst++;
}
cout << ans;
```
But careful with order. The classic solution traverses and counts first segments; when we see a prefix sum == 2*target, we add the number of first segments seen so far. We must ensure we don't count the last element as a split point. Works O(n).

#### 3.4 Subarray Sum Equals K (CSES 1661)
Using prefix sum and a map: count number of subarrays with sum = x.
```cpp
map<long long, int> cntPref;
cntPref[0] = 1;
long long cur = 0, ans = 0;
for (int i = 0; i < n; ++i) {
    cur += a[i];
    ans += cntPref[cur - x];
    cntPref[cur]++;
}
```
This is not strictly two pointers but uses prefix sums. Important pattern.

---

### Part 4 — Advanced Sorting: The Gate to Efficient Greedy and Two Pointers

You already know `sort` with a lambda. Now you must become fluent in sorting complex objects so that your greedy or two‑pointer logic becomes trivial.

#### 4.1 Sorting by Multiple Criteria
Use a lambda that returns `true` if the first argument should appear before the second.
```cpp
struct Person { string name; int age, height; };
vector<Person> people;
sort(people.begin(), people.end(), [](const Person& a, const Person& b) {
    if (a.age != b.age) return a.age < b.age;       // primary: younger first
    if (a.height != b.height) return a.height > b.height; // secondary: taller first
    return a.name < b.name;                          // tertiary: alphabetical
});
```

#### 4.2 Sorting Intervals for Sweep Line
For interval problems (like maximum overlapping intervals, room allocation), you often sort endpoints. For CSES "Room Allocation", you sort by start time and use a priority queue for end times. But that's a sweep line. We can mention it but not dive deep.

#### 4.3 `nth_element` and Partial Sort
Sometimes you only need the k‑th smallest element or the smallest k elements. `nth_element` is O(n). `partial_sort` puts the first k elements in sorted order. For example, to find median:
```cpp
nth_element(v.begin(), v.begin() + v.size()/2, v.end());
int median = v[v.size()/2];
```

These are part of `<algorithm>` and can be useful.

---

### Part 5 — Combined Problem Walkthroughs

Let's solve a few problems together, step by step, integrating the techniques.

#### 5.1 CSES 1629 — Movie Festival (already done, but we formalize)
We already implemented the greedy by earliest finish. I want you to also write a brute force for small n to verify that greedy works. Use subset generation: try all subsets of movies, check if they are non-overlapping, keep max count. Compare with greedy on random small inputs. This builds your confidence.

#### 5.2 CSES 1641 — Sum of Three Values
Given n numbers, find three distinct positions whose sum is X. n ≤ 5000, so O(n²) is okay. You can sort and for each `i`, use two pointers on the remaining array to find `j, k` with sum `X - a[i]`. Careful with indices (store original indices).

**Algorithm:**
```cpp
vector<pair<int,int>> v; // (value, original index)
for each i: ...
sort(v.begin(), v.end());
for (int i = 0; i < n; ++i) {
    int target = x - v[i].first;
    int l = i+1, r = n-1;
    while (l < r) {
        int cur = v[l].first + v[r].first;
        if (cur == target) {
            cout << v[i].second+1 << " " << v[l].second+1 << " " << v[r].second+1;
            return 0;
        } else if (cur < target) l++;
        else r--;
    }
}
cout << "IMPOSSIBLE";
```
This uses sorting and two pointers inside a loop — O(n²). Good for n up to 5000.

#### 5.3 CF 279B — Books (already solved)
Revisit with stress test: write brute O(n²) for n ≤ 100, compare with sliding window.

#### 5.4 CF 1354B — Ternary String
Sliding window as described. Practice writing it from scratch.

#### 5.5 CSES 1661 — Subarray Sums II (Count subarrays with sum = X)
Use prefix sum + map as shown. This problem appears frequently, so memorize the pattern.

---

### Part 6 — When Greedy Fails and What to Do
- If greedy choice is not obvious or you suspect it might fail, try to craft a counterexample. If you find one, you likely need DP or flow.  
- In contests, you can sometimes combine greedy with brute force for small parts (e.g., greedy for large numbers, but exhaust small cases).  
- Never use greedy without either a proof or a small‑scale stress test. The strongest programmers habitually try to break their own greedy before submitting.

---

### Part 7 — Your Assignment for Week 5

Complete these problems. For each greedy problem, write down the greedy rule and attempt a proof (even an informal one). For two‑pointer problems, trace the window movements on a small example.

1. **CSES 1629 Movie Festival** (if not yet submitted)
2. **CSES 1630 Tasks and Deadlines** — you have tasks with duration and deadline; schedule them to minimize total reward. Hint: sort by duration.
3. **CSES 1641 Sum of Three Values** (using sorting + two pointers)
4. **CF 279B Books** (sliding window)
5. **CF 1354B Ternary String** (sliding window)
6. **CF 466C Number of Ways** (prefix sum)
7. **CSES 1661 Subarray Sums II** (prefix sum + map)
8. **CSES 1163 Traffic Lights** (if not yet done, uses set and multiset — revisit with fresh eyes)
9. **USACO Training Section 1.3: milk, crypt1** (these mix complete search and greedy)

**Challenge:** Write a program that takes a list of intervals and determines the maximum number of non-overlapping intervals using greedy, and then stress-tests it against a brute-force over subsets for n ≤ 15. Make sure the greedy always matches the brute. This will cement the exchange argument in your mind.

---

### Part 8 — Recap and Mental Models
- **Greedy**: Sort by a key (finish time, duration, ratio). Iterate, keeping the feasible best so far. Always ask: can I prove that replacing an optimal solution with my greedy choice never hurts?
- **Two Pointers / Sliding Window**: Works when the condition is monotonic. `while` the window violates the condition, move `l` right. The loop runs O(n) because each pointer moves at most n steps.
- **Prefix Sums**: `pref[i]` up to i-1. Range sum O(1). Combined with map to count subarrays with given sum. Difference array for range updates.
- **Sorting**: The universal preprocessor. Sorting by the right comparator can transform a problem from O(n²) to O(n log n) and make greedy or two pointers applicable.

You are now armed with the three fundamental techniques that, together with complete search, solve roughly half of all Div.2 A/B problems. Next week we tackle Binary Search — the most versatile paradigm in competitive programming. You will learn to binary search over the answer, over arrays, and over functions, and you will see why it is my favorite tool.

Now, code. Open the problems and start with Movie Festival. I will be here when you return for Week 6.
