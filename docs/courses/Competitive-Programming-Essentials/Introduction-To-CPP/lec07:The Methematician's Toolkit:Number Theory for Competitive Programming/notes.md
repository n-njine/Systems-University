## Lesson 7 — The Mathematician’s Toolkit: Number Theory for Competitive Programming

You have been wielding algorithms that operate on arrays and sequences. Today we go deeper — into the integers themselves. Number theory is not a sideshow; it is one of the pillars of competitive programming. From primality to combinatorics, the problems in this domain are ubiquitous. They can seem intimidating, but with the right foundation you will reduce them to a small set of standard techniques that you can code in your sleep.

In this session I will teach you:

- How to test if a number is prime — and generate all primes up to 10⁷ in a fraction of a second.
- The Euclidean algorithm for GCD, and its extended version.
- Modular arithmetic, including fast exponentiation and modular inverses.
- How to compute binomial coefficients without overflow.

By the end, you will solve every math problem on CSES that appears in the first half of the list. You will also have a permanent reference for the most common number‑theoretic code snippets.

Open your editor. Write every function as I describe it. Then test it on the examples I give. This is dense material, but it is also incredibly satisfying. Let’s begin.

---

### Part 1 — Primality Testing: Is This Number Prime?

Given a positive integer `n`. You want to return `true` if it is prime, `false` otherwise.

The simplest correct method is **trial division** up to √n. If `n` is divisible by any integer from 2 to ⌊√n⌋, it is composite. Otherwise it is prime.

```cpp
bool isPrime(long long n) {
    if (n <= 1) return false;
    if (n <= 3) return true;   // 2 and 3 are prime
    if (n % 2 == 0 || n % 3 == 0) return false;
    // all primes > 3 are of form 6k ± 1
    for (long long i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}
```

This runs in O(√n). For n up to 10¹², √n is 10⁶ — it will pass in competitive programming if called only a few times. If you need to test millions of numbers, you need a sieve.

---

### Part 2 — Sieve of Eratosthenes: Generating All Primes Up to N

When N ≤ 10⁷ (or even 10⁸ with careful memory), the sieve is your best friend. The idea: create a boolean array `is_prime` of size `N+1`, initially all `true`. Then for each `i` from 2 to √N, if `is_prime[i]` is true, mark all multiples of `i` starting from `i*i` as false.

```cpp
vector<int> getPrimes(int N) {
    vector<bool> is_prime(N + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= N; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= N; j += i)
                is_prime[j] = false;
        }
    }
    vector<int> primes;
    for (int i = 2; i <= N; ++i)
        if (is_prime[i])
            primes.push_back(i);
    return primes;
}
```

The complexity is O(N log log N), essentially linear for N up to 10⁷. Use `vector<char>` or `bitset` to save memory if N is large.

**Why start at i*i?** Any smaller multiple `i*k` with `k < i` has already been marked by a smaller prime factor.

**Practice:** Generate all primes up to 10⁷ and print the 10,001st prime (Project Euler 7). Or simply use it to answer many queries fast.

---

### Part 3 — Prime Factorization Using Smallest Prime Factor (SPF)

Often you need to factor many numbers quickly. The **SPF sieve** precomputes for each integer up to N its smallest prime factor. Then you can factor any number ≤ N in O(log n) time by repeatedly dividing by SPF.

```cpp
vector<int> spf; // smallest prime factor

void buildSPF(int N) {
    spf.assign(N + 1, 0);
    for (int i = 2; i <= N; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            for (long long j = (long long)i * i; j <= N; j += i) {
                if (spf[j] == 0)
                    spf[j] = i;
            }
        }
    }
}

vector<pair<int, int>> factorize(int x) {
    vector<pair<int, int>> factors;
    while (x > 1) {
        int p = spf[x];
        int cnt = 0;
        while (x % p == 0) {
            x /= p;
            cnt++;
        }
        factors.emplace_back(p, cnt);
    }
    return factors;
}
```

This is O(N log log N) to build and O(log x) per factorization. Perfect for problems that ask for the number of divisors, sum of divisors, etc., for many numbers.

**Number of divisors:** If n = ∏ p_i^a_i, then d(n) = ∏ (a_i + 1). You can compute this using the factorization.

**Sum of divisors:** ∏ (p_i^(a_i+1) - 1) / (p_i - 1).

---

### Part 4 — Greatest Common Divisor and Least Common Multiple

