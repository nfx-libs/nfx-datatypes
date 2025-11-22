# nfx-datatypes

<!-- Project Info -->

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?style=flat-square)](https://github.com/nfx-libs/nfx-datatypes/blob/main/LICENSE.txt) [![GitHub release (latest by date)](https://img.shields.io/github/v/release/nfx-libs/nfx-datatypes?style=flat-square)](https://github.com/nfx-libs/nfx-datatypes/releases) [![GitHub tag (latest by date)](https://img.shields.io/github/tag/nfx-libs/nfx-datatypes?style=flat-square)](https://github.com/nfx-libs/nfx-datatypes/tags)<br/>

![C++20](https://img.shields.io/badge/C%2B%2B-20-blue?style=flat-square) ![CMake](https://img.shields.io/badge/CMake-3.20%2B-green.svg?style=flat-square) ![Cross Platform](https://img.shields.io/badge/Platform-Linux_Windows-lightgrey?style=flat-square)

<!-- CI/CD Status -->

[![Linux GCC](https://img.shields.io/github/actions/workflow/status/nfx-libs/nfx-datatypes/build-linux-gcc.yml?branch=main&label=Linux%20GCC&style=flat-square)](https://github.com/nfx-libs/nfx-datatypes/actions/workflows/build-linux-gcc.yml) [![Linux Clang](https://img.shields.io/github/actions/workflow/status/nfx-libs/nfx-datatypes/build-linux-clang.yml?branch=main&label=Linux%20Clang&style=flat-square)](https://github.com/nfx-libs/nfx-datatypes/actions/workflows/build-linux-clang.yml) [![Windows MinGW](https://img.shields.io/github/actions/workflow/status/nfx-libs/nfx-datatypes/build-windows-mingw.yml?branch=main&label=Windows%20MinGW&style=flat-square)](https://github.com/nfx-libs/nfx-datatypes/actions/workflows/build-windows-mingw.yml) [![Windows MSVC](https://img.shields.io/github/actions/workflow/status/nfx-libs/nfx-datatypes/build-windows-msvc.yml?branch=main&label=Windows%20MSVC&style=flat-square)](https://github.com/nfx-libs/nfx-datatypes/actions/workflows/build-windows-msvc.yml) [![CodeQL](https://img.shields.io/github/actions/workflow/status/nfx-libs/nfx-datatypes/codeql.yml?branch=main&label=CodeQL&style=flat-square)](https://github.com/nfx-libs/nfx-datatypes/actions/workflows/codeql.yml)

> A C++20 library providing cross-platform 128-bit integer and high-precision decimal arithmetic

## Overview

nfx-datatypes is a modern C++20 library providing numeric types for applications requiring precise integer and decimal arithmetic. It features cross-platform 128-bit signed integers and high-precision decimal arithmetic.

## Key Features

### 🔢 High-Precision Numeric Types

- **Int128**: Cross-platform 128-bit signed integer arithmetic
  - Native `__int128` support on GCC/Clang for maximum performance
  - Hand-optimized implementation for MSVC using 64-bit operations
  - Full range: -2^127 to 2^127-1
  - Complete arithmetic operator support (+, -, \*, /, %)
- **Decimal**: High-precision decimal arithmetic
  - 96-bit mantissa with configurable scale (0-28)
  - Up to 28-29 significant digits
  - Exact decimal representation (no binary floating-point errors)
  - Financial and scientific computation support

### ➕ Complete Operator Support

- Arithmetic: `+`, `-`, `*`, `/`, `%`, unary `-`
- Comparison: `==`, `!=`, `<`, `<=`, `>`, `>=`
- Type conversions: int32, int64, uint64, float, double
- Cross-type operations: Int128 ↔ Decimal interoperability
- String parsing: `fromString()`
- String formatting: `toString()`

### 📊 Real-World Applications

- **Data Analytics**: Processing large datasets with exact numerical precision
- **Financial Systems**: Exact decimal arithmetic for banking, accounting, and trading platforms
- **Scientific Computing**: High-precision calculations for research and simulation
- **E-commerce**: Precise price calculations and tax computations

### ⚡ Performance Optimized

- Sub-nanosecond basic operations (construction, comparisons)
- Highly optimized arithmetic (addition, multiplication, division)
- Fast string parsing and formatting
- Zero-cost abstractions with constexpr support

### 🌍 Cross-Platform Support

- **Platforms**: Linux, Windows
- **Architecture**: x86-64 (x86 SIMD features: SSE4.2, AVX, AVX2)
- **Compilers**: GCC 14+, Clang 18+, MSVC 2022+
- Thread-safe operations
- Consistent behavior across platforms
- CI/CD testing on multiple compilers

## Quick Start

### Requirements

- C++20 compatible compiler:
  - **GCC 14+** (14.2.0 tested)
  - **Clang 18+** (19.1.7 tested)
  - **MSVC 2022+** (19.44+ tested)
- CMake 3.20 or higher

### CMake Integration

```cmake
# Build options
option(NFX_DATATYPES_BUILD_STATIC         "Build static library"               OFF )
option(NFX_DATATYPES_BUILD_SHARED         "Build shared library"               OFF )

# Development options
option(NFX_DATATYPES_BUILD_TESTS          "Build tests"                        OFF )
option(NFX_DATATYPES_BUILD_SAMPLES        "Build samples"                      OFF )
option(NFX_DATATYPES_BUILD_BENCHMARKS     "Build benchmarks"                   OFF )
option(NFX_DATATYPES_BUILD_DOCUMENTATION  "Build Doxygen documentation"        OFF )

# Installation
option(NFX_DATATYPES_INSTALL_PROJECT      "Install project"                    OFF )

# Packaging
option(NFX_DATATYPES_PACKAGE_SOURCE       "Enable source package generation"   OFF )
option(NFX_DATATYPES_PACKAGE_ARCHIVE      "Enable TGZ/ZIP package generation"  OFF )
option(NFX_DATATYPES_PACKAGE_DEB          "Enable DEB package generation"      OFF )
option(NFX_DATATYPES_PACKAGE_RPM          "Enable RPM package generation"      OFF )
option(NFX_DATATYPES_PACKAGE_WIX          "Enable WiX MSI installer"           OFF )
```

### Using in Your Project

#### Option 1: Using FetchContent (Recommended)

```cmake
include(FetchContent)
FetchContent_Declare(
  nfx-datatypes
  GIT_REPOSITORY https://github.com/nfx-libs/nfx-datatypes.git
  GIT_TAG        main  # or use specific version tag like "0.1.0"
)
FetchContent_MakeAvailable(nfx-datatypes)

# Link with static library
target_link_libraries(your_target PRIVATE nfx-datatypes::static)
```

#### Option 2: As a Git Submodule

```bash
# Add as submodule
git submodule add https://github.com/nfx-libs/nfx-datatypes.git third-party/nfx-datatypes
```

```cmake
# In your CMakeLists.txt
add_subdirectory(third-party/nfx-datatypes)
target_link_libraries(your_target PRIVATE nfx-datatypes::static)
```

#### Option 3: Using find_package (After Installation)

```cmake
find_package(nfx-datatypes REQUIRED)
target_link_libraries(your_target PRIVATE nfx-datatypes::static)
```

### Building

**Build Commands:**

```bash
# Clone the repository
git clone https://github.com/nfx-libs/nfx-datatypes.git
cd nfx-datatypes

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build the library
cmake --build . --config Release --parallel

# Run tests (optional)
ctest -C Release --output-on-failure

# Run benchmarks (optional)
./bin/benchmarks/BM_Decimal
./bin/benchmarks/BM_Int128
```

### Documentation

nfx-datatypes includes API documentation generated with Doxygen.

#### 📚 Online Documentation

The complete API documentation is available online at:
**https://nfx-libs.github.io/nfx-datatypes**

#### Building Documentation Locally

```bash
# Configure with documentation enabled
cmake .. -DCMAKE_BUILD_TYPE=Release -DNFX_DATATYPES_BUILD_DOCUMENTATION=ON

# Build the documentation
cmake --build . --target documentation
```

#### Requirements

- **Doxygen** - Documentation generation tool
- **Graphviz Dot** (optional) - For generating class diagrams

#### Accessing Local Documentation

After building, open `./build/doc/html/index.html` in your web browser.

## Usage Examples

### Int128 - 128-bit Integer Arithmetic

```cpp
#include <nfx/datatypes/Int128.h>

using namespace nfx::datatypes;

// Construction from various types
Int128 a{42};                                            // From int32
Int128 b{1234567890123456789LL};                         // From int64
Int128 c{0xFEDCBA0987654321ULL, 0x0123456789ABCDEFULL};  // From two 64-bit words

// String parsing
Int128 d{"123456789012345678901234567890"};
Int128 e;
if (Int128::fromString("-987654321098765432109876543210", e))
{
	// Successfully parsed
}

// Arithmetic operations
Int128 sum = a + b;
Int128 diff = b - a;
Int128 product = a * Int128{100};
Int128 quotient = b / Int128{10};
Int128 remainder = b % Int128{7};

// Comparison operations
bool equal = (a == Int128{42});
bool less = (a < b);
bool greater = (b > a);

// Cross-type comparisons
bool eqInt64 = (b == 1234567890123456789LL);
bool eqDouble = (a == 42.0);

// String conversion
std::string str = c.toString();  // "-1311768464867721559164238360763045071"

// Access high/low words
std::uint64_t low = c.toLow();
std::uint64_t high = c.toHigh();

// Mathematical operations
Int128 absValue = Int128{-42}.abs();    // 42
bool isNeg = Int128{-42}<0;  // true
```

### Decimal - High-Precision Decimal Arithmetic

```cpp
#include <nfx/datatypes/Decimal.h>

using namespace nfx::datatypes;

// Construction from various types
Decimal a{ 42 };		   // Integer
Decimal b{ 123.456 };	   // From double
Decimal c{ 9876543210LL }; // From int64

// String parsing with exact precision
Decimal d{"123.456789012345678901234567890"};
Decimal e;
if ( Decimal::fromString( "99.99", e ) )
{
	// Successfully parsed
}

// Arithmetic operations (exact decimal arithmetic)
Decimal sum = Decimal{ 10.5 } + Decimal{ 20.3 };	 // 30.8
Decimal diff = Decimal{ 50.0 } - Decimal{ 12.5 };	 // 37.5
Decimal product = Decimal{ 3.5 } * Decimal{ 2.0 };	 // 7.0
Decimal quotient = Decimal{ 10.0 } / Decimal{ 4.0 }; // 2.5

// Financial calculations (no rounding errors)
Decimal price{ 19.99 };
Decimal quantity{ 100 };
Decimal total = price * quantity; // Exactly 1999.00

// Comparison operations
bool equal = ( Decimal{ 10.5 } == Decimal{ 10.5 } );
bool less = ( Decimal{ 5.0 } < Decimal{ 10.0 } );

// Mathematical operations
Decimal abs = Decimal{ -42.5 }.abs();			  // 42.5
Decimal truncated = Decimal{ 42.789 }.trunc( );   // 42
Decimal floor = Decimal{ 42.789 }.floor();		  // 42
Decimal ceiling = Decimal{ 42.123 }.ceil( );	  // 43
Decimal rounded = Decimal{ 42.567 }.round();	  // 43

// Property access
std::uint8_t scale = Decimal{ 123.456 }.scale(); // Number of decimal places
int places = Decimal{ 123.456 }.decimalPlacesCount();

// State checking
bool isNeg = Decimal{ -10.5 }<0;

// Numeric limits (use std::numeric_limits)
Decimal min = std::numeric_limits<Decimal>::min();
Decimal max = std::numeric_limits<Decimal>::max();

// String conversion
std::string str = Decimal{ 123.456 }.toString(); // "123.456"
```

### Int128 ↔ Decimal Interoperability

```cpp
#include <nfx/datatypes/Int128.h>
#include <nfx/datatypes/Decimal.h>

using namespace nfx::datatypes;

// Convert Int128 to Decimal
Int128 bigInt{1234567890123456789LL};
Decimal dec{bigInt};  // Exact conversion

// Convert Decimal to Int128 (truncates fractional part)
Decimal price{99.95};
Int128 wholeDollars{price};  // 99 (fractional part truncated)

// Mixed arithmetic comparisons
Int128 intVal{100};
Decimal decVal{100.0};
bool equal = (intVal == decVal);  // true

Decimal withFraction{100.5};
bool notEqual = (intVal == withFraction);  // false (Decimal has fractional part)

// Use in calculations
Int128 count{1000};
Decimal pricePerUnit{19.99};
Decimal totalCost = Decimal{count} * pricePerUnit;
```

### Complete Example

```cpp
#include <iostream>
#include <nfx/datatypes/Int128.h>
#include <nfx/datatypes/Decimal.h>

int main()
{
	using namespace nfx::datatypes;

	// Financial calculation example
	std::cout << "=== Financial Calculation ===" << std::endl;
	Decimal price{ "19.99" };
	Int128 quantity{ 1000 };
	Decimal total = price * Decimal{quantity};
	std::cout << "Price: " << price.toString() << std::endl;
	std::cout << "Quantity: " << quantity.toString() << std::endl;
	std::cout << "Total: " << total.toString() << std::endl;

	// Large number arithmetic
	std::cout << "\n=== Large Number Arithmetic ===" << std::endl;
	Int128 large1{"123456789012345678901234567890"};
	Int128 large2{"987654321098765432109876543210"};
	Int128 sum = large1 + large2;
	std::cout << "Sum: " << sum.toString() << std::endl;

	// Decimal precision
	std::cout << "\n=== Decimal Precision ===" << std::endl;
	Decimal a{0.1};
	Decimal b{0.2};
	Decimal result = a + b;
	std::cout << "0.1 + 0.2 = " << result.toString() << std::endl;
	std::cout << "Equals 0.3? " << (result == Decimal{0.3} ? "Yes" : "No") << std::endl;

	return 0;
}
```

**Sample Output:**

```
=== Financial Calculation ===
Price: 19.99
Quantity: 1000
Total: 19990

=== Large Number Arithmetic ===
Sum: 1111111110111111111011111111100

=== Decimal Precision ===
0.1 + 0.2 = 0.3
Equals 0.3? Yes
```

## Installation & Packaging

nfx-datatypes provides packaging options for distribution.

### Package Generation

```bash
# Configure with packaging options
cmake .. -DCMAKE_BUILD_TYPE=Release \
         -DNFX_DATATYPES_BUILD_STATIC=ON \
         -DNFX_DATATYPES_BUILD_SHARED=ON \
         -DNFX_DATATYPES_PACKAGE_ARCHIVE=ON \
         -DNFX_DATATYPES_PACKAGE_DEB=ON \
         -DNFX_DATATYPES_PACKAGE_RPM=ON

# Generate binary packages
cmake --build . --target package
# or
cd build && cpack

# Generate source packages
cd build && cpack --config CPackSourceConfig.cmake
```

### Supported Package Formats

| Format      | Platform       | Description                        | Requirements |
| ----------- | -------------- | ---------------------------------- | ------------ |
| **TGZ/ZIP** | Cross-platform | Compressed archive packages        | None         |
| **DEB**     | Debian/Ubuntu  | Native Debian packages             | `dpkg-dev`   |
| **RPM**     | RedHat/SUSE    | Native RPM packages                | `rpm-build`  |
| **WiX**     | Windows        | Professional MSI installer         | `WiX 3.11+`  |
| **Source**  | Cross-platform | Source code distribution (TGZ+ZIP) | None         |

### Installation

```bash
# Linux (DEB-based systems)
sudo dpkg -i nfx-datatypes_*_amd64.deb

# Linux (RPM-based systems)
sudo rpm -ivh nfx-datatypes-*-Linux.rpm

# Windows (MSI installer)
nfx-datatypes-0.1.0-MSVC.msi

# Manual installation (extract archive)
tar -xzf nfx-datatypes-*-Linux.tar.gz -C /usr/local/
```

## Project Structure

```
nfx-datatypes/
├── benchmark/             # Performance benchmarks with Google Benchmark
├── cmake/                 # CMake modules and configuration
├── include/nfx/           # Public headers: Int128, Decimal
├── samples/               # Example usage and demonstrations
├── src/                   # Implementation files
└── test/                  # Comprehensive unit tests with GoogleTest
```

## Performance

For detailed performance metrics and benchmarks, see the [benchmark documentation](benchmark/README.md).

## Roadmap

See [TODO.md](TODO.md) for upcoming features and project roadmap.

## Changelog

See the [CHANGELOG.md](CHANGELOG.md) for a detailed history of changes, new features, and bug fixes.

## License

This project is licensed under the MIT License.

## Dependencies

### Development Dependencies

- **[GoogleTest](https://github.com/google/googletest)**: Testing framework (BSD 3-Clause License) - Development only
- **[Google Benchmark](https://github.com/google/benchmark)**: Performance benchmarking framework (Apache 2.0 License) - Development only

All dependencies are automatically fetched via CMake FetchContent when building the library, tests, or benchmarks.

---

_Updated on November 22, 2025_
