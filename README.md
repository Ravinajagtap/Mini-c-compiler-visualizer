# { miniC } Compiler Visualizer

> A Mini C Compiler built using Lex, Bison, C, and Flask — visualizing all 4 compiler phases with a parse tree on a web interface.

![Python](https://img.shields.io/badge/Python-3.x-blue?logo=python)
![Flask](https://img.shields.io/badge/Flask-Web_App-lightgrey?logo=flask)
![C](https://img.shields.io/badge/Language-C-blue?logo=c)
![Lex](https://img.shields.io/badge/Lexer-Flex/Lex-green)
![Bison](https://img.shields.io/badge/Parser-Bison/YACC-orange)
![License](https://img.shields.io/badge/License-MIT-brightgreen)

---

## Table of Contents

- [About the Project](#about-the-project)
- [Features](#features)
- [Project Structure](#project-structure)
- [Technologies Used](#technologies-used)
- [Compiler Phases](#compiler-phases)
- [Semantic Rules](#semantic-rules)
- [Supported C Syntax](#supported-c-syntax)
- [Installation & Setup](#installation--setup)
- [How to Run](#how-to-run)
- [Sample Inputs](#sample-inputs)
- [Web Interface](#web-interface)
- [System Architecture](#system-architecture)
- [Team / Author](#team--author)

---

## About the Project

The **Mini C Compiler Visualizer** is a course project for **Compiler Design (Semester 6, VIT College)**. It implements a subset of the C programming language and visualizes all four compilation phases — Lexical Analysis, Syntax Analysis, Semantic Analysis, and Intermediate Code Generation — through an interactive web interface.

The compiler is built using **Flex (Lex)** for tokenization and **Bison (YACC)** for grammar parsing, with the backend written in **C**. A **Flask** web server connects the compiler binary to a browser-based frontend, displaying outputs in 5 tabs including a **D3.js visual parse tree**.

This project demonstrates a complete compiler pipeline from raw source code to Three Address Code (TAC), making it a valuable educational tool for understanding how compilers work.

---

## Features

- **Lexical Analysis** — Tokenizes C source code into keywords, identifiers, operators, and punctuation
- **Syntax Analysis** — Validates grammar using LALR(1) parsing with Bison
- **Semantic Analysis** — Performs 5 semantic checks during parsing
- **Symbol Table** — Tracks all variables with name, type, value, and initialization status
- **TAC Generation** — Generates Three Address Code for all expressions and control flow
- **Visual Parse Tree** — Interactive D3.js tree diagram showing grammar structure
- **Web Interface** — Clean dark-themed browser UI with 5 output tabs
- **Sample Programs** — Built-in sample buttons for quick testing
- **Ctrl+Enter** shortcut to compile quickly

---

## Project Structure

```
Mini-c-compiler-visualizer/
│
├── compiler/                  # All compiler source files
│   ├── lexer.l                # Phase 1 — Lex tokenizer rules
│   ├── parser.y               # Phase 2 — Bison grammar rules + semantic checks
│   ├── symtable.h             # Phase 3 — Symbol table header
│   ├── symtable.c             # Phase 3 — Symbol table implementation
│   ├── tac.h                  # Phase 4 — TAC generator header
│   ├── tac.c                  # Phase 4 — TAC generator implementation
│   ├── main.c                 # Entry point — ties all phases together
│   ├── Makefile               # Build script (flex → bison → gcc)
│   └── compiler.exe           # Built binary (generated after make)
│
├── templates/
│   └── index.html             # Web frontend (5 tabs + D3.js parse tree)
│
├── static/
│   └── style.css              # Dark terminal-themed styling
│
├── app.py                     # Flask web server
├── requirements.txt           # Python dependencies
└── README.md                  # Project documentation
```

---

## Technologies Used

| Technology | Purpose |
|---|---|
| **Flex (win_flex)** | Lexical analyzer — tokenizes source code |
| **Bison (win_bison)** | Parser — checks grammar and builds parse tree |
| **C Language** | Symbol table, TAC generator, main entry point |
| **GCC (MinGW)** | Compiles all C files into executable |
| **Python 3** | Flask web server backend |
| **Flask** | Serves web UI and runs compiler via subprocess |
| **HTML + CSS** | Frontend web interface |
| **JavaScript** | Tab switching, fetch API for compile requests |
| **D3.js (v7)** | Visual parse tree rendering |
| **JetBrains Mono** | Code font for terminal aesthetic |
| **Syne** | Display font for UI headers |

---

## Compiler Phases

### Phase 1 — Lexical Analysis (`lexer.l`)

The lexer reads source code character by character and groups them into **tokens** using pattern matching rules defined in Lex format.

**Tokens recognized:**

| Category | Examples |
|---|---|
| Keywords | `int`, `float`, `if`, `else`, `while`, `return`, `void` |
| Identifiers | `x`, `myVar`, `counter` |
| Numbers | `42`, `3.14` |
| Operators | `+`, `-`, `*`, `/`, `=`, `==`, `!=`, `<`, `>`, `<=`, `>=` |
| Logical | `&&`, `\|\|`, `!` |
| Punctuation | `(`, `)`, `{`, `}`, `;`, `,` |
| Comments | `// single line` and `/* multi-line */` |

**Output example** for `int x = 5 + 3;`:
```
KEYWORD         : int
IDENTIFIER      : x
ASSIGN          : =
NUMBER          : 5
PLUS            : +
NUMBER          : 3
SEMICOLON       : ;
```

---

### Phase 2 — Syntax Analysis (`parser.y`)

The parser uses **LALR(1) grammar** rules defined in Bison to check if the token sequence forms a valid C program. It uses operator precedence declarations to handle ambiguity.

**Grammar rules supported:**
- Variable declarations (`int x;`, `int x = 5;`, `float y = 3.14;`)
- Assignment statements (`x = x + 1;`)
- If statement (`if (cond) { }`)
- If-else statement (`if (cond) { } else { }`)
- While loop (`while (cond) { }`)
- Return statement (`return x;`)
- Function declarations (`int main() { }`)
- Arithmetic expressions with precedence (`+`, `-`, `*`, `/`)
- Comparison expressions (`==`, `!=`, `<`, `>`, `<=`, `>=`)

**Operator precedence (lowest to highest):**
```
OR → AND → NOT → Comparisons → +/- → */÷ → Unary minus
```

---

### Phase 3 — Symbol Table (`symtable.h`, `symtable.c`)

The symbol table is an **array of structs** that records every variable declared in the program.

**Each entry stores:**

| Field | Type | Description |
|---|---|---|
| `name` | char[64] | Variable name e.g. `x` |
| `type` | char[16] | Data type e.g. `int`, `float` |
| `value` | float | Current value |
| `is_initialized` | int | 1 = assigned, 0 = declared only |

**Operations:**
- `insert_symbol()` — Adds a new variable (detects duplicates)
- `lookup_symbol()` — Finds a variable by name (returns NULL if not found)
- `update_symbol()` — Updates value on assignment
- `print_symbol_table()` — Formats table for web display

**Output example:**
```
============================================
           SYMBOL TABLE
============================================
Name            Type      Value     Init?
--------------------------------------------
x               | int      | 0.00     | yes
y               | int      | 0.00     | yes
============================================
Total: 2 variable(s)
============================================
```

---

### Phase 4 — Three Address Code / TAC (`tac.h`, `tac.c`)

TAC is an **intermediate representation** where each instruction has at most one operator and three addresses (result, operand1, operand2). Complex expressions are broken into simpler steps using temporary variables.

**TAC format:**
```
result = operand1 operator operand2
```

**Examples:**

For `z = x + y * 2;`:
```
t1 = y * 2
t2 = x + t1
z  = t2
```

For `if (x > 5) { ... } else { ... }`:
```
if x > 5 goto L1
goto L2
L1:
  x = 1
  goto L3
L2:
  x = 0
L3:
```

For `while (i < 10) { i = i + 1; }`:
```
L1:
  if i < 10 goto L2
  goto L3
L2:
  t1 = i + 1
  i = t1
  goto L1
L3:
```

---

### Phase 5 — Parse Tree (Visual)

The parse tree is generated as a **JSON string** during parsing and rendered using **D3.js** as an interactive top-down tree diagram in the browser.

**Node colors by type:**

| Node Type | Color |
|---|---|
| `program` (root) | Green |
| `func_decl` | Purple |
| `if_stmt` / `while` | Blue |
| `var_decl` | Orange |
| `assign` | Amber |
| `return` | Pink |
| `condition` | Lime green |
| Leaf nodes | Gray |

---

## Semantic Rules

The compiler implements **5 semantic checks** during parsing:

| Rule | Description | Message |
|---|---|---|
| **1. Undeclared Variable** | Variable used before declaring with `int`/`float` | `[SEMANTIC ERROR] Variable 'x' used without declaration` |
| **2. Uninitialized Variable** | Variable declared but never assigned before use | `[SEMANTIC WARNING] Variable 'x' used before initialization` |
| **3. Type Mismatch** | Float value assigned to int variable | `[SEMANTIC WARNING] Type mismatch: assigning float to int 'x'` |
| **4. Division by Zero** | Literal `0` used as divisor | `[SEMANTIC ERROR] Division by zero detected` |
| **5. Return Type Mismatch** | `void` function returning a value, or `int` function with empty return | `[SEMANTIC WARNING] void function should not return a value` |

All semantic messages appear in the **Parse** output tab of the web interface.

---

## Supported C Syntax

```c
// Variable declarations
int x;
int x = 5;
float y = 3.14;

// Arithmetic
int z = x + y * 2;
int r = (a - b) / c;

// Assignment
x = x + 1;

// If statement
if (x > 0) {
    x = 1;
}

// If-else statement
if (x > y) {
    x = 1;
} else {
    y = 1;
}

// While loop
while (i < 10) {
    i = i + 1;
}

// Function
int main() {
    int x = 10;
    return x;
}

// Void function
void foo() {
    int x = 5;
}
```

---

## Installation & Setup

### Prerequisites

Make sure the following are installed on your Windows machine:

| Tool | Download |
|---|---|
| **WinFlexBison** | https://github.com/lexxmark/winflexbison/releases |
| **MinGW (GCC)** | https://www.mingw-w64.org/ |
| **Python 3** | https://www.python.org/downloads/ |
| **Git** | https://git-scm.com/ |

### Step 1 — Clone the Repository

```bash
git clone https://github.com/YOUR_USERNAME/Mini-c-compiler-visualizer.git
cd Mini-c-compiler-visualizer
```

### Step 2 — Install Python Dependencies

```bash
pip install flask
```

### Step 3 — Build the Compiler

```bash
cd compiler
mingw32-make clean
mingw32-make
cd ..
```

This runs:
1. `win_bison -d -v parser.y` → generates `parser.tab.c` and `parser.tab.h`
2. `win_flex lexer.l` → generates `lex.yy.c`
3. `gcc` links everything → produces `compiler.exe`

---

## How to Run

After building the compiler once, just run:

```bash
python app.py
```

Then open your browser and go to:

```
http://localhost:5000
```

> To stop the server press `Ctrl+C` in the terminal.

---

## Sample Inputs

### Simple Variable
```c
int x = 5 + 3;
```

### If-Else
```c
int main() {
    int x = 10;
    int y = 5;
    if (x > y) {
        x = 1;
    } else {
        y = 1;
    }
    return x;
}
```

### While Loop
```c
int main() {
    int i = 0;
    while (i < 10) {
        i = i + 1;
    }
    return i;
}
```

### Full Program
```c
int main() {
    int x = 20;
    int y = 10;
    int result = 0;
    if (x > y) {
        result = x - y;
    } else {
        result = y - x;
    }
    int i = 0;
    while (i < result) {
        i = i + 1;
    }
    return i;
}
```

### Semantic Error Test — Division by Zero
```c
int main() {
    int x = 10 / 0;
    return x;
}
```

### Semantic Error Test — Undeclared Variable
```c
int main() {
    x = 5;
    return x;
}
```

---

## Web Interface

The web app has a **split-panel layout**:

- **Left panel** — C code editor with syntax placeholder, sample buttons, compile button, and status bar
- **Right panel** — 5 output tabs:

| Tab | Phase | Content |
|---|---|---|
| **1 — Tokens** | Lexer | List of all tokens with type and value |
| **2 — Parse** | Parser + Semantic | Grammar log and semantic errors/warnings |
| **3 — Symbol Table** | Symbol Table | All variables with name, type, value, init status |
| **4 — TAC** | TAC Generator | Numbered three-address code instructions |
| **5 — Parse Tree** | Visual | Interactive D3.js top-down tree diagram |

**Keyboard shortcut:** `Ctrl+Enter` to compile

---

## System Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    Browser (localhost:5000)                  │
│   ┌──────────────────┐         ┌────────────────────────┐   │
│   │   Code Editor    │         │   5 Output Tabs        │   │
│   │   (textarea)     │         │   Tokens / Parse /     │   │
│   │                  │         │   SymTable / TAC /     │   │
│   │   [Compile Btn]  │         │   Parse Tree (D3.js)   │   │
│   └────────┬─────────┘         └──────────┬─────────────┘   │
└────────────┼──────────────────────────────┼─────────────────┘
             │  POST /compile (JSON)         │  JSON response
             ▼                              ▲
┌─────────────────────────────────────────────────────────────┐
│                    Flask (app.py)                            │
│   - Receives C code from browser                            │
│   - Runs compiler.exe via subprocess (stdin pipe)           │
│   - Splits output at ===MARKERS===                          │
│   - Returns 5 sections as JSON                              │
└───────────────────────────┬─────────────────────────────────┘
                            │  stdin pipe
                            ▼
┌─────────────────────────────────────────────────────────────┐
│                  compiler.exe (C binary)                     │
│                                                             │
│   main.c                                                    │
│   ├── lexer.l    → Phase 1: Tokenize → token_output         │
│   ├── parser.y   → Phase 2: Parse   → parse_output          │
│   │              → Phase 5: Tree    → tree_output (JSON)    │
│   ├── symtable.c → Phase 3: Symbols → symtable_output       │
│   └── tac.c      → Phase 4: TAC    → tac_output             │
│                                                             │
│   Prints:  ===TOKENS=== ===PARSE=== ===SYMTABLE===          │
│            ===TAC===    ===TREE===                          │
└─────────────────────────────────────────────────────────────┘
```

---

## Team / Author

| Detail | Info |
|---|---|
| **Name** | Ravina |
| **College** | VIT College, Pune |
| **Year** | Third Year (TY) Engineering |
| **Semester** | Semester 6 |
| **Subject** | Compiler Design (CD-CP) |
| **Branch** | Computer Engineering |

---

## Subject Details

- **Course:** Compiler Design
- **Project Type:** Course Project (CD-CP)
- **Academic Year:** 2025-26

---

## License

This project is submitted as an academic course project at VIT College, Pune.

---

> Built with Flex, Bison, C, Python, Flask, and D3.js
