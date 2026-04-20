# CS 211 — Fundamentals of Computer Programming II
**Northwestern University · McCormick School of Engineering**

Advanced C/C++ programming course covering data structures, algorithms, compiler construction, and graph-based systems. Three major projects: a SQL execution engine, a campus navigation system, and a generic graph class with unit testing.

---

## Projects

### SQL Terminal (SimpleSQL)
A from-scratch SQL execution environment built in C/C++, divided into three phases:

**Phase 1 — Lexical Analysis (Scanner)**
Tokenizes SQL input strings into typed tokens (keywords, identifiers, literals, operators). Handles edge cases including string literals, whitespace, and comment stripping.

**Phase 2 — Parser & Semantic Analyzer**
Recursive descent parser builds an Abstract Syntax Tree (AST) representing the query's structure. The semantic analyzer validates column names, table references, and type consistency.

**Phase 3 — Query Execution**
Executes the query plan against an in-memory linked-list-of-arrays data store. Supports `SELECT`, `WHERE`, and basic aggregation. Returns typed result sets.

**Testing:** Google Test framework with comprehensive unit tests at each phase boundary.

```
SQL Input → [Scanner] → Tokens → [Parser] → AST → [Executor] → Results
```

---

### Open Street Map Navigator
C++ campus navigation system for Northwestern University's Evanston campus using real OpenStreetMap data.

- Parses OSM XML to extract building nodes and footway edges
- Constructs a weighted graph (Dijkstra-ready adjacency structure)
- Finds shortest walking paths between campus buildings by name
- Refactored for runtime efficiency — optimized graph construction and lookup

---

### Graph Class Implementation
Generic C++ graph container with full unit testing via Google Test.

- Templated graph class supporting string vertices and double-weight edges
- Directed and undirected edge support
- BFS/DFS traversal
- 100% unit test coverage via Google Test framework

---

## Stack
`C` `C++` `Google Test` `OpenStreetMap` `Makefile`

---

*Northwestern University — CS 211, Winter 2023*
