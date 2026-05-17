## Lesson 8 — The Hidden Engines: Stack, Queue, Deque, and Priority Queue

You have learned to loop, sort, search, and compute. But the most elegant solutions often hinge on the *order* in which you process elements. This week I will give you four data structures that control order: **stack** (last‑in‑first‑out), **queue** (first‑in‑first‑out), **deque** (both ends), and **priority queue** (always the “most important” first). You have used them lightly before; now we will master their competitive programming patterns.

These structures are not abstract. They are the backbone of:
- **Monotonic stacks** for finding the next greater element in O(n).
- **Sliding window maximum/minimum** using a deque in O(n).
- **Greedy scheduling** using a priority queue in O(n log n).
- **BFS** (next month) uses a queue; **DFS** uses a stack.

By the end of this session, you will not only know the syntax — you will instinctively recognise *when* a problem is a stack problem, *when* it needs a monotonic queue, and *when* a priority queue transforms a naive O(n²) into an elegant O(n log n).

Open your editor. Every data structure must be written, tested, and broken. Let’s begin.

---

### Part 1 — The Stack: Last In, First Out

A stack is a sequence where you can only push to the top, pop from the top, and inspect the top. STL provides `std::stack<T>`.

```cpp
stack<int> st;
st.push(10);
st.push(20);
cout << st.top();   // 20
st.pop();           // removes 20
cout << st.size();  // 1
```

#### 1.1 Balanced Parentheses — The Classic
Check if a string of `()` is balanced. Scan left to right. Push open brackets, pop when matching close brackets.
```cpp
bool isValid(string s) {
    stack<char> st;
    for (char c : s) {
        if (c == '(') st.push(c);
        else {
            if (st.empty()) return false;
            st.pop();
        }
    }
    return st.empty();
}
```

If we have multiple bracket types `()[]{}`, match top with closing character. This is a classic stack problem. Practice: CF 5C “Longest Regular Bracket Sequence”. It requires stack of indices and a DP/stack trick.

#### 1.2 The Monotonic Stack — The Silent Killer
A monotonic stack maintains elements in increasing or decreasing order. When a new element violates the monotonicity, we pop elements and process them. It answers questions like **“find the nearest smaller element to the left”** in O(n).

**Problem:** For each element, find the nearest smaller element to its left.
```cpp
vector<int> nearestSmallerLeft(vector<int>& a) {
    int n = a.size();
    vector<int> res(n, -1);
    stack<int> st; // stores indices
    for (int i = 0; i < n; ++i) {
        while (!st.empty() && a[st.top()] >= a[i]) {
            st.pop();
        }
        res[i] = st.empty() ? -1 : st.top();
        st.push(i);
    }
    return res; // indices
}
```
The key: we pop elements that are *larger* than the current because they can never be the “nearest smaller” for any future element (the current element is smaller and closer). This gives O(n) total pops/pushes.

**CSES 1645 – Nearest Smaller Values** is exactly this. Use `res[i] = (st.empty() ? 0 : st.top()+1)` (1‑indexed). Solve it now: scan, maintain stack of indices with increasing values. You’ll see how powerful this is.

#### 1.3 Next Greater Element (Monotonic Stack variant)
To find the next greater element to the right, iterate from right to left (or left to right with stack of indices, but pop when top ≤ current). This is used in many problems.
```cpp
vector<int> nextGreaterRight(vector<int>& a) {
    int n = a.size();
    vector<int> res(n, -1);
    stack<int> st;
    for (int i = n-1; i >= 0; --i) {
        while (!st.empty() && a[st.top()] <= a[i]) st.pop();
        res[i] = st.empty() ? -1 : st.top();
        st.push(i);
    }
    return res;
}
```
For CF 547B “Mike and Chocolate Thieves” (not exactly, but many). Actually a classic is “Largest Rectangle in Histogram”, which uses both left and right smaller elements.

---

### Part 2 — The Queue and Deque: First In, First Out, and Both Ends

#### 2.1 STL Queue and Deque
`std::queue` is a simple FIFO. `std::deque` (double‑ended queue) allows push/pop from front and back in O(1). `deque` can also access elements by index `[]`, but unlike vector, it's not guaranteed to be contiguous memory, but still O(1) random access. For CP, `deque` is most useful as a sliding window assistant.

