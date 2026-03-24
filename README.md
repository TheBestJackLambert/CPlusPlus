# C++ Projects

A collection of projects I built while learning C++ from scratch. These go roughly in order of complexity — the room game was my first day writing C++, and the contact book was the last. Everything here was written to learn by doing: pick a problem, figure out the language features needed to solve it, and build.

---

## Room Game

**`roomcreater.cpp` → `game.cpp`**

This is actually two programs that work together as a pipeline. The generator (`roomcreater.cpp`) takes room names with grid coordinates as input, computes adjacency from the coordinate positions, and writes a structured file (`rooms.txt`) describing the room graph. The game engine (`game.cpp`) reads that file in two passes — first to allocate every room as a heap node and register it in a lookup map, then a second pass to wire the directional pointers between rooms based on the adjacency data. At runtime, navigation is just following raw pointers: your current location is a `room*`, and moving north means reseating that pointer to wherever `loc->north` points. Null checks handle walls.

The two-program architecture was a deliberate choice. Hardcoding room layouts in the game itself would have been simpler, but splitting generation from gameplay meant I could redesign maps without touching the engine. It also forced me to define a file format and parse it correctly on both ends, which turned out to be the harder problem — the first version had a mismatch between what the generator wrote and what the reader expected, and debugging that taught me more about C++ strings and file I/O than any tutorial would have.

> **Skills:** raw pointers and heap allocation (`new`), `struct` with pointer members, `std::map` for string-to-pointer lookup, `std::fstream` two-pass file parsing, `std::getline`, pointer reseating for state transitions

---

## Calculator

**`Calculator.cpp`**

An accumulator-style calculator — you give it a starting number, then chain operations (`add`, `sub`, `mult`, `div`) until you enter something unrecognized, at which point it prints the final result. The `number` class wraps a float and exposes arithmetic methods, with division-by-zero handling.

The interesting decision here was how to dispatch operations. I didn't want an if-chain mapping input strings to method calls — it felt like the wrong tool. Instead, I built a `std::map<std::string, std::function<void(float)>>` where each entry is a lambda that captures the accumulator by reference and calls the right method. Typing `"add"` does a map lookup and invokes the returned function directly. It's overkill for four operations, but it was the first time I used lambdas and `std::function` in C++, and the pattern showed up again in later projects.

> **Skills:** classes with private state, `std::map` with `std::function` values, lambda captures (`[&num]`), `std::cin` error handling and stream recovery, `const` member functions

---

## Encrypter

**`encrypter.cpp`**

A file encryption tool using a custom encoding scheme I designed from scratch. The core idea: pack every 4 characters into a single integer using base-128 positional encoding (`c0 + c1×128 + c2×128² + c3×128³`), reverse the block order, then apply an affine transformation that maps each integer to its complement within the printable ASCII value range. Decryption is the same sequence of operations — the transformation is its own inverse.

Getting here involved a lot of debugging. The first version used `pow()` for the base-128 exponentiation, which returned doubles and silently mangled values through floating-point truncation. Then I hit C++'s comma operator — writing `266,321,790` as a number literal doesn't give you 266 million, it evaluates three separate expressions via the comma operator and keeps only the last one. Input strings that aren't a multiple of 4 characters long needed padding, and I used ASCII 31 (Unit Separator) as the pad byte since it never appears in real text, which avoids the ambiguity of stripping null bytes that might be legitimate ciphertext. The `intotochar` function extracts characters back out via repeated division and modulo in descending powers of 128, reconstructing the original string.

It's not cryptographically secure — it's a substitution cipher, and identical 4-byte blocks always produce identical output. But designing the encoding from nothing and making encrypt/decrypt perfectly inverse was a good problem.

> **Skills:** custom binary encoding (base-128 packing), bitwise-adjacent integer arithmetic, `std::vector` manipulation, `std::reverse`, `std::pow` pitfalls and integer math, file I/O with `std::ifstream`/`std::ofstream`, ASCII control characters for data framing

---

## Contact Book

**`book.cpp`**

A terminal contact manager built with ncurses. Contacts have a name, email, phone number, and notes field. You can create, view, edit, and delete contacts, and everything persists to a `contacts.txt` file in a simple `key:value` format that gets parsed back on startup. The UI uses ncurses for bold/plain text formatting, screen clearing, and string input via `getstr`.

The `contact` class uses the same lambda-dispatch pattern from the calculator — `edit()` maps field names to setter lambdas via `std::map<std::string, std::function>`, so adding a new field doesn't require touching the edit logic. Renaming a contact is the trickiest operation: since contacts are keyed by name in the outer map, a name change means erasing the old entry and reinserting under the new key while preserving all other data. The file parser handles the `key:value` format with a manual character-by-character split that strips the leading space after the colon, and uses `|` as a record delimiter.

This was my first time using ncurses, and I picked it up without any prior terminal UI experience. It's rough around the edges — the UI redraws could be cleaner and there's no arrow-key navigation — but the data layer works correctly and the file persistence round-trips without data loss.

> **Skills:** ncurses (`initscr`, `addstr`, `getstr`, `attron`/`attroff`, `clear`, `refresh`), classes with `std::map`-based method dispatch, `std::function` and lambdas, custom file format design and parsing, `std::set` for input validation, `std::map` key mutation (erase + reinsert), `std::ofstream` serialization

---

## Build

All projects compile with `g++` or `clang++` on C++20. The contact book requires ncurses.

```bash
# Calculator
g++ -std=c++20 -o calculator Calculator.cpp

# Room Generator
g++ -std=c++20 -o roomcreater roomcreater.cpp

# Room Game (run roomcreater first to generate rooms.txt)
g++ -std=c++20 -o game game.cpp

# Encrypter
g++ -std=c++20 -o encrypter encrypter.cpp

# Contact Book
g++ -std=c++20 -o book book.cpp -lncurses
```g++ -std=c++17 -o generator generator.cpp
g++ -std=c++17 -o game game.cpp

# ncurses project
g++ -std=c++17 -o contacts contacts.cpp -lncurses
```
