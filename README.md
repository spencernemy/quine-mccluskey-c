# 🔢 Quine–McCluskey Boolean Minimizer (C CLI)

An exact Boolean minimization tool implemented from scratch in C, demonstrating bit-level computation and multi-stage algorithm design.

A C-based command-line project that simplifies Boolean expressions using the **Quine–McCluskey algorithm**, an exact method for Boolean minimization.  
The program takes a set of minterms and outputs a minimized Boolean expression while displaying each step of the algorithm.

---

## Project Overview

**Quine–McCluskey Boolean Minimizer** demonstrates how Boolean simplification works internally without relying on visual tools like Karnaugh maps.

The program represents Boolean terms using a combination of **bit values** and **bit masks**, where masked bits represent “don’t care” conditions.  
It systematically groups, combines, and reduces minterms into **prime implicants**, then selects a minimal set of implicants that fully cover the original function.

The implementation emphasizes:

- Bit manipulation for Boolean representation
- Dynamic memory management with careful allocation, resizing, and cleanup of complex data structures
- Multi-phase algorithm design
- Efficient data organization using structs and dynamic arrays
- Careful tracking of memory ownership and lifecycle
- Clean separation between computation and output

## Why This Project Matters

Boolean minimization is a fundamental problem in digital logic design and compiler optimization. While tools like Karnaugh maps work for small cases, the Quine–McCluskey algorithm provides a systematic, scalable approach to exact minimization.

This project demonstrates how a non-trivial algorithm can be implemented from scratch in C, combining bit-level manipulation, dynamic memory management, and multi-phase problem solving. It highlights the transition from theoretical concepts to a working system with real input, processing, and output.

Beyond Boolean algebra, the project reinforces core computer science skills including:

- Designing and structuring multi-stage algorithms
- Managing memory safely in low-level languages
- Translating mathematical logic into efficient code

---

## Features

### Step-by-Step Algorithm Output

The program prints each stage of the Quine–McCluskey process:

- Initial binary conversion of minterms
- Grouping by number of 1s
- Combination rounds showing how terms are merged
- Identification of unused terms (prime implicants)
- Prime implicant chart visualization
- Essential prime implicant selection
- Remaining uncovered minterms
- Final selected implicants
- Final minimized Boolean expression

### Quine–McCluskey Algorithm

- Groups minterms by number of 1s
- Iteratively combines terms differing by one bit
- Tracks usage of terms across rounds
- Generates all prime implicants

### Prime Implicant Selection

- Builds a **prime implicant chart**
- Identifies **essential prime implicants**
- Applies a **greedy strategy** to cover any remaining uncovered minterms
- Produces a minimal covering set

### Expression Output

- Converts bit-level representation into Boolean variables
- Supports standard notation (e.g., `A'BC + AC`)
- Skips masked (“don’t care”) variables

---

## Compilation & Execution

```bash
make
./qm <num_variables> <minterms...>
```

### Example

```bash
./qm 4 1 3 4 6 8 10 12
```

### Example Output

```text
========================================
   Quine-McCluskey Boolean Minimizer
========================================
Variables: 4
Minterms: 1 3 4 6 8 10 12

Step 1: Binary terms
0001, 0011, 0100, ...

Step 2: Initial groups
Group 0:
(None)
Group 1:
0001, 0100, 1000
...

Step 3: Combine rounds

Combine Round 1
0001 + 0011 -> 00-1
0100 + 0110 -> 01-0
...

Step 4: Prime implicants
00-1, 01-0, -100, 10-0, 1-00

Step 5: Prime implicant chart

PI      | 1  3  4  6  8  10 12
------------------------------
00-1    | X  X
01-0    |       X  X
-100    |       X           X
10-0    |             X  X
1-00    |             X     X

Step 6: Essential prime implicants
00-1 is essential (only implicant covering minterm 1)
...

Step 7: Remaining uncovered minterms
12

Step 8: Final selected implicants
00-1, 01-0, 10-0, -100

Final minimized Boolean expression = A'B'D + A'BD' + AB'D' + BC'D'
```

---

## Key Concepts

### Bitmask Representation

- `value` → stores the bit pattern
- `mask` → marks “don’t care” positions

This allows efficient comparison and combination of terms.

### Dynamic Memory Management

- All term lists use dynamically allocated arrays
- Careful ownership tracking prevents memory leaks and double frees
- Cleanup handled through centralized logic

### Greedy Selection Strategy

- Essential implicants selected first
- Remaining coverage handled by selecting the row covering the most uncovered minterms

## Future Improvements

- Implement **Petrick’s method** for a more optimal (non-greedy) prime implicant selection
- Add support for **don’t care conditions** in input
