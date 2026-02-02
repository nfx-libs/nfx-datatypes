# Changelog

## [Unreleased]

### Added

- NIL

### Changed

- NIL

### Deprecated

- NIL

### Removed

- NIL

### Fixed

- NIL

### Security

- NIL

## [0.3.0] - 2026-02-02

### Added

- Added `Decimal(const char*)` and  `Int128(const char*)` constructors for C-string initialization

### Changed

- Updated Google Benchmark dependency from 1.9.4 to 1.9.5
- Optimized `Int128::toString()` with platform-specific fast paths and pre-allocated digit buffer
- Optimized `Int128(const Decimal&)` constructor using power-of-10 lookup tables instead of iterative multiplication
- MSVC: Optimized `Int128::operator/` with `_udiv128` hardware intrinsic for 128/64-bit division
- MSVC: Optimized binary long division algorithm with `_BitScanReverse64` to reduce loop iterations

## [0.2.0] - 2026-01-09

### Changed

- **Performance**: Added `DOUBLE_POWERS_OF_10` lookup table for `Decimal::toDouble()` conversion.


### Fixed

- Fixed IEEE 754 binary representation artifacts in `Decimal(float)` and `Decimal(double)` constructors causing incorrect decimal displays (e.g., 273.15f showing as "273.149994", 380.4 showing as "380.399999999999977")
- Fixed cumulative floating-point rounding errors in `Decimal::toDouble()` conversion by replacing iterative division loop with single division operation

## [0.1.1] - 2025-11-27

### Changed

- Simplify `Int128::operator<<` by delegating to `toString()`
- Consolidated packaging tool detection in CMake configuration

### Fixed

- Removed incorrect runtime dependencies from DEB/RPM packages

## [0.1.0] - 2025-11-22 - Initial Release

### Added

- **Int128**: Cross-platform 128-bit signed integer arithmetic

  - Native `__int128` support on GCC/Clang for maximum performance
  - Hand-optimized implementation for MSVC using 64-bit operations
  - Full range: -2^127 to 2^127-1
  - Complete arithmetic operators (+, -, \*, /, %, unary -)
  - Comparison operators (==, !=, <, <=, >, >=)
  - Type conversions (int32, int64, uint64, float, double, Decimal)
  - String parsing and formatting (`fromString()`, `toString()`)
  - Bit manipulation (`toLow()`, `toHigh()`, `toBits()`)

- **Decimal**: High-precision decimal arithmetic

  - 96-bit mantissa with 8-bit scale (0-28 decimal places)
  - Up to 28-29 significant digits
  - Exact decimal representation (no binary floating-point errors)
  - Complete arithmetic operators (+, -, \*, /, unary -)
  - Comparison operators (==, !=, <, <=, >, >=)
  - Type conversions (int32, int64, uint64, float, double, Int128)
  - String parsing and formatting (`fromString()`, `toString()`)
  - Mathematical operations (`abs()`, `trunc()`, `floor()`, `ceil()`, `round()`)
  - State checking methods (`scale()`, `decimalPlacesCount()`)
  - Cross-type arithmetic (Decimal ↔ Int128 interoperability)

- **Documentation**

  - README with feature overview
  - Detailed API documentation with Doxygen comments
  - Sample application demonstrating library usage
  - Build and installation instructions

- **Testing & Benchmarking**
  - Unit test suite
  - Performance benchmarks for all operations
  - Cross-compiler performance validation