#### 4.1 Euclidean Algorithm (O(log min(a,b)))
```cpp
int gcd(int a, int b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}
// or recursively:
int gcd(int a, int b) { return b == 0 ? a : gcd(b, a % b); }
```
For `long long`, use `ll`.

**LCM:** `lcm(a, b) = a / gcd(a, b) * b`. Note the order: divide first to avoid overflow.
```cpp
long long lcm(long long a, long long b) {
    return a / gcd(a, b) * b;
}
```

#### 4.2 Extended Euclidean Algorithm
Finds integers `x, y` such that `a*x + b*y = gcd(a, b)`. This is essential for modular inverses and solving linear Diophantine equations.

```cpp
int extgcd(int a, int b, int &x, int &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    int x1, y1;
    int d = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return d;
}
```

In CP, you rarely need the full extended version except to compute modular inverses when the modulus is not prime (see next section).

---

### Part 5 — Modular Arithmetic: The Language of Large Numbers

In many problems, answers are huge and you must output them modulo `M` (often 10⁹+7). All arithmetic must be done modulo M.

**Basic operations:**
```
(a + b) % M
(a - b + M) % M   // to avoid negative
(a * b) % M
```
Since C++ `%` can return negative for negative numbers, always add `M` after subtraction.

**No division directly:** Division is multiplication by the **modular inverse**. To compute `(a / b) % M`, you need `a * modinv(b, M) % M`, which exists only if `gcd(b, M) == 1`.

#### 5.1 Modular Inverse via Fermat’s Little Theorem (when M is prime)
If M is prime, then `b^(M-1) ≡ 1 (mod M)` ⇒ `b^(M-2) ≡ b^(-1) (mod M)`. So we can compute the inverse using fast exponentiation.

#### 5.2 Fast Exponentiation (Binary Exponentiation)
Compute `a^b mod M` in O(log b).
```cpp
long long modPow(long long a, long long b, long long M) {
    long long res = 1;
    a %= M;
    while (b) {
        if (b & 1) res = (res * a) % M;
        a = (a * a) % M;
        b >>= 1;
    }
    return res;
}
```
Now modular inverse when M is prime:
```cpp
long long modInv(long long a, long long M) {
    return modPow(a, M - 2, M);
}
```
Note: This only works when `M` is prime and `a` not divisible by `M`. The standard prime modulus in CP is `1e9+7`.

#### 5.3 Modular Inverse via Extended Euclidean (when M is not prime but gcd(a,M)=1)
Using `extgcd(a, M, x, y)`, the inverse is `(x % M + M) % M`.
```cpp
int inv = (x % M + M) % M;
```
This is general and works for any modulus as long as `a` and `M` are coprime.

#### 5.4 Precomputing Factorials and Inverse Factorials
For combinatorics, we often need factorials modulo M and their inverses.
```cpp
const int MAX = 100000;
vector<long long> fact(MAX+1), invFact(MAX+1);

void precompute() {
    fact[0] = 1;
    for (int i = 1; i <= MAX; ++i)
        fact[i] = fact[i-1] * i % M;
    invFact[MAX] = modPow(fact[MAX], M-2, M);  // Fermat
    for (int i = MAX-1; i >= 0; --i)
        invFact[i] = invFact[i+1] * (i+1) % M;
}
```
Then nCr modulo M is `fact[n] * invFact[r] % M * invFact[n-r] % M`.

---

### Part 6 — Combinatorics Basics

- **Permutations:** number of ways to arrange n distinct objects: `n!`.
- **Binomial coefficient C(n, k):** `n! / (k! (n-k)!)`.
- For small n (≤ 5000), you can build Pascal’s triangle in O(n²):
```cpp
vector<vector<long long>> C(n+1, vector<long long>(n+1, 0));
for (int i = 0; i <= n; ++i) {
    C[i][0] = C[i][i] = 1;
    for (int j = 1; j < i; ++j)
        C[i][j] = (C[i-1][j-1] + C[i-1][j]) % M;
}
```
- **Catalan numbers:** many combinatorial structures (parentheses, trees) counted by `C(2n, n) / (n+1)`. Often need modulo.

---

### Part 7 — Application Walkthroughs

Let’s apply these tools to real CP problems.

#### 7.1 CSES 1712 – Exponentiation
Compute `a^b mod (1e9+7)`. Exactly `modPow` above.

