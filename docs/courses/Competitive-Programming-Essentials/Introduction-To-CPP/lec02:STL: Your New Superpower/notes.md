## Lesson 2 — STL: Your New Superpower

Last week I taught you to write C++ that works. This week I will teach you to write C++ that flies.

The Standard Template Library (STL) is not a collection of “advanced topics” you can postpone. It is the language you must speak to compete. Every data structure, every algorithm you will ever need has already been written, tested by millions of programmers, and polished to an edge. Your job is not to reinvent the wheel but to learn how to use it instantly and without mistakes.

Today’s session is dense. I will give you every container you will need for the first six months, every associated function, and the mental model to choose the right one. By the end, you will solve problems in a few lines that would take fifty without the STL.

Open your editor. Code along with every example. I will not repeat these patterns — you must make them muscle memory.

---

### 1. `std::vector` — The Dynamic Array
The C‑style array you learned last week is a fixed‑size box. `std::vector` is a box that grows. This is the single most used container in competitive programming.

```cpp
#include <vector>
using namespace std;

vector<int> v;                 // empty vector
vector<int> v2(10);            // size 10, elements default to 0
vector<int> v3(5, -1);         // size 5, all elements = -1
vector<int> v4 = {1, 2, 3};    // initializer list
```

#### Fundamental Operations
```cpp
v.push_back(42);         // add to end, amortised O(1)
v.pop_back();            // remove last element, O(1)
int sz = v.size();       // number of elements, O(1)
bool e = v.empty();      // true if size == 0
v.clear();               // remove all elements, O(n)
v.resize(20);            // change size to 20, new elements = 0
v.reserve(1000);         // pre-allocate memory for 1000 elements, avoids reallocation
```

**Access** is exactly like an array, with `operator[]` (no bounds checking, faster) or `.at()` (bounds check, throws exception — slower, almost never used in CP).

```cpp
v[0] = 5;
cout << v.back();        // last element
cout << v.front();       // first element
```

**Iteration** can be done with indices or, better, with range‑based for loops:
```cpp
for (int x : v)
    cout << x << ' ';
```
If you need to modify the elements, use a reference:
```cpp
for (int &x : v)
    x *= 2;
```

**Beware of invalidated references and iterators:** When a `vector` reallocates (because its capacity is exceeded), all pointers and iterators to its elements become invalid. This means that code like this is dangerous:

```cpp
vector<int> v = {1,2,3};
for (int &x : v) {
    if (x == 2)
        v.push_back(4);   // may reallocate, loop crashes
}
```
Never change a container’s structure while you are iterating over it unless you know exactly what you are doing.

**Two‑dimensional vectors:** A matrix is `vector<vector<int>> mat(rows, vector<int>(cols, 0));`. Access with `mat[i][j]`. Don’t forget the space between `>>` in older C++ standards; C++11 and later allow `>>`.

---

### 2. `std::pair` — Two Things Together
Often you need to carry two values as one unit — coordinates, edges, key‑value pairs when you don’t need a map.

```cpp
pair<int, int> p = {3, 5};
pair<int, string> student = {17, "Alice"};

// access
cout << p.first << ' ' << p.second;
```
Pairs can be compared lexicographically: first element first, then second. This makes them perfect for sorting.

```cpp
vector<pair<int, int>> points;
points.push_back({2, 5});
points.push_back({1, 8});
sort(points.begin(), points.end()); // sorts by first, then second
// points = [{1,8}, {2,5}]
```

**Tie** can unpack a pair:
```cpp
int a, b;
tie(a, b) = p;   // a=3, b=5
```

---

### 3. `std::map` and `std::unordered_map` — Dictionaries
A map stores key‑value pairs and allows fast lookup by key.

| Container              | Implementation | Key order | Operations      |
|------------------------|----------------|-----------|-----------------|
| `std::map`             | Red‑black tree | sorted    | O(log n)        |
| `std::unordered_map`   | Hash table     | arbitrary | O(1) average    |

Use `unordered_map` when you don’t need sorted keys and want maximum speed. **But:** `unordered_map` has high constant factors and can be hacked if someone feeds many collisions. For now, I’ll teach you both; later you’ll learn when to switch.

**Basic usage:**
```cpp
map<string, int> age;
age["Alice"] = 30;
age["Bob"] = 25;
cout << age["Alice"] << endl;          // 30
cout << age["Charlie"] << endl;        // 0 (default constructed, inserted)
```

The expression `age["Charlie"]` inserts a key if it doesn’t exist. That can be a bug if you only wanted to check existence. So we have:

```cpp
if (age.find("Eve") != age.end())
    cout << "Found";
else
    cout << "Not found";

// C++20 adds .contains(), but on Codeforces you can’t always rely on it yet.
```

**Iterating:**
```cpp
for (const auto &p : age) {
    cout << p.first << " is " << p.second << '\n';
}
```
For `map`, the order is alphabetical by key; for `unordered_map`, it’s random.

**Erase:**
```cpp
age.erase("Bob");
```

**Performance tip:** If you use `unordered_map` for speed, always call `reserve` with a rough expected size to avoid many rehashes, and use `max_load_factor` if necessary. For most CP problems, `map` is safer and fast enough up to ~10⁵ elements.

---

### 4. `std::set` and `std::unordered_set` — Unique Collections
A set keeps only unique elements, also with sorted (`set`) or hashed (`unordered_set`) variants.

```cpp
set<int> s;
s.insert(3);
s.insert(1);
s.insert(3);        // ignored
s.insert(2);
// s is now {1, 2, 3}

cout << s.count(2); // 0 or 1
s.erase(2);
```

**Finding the smallest element larger than x:**
```cpp
auto it = s.lower_bound(5);
if (it != s.end())
    cout << *it;
```
`lower_bound` returns an iterator to the first element **not less than** the given value, `upper_bound` to the first element **greater**. Both work on `map` and `set` but also on sorted sequences.

**Multiset:** `std::multiset` allows duplicates. It is a tree as well. There is also `std::unordered_multiset`. You will need them when counting isn’t enough.

Be careful: `s.erase(value)` erases all copies in a multiset; use `s.erase(s.find(value))` to erase just one.

---

### 5. Iterators and `auto` — Stop Typing `std::vector<int>::iterator`
An **iterator** is like a pointer into a container. `begin()` and `end()` give you the range.

```cpp
vector<int> v = {10, 20, 30};
vector<int>::iterator it = v.begin();
cout << *it;                  // 10
it++;
cout << *it;                  // 20
```

But the type is long and boring. Use **`auto`**:
```cpp
auto it = v.begin();
for (auto it = v.begin(); it != v.end(); ++it)
    *it *= 2;
```
Even better, when you don’t need the iterator, use range‑based `for`.

`auto` deduces the type from the initialiser. Use it freely for long type names, but don’t overuse it when the type is unclear to a human reader.

---

### 6. The `<algorithm>` Toolbox — Operations on Ranges
The real power of STL lives in the `<algorithm>` header. It’s already included by `<bits/stdc++.h>`. Let’s master the most important ones.

All algorithms work on iterator ranges: `[first, last)`. The last is one past the end.

#### Sorting
```cpp
vector<int> a = {5, 2, 8, 1};
sort(a.begin(), a.end());                // ascending
sort(a.begin(), a.end(), greater<int>()); // descending
```

#### Binary Search (on sorted ranges)
```cpp
bool exists = binary_search(a.begin(), a.end(), 8); // true
auto it = lower_bound(a.begin(), a.end(), 5);       // first element >= 5
auto it2 = upper_bound(a.begin(), a.end(), 5);      // first element > 5
int idx = lower_bound(a.begin(), a.end(), 5) - a.begin(); // index
```
**Important:** The range must be sorted. For `set` and `map`, use their member functions `.lower_bound()` — they are O(log n) versus the generic `std::lower_bound` which for sets is O(n) because it cannot take advantage of the tree structure.

#### Minimum and Maximum
```cpp
int m = *min_element(a.begin(), a.end());
auto it = max_element(a.begin(), a.end());
int maxIdx = it - a.begin();
```
You can also use `min` and `max` for two values, and `minmax` for both at once.

#### Reversing and Rotating
```cpp
reverse(a.begin(), a.end());   // in-place reversal
rotate(a.begin(), a.begin()+1, a.end()); // left shift by 1
```

#### Removing Duplicates
```cpp
sort(a.begin(), a.end());
auto last = unique(a.begin(), a.end());   // moves duplicates to end, returns new end
a.erase(last, a.end());                   // actually remove them
```
`unique` requires the range to be sorted. This pattern of “unique‑erase” is so common that you should memorise it.

#### Counting and Finding
```cpp
int cnt = count(a.begin(), a.end(), 5);
auto it = find(a.begin(), a.end(), 5);
bool has = (find(a.begin(), a.end(), 5) != a.end());
```

#### Permutations
```cpp
string s = "abc";
do {
    cout << s << '\n';
} while (next_permutation(s.begin(), s.end()));
```
This generates all permutations in lexicographic order. To get all permutations of a vector, you must sort it first.