```cpp
queue<int> q;
q.push(1); q.push(2); q.pop(); // removes 1

deque<int> dq;
dq.push_back(1);
dq.push_front(2);
int f = dq.front(); // 2
dq.pop_front();
```

#### 2.2 Sliding Window Maximum — The Deque Magic
**Problem:** Given array and window size `k`, find the maximum in each sliding window. Naively O(n*k) is too slow. We can use a deque that stores *indices* of useful elements. The deque maintains decreasing order of values. For each new element, we:
- Remove indices from the back while their value ≤ current (they can never be maximum for future windows because the current element is larger and further right).
- Push current index.
- Remove index from front if it’s outside the window (i.e., ≤ i‑k).
- The maximum is the value at `dq.front()`.

```cpp
vector<int> maxSlidingWindow(vector<int>& a, int k) {
    deque<int> dq;
    vector<int> res;
    for (int i = 0; i < a.size(); ++i) {
        while (!dq.empty() && a[dq.back()] <= a[i])
            dq.pop_back();
        dq.push_back(i);
        if (dq.front() <= i - k)
            dq.pop_front();
        if (i >= k - 1)
            res.push_back(a[dq.front()]);
    }
    return res;
}
```
Complexity O(n). Similarly for minimum sliding window. This pattern is a must for CSES “Sliding Median” (which uses two heaps, not deque), but the max/min sliding window is essential. CSES problem “Subarray Sums I/II” use prefix sums, not this. But there is CSES “Sliding Window Median” (1076) which uses two multisets/heaps. Still, the deque sliding maximum is common.

#### 2.3 Queue for BFS (Preview)
Breadth‑first search uses a queue to explore level by level. We will do this in Week 10. For now, just know that `queue` is for BFS, `stack` for DFS.

---

### Part 3 — Priority Queue: The Element That Always Rises to the Top

A priority queue is a heap. `std::priority_queue<T>` is a max‑heap by default: the largest element is on top. For min‑heap, use `priority_queue<T, vector<T>, greater<T>>`. Custom comparators can be provided.

#### 3.1 Basic Usage
```cpp
priority_queue<int> pq; // max-heap
pq.push(10); pq.push(30); pq.push(20);
cout << pq.top(); // 30
pq.pop();

// min-heap
priority_queue<int, vector<int>, greater<int>> min_pq;
min_pq.push(10); min_pq.push(30); min_pq.push(20);
cout << min_pq.top(); // 10
```

#### 3.2 Custom Comparator for Complex Objects
For a pair where we want min‑heap by first, then by second:
```cpp
using pii = pair<int, int>;
priority_queue<pii, vector<pii>, greater<pii>> pq;
// Or custom lambda with decltype:
auto cmp = [](pii a, pii b) { return a.second > b.second; }; // min by second
priority_queue<pii, vector<pii>, decltype(cmp)> pq(cmp);
```

#### 3.3 Greedy Scheduling with Priority Queue
Many problems require you to maintain a pool of ongoing tasks and always pick the one that ends earliest (or costs least). Example: **CSES 1630 – Tasks and Deadlines** (already solved with sorting). But a more advanced use: **CSES 1163 – Traffic Lights** uses set, not heap. However, the classic “Schedule to maximize number of jobs” can use a heap to decide which job to drop.

Another classic: **CF 1133C – Yet Another Ball Problem** (no), but there's **CF 1353D – Constructing the Array**. That problem: start with array of zeros, repeatedly pick the longest contiguous subsegment of zeros, choose the leftmost if tie, and set the middle element to the operation number. This can be done with a priority queue of segments ordered by length (desc) and then left index (asc). That is a perfect use of custom priority queue.

#### 3.4 Two Heaps for Median Maintenance
To support adding numbers and querying median online, maintain two heaps: max‑heap for lower half, min‑heap for upper half. This solves CSES 1076 “Sliding Median”. But that's for later. We'll mention it as an advanced pattern.

---

### Part 4 — Linked List (Almost Never Used, But Know It Exists)

`std::list` is a doubly‑linked list. O(1) insert/erase anywhere if you have an iterator, but no random access. In CP, it's rarely needed because `vector` and `set` cover most use‑cases. Sometimes problems require O(1) removal from the middle given a reference (e.g., LRU cache). In that case, `list` with `unordered_map` of iterators works. I'll mention it but won't spend much time.

---

