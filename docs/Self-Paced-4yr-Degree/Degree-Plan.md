You’re absolutely right—the previous roadmap was pure systems and security, skipping the foundational CS courses that every well‑rounded degree demands. Let’s fix that. Here is a complete, semester‑by‑semester, **holistic BS in Computer Science** that satisfies the typical credit requirements of a top‑tier university (≈120–128 credits), builds strong foundations in programming, data structures & algorithms, and math, **and** still plunges deep into systems and security with hands‑on projects. Every course uses open, free materials from Stanford, MIT, Carnegie Mellon, and Princeton. By the end, you’ll have the intellectual depth of a CS graduate and a GitHub that proves you built it from the ground up.

---

## Degree Structure Overview

| Component | Courses | Credits |
|-----------|---------|---------|
| **Mathematics & Natural Science** | Calculus I, II; Linear Algebra/Multivariable; 2 lab sciences | 20 |
| **Technology in Society / Ethics** | 1 course | 3 |
| **Core Computer Science** | Intro Programming (2), Systems, Computer Organization, Probability, Discrete Math, Algorithms | 34 |
| **Systems & Security Depth Electives** | OS, Networking, Compilers, Security, Advanced Security | 20 |
| **Additional CS Electives** | Advanced Data Structures, Nand2Tetris (optional but included) | 12 |
| **Senior Capstone** | 2‑semester project | 8 |
| **General Electives** | (room for a couple of lighter courses) | 6 |
| **Total** | | ≈123 |

All courses are semester‑long (≈15 weeks), except where noted. Credits follow the US system (1 credit ≈ 1 lecture hour + 2‑3 lab hours per week). The load is 15‑16 credits per semester—full‑time, rigorous, and doable.

---

## Year 1 – Foundations of Programming & Math

