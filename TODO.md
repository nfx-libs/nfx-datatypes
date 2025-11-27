# nfx-datatypes TODO

Project roadmap and task tracking for the nfx-datatypes library.

### Todo

- [ ] Int128 Bitwise Operations: Implement `&`, `|`, `^`, `<<`, `>>` operators
- [ ] String Formatting: Custom format strings with locale support and thousands separators
- [ ] Math Functions: Add `pow()`, `exp()`, `log()`, `mod()`
- [ ] Fixed-Point Types: Int128-based fixed-point arithmetic for game/embedded systems
- [ ] Performance Optimizations: SIMD operations where applicable

### In Progress

- NIL

### Done ✓

#### v0.1.1 (2025-11-27)

- [x] **Refactor Int128 Stream Operator**
  - [x] Deduplicated `operator<<` to delegate to `toString()` for maintainability
