# 🔢 Quine–McCluskey Boolean Minimizer (C CLI)

An exact Boolean minimization tool implemented from scratch in C, demonstrating bit-level computation and multi-stage algorithm design.

A C-based command-line project that simplifies Boolean expressions using the **Quine–McCluskey algorithm**, an exact method for Boolean minimization.  
The program takes a set of minterms and outputs a minimized Boolean expression using bit-level operations and structured algorithmic steps.

---

## Project Overview

**Quine–McCluskey Boolean Minimizer** demonstrates how Boolean simplification works internally without relying on visual tools like Karnaugh maps.

The program represents Boolean terms using a combination of **bit values** and **bit masks**, where masked bits represent “don’t care” conditions.  
It systematically groups, combines, and reduces minterms into **prime implicants**, then selects a minimal set of implicants that fully cover the original function.

The implementation emphasizes:

- Bit manipulation for Boolean representation
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

### Quine–McCluskey Algorithm

- Groups minterms by number of 1s
- Iteratively combines terms differing by one bit
- Tracks usage of terms across rounds
- Generates all prime implicants

### Prime Implicant Selection

- Builds a **prime implicant chart**
- Identifies **essential prime implicants**
- Applies a **greedy method** to cover remaining minterms
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
./qm 3 1 2 4 5
```

### Example Output

```bash
Variables: 3
Minterms: 1 2 4 5 
Final minimized Boolean expression: B'C + A'BC' + AB'
```

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

- Add **step-by-step output display** to visualize grouping and combination rounds
- Implement **Petrick’s method** for a more optimal (non-greedy) prime implicant selection
- Add support for **don’t care conditions** in input