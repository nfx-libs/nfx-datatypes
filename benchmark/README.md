# Benchmarks

---

## Test Environment

### Hardware Configuration

| Component                | Specification                                                 |
| ------------------------ | ------------------------------------------------------------- |
| **Computer Model**       | Lenovo ThinkPad P15v Gen 3                                    |
| **CPU**                  | 12th Gen Intel Core i7-12800H (20 logical, 14 physical cores) |
| **Base Clock**           | 2.80 GHz                                                      |
| **Turbo Clock**          | 4.80 GHz                                                      |
| **L1 Data Cache**        | 48 KiB (×6 P-cores) + 32 KiB (×8 E-cores)                     |
| **L1 Instruction Cache** | 32 KiB (×6 P-cores) + 64 KiB (×2 E-core clusters)             |
| **L2 Unified Cache**     | 1.25 MiB (×6 P-cores) + 2 MiB (×2 E-core clusters)            |
| **L3 Unified Cache**     | 24 MiB (×1 shared)                                            |
| **RAM**                  | DDR4-3200 (32GB)                                              |
| **GPU**                  | NVIDIA RTX A2000 4GB GDDR6                                    |

### Software Configuration

| Platform    | OS         | Benchmark Framework     | C++ Compiler           | nfx-datatypes Version |
| ----------- | ---------- | ----------------------- | ---------------------- | --------------------- |
| **Linux**   | LMDE 7     | Google Benchmark v1.9.5 | Clang 19.1.7-x64       | v0.2.0                |
| **Windows** | Windows 10 | Google Benchmark v1.9.5 | MSVC 19.44.35217.0-x64 | v0.2.0                |

---

# Performance Results

## Decimal Operations

### Construction Benchmarks

| Operation                 | Linux Clang | Windows MSVC |
| ------------------------- | ----------: | -----------: |
| **Construct Default**     |    0.238 ns |     0.777 ns |
| **Construct from Int32**  |    0.292 ns |     0.967 ns |
| **Construct from Int64**  |    0.228 ns |     0.976 ns |
| **Construct from Uint32** |    0.229 ns |     0.978 ns |
| **Construct from Uint64** |    0.235 ns |     0.974 ns |
| **Construct from Float**  |     57.6 ns |      55.5 ns |
| **Construct from Double** |     68.8 ns |      64.0 ns |
| **Construct from Int128** |     1.86 ns |      1.79 ns |
| **Copy Construct**        |    0.242 ns |     0.761 ns |

### Arithmetic Operations

| Operation                     | Linux Clang | Windows MSVC |
| ----------------------------- | ----------: | -----------: |
| **Addition**                  |     53.8 ns |      83.5 ns |
| **Subtraction**               |     35.4 ns |      52.1 ns |
| **Multiplication**            |     43.3 ns |      54.4 ns |
| **Multiplication Large**      |     43.6 ns |       123 ns |
| **Division**                  |      142 ns |       277 ns |
| **Division High Precision**   |      122 ns |       222 ns |
| **Unary Minus**               |    0.246 ns |     0.771 ns |
| **Addition Assignment**       |     52.3 ns |      72.6 ns |
| **Subtraction Assignment**    |     36.0 ns |      54.7 ns |
| **Multiplication Assignment** |     43.8 ns |      55.5 ns |
| **Division Assignment**       |      137 ns |       285 ns |

### Parsing Operations

| Operation                | Linux Clang | Windows MSVC |
| ------------------------ | ----------: | -----------: |
| **Parse Integer**        |     11.6 ns |      31.4 ns |
| **Parse Small Decimal**  |     18.8 ns |      38.7 ns |
| **Parse Large Decimal**  |     49.4 ns |       148 ns |
| **Parse High Precision** |     52.3 ns |       158 ns |
| **Parse Negative**       |     33.6 ns |       112 ns |
| **FromString Valid**     |     21.8 ns |      54.7 ns |
| **FromString Invalid**   |     5.29 ns |      16.4 ns |

### Conversion & Formatting

| Operation             | Linux Clang | Windows MSVC |
| --------------------- | ----------: | -----------: |
| **ToDouble**          |     1.81 ns |      1.79 ns |
| **ToBits**            |    0.528 ns |      1.66 ns |
| **ToString Integer**  |     23.7 ns |      54.4 ns |
| **ToString Small**    |     26.7 ns |      55.8 ns |
| **ToString Large**    |     45.6 ns |      70.9 ns |
| **ToString Negative** |     41.6 ns |      69.1 ns |