#### Other useful ones:
- `fill(a.begin(), a.end(), value)` — assign a value to the entire range.
- `iota(a.begin(), a.end(), start)` — fills with increasing values (needs `<numeric>`).
- `partial_sum` — prefix sums, also in `<numeric>`.

---

### 7. Custom Comparators — Making STL Obey You
All functions like `sort`, `lower_bound`, `priority_queue` accept an optional comparator that defines the order.

**Using a lambda (modern C++, clean and fast):**
```cpp
vector<pair<int, int>> vec = {{2,5}, {1,3}, {2,1}};
sort(vec.begin(), vec.end(), [](const pair<int,int> &a, const pair<int,int> &b) {
    if (a.first != b.first) return a.first < b.first; // sort by first increasing
    return a.second > b.second;                        // if equal, second decreasing
});
// Result: [{1,3}, {2,5}, {2,1}]
```

**For a `set` with a custom order:**
```cpp
auto cmp = [](int a, int b) { return a > b; };
set<int, decltype(cmp)> s(cmp);
s.insert(1); s.insert(2); // stores as 2,1
```

**Sorting an array by a key computed from each element:**
```cpp
vector<string> names = {"Alice", "Bob", "Charlie"};
sort(names.begin(), names.end(), [](const string &a, const string &b) {
    return a.size() < b.size();   // shortest first
});
```

Lambdas can capture variables from outside using `[&]` or `[=]`, but for comparators you usually don’t need captures. Just use `[]`.

---

### 8. Priority Queue — The Greedy Algorithm Engine
`priority_queue` is a max‑heap by default.

```cpp
priority_queue<int> pq;          // max-heap
pq.push(10); pq.push(5); pq.push(20);
cout << pq.top();                 // 20
pq.pop();                         // removes 20

// min-heap
priority_queue<int, vector<int>, greater<int>> min_pq;
```
Custom comparator for complex elements:
```cpp
priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
// smallest pair at top
```
If you need to update a priority (like decrease‑key), `priority_queue` does not support it. You will later learn to use `std::set` or push pairs multiple times. For now, it’s fine.

---

### 9. Complexity Cheat Sheet — Choose the Right Container
**Operations you will care about:**

| Task                               | Best choice             | Time                |
|------------------------------------|-------------------------|---------------------|
| Append / pop back                  | `vector`                | O(1) amortised      |
| Insert / delete anywhere           | `list` (rarely) / `set` | O(1) / O(log n)     |
| Find element by value in unsorted  | `unordered_set`         | O(1) average        |
| Check if key exists                | `unordered_map`         | O(1) average        |
| Smallest / largest element         | `set` / `priority_queue`| O(1) / O(log n)     |
| k‑th smallest element              | `vector` + `sort` + `nth_element` | O(n)       |
| Prefix sums / range queries        | `vector` + `partial_sum`| O(n) prep, O(1) query |
| Nearest larger element             | monotonic stack (use `vector`) | O(n)            |

**Never use** `std::list` in CP unless a problem explicitly requires O(1) insert/delete in the middle and you never need random access — even then, `vector` with careful erasing often wins because of cache.

---

### 10. Putting the STL to Work — Classic Patterns
Let’s solve a few small tasks that will teach you the STL in context.

#### Pattern 1: Counting distinct elements
```cpp
vector<int> arr = {1, 2, 2, 3, 3, 3};
set<int> s(arr.begin(), arr.end());
cout << s.size(); // 3
```
(Using `unordered_set` would be O(n) average.)

#### Pattern 2: Frequency counting
```cpp
map<int, int> freq;
for (int x : arr) freq[x]++;
for (auto &p : freq)
    cout << p.first << " appears " << p.second << " times\n";
```

#### Pattern 3: Removing duplicates from a vector while preserving order
```cpp
vector<int> v = {1,2,2,3,1};
unordered_set<int> seen;
vector<int> result;
for (int x : v) {
    if (!seen.count(x)) {
        seen.insert(x);
        result.push_back(x);
    }
}
```

#### Pattern 4: Two‑sum using a set
“Find two numbers that sum to target”:
```cpp
unordered_set<int> seen;
int target = 10;
for (int x : arr) {
    if (seen.count(target - x)) {
        cout << x << ' ' << target - x << endl;
        break;
    }
    seen.insert(x);
}
```

#### Pattern 5: Keeping the K largest elements
```cpp
priority_queue<int, vector<int>, greater<int>> min_heap;
for (int x : stream) {
    min_heap.push(x);
    if (min_heap.size() > k) min_heap.pop();
}
// min_heap now holds the k largest elements
```

