## Lesson 11 — Directed Acyclic Worlds: Topological Sort and DP on DAGs

Last week you learned to walk through graphs with DFS and BFS. This week we turn up the complexity: **directed edges** and **order constraints**. Many real‑world problems — course prerequisites, project dependencies, game levels — impose a partial order. To solve them you need to linearise the graph, and that is **topological sorting**. And when that order is combined with DP, you unlock the ability to solve longest‑path, counting, and scheduling problems on acyclic directed graphs in linear time.

Today you will:
- Represent directed graphs and understand in‑degree/out‑degree.
- Topologically sort a DAG using both Kahn’s algorithm (BFS) and DFS.
- Detect cycles in directed graphs — because a topological order exists **iff** there is no cycle.
- Solve DP on DAGs: longest path, number of paths, etc.

Open your editor. I will guide you through every concept and every line of code. You will write everything from scratch.

---

### Part 1 — Directed Graphs: The One‑Way Streets

A directed graph \( G = (V, E) \) consists of vertices and ordered pairs \( (u \rightarrow v) \). Edges have direction: you can travel from `u` to `v` but not necessarily back.

Terminology:
- **In‑degree:** number of edges entering a vertex.
- **Out‑degree:** number of edges leaving a vertex.
- **Source:** vertex with in‑degree 0.
- **Sink:** vertex with out‑degree 0.
- **DAG:** Directed Acyclic Graph — no directed cycles. Many real problems are DAGs (e.g., course prerequisites).

**Representation:** the same adjacency list but only adding directed edges.
```cpp
int n, m;
cin >> n >> m;
vector<vector<int>> g(n+1);
vector<int> indeg(n+1, 0);
for (int i = 0; i < m; ++i) {
    int u, v;
    cin >> u >> v;
    g[u].push_back(v);       // u → v
    indeg[v]++;
}
```

---

### Part 2 — Topological Sort: Linearising a DAG

A topological ordering of a DAG is a permutation of vertices such that for every directed edge \( u \to v \), `u` appears before `v`. It exists if and only if the graph has no directed cycles.

Applications:
- Scheduling tasks with dependencies.
- Finding the longest path in a DAG.
- Counting the number of ways to reach a node in a DAG.

#### 2.1 Kahn’s Algorithm (BFS using in‑degrees)
Repeatedly remove vertices with in‑degree 0. These have no prerequisites; they can be placed next in the order. When you remove a vertex, decrement the in‑degree of its neighbours.

```cpp
vector<int> topo;
queue<int> q;
for (int i = 1; i <= n; ++i)
    if (indeg[i] == 0)
        q.push(i);

while (!q.empty()) {
    int u = q.front(); q.pop();
    topo.push_back(u);
    for (int v : g[u]) {
        if (--indeg[v] == 0)
            q.push(v);
    }
}

if (topo.size() != n) {
    cout << "Graph has a cycle, no topological order.\n";
}
```
Complexity: O(V + E). This also detects cycles: if fewer than `n` vertices are processed, a cycle exists.

**CSES 1679 – Course Schedule:** Determine if it’s possible to complete all courses given prerequisites. If possible, output any valid order. Exactly Kahn’s algorithm.

#### 2.2 DFS‑Based Topological Sort
Perform DFS, but add a vertex to the front of the order **after** exploring all its outgoing edges (post‑order). This naturally puts sinks first; reversing the post‑order gives a topological order.

```cpp
vector<int> topo;
vector<int> vis(n+1, 0); // 0=unvisited, 1=visiting, 2=visited

bool dfs(int u) {
    vis[u] = 1;
    for (int v : g[u]) {
        if (vis[v] == 1) return false; // back edge → cycle
        if (vis[v] == 0 && !dfs(v)) return false;
    }
    vis[u] = 2;
    topo.push_back(u);   // post-order
    return true;
}

bool ok = true;
for (int i = 1; i <= n; ++i)
    if (vis[i] == 0 && !dfs(i))
        ok = false;
if (!ok) cout << "IMPOSSIBLE";
else {
    reverse(topo.begin(), topo.end()); // now it's topological order
    for (int v : topo) cout << v << ' ';
}
```
This DFS three‑colour method simultaneously detects cycles. Use either algorithm; Kahn’s is often simpler, but DFS gives direct cycle detection and is more flexible for DP on DAGs (you can compute DP during the post‑order).