#### 7.2 CSES 1713 – Counting Divisors
Given `n` queries, each asking for the number of divisors of `x` (x ≤ 10⁶). Precompute SPF up to 10⁶, then factorize each x and compute product of (exp+1). O(x log log x + n log x).

#### 7.3 CSES 1081 – Common Divisors
Given array, find the maximum `d` that divides at least two numbers. Idea: count frequencies of each number, then for each `d` sum multiples of `d`. Use a frequency array of size max_val. Iterate d from max_val down to 1, count multiples; if ≥2, output d. O(max_val log max_val).

```cpp
int max_val = *max_element(arr.begin(), arr.end());
vector<int> freq(max_val+1, 0);
for (int x : arr) freq[x]++;
for (int d = max_val; d >= 1; --d) {
    int cnt = 0;
    for (int m = d; m <= max_val; m += d)
        cnt += freq[m];
    if (cnt >= 2) {
        cout << d << endl;
        break;
    }
}
```

#### 7.4 CSES 1717 – Christmas Party (Derangements)
Number of permutations where no element stays in its original place: `!n = (n-1)(!(n-1) + !(n-2))`, or formula with inclusion-exclusion. Use DP with modulo.

#### 7.5 CSES 2064 – Bracket Sequences II
This uses Catalan numbers. But we’ll save for later.

#### 7.6 CSES 2205 – Gray Code (not number theory, but a bit manipulation problem.)

---

### Part 8 — Modular Arithmetic Traps

- **Negative numbers:** Always do `(a % M + M) % M`.
- **Overflow during multiplication:** Even if you do `(a * b) % M`, `a * b` might overflow `long long` if both are up to 10⁹ and M=10⁹+7. Use 128‑bit or modular multiplication with `__int128` (if available) or Russian peasant multiplication.
   ```cpp
   long long mulMod(long long a, long long b, long long M) {
       return (__int128)a * b % M; // GCC/Clang
   }
   ```
   If not available, implement binary multiplication (O(log b)) but rarely needed unless a and b > 10⁹.
- **Modular inverse of 0** is undefined; ensure you never divide by zero.

---

### Part 9 — Problem Set for Week 7

Now it is your turn to harden these concepts. Do not move on until you have solved every one of these. Write all helper functions from scratch.

1. **CSES 1068 Weird Algorithm** (if not done) — no math, but it’s a warm‑up.
2. **CSES 1083 Missing Number** — sum formula.
3. **CSES 1069 Repetitions** — string.
4. **CSES 1617 Bit Strings** — compute 2^n mod M, just `modPow(2, n, MOD)`.
5. **CSES 1618 Trailing Zeros** — number of 5s in n!.
6. **CSES 1712 Exponentiation** — fast pow.
7. **CSES 1713 Counting Divisors** — using SPF or sieve.
8. **CSES 1081 Common Divisors** — using frequency and multiples.
9. **CSES 1092 Two Knights** — combinatorics: total ways minus attack positions.
10. **CSES 1079 Binomial Coefficients** — precompute factorials and invFactorials.
11. **CF 230B T‑primes** — sieve, then for each x check if sqrt(x) is integer and prime.
12. **CF 1349A Orac and LCM** — this is more advanced, but try it for a challenge.

In addition, write a program that:
- Reads a, b, M, computes `(a^b) % M`.
- Reads n, k, computes `C(n, k) % M` using factorial precomputation.
- Builds the SPF array and factors 100 random numbers up to 10⁶.

---

### Part 10 — The Number‑Theory Mindset

After this lesson, every time you see a problem that involves large numbers, counting, divisors, or modular outputs, you will instantly know which tool to apply. You will reach for the sieve, the extended Euclidean algorithm, or the factorial precomputation as naturally as you reach for a vector.

The key is to **internalize the patterns**:
- “Count ways” → combinatorics, nCr.
- “Output modulo 1e9+7” → modular arithmetic, inverses.
- “Prime, divisors, factorization” → sieve, SPF.
- “GCD, LCM” → Euclidean.

You now have the mathematical foundation that many competitors lack. But remember: these tools are most powerful when combined with the algorithmic thinking from earlier weeks. In Week 8, we will blend number theory with binary search, DP, and greedy to solve the hardest introductory problems. And we will begin learning Graph Theory — the biggest topic in competitive programming.

For now, live in the integers. Open CSES, start submitting, and watch the green “Accepted” pile up. I’ll see you next week.
