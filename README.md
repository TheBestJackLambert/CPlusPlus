# C++ Projects

A collection of projects built while learning C++, progressing from basic syntax through pointers, file I/O, custom data encoding, and terminal UI with ncurses.

---

## Calculator

A command-line calculator that supports chained operations. Enter a starting number, then apply `add`, `sub`, `mult`, or `div` repeatedly until you exit. Operations are dispatched through a `std::map` of lambdas wrapping class methods — no switch statement needed.

**Concepts:** classes, encapsulation, `std::map`, `std::function`, lambdas, input validation, division-by-zero handling

## Room Game

A two-program system: a **room generator** and a **game engine**.

The generator takes room names and grid coordinates as input, automatically determines adjacency, and writes a structured room file. The game engine reads that file in two passes — first to allocate rooms, then to wire directional pointers between them — and drops you into a navigable world.

**Concepts:** structs, heap allocation with `new`, raw pointers (`room*`), two-pass file parsing, `std::map` for dynamic lookup, `nullptr` checks, pointer reseating

## Encrypter

Reads a text file, encodes characters into base-128 packed integers (4 characters per int), applies a reversible affine transformation, and writes the result to an output file. Decryption reverses the process. Handles partial character groups with a padding scheme using ASCII control characters to avoid data loss.

**Concepts:** custom encoding/decoding, bitwise packing, file I/O with `std::ifstream`/`std::ofstream`, `std::vector`, `std::reverse`, integer arithmetic at scale

## Contact Book

A terminal-based contact manager built with ncurses. Supports creating, viewing, editing, and deleting contacts with persistent file storage. Contacts are loaded from a custom-format text file on startup and saved on exit. Editing a contact's name correctly re-keys it in the underlying map.

**Concepts:** ncurses (`initscr`, `addstr`, `getstr`, `attron`/`attroff`, `refresh`, `clear`), classes with accessor/mutator patterns, `std::map`, `std::set`, `std::function`, custom file format parsing, string manipulation

---

## Build

All projects compile with `g++` or `clang++` on C++17 or later.

```bash
# standard project
g++ -std=c++17 -o calculator calculator.cpp

# ncurses project
g++ -std=c++17 -o contacts contacts.cpp -lncurses
```