---

### Part 3 — Cycle Detection in Directed Graphs

In an undirected graph we just needed a parent check. In a directed graph, we must distinguish between a **back edge** (which creates a cycle) and a **cross/forward edge** (which does not). The three‑colour DFS (0=unvisited, 1=visiting, 2=visited) detects a cycle whenever we encounter an edge to a node that is currently in the “visiting” state (i.e., part of the current recursion stack). This is a back edge.

The same logic can be used to extract the cycle: maintain a stack `path` of the current DFS branch; when a back edge is found, the nodes from the visited node to the current node form a cycle.

For CSES 1678 – Round Trip II (finding a directed cycle), you need to find any cycle and output it. This is the directed version of last week’s undirected cycle finding. Use three‑colour DFS and a `parent` or `stack`.

```cpp
vector<int> path;
bool dfsCycle(int u) {
    vis[u] = 1;
    path.push_back(u);
    for (int v : g[u]) {
        if (vis[v] == 1) {
            // found cycle: path contains v..u
            // print from v to end of path
            auto it = find(path.begin(), path.end(), v);
            vector<int> cycle(it, path.end());
            cycle.push_back(v);
            cout << cycle.size() << '\n';
            for (int x : cycle) cout << x << ' ';
            exit(0);
        }
        if (vis[v] == 0 && dfsCycle(v)) return true;
    }
    vis[u] = 2;
    path.pop_back();
    return false;
}
```

---

### Part 4 — Dynamic Programming on DAGs

Because a DAG has no cycles, we can process vertices in topological order and use DP to solve problems like:

- Longest path from a source to a sink (or between any nodes).
- Number of distinct paths between two vertices.
- Maximum sum path.

#### 4.1 Longest Path in a DAG (CSES 1680 – Longest Flight Route)
Given a DAG, find the longest path from node 1 to node n, and print the path (if no path, “IMPOSSIBLE”).

We can compute `dp[u]` = length of longest path from `u` to `n`. We can push DP forward or pull DP backward. Using topological order and processing in reverse order, we can set `dp[n] = 1` and then for each `u` in reverse topo, `dp[u] = 1 + max(dp[v])` for all `v` in `g[u]`. To reconstruct the path, we store the successor `nxt[u]` that gives the max.

Alternatively, process in topological order and update successors: `dp[v] = max(dp[v], dp[u] + 1)`. This is forward DP. Initialize `dp[1] = 1`, others `-INF`. Then iterate over `u` in topo order, and for each `v` in `g[u]`, if `dp[u] != -INF` and `dp[u] + 1 > dp[v]`, update `dp[v]` and `nxt[v] = u`. Finally, if `dp[n] == -INF`, no path; else reconstruct backward from `n` using `nxt`.

**Implementation (forward DP):**
```cpp
vector<int> topo; // from Kahn's
vector<int> dp(n+1, -1e9);
vector<int> nxt(n+1, -1);
dp[1] = 1;
for (int u : topo) {
    if (dp[u] == -1e9) continue;
    for (int v : g[u]) {
        if (dp[u] + 1 > dp[v]) {
            dp[v] = dp[u] + 1;
            nxt[v] = u;
        }
    }
}
if (dp[n] == -1e9) cout << "IMPOSSIBLE\n";
else {
    cout << dp[n] << '\n';
    vector<int> path;
    for (int v = n; v != -1; v = nxt[v])
        path.push_back(v);
    reverse(path.begin(), path.end());
    for (int v : path) cout << v << ' ';
}
```

Note: We must ensure that 1 and n are in the same topological order reachable. Since we start with Kahn’s, the topo includes all vertices. DP will only be computed for vertices reachable from 1 because we initialize only dp[1].

#### 4.2 Counting Number of Paths (CSES 1681 – Game Routes)
Count number of ways to go from 1 to n modulo M. This is forward DP: `ways[v] += ways[u]` for each edge `u→v` in topological order. Initialize `ways[1] = 1`, others 0. Process u in topo, propagate to neighbours.

