## Lesson 10 — The World as Nodes and Edges: Graph Theory I

You have mastered arrays, stacks, queues, and recursion. Now we connect everything. A **graph** is the most flexible model in computing. A social network, a road map, a dependency tree — all are graphs. For the next three weeks we will live inside graphs, and by the end you will wield them as naturally as a vector.

Today we lay the absolute foundation: representation, traversal, connectivity, and basic cycle detection. Everything else (shortest paths, minimum spanning trees, flows) builds on these two algorithms: **Depth‑First Search (DFS)** and **Breadth‑First Search (BFS)**.

Open your editor. Every data structure and every traversal will be typed, compiled, and run. I will walk with you through every line.

---

### Part 1 — What Is a Graph?

A graph \( G = (V, E) \) consists of a set of **vertices** (nodes) \( V \) and a set of **edges** \( E \). An edge connects two vertices. In an **undirected** graph, edges have no direction; in a **directed** graph, each edge goes from \( u \) to \( v \). Edges may have **weights** (costs, distances). For today we focus on **unweighted, undirected** graphs. That is where your intuition must solidify first.

Terminology:
- **Adjacent:** two vertices connected by an edge.
- **Degree:** number of incident edges.
- **Path:** sequence of vertices where each adjacent pair is connected.
- **Connected:** there is a path between every pair of vertices.
- **Component:** maximal connected subgraph.
- **Cycle:** a path that starts and ends at the same vertex without reusing edges.
- **Tree:** connected acyclic graph.

In competitive programming, graphs appear in two forms:
- **Explicit:** given \( n \) nodes and \( m \) edges.
- **Implicit:** e.g., a grid where each cell is a node and neighbours are edges (up/down/left/right).

---

### Part 2 — Representing a Graph in C++

You need a representation that allows fast traversal of neighbours. Three common ways:

#### 2.1 Edge List
Simply store all edges as pairs. Used for Kruskal’s MST.
```cpp
vector<pair<int,int>> edges;
edges.push_back({1,2});
```

#### 2.2 Adjacency Matrix
`bool adj[1000][1000]` or `vector<vector<int>>`. `adj[u][v] = 1` if edge exists. O(V²) memory, only suitable when V ≤ 5000 and edges are dense. Rarely used.

#### 2.3 Adjacency List — The Champion
For each vertex, keep a vector of its neighbours. Memory O(V + E). Traversing all neighbours of a vertex is O(degree).
```cpp
int n, m;
cin >> n >> m;
vector<vector<int>> g(n+1);  // 1‑based indexing
for (int i = 0; i < m; ++i) {
    int u, v;
    cin >> u >> v;
    g[u].push_back(v);
    g[v].push_back(u);  // undirected
}
```
This is your default for 99% of problems. For weighted edges, store `vector<vector<pair<int,int>>>` where pair is (neighbour, weight).

**Grid implicit graph:** For an `n x m` grid, we treat each cell `(r,c)` as a node. We do not build an explicit adjacency list; instead we check neighbours via direction arrays.

---

### Part 3 — Depth‑First Search (DFS): Going Deep

DFS explores as far as possible along each branch before backtracking. It naturally uses recursion (or an explicit stack). DFS is the Swiss Army knife: connectivity, cycle detection, topological order, bridges, etc.

#### 3.1 Recursive DFS on Adjacency List
We need a `visited` array to avoid cycles.
```cpp
vector<bool> vis(n+1, false);

void dfs(int u) {
    vis[u] = true;
    cout << "Visiting " << u << '\n';
    for (int v : g[u]) {
        if (!vis[v]) {
            dfs(v);
        }
    }
}
```
Call `dfs(start_node)` for one component. To cover all components:
```cpp
for (int i = 1; i <= n; ++i) {
    if (!vis[i]) {
        dfs(i);
    }
}
```
Time complexity: O(V + E) — each vertex visited once, each edge examined twice in undirected graph.

#### 3.2 DFS on a Grid — Flood Fill
Classic example: CSES 1192 **Counting Rooms**. Given a grid of `'.'` (floor) and `'#'` (wall), count the number of rooms (connected floor areas). We can treat each floor cell as a node, edges to adjacent floor cells.