### Comparison Operations

| Operation               | Linux Clang | Windows MSVC |
| ----------------------- | ----------: | -----------: |
| **Equality Decimal**    |     6.68 ns |      8.13 ns |
| **LessThan Decimal**    |     2.84 ns |      8.60 ns |
| **GreaterThan Decimal** |     2.79 ns |      9.41 ns |
| **Equality Int32**      |     9.94 ns |      8.39 ns |
| **Equality Int64**      |     6.98 ns |      8.29 ns |
| **Equality Uint64**     |     12.9 ns |      18.4 ns |
| **Equality Double**     |     61.6 ns |      64.3 ns |
| **LessThan Double**     |     59.4 ns |      65.1 ns |
| **Equality Float**      |     70.2 ns |      90.3 ns |
| **Equality Int128**     |     6.52 ns |      12.1 ns |
| **LessThan Int128**     |     17.7 ns |      35.5 ns |

### State Checking Operations

| Operation                 | Linux Clang | Windows MSVC |
| ------------------------- | ----------: | -----------: |
| **IsZero (Zero)**         |     1.20 ns |      2.38 ns |
| **IsZero (NonZero)**      |    0.251 ns |     0.811 ns |
| **IsNegative (Negative)** |    0.990 ns |      2.05 ns |
| **IsNegative (Positive)** |     7.81 ns |      13.8 ns |

### Mathematical Operations

| Operation            | Linux Clang | Windows MSVC |
| -------------------- | ----------: | -----------: |
| **Abs Positive**     |     8.35 ns |      13.6 ns |
| **Abs Negative**     |     1.28 ns |      2.23 ns |
| **Sqrt**             |      517 ns |      1500 ns |
| **Sqrt Perfect Sqr** |     35.0 ns |      69.2 ns |
| **Sqrt Large**       |     4263 ns |     12996 ns |
| **Truncate**         |     28.5 ns |      66.4 ns |
| **Floor**            |     29.3 ns |      67.2 ns |
| **Ceiling**          |     31.8 ns |      73.0 ns |
| **Round**            |     30.9 ns |      76.7 ns |

### Free Functions

| Operation          | Linux Clang | Windows MSVC |
| ------------------ | ----------: | -----------: |
| **abs() Positive** |     8.44 ns |      14.5 ns |
| **abs() Negative** |     1.33 ns |      2.30 ns |
| **sqrt()**         |      545 ns |      1416 ns |
| **round()**        |     30.9 ns |      64.9 ns |
| **floor()**        |     27.6 ns |      60.9 ns |
| **ceil( )**        |     32.0 ns |      64.3 ns |
| **trunc( )**       |     27.9 ns |      60.2 ns |

### Formatting Operations

| Operation           | Linux Clang | Windows MSVC |
| ------------------- | ----------: | -----------: |
| **Format**          |      146 ns |       118 ns |
| **Format Negative** |      151 ns |       117 ns |

### Property Accessors & Constants

| Operation              | Linux Clang | Windows MSVC |
| ---------------------- | ----------: | -----------: |
| **GetScale**           |    0.235 ns |     0.790 ns |
| **DecimalPlacesCount** |     2.72 ns |      3.31 ns |
| **Constant MinValue**  |    0.375 ns |     0.999 ns |
| **Constant MaxValue**  |    0.230 ns |     0.985 ns |

---

## Int128 Operations

### Construction Benchmarks

| Operation                   | Linux Clang | Windows MSVC |
| --------------------------- | ----------: | -----------: |
| **Construct Default**       |    0.225 ns |     0.763 ns |
| **Construct from Int32**    |    0.276 ns |     0.750 ns |
| **Construct from Int64**    |    0.229 ns |     0.775 ns |
| **Construct from Uint64**   |    0.272 ns |     0.751 ns |
| **Construct from TwoWords** |    0.218 ns |     0.766 ns |
| **Construct from Float**    |     1.57 ns |      10.1 ns |
| **Construct from Double**   |      372 ns |       822 ns |
| **Construct from Decimal**  |     16.3 ns |      23.4 ns |
| **Copy Construct**          |    0.221 ns |     0.745 ns |