### Part 5 — Combined Problem Walkthroughs

Let's apply these structures to solve classic problems from scratch.

#### 5.1 CSES 1645 – Nearest Smaller Values (Monotonic Stack)
Given array `x`, for each position output the nearest position to the left with a smaller value. Implementation already described. The key is understanding why the stack pops: if `x[st.top()] >= x[i]`, that stack element is useless for any future `j > i` because `x[i]` is smaller and closer. So we discard it. This gives O(n).

#### 5.2 CSES 1087 – Sliding Window Median (Two Heaps) — Advanced
We will not fully dive today, but I'll outline: maintain a max-heap `left` and min-heap `right`. For each new element, add to `left` or `right`, then rebalance to keep sizes equal or left one larger. Median is `left.top()`. When removing an element that falls out of window, we lazily delete using a map of pending removals. This is a classic hard problem. I'll assign it as a challenge.

#### 5.3 CSES 1163 – Traffic Lights (Set, Not Stack/Queue)
We solved with set/multiset before. Revisit to see that we used a set for positions and multiset for gaps. Not this lesson's focus.

#### 5.4 CF 1353D – Constructing the Array (Priority Queue)
We have a priority queue of intervals `[l, r]` sorted by length descending, then l ascending. In each step, pop the top interval, compute mid, assign value, push new intervals if non-empty. This is a great exercise.

#### 5.5 CF 547B – Mike and Feet (Monotonic Stack)
We can find for each element the maximum subarray size where it is the minimum. Use left smaller and right smaller indices to compute width. Then answer queries. Worth doing.

---

### Part 6 — When to Use Which Structure: Cheat Sheet

- **Need LIFO processing (parsing, backtracking, undo operations):** Stack.
- **Need FIFO processing (BFS, level order):** Queue.
- **Need both ends push/pop and sliding window max/min:** Deque.
- **Always need the largest/smallest element, dynamic insertions, scheduling:** Priority Queue.
- **Find nearest greater/smaller element in O(n):** Monotonic Stack.
- **Median of a stream / sliding window median:** Two Heaps (Priority Queues).
- **Range queries with point updates:** Fenwick/Segment Tree (later).

---

### Part 7 — Complexity Recap

- Stack, Queue, Deque operations: O(1) push/pop/top.
- Priority Queue push/pop: O(log n), top O(1).
- Monotonic stack/queue: amortized O(n) because each element enters and leaves at most once.

Always analyse the number of times each element is pushed and popped. That's the trick to seeing O(n) despite nested loops.

---

### Part 8 — Your Problem Set for Week 8

Do these problems in order, implementing from scratch. Do not copy templates; write them yourself until they become muscle memory.

1. **CSES 1645 – Nearest Smaller Values** (monotonic stack)
2. **CF 5C – Longest Regular Bracket Sequence** (stack of indices, DP for lengths)
3. **CF 1353D – Constructing the Array** (priority queue with custom comparator)
4. **CF 279B – Books** (two pointers, but you can also use queue while maintaining sum)
5. **CSES 1076 – Sliding Median** (two heaps, challenge yourself)
6. **CF 547B – Mike and Feet** (monotonic stack for min ranges)
7. **Leetcode 239 – Sliding Window Maximum** (deque) — implement and test.
8. **CSES 1087 – Sliding Median** (actually same as 1076, do it).
9. **CF 343B – Alternating Current** (stack simulation)
10. **CF 1585C – Minimize Distance** (greedy with sorting, but can use stack of trips)

As a mini‑project, implement a generic monotonic stack function that takes a vector and a comparator, and returns the index of the nearest element that satisfies the comparator (e.g., less, greater). Use it to solve both nearest smaller left and nearest greater left.

---

### Part 9 — What You Have Gained

You now control the flow of data. You know that a stack can be the key to solving parentheses and histogram problems in linear time; that a deque makes sliding windows trivial; that a priority queue can simulate an entire greedy strategy in a few lines. These structures are not just containers — they are algorithmic patterns that will appear again and again in graphs, dynamic programming, and beyond.

Next week we will finally step into the grand arena: **Graph Theory**. You will learn DFS, BFS, and how to traverse a connected world. The queue and stack you learned today will be the engine of those traversals. You are ready.

Now go solve Nearest Smaller Values. When you see that green “Accepted”, you will understand the beauty of O(n). See you next week.