**Implementation:**
```cpp
int n, m;
vector<string> grid;
vector<vector<bool>> vis;
int dx[] = {0, 0, 1, -1};
int dy[] = {1, -1, 0, 0};

void dfs(int x, int y) {
    vis[x][y] = true;
    for (int d = 0; d < 4; ++d) {
        int nx = x + dx[d];
        int ny = y + dy[d];
        if (nx >= 0 && nx < n && ny >= 0 && ny < m &&
            !vis[nx][ny] && grid[nx][ny] == '.') {
            dfs(nx, ny);
        }
    }
}
```
Then count components:
```cpp
int rooms = 0;
for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
        if (grid[i][j] == '.' && !vis[i][j]) {
            ++rooms;
            dfs(i, j);
        }
    }
}
cout << rooms << '\n';
```
Note: recursion depth may be up to n*m = 10⁶, which could overflow stack on some judges. In that case, increase stack size (`-Wl,-stack,268435456` on Windows) or use an explicit stack. In practice, most online judges allow a few hundred thousand recursion depth. If you hit recursion limit, we can rewrite DFS iteratively with `stack`.

#### 3.3 Iterative DFS (Stack)
```cpp
stack<pair<int,int>> st;
st.push({x,y});
vis[x][y] = true;
while (!st.empty()) {
    auto [cx, cy] = st.top(); st.pop();
    // process
    for (int d=0; d<4; ++d) {
        int nx = cx+dx[d], ny = cy+dy[d];
        if (inside(nx,ny) && !vis[nx][ny] && grid[nx][ny]=='.') {
            vis[nx][ny] = true;
            st.push({nx, ny});
        }
    }
}
```
This uses the same logic but avoids recursion.

---

### Part 4 — Breadth‑First Search (BFS): Layer by Layer

BFS explores vertices in order of distance from the source. It uses a queue and finds **shortest paths in unweighted graphs**.

#### 4.1 BFS on Adjacency List
```cpp
vector<int> dist(n+1, -1);
queue<int> q;
dist[start] = 0;
q.push(start);
while (!q.empty()) {
    int u = q.front(); q.pop();
    for (int v : g[u]) {
        if (dist[v] == -1) {   // not visited
            dist[v] = dist[u] + 1;
            q.push(v);
        }
    }
}
```
After BFS, `dist[v]` is the shortest distance from `start` to `v` (or -1 if unreachable). BFS runs in O(V+E).

#### 4.2 Grid BFS: CSES 1193 Labyrinth
Find shortest path from 'A' to 'B' in a grid. BFS from start, store predecessor direction to reconstruct path.

```cpp
int n, m;
vector<string> grid;
vector<vector<int>> dist;
vector<vector<pair<int,int>>> pred;
int dx[4] = {0,0,1,-1};
int dy[4] = {1,-1,0,0};
char dir[4] = {'R','L','D','U'};

queue<pair<int,int>> q;
int sx, sy, ex, ey; // find 'A' and 'B' positions
dist.assign(n, vector<int>(m, -1));
pred.assign(n, vector<pair<int,int>>(m, {-1,-1}));

dist[sx][sy] = 0;
q.push({sx, sy});
while (!q.empty()) {
    auto [x,y] = q.front(); q.pop();
    if (x == ex && y == ey) break;
    for (int d=0; d<4; ++d) {
        int nx = x+dx[d], ny = y+dy[d];
        if (nx>=0 && nx<n && ny>=0 && ny<m && grid[nx][ny]!='#' && dist[nx][ny]==-1) {
            dist[nx][ny] = dist[x][y] + 1;
            pred[nx][ny] = {x,y};
            q.push({nx, ny});
        }
    }
}
```
Then if `dist[ex][ey] != -1`, reconstruct path from `ex,ey` back to start by following `pred` and reverse. This is the classic BFS shortest path.

---

### Part 5 — Connected Components & Counting

We already saw Counting Rooms. For any graph:
```cpp
int components = 0;
for (int i = 1; i <= n; ++i) {
    if (!vis[i]) {
        components++;
        dfs(i);
    }
}
```
You can also label each node with its component ID, store component sizes, etc. This is used in many DSU (union‑find) problems, but DFS suffices for static graphs.

---

### Part 6 — Cycle Detection in Undirected Graph

Detect if there is a cycle. Using DFS, keep track of the parent vertex. If we encounter an already visited neighbour that is **not** the parent, we have found a cycle.

```cpp
bool hasCycle = false;

void dfs(int u, int parent) {
    vis[u] = true;
    for (int v : g[u]) {
        if (!vis[v]) {
            dfs(v, u);
        } else if (v != parent) {
            hasCycle = true;
        }
    }
}
```
This detects any cycle. To find the cycle itself, we can maintain a stack of current path. That is more involved and covered in CSES 1669 Round Trip.