### Arithmetic Operations

| Operation                | Linux Clang | Windows MSVC |
| ------------------------ | ----------: | -----------: |
| **Addition**             |    0.220 ns |     0.738 ns |
| **Subtraction**          |    0.220 ns |     0.726 ns |
| **Multiplication**       |    0.229 ns |      2.74 ns |
| **Multiplication Large** |    0.227 ns |      2.82 ns |
| **Division**             |    0.241 ns |      7.06 ns |
| **Division Large**       |    0.227 ns |       188 ns |
| **Modulo**               |    0.225 ns |      8.83 ns |
| **Unary Minus**          |    0.247 ns |     0.738 ns |

### Parsing Operations

| Operation                 | Linux Clang | Windows MSVC |
| ------------------------- | ----------: | -----------: |
| **Parse Small Number**    |     3.03 ns |      5.30 ns |
| **Parse Medium Number**   |     26.1 ns |      47.8 ns |
| **Parse Large Number**    |     62.7 ns |      97.4 ns |
| **Parse Negative Number** |     45.9 ns |      76.9 ns |
| **FromString Valid**      |     44.7 ns |      73.6 ns |
| **FromString Invalid**    |     1.76 ns |      3.06 ns |

### Conversion & Formatting

| Operation             | Linux Clang | Windows MSVC |
| --------------------- | ----------: | -----------: |
| **ToLow**             |    0.223 ns |     0.748 ns |
| **ToHigh**            |    0.219 ns |     0.753 ns |
| **ToBits**            |    0.222 ns |     0.533 ns |
| **ToString Small**    |     7.30 ns |      19.2 ns |
| **ToString Medium**   |     46.6 ns |      73.6 ns |
| **ToString Large**    |      158 ns |       508 ns |
| **ToString Negative** |      136 ns |       513 ns |

### Comparison Operations

| Operation              | Linux Clang | Windows MSVC |
| ---------------------- | ----------: | -----------: |
| **Equality Int128**    |    0.227 ns |     0.727 ns |
| **LessThan Int128**    |    0.229 ns |     0.741 ns |
| **GreaterThan Int128** |    0.228 ns |     0.729 ns |
| **Equality Int64**     |    0.229 ns |     0.733 ns |
| **LessThan Int64**     |    0.224 ns |     0.746 ns |
| **Equality Uint64**    |    0.226 ns |     0.746 ns |
| **Equality Double**    |    0.225 ns |      3.31 ns |
| **LessThan Double**    |    0.226 ns |      3.32 ns |
| **Equality Float**     |    0.226 ns |      3.31 ns |
| **Equality Decimal**   |     8.08 ns |      12.2 ns |
| **LessThan Decimal**   |     7.01 ns |      12.3 ns |

### State Checking Operations

| Operation                 | Linux Clang | Windows MSVC |
| ------------------------- | ----------: | -----------: |
| **IsZero (Zero)**         |    0.227 ns |     0.741 ns |
| **IsZero (NonZero)**      |    0.223 ns |     0.736 ns |
| **IsNegative (Negative)** |    0.225 ns |     0.736 ns |
| **IsNegative (Positive)** |    0.224 ns |     0.730 ns |

### Mathematical Operations

| Operation                | Linux Clang | Windows MSVC |
| ------------------------ | ----------: | -----------: |
| **Abs Positive**         |    0.224 ns |     0.755 ns |
| **Abs Negative**         |    0.224 ns |     0.745 ns |
| **isqrt**                |     18.0 ns |      51.1 ns |
| **isqrt Large**          |     18.0 ns |      65.3 ns |
| **isqrt Perfect Square** |     14.6 ns |      37.3 ns |

### Free Functions

| Operation   | Linux Clang | Windows MSVC |
| ----------- | ----------: | -----------: |
| **abs()**   |    0.223 ns |     0.731 ns |
| **isqrt()** |     18.3 ns |      51.7 ns |
| **sqrt()**  |     27.6 ns |      63.0 ns |

### Formatting Operations

| Operation           | Linux Clang | Windows MSVC |
| ------------------- | ----------: | -----------: |
| **Format**          |      146 ns |       391 ns |
| **Format Negative** |      151 ns |       404 ns |

---

_Updated on Frebruary 02, 2026_