#### Pattern 6: Next greater element using a stack (monotonic)
```cpp
vector<int> arr = {2,1,2,4,3};
vector<int> nge(arr.size());
stack<int> st; // stores indices
for (int i = 0; i < arr.size(); ++i) {
    while (!st.empty() && arr[st.top()] < arr[i]) {
        nge[st.top()] = arr[i];
        st.pop();
    }
    st.push(i);
}
```
You will see this pattern frequently. Notice how I use `vector` and `stack` together.

---

### 11. Debugging with the STL — Avoid Silent Death
STL containers are safe, but they can hide mistakes:

- **Accessing `map` with `[]` inserts a new element.** If you only want to read, use `map.find()` or C++20’s `.contains()`.
- **Iterators are invalidated after structural changes.** For `vector`, `push_back` may invalidate everything. For `map`/`set`, insertion and erasure of other elements does not invalidate iterators to existing ones, but erasing the element an iterator points to invalidates that iterator. Always do `it = s.erase(it);` in a loop.
- **`lower_bound` on a `set`:** Use the member function `s.lower_bound(val)`, not `std::lower_bound(s.begin(), s.end(), val)`. The latter is O(n) because it doesn’t know the tree structure.
- **Unordered containers can be slow** if you don’t reserve, or if you hit pathological collisions. In Codeforces, prefer `map`/`set` when log n is fine; use `unordered` only when you need O(1) and the constraints are tight, and then use a custom hash for safety (advanced).

---

### 12. Your Problem Set — This Is Where You Become Dangerous
You will now implement everything you learned. Do these in order. Do not look at solutions until you have spent at least 30 minutes on each.

1. **Distinct Numbers** (CSES 1621)  
   Read `n` integers, output the number of distinct values.  
   *Trivial with set, but try also with sort+unique.*

2. **Concert Tickets** (CSES 1091)  
   `n` ticket prices, `m` customer maximum bids. Each customer buys the most expensive ticket ≤ their bid, and that ticket is removed.  
   *Hint: Use a `multiset` (or `set` with a map of counts). Insert all tickets. For each bid, use `upper_bound` to find the first > bid, then go back one. Erase that ticket.*

3. **Sum of Two Values** (CSES 1640)  
   Find two distinct positions whose values sum to `x`.  
   *Hint: Use an unordered_map to store value → index as you scan. For each element, check if `x - a[i]` is already in the map.*

4. **Maximum Subarray Sum** (CSES 1643)  
   Classic Kadane’s algorithm. No STL magic, but you’ll use `vector` and a single loop.  
   *Keep current sum, if negative reset to 0, update maximum.*

5. **Traffic Lights** (CSES 1163)  
   There is a road of length `x`, and you add traffic lights one by one. After each addition, output the longest passage without a traffic light.  
   *Hint: Use a `set` of light positions and a `multiset` of gap lengths. Initially, the whole road is one gap. When adding light at `p`, find its neighbours in the set, remove the old gap, add two new gaps.*

6. **Playlist** (CSES 1141)  
   Longest sequence of songs with no repeats.  
   *Hint: Two pointers + set. Keep a window, if the new song is already in the set, remove songs from the left until it’s unique.*

After you solve these (or struggle enough to learn), go to Codeforces and filter problems with rating 1000–1200, tags “data structures” and “sortings”. Solve at least five. I recommend:  
- 160A (Twins) — sort + greedy  
- 58A (Chat room) — basic string  
- 977A (Wrong Subtraction) — simple loop  
- 133A (HQ9+) — string check  
- 281A (Word Capitalization) — string manipulation

---

### 13. What You Have Gained Today
You have not merely “learned some library”. You have acquired the standard toolkit of every competitive programmer. When you see a problem now, your brain should automatically ask:

- Do I need fast lookups? → map/set/unordered  
- Do I need the smallest/largest quickly? → priority_queue or set  
- Do I need to sort? → vector + sort  
- Do I need to eliminate duplicates? → set or sort+unique  
- Do I need to count frequencies? → map  
- Do I need to maintain a sliding window? → two pointers + set/map  

That mental reflex is what we are building.

**Before we meet again for Week 3, complete the six CSES problems and the five Codeforces warmups. Write a short program that reads a paragraph and prints the frequency of each word using a map and stringstream. Then, modify it to ignore case by converting to lowercase. If you get stuck, Google is allowed, but only to look up syntax. The algorithm must come from your head.**

You are no longer a beginner. You are a coder with the power of the STL. In Week 3, we will start thinking about time complexity, and I will teach you to never again submit a solution that gets “Time Limit Exceeded” without knowing exactly why.

Now go code.