| Semester | Course (Credits) | Title & Open Resources | What You Build / Do |
|----------|------------------|------------------------|----------------------|
| **Fall** | CS106A (4) | **Programming Methodology** – Stanford. Python. [cs106a.stanford.edu](https://cs106a.stanford.edu/) – full lectures, assignments, handouts. | Weekly programming problems, games, simple algorithms. Master Python fundamentals. |
|  | MATH 19 (4) | **Single Variable Calculus I** – MIT 18.01. [OCW 18.01](https://ocw.mit.edu/courses/18-01sc-single-variable-calculus-fall-2010/) | Problem sets + exams. |
|  | PHYSICS I (4) | **Classical Mechanics** – MIT 8.01. [OCW 8.01](https://ocw.mit.edu/courses/8-01sc-classical-mechanics-fall-2016/) | Labs (can use PhET simulations), problem sets. |
|  | Elective (3) | **Nand2Tetris Part I (optional)** – Build a computer from NAND gates. Can be taken later or spread over summer. | (optional: build Hack computer; we’ll formally include it in Year 2 if you love ground‑up; I’ll list it as an elective now but you can defer) |
| **Winter** | CS106B (4) | **Programming Abstractions** – Stanford. C++. [cs106b.stanford.edu](https://cs106b.stanford.edu/) – lectures, assignments, starter code. | Recursion, dynamic data structures, BFS/DFS, expression trees. All in C++. |
|  | MATH 20 (4) | **Single Variable Calculus II** – MIT 18.02. [OCW 18.02](https://ocw.mit.edu/courses/18-02sc-multivariable-calculus-fall-2010/) | Multivariable calculus, problem sets. |
|  | PHYSICS II (4) | **Electricity & Magnetism** – MIT 8.02. [OCW 8.02](https://ocw.mit.edu/courses/8-02-physics-ii-electricity-and-magnetism-spring-2007/) | Labs, problem sets. |
| **Spring** | CS103 (4) | **Mathematical Foundations of Computing** – Stanford. [cs103.stanford.edu](https://cs103.stanford.edu/) | Proofs, set theory, induction, finite automata, Turing machines. Problem sets. |
|  | MATH 51 (4) | **Linear Algebra & Multivariable Calculus** – MIT 18.06 (Strang). [OCW 18.06](https://ocw.mit.edu/courses/18-06sc-linear-algebra-fall-2011/) | Homework, MATLAB/Octave exercises. |
|  | CS181 / Ethics (3) | **Technology in Society** – Harvard “Justice” on edX or Stanford CS181 if material found. [Justice on edX](https://www.edx.org/course/justice) | Essays, discussions (self‑reflect in notes). |

**Year 1 total:** 33‑35 credits. You’ve learned programming, solid math, and started to think like a computer scientist.

---

## Year 2 – Systems, Probability, and Core Theory

| Semester | Course (Credits) | Title & Open Resources | What You Build / Do |
|----------|------------------|------------------------|----------------------|
| **Fall** | CS107 (4) | **Computer Organization & Systems** – Stanford (Jerry Cain). [cs107.stanford.edu](https://cs107.stanford.edu/) – lectures on YouTube, assignments: **data lab, bomb lab, attack lab, malloc lab**. This is the famous CS:APP‑style hands‑on course. | Binary bomb, buffer overflow exploits, a working memory allocator. In C and assembly. |
|  | CS109 (4) | **Probability for Computer Scientists** – Stanford. [cs109.stanford.edu](https://cs109.stanford.edu/) – videos, problem sets, “coursenotes” textbook. | Probabilistic modeling, problem sets, mini‑projects. |
|  | Nand2Tetris Part I (3) | **Build a Modern Computer from First Principles** – [nand2tetris.org](https://www.nand2tetris.org) | Logic gates → ALU → CPU → assembler. Complete the Hack computer. |
| **Winter** | CS110 (4) | **Principles of Computer Systems** – Stanford. [cs110.stanford.edu](https://cs110.stanford.edu/) | Multiprocessing, threading, HTTP proxy, map‑reduce, file systems. Seven assignments (C/C++). |
|  | CS161 (4) | **Design and Analysis of Algorithms** – Stanford (Roughgarden). [SEE CS161](https://see.stanford.edu/Course/CS161) – videos, problem sets. | Algorithmic problem sets, proofs, graph algorithms, dynamic programming. |
|  | Nand2Tetris Part II (3) | **Build a Virtual Machine, Compiler, and OS** – same site. | VM translator, simple compiler (Jack language), mini‑OS. Continues hardware story. |
| **Spring** | CS144 (4) | **Introduction to Computer Networking** – Stanford. [cs144.stanford.edu](https://cs144.stanford.edu/) | Sponge: a complete TCP/IP stack in C++. Your own TCP, IP router, network interface. |
|  | CS166 (4) | **Advanced Data Structures** – Stanford (optional but highly recommended). Materials from [web.stanford.edu/class/cs166](https://web.stanford.edu/class/cs166/) (lecture slides, some assignments). | Amortized analysis, van Emde Boas trees, splay trees, etc. Implement some. |
|  | General Elective (3) | Maybe a course on technical writing, or start learning Rust via self‑study (later useful for security projects). | |

**Year 2 total:** 34‑36 credits. You now have deep systems intuition, built a computer from NANDs, written memory allocators and a TCP stack, and mastered algorithms.

---

## Year 3 – Compilers, Security, and Building Hardened Systems

| Semester | Course (Credits) | Title & Open Resources | What You Build / Do |
|----------|------------------|------------------------|----------------------|
| **Fall** | CS143 (4) | **Compilers** – Stanford (Alex Aiken). [edX Compilers](https://www.edx.org/course/compilers) (free audit). | Full compiler for COOL: lexer, parser, semantic analysis, MIPS code generation. |
|  | CS155 (4) | **Computer and Network Security** – Stanford. [cs155.stanford.edu](https://cs155.stanford.edu/) | Web exploits (XSS, CSRF), network attacks, crypto exercises. Three labs + final. |
|  | General Elective (3) | Deepen Rust or read “The Tangled Web” for security. | |
| **Winter** | MIT 6.858 (4) | **Computer Systems Security** – MIT OCW. [6.858 Fall 2014](https://ocw.mit.edu/courses/6-858-computer-systems-security-fall-2014/) | Buffer overflow bypass, privilege‑separated web server, sandboxing, final project. |
|  | CS140 (4) | **Operating Systems** – Stanford SEE (legacy) or modern MIT 6.828 (if you want JOS). I recommend **MIT 6.828** for ground‑up OS building: [6.828 schedule](https://pdos.csail.mit.edu/6.828/schedule.html) | JOS kernel: boot, virtual memory, processes, IPC, file system. |
|  | Elective (3) | CS347 (Parallel Computing) or CS242 (Programming Languages) if available; otherwise, a database course (CMU 15‑445 intro to database systems, free videos). | |
| **Spring** | CS248 (4) | **Interactive Computer Graphics** – Stanford (optional but fun and still systems‑adjacent). [cs248.stanford.edu](https://cs248.stanford.edu/) | Ray tracer, OpenGL projects. |
|  | Capstone Prep (2) | Define your senior project, do literature review, choose tools. | |
|  | Elective (3) | Continue database course or a security elective (e.g., crypto). | |

**Year 3 total:** 31‑34 credits. At this point you’ve broken and defended systems, written a compiler and an OS kernel. You’re ready to build something original.

---

## Year 4 – Capstone & Portfolio

| Semester | Activity (Credits) | Description | Deliverables |
|----------|--------------------|-------------|--------------|
| **Fall** | **Capstone Project I** (4) | Design and begin implementation of a major secure system. Example: a distributed, end‑to‑end encrypted file store with custom access control, using your own TCP stack or a kernel module. | Design document, threat model, prototype, weekly GitHub commits. |
|  | **Open‑Source Contribution** (2) | Contribute a meaningful patch to a security project (e.g., WireGuard, OSS‑Fuzz). | Merged PR or well‑documented attempt. |
|  | **Tech Elective** (4) | Pick any remaining interest: cryptography (Coursera Boneh crypto), advanced compilers, or hardware security. | |
| **Spring** | **Capstone Project II** (4) | Complete implementation, test rigorously, write final report, record demo video. | Polished GitHub repo, report, demo. |
|  | **Portfolio & Blog** (2) | Write about each major project, what you learned, and how it fits together. Publish on a personal site. | |
|  | **Career Prep** (2) | Mock interviews, algorithm drill (use LeetCode with your deep knowledge), apply to security/systems roles. | |

**Year 4 total:** 18 credits, lighter load to allow for job hunting and project polish.

---

## GitHub Tracking for the Holistic Degree

Create a monorepo: `holistic-cs-degree`. Structure it by year and course:

```
holistic-cs-degree/
├── year1/
│   ├── cs106a/   # assignments
│   ├── cs106b/
│   ├── cs103/
│   ├── math/     # 18.01, 18.02, 18.06 psets
│   └── physics/
├── year2/
│   ├── cs107/    # bomb, malloc, attack labs
│   ├── cs110/
│   ├── cs161/
│   ├── nand2tetris/
│   └── cs144/    # sponge
├── year3/
│   ├── cs143/    # compiler phases
│   ├── cs155/
│   ├── 6.858-labs/
│   ├── 6.828-jos/
│   └── ...
├── year4/
│   └── capstone/
├── README.md     # your “transcript”, skills matrix
└── progress.md   # weekly updates, time log
```

Commit discipline: one commit per finished assignment. Write a `NOTES.md` in each lab folder explaining your design. Tag major completions (`v1.0-year1`, `v2.0-core-done`, etc.). This repository will be a living, verifiable diploma.

---

## Final Word from Your Advisor

This is now a **complete, accredited‑level CS degree program**—with all the foundational courses you’d expect (intro programming, data structures, algorithms, discrete math, probability, computer organization) **and** the ground‑up, build‑everything systems/security depth you wanted. Every lecture and assignment is free. Every semester pushes you to produce real, working artifacts, not just notes.

You’ll graduate as a CS fellow who can:
- Write a recursive descent parser and a concurrent web proxy.
- Exploit a stack overflow and then redesign the OS to prevent it.
- Explain how a transistor becomes a computer and a computer becomes a secure distributed system.

Now pick a start date, clone the plan, and make that first commit. Your journey from NAND gate to hardened kernel starts now.