```cpp
vector<long long> ways(n+1, 0);
ways[1] = 1;
for (int u : topo) {
    for (int v : g[u]) {
        ways[v] = (ways[v] + ways[u]) % MOD;
    }
}
cout << ways[n] << '\n';
```

---

### Part 5 — Combined Problem Walkthroughs

#### 5.1 CSES 1679 Course Schedule (Kahn’s)
Simple: read edges, Kahn, if topo.size() < n → IMPOSSIBLE, else print order.

#### 5.2 CSES 1680 Longest Flight Route (described above)
Implement forward DP with topo from Kahn. But careful: you need topo order that respects reachability; Kahn from all indegree‑0 nodes works, and then we can still use it for DP. The DP will only affect reachable nodes because we only update from nodes with dp != -INF. Works.

#### 5.3 CSES 1681 Game Routes (counting)
As above. Ensure to handle modulo.

#### 5.4 CSES 1678 Round Trip II (directed cycle)
Implement DFS three‑color with cycle extraction as described. If no cycle, print "IMPOSSIBLE".

#### 5.5 CSES 1682 Flight Routes Check (not DAG, but reachability) — not this week; we can mention later.

#### 5.6 CSES 1750 Planets Queries I (binary lifting) — not now.

---

### Part 6 — When to Use Topological Sort and DP on DAGs

- The problem mentions **“before/after”**, **“prerequisite”**, **“dependency”**.
- You need to **schedule tasks** or **check feasibility** of a set of constraints.
- You need the **longest path** or **number of paths** in a directed graph that is guaranteed acyclic (or you need to detect cycles).
- Any DAG can be transformed into a DP problem: order = topological sort, state = dp[node], transitions = outgoing edges.

---

### Part 7 — Common Pitfalls

- **Forgetting to check for cycles:** If the problem doesn’t guarantee a DAG, always check. Kahn’s gives you that check for free.
- **Using DFS for DP without reverse post‑order:** If you compute DP during DFS, you must process children first (post‑order). The easiest method is to use topological order from Kahn’s and iterate.
- **Large recursion depth in DFS:** Use Kahn’s or an explicit stack for topological sort if recursion limit is a concern.
- **Multiple sources/sinks:** In DP, ensure you initialise all sources correctly. Often we set dp[source] = 0 or 1, others -INF/0.
- **Directed graph may be disconnected:** Always loop over all vertices to initialise indegrees/DFS.

---

### Part 8 — Your Assignment for Week 11

These are all CSES problems; they are the standard ladder for directed graphs. Solve them in order.

1. **CSES 1679 Course Schedule** — topological sort (Kahn).
2. **CSES 1680 Longest Flight Route** — DP on DAG.
3. **CSES 1681 Game Routes** — counting paths.
4. **CSES 1678 Round Trip II** — directed cycle detection.
5. **CSES 1682 Flight Routes Check** — not a DAG problem but checks if there is a vertex that can reach all others and be reached by all others (strongly connected components); we will learn SCC later, but try to solve with DFS and reachability from 1 and reverse graph (not this week’s focus, but a challenge).
6. **CSES 1750 Planets Queries I** — jumping in a functional graph (each node has exactly one outgoing edge). This is a tree of cycles; we will cover later. For now, just attempt with binary lifting after studying that technique.

**Core focus: problems 1‑4.** Master them completely. Write both Kahn and DFS‑based topological sort. For DP on DAG, write both forward and backward variations and test on random small DAGs against brute force (enumerate all paths for longest).

---

### Part 9 — What You Have Gained

You now command the directed world. You can take a set of constraints, build a graph, detect cycles, and if the graph is acyclic, you can extract a linear order and compute longest paths or count routes in O(V+E). This combination of graph theory and DP is one of the most elegant and powerful tools in competitive programming.

Next week we will add weights: **Dijkstra’s algorithm** and the concept of **shortest paths**. You will see how a priority queue can find the shortest path in a weighted graph as efficiently as BFS does in an unweighted one. The topological sort you learned today will be the basis for understanding why Dijkstra works.

Now, open CSES 1679, type out Kahn’s algorithm, and don’t stop until you have accepted solutions for all four core problems. See you next week.