For an undirected graph, a DFS tree with a back edge to a non‑parent indicates a cycle.

---

### Part 7 — Bipartite (Two‑Colouring) Check

A graph is bipartite if its vertices can be divided into two sets such that every edge connects vertices from different sets. Equivalent to having no odd‑length cycles. We can check via BFS or DFS while assigning colours (0/1). If we ever try to assign a different colour to an already coloured node, it's not bipartite.

```cpp
vector<int> color(n+1, -1); // -1 uncolored

bool dfsBipartite(int u, int c) {
    color[u] = c;
    for (int v : g[u]) {
        if (color[v] == -1) {
            if (!dfsBipartite(v, c ^ 1)) return false;
        } else if (color[v] == c) {
            return false; // same color on both ends
        }
    }
    return true;
}
```
Check all components: `if (color[i]==-1 && !dfsBipartite(i,0)) { not bipartite; }`. CSES 1668 Building Teams.

---

### Part 8 — CSES Graph Problem Walkthroughs

Let's apply these directly to the CSES Graph Algorithms set.

#### 8.1 Counting Rooms (1192) — DFS flood fill
Already coded.

#### 8.2 Labyrinth (1193) — BFS grid + path reconstruction
As above. Print "YES", distance, and path string. If no path, "NO".

#### 8.3 Building Roads (1666) — Connected components
Count components in a graph. To connect them into a single tree, you need `k-1` new roads. Store one node from each component and connect them sequentially. DFS to find components.
```cpp
int comps = 0;
vector<int> rep;
for (int i=1; i<=n; ++i) {
    if (!vis[i]) {
        rep.push_back(i);
        dfs(i);
    }
}
cout << rep.size()-1 << '\n';
for (size_t i=0; i+1<rep.size(); ++i)
    cout << rep[i] << ' ' << rep[i+1] << '\n';
```

#### 8.4 Message Route (1667) — BFS single‑source shortest path
Find shortest path between 1 and n. BFS from 1, store predecessor, reconstruct.

#### 8.5 Building Teams (1668) — Bipartite check
Exactly as described. Print 1 or 2 for each pupil, or "IMPOSSIBLE".

#### 8.6 Round Trip (1669) — Cycle finding
Slightly advanced. DFS with a parent array and also a `path` stack. When a back edge is found, the stack from that node to the current node forms a cycle. We'll tackle that later; for now just detect if cycle exists.

---

### Part 9 — Complexity and Pitfalls

- **Stack overflow:** Recursive DFS may crash for large grids (e.g., 1000×1000 = 10⁶ depth). Use explicit stack or increase stack size.
- **Adjacency list size:** Always allocate `n+1` when 1‑based. If input is 0‑based, adjust.
- **Directed graphs:** In directed, the cycle detection is different (needs three‑color DFS). We'll do next week.
- **BFS queue:** Use `queue<int>`. Avoid `while(!q.empty())` with `q.front(); q.pop();`. Always mark visited when pushing, not when popping, to avoid duplicate enqueues.
- **Disconnected graphs:** Always loop over all nodes to cover all components.

---

### Part 10 — Problem Set for Week 10

Your task: solve these CSES Graph problems using DFS/BFS from scratch. Do not use pre‑written templates; write each traversal by hand.

1. **CSES 1192 Counting Rooms** (DFS flood fill)
2. **CSES 1193 Labyrinth** (BFS shortest path)
3. **CSES 1666 Building Roads** (connected components)
4. **CSES 1667 Message Route** (BFS, path reconstruction)
5. **CSES 1668 Building Teams** (bipartite check)
6. **CSES 1669 Round Trip** (detect cycle and find one cycle; DFS with parent and path stack)

Also, as a bridge to next week, implement an **iterative DFS** for Counting Rooms using an explicit stack, and compare performance with recursive DFS.

**Challenge:** Write a BFS that computes the distance from a given start to all nodes, and then find the node with the maximum distance (the diameter endpoint). This will be used later for tree diameter.

---

### Part 11 — Looking Ahead

Next week, we will move to **directed graphs** and **weighted graphs**. You will learn topological sorting, Dijkstra’s shortest path algorithm, and start seeing how DP and graphs combine (DP on DAGs). But first, master the undirected graph in your sleep. BFS and DFS are the two breaths of graph theory. Everything else is built on them.

Open CSES, start with Counting Rooms, and do not stop until you have “Accepted” on all six problems. I will be here when you return for Week 11, ready to show you the directed world. Now go. Graph the world.
