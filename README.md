# C++ Projects

A collection of projects built while learning C++, progressing from basic syntax through pointers, file I/O, custom data encoding, and terminal UI with ncurses.

---

## Calculator

A command-line calculator that supports chained operations. Enter a starting number, then apply `add`, `sub`, `mult`, or `div` repeatedly until you exit. Operations are dispatched through a map of lambdas rather than an if/else chain, keeping the logic extensible — adding a new operation means adding one line to the map, not another branch.

> **Skills & Libraries:** classes, encapsulation, `std::map`, `std::function`, lambda expressions, input validation, do-while loops

## Room Game

A two-program pipeline: a **room generator** and a **game engine** that communicate through a shared file format.

The generator takes room names and grid coordinates, computes adjacency automatically from spatial relationships, and writes a structured layout file. The game engine reads that file in two passes — first to allocate all rooms on the heap, then to wire directional pointers between them — and drops you into a pointer-based graph you can navigate by cardinal direction. Rooms don't store coordinates at runtime; the connections *are* the map.

Designing the generator as a separate tool means the game engine has no idea how the rooms were arranged — it just follows pointers. New maps can be created without touching the game code.

> **Skills & Libraries:** structs, heap allocation (`new`), raw pointers, pointer reseating, `nullptr` checks, two-pass file parsing, `std::fstream`, `std::map`, graph traversal

## Encrypter

Reads a text file, packs characters into base-128 integers (4 characters per int), applies a reversible affine transformation, and writes the result to an output file. Decryption reverses the process exactly.

The base-128 packing was designed from scratch — each character occupies one "digit" in a base-128 number, so a 4-character group becomes a single integer up to ~266 million. Partial groups at the end of the input are padded with an ASCII control character (Unit Separator, value 31) chosen specifically because it never appears in normal text, avoiding ambiguity during decryption. The transformation is its own inverse, so the same operation encrypts and decrypts.

> **Skills & Libraries:** custom encoding/decoding, base-128 integer packing, affine transformations, `std::vector`, `std::reverse`, `std::ifstream`/`std::ofstream`, `std::algorithm`

## Contact Book

A terminal contact manager with a ncurses interface, built with no prior ncurses experience. Supports creating, viewing, editing, and deleting contacts. Data persists across sessions through a custom file format that loads on startup and saves on exit.

Editing a contact's name re-keys it in the underlying map — the old entry is erased and a new one is inserted — so lookups stay consistent. The contact class tracks which fields are populated with a boolean vector, so the display only renders fields that exist rather than showing blank rows.

> **Skills & Libraries:** `ncurses` (windows, attributes, input handling), classes with accessor/mutator patterns, `std::map`, `std::set`, `std::function`, custom file format design, string parsing

---

## Build

All projects compile with `g++` or `clang++` on C++17 or later.

```bash
# standard project
g++ -std=c++17 -o calculator calculator.cpp

# room generator + game
g++ -std=c++17 -o generator generator.cpp
g++ -std=c++17 -o game game.cpp

# ncurses project
g++ -std=c++17 -o contacts contacts.cpp -lncurses
```
