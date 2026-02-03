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
| **Linux**   | LMDE 7     | Google Benchmark v1.9.5 | GCC 14.2.0-x64         | v0.4.0                |
| **Linux**   | LMDE 7     | Google Benchmark v1.9.5 | Clang 19.1.7-x64       | v0.4.0                |
| **Windows** | Windows 10 | Google Benchmark v1.9.5 | MinGW GCC 14.2.0-x64   | v0.4.0                |
| **Windows** | Windows 10 | Google Benchmark v1.9.5 | MSVC 19.44.35217.0-x64 | v0.4.0                |


---

# Performance Results

## Decimal Operations

### Construction Benchmarks

| Operation                 | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ------------------------- | --------: | ----------: | ----------------: | -----------: |
| **Construct Default**     |  0.220 ns |    0.221 ns |          0.229 ns |     0.767 ns |
| **Construct from Int32**  |  0.226 ns |    0.277 ns |          0.225 ns |     0.963 ns |
| **Construct from Int64**  |  0.228 ns |    0.226 ns |          0.229 ns |     0.942 ns |
| **Construct from Uint32** |  0.239 ns |    0.225 ns |          0.246 ns |     0.942 ns |
| **Construct from Uint64** |  0.241 ns |    0.226 ns |          0.225 ns |     0.963 ns |
| **Construct from Float**  |   66.2 ns |     52.3 ns |           82.0 ns |      54.4 ns |
| **Construct from Double** |   82.3 ns |     61.5 ns |           92.1 ns |      64.2 ns |
| **Construct from Int128** |   1.32 ns |     1.72 ns |           1.31 ns |      1.88 ns |
| **Copy Construct**        |  0.235 ns |    0.232 ns |          0.235 ns |     0.767 ns |

### Arithmetic Operations

| Operation                     | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ----------------------------- | --------: | ----------: | ----------------: | -----------: |
| **Addition**                  |   38.1 ns |     51.8 ns |           33.7 ns |      82.0 ns |
| **Subtraction**               |   27.8 ns |     34.2 ns |           23.0 ns |      53.1 ns |
| **Multiplication**            |   28.3 ns |     42.0 ns |           23.9 ns |      55.8 ns |
| **Multiplication Large**      |   33.2 ns |     44.0 ns |           29.2 ns |       120 ns |
| **Division**                  |   52.2 ns |      145 ns |           57.8 ns |       276 ns |
| **Division High Precision**   |   45.9 ns |      121 ns |           46.5 ns |       220 ns |
| **Unary Minus**               |  0.525 ns |    0.245 ns |          0.366 ns |     0.802 ns |
| **Addition Assignment**       |   32.0 ns |     50.1 ns |           27.6 ns |      72.5 ns |
| **Subtraction Assignment**    |   26.4 ns |     34.5 ns |           23.5 ns |      53.1 ns |
| **Multiplication Assignment** |   27.7 ns |     42.4 ns |           23.5 ns |      56.2 ns |
| **Division Assignment**       |   51.5 ns |      135 ns |           57.2 ns |       272 ns |

### Parsing Operations

| Operation                | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ------------------------ | --------: | ----------: | ----------------: | -----------: |
| **Parse Integer**        |   7.99 ns |     7.07 ns |           9.07 ns |      16.0 ns |
| **Parse Small Decimal**  |   10.9 ns |     11.7 ns |           11.4 ns |      19.2 ns |
| **Parse Large Decimal**  |   57.2 ns |     50.4 ns |           56.2 ns |       144 ns |
| **Parse High Precision** |   59.9 ns |     50.3 ns |           59.3 ns |       173 ns |
| **Parse Negative**       |   21.1 ns |     23.6 ns |           22.0 ns |      32.8 ns |
| **FromString Valid**     |   13.2 ns |     12.2 ns |           13.8 ns |      23.5 ns |
| **FromString Invalid**   |   6.27 ns |     7.21 ns |           10.5 ns |      15.3 ns |

### Conversion & Formatting

| Operation             | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| --------------------- | --------: | ----------: | ----------------: | -----------: |
| **ToDouble**          |   2.28 ns |     1.88 ns |           4.05 ns |      1.73 ns |
| **ToBits**            |  0.887 ns |    0.527 ns |          0.516 ns |      1.46 ns |
| **ToString Integer**  |   26.1 ns |     22.8 ns |           41.0 ns |      51.6 ns |
| **ToString Small**    |   28.0 ns |     26.0 ns |           43.0 ns |      54.4 ns |
| **ToString Large**    |   43.7 ns |     46.5 ns |           64.2 ns |      69.8 ns |
| **ToString Negative** |   39.6 ns |     45.0 ns |           60.0 ns |      68.4 ns |

### Comparison Operations

| Operation               | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ----------------------- | --------: | ----------: | ----------------: | -----------: |
| **Equality Decimal**    |   2.82 ns |     6.72 ns |           2.93 ns |      8.54 ns |
| **LessThan Decimal**    |   3.18 ns |     2.62 ns |           3.37 ns |      8.54 ns |
| **GreaterThan Decimal** |   3.59 ns |     2.91 ns |           3.60 ns |      9.63 ns |
| **Equality Int32**      |   3.15 ns |     10.0 ns |           3.01 ns |      9.24 ns |
| **Equality Int64**      |   3.26 ns |     6.96 ns |           3.00 ns |      9.03 ns |
| **Equality Uint64**     |   6.87 ns |     12.8 ns |           6.80 ns |      20.1 ns |
| **Equality Double**     |   65.6 ns |     59.7 ns |            115 ns |      68.4 ns |
| **LessThan Double**     |   64.7 ns |     58.6 ns |            103 ns |      67.2 ns |
| **Equality Float**      |   87.2 ns |     72.3 ns |            100 ns |      78.5 ns |
| **Equality Int128**     |   5.13 ns |     6.61 ns |           1.76 ns |      10.0 ns |
| **LessThan Int128**     |   12.9 ns |     17.6 ns |           9.77 ns |      32.1 ns |

### State Checking Operations

| Operation                 | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ------------------------- | --------: | ----------: | ----------------: | -----------: |
| **IsZero (Zero)**         |   1.31 ns |     1.17 ns |           1.80 ns |      1.93 ns |
| **IsZero (NonZero)**      |  0.293 ns |    0.248 ns |          0.235 ns |     0.767 ns |
| **IsNegative (Negative)** |  0.803 ns |    0.971 ns |           1.69 ns |      1.80 ns |
| **IsNegative (Positive)** |   4.53 ns |     8.04 ns |           4.33 ns |      12.8 ns |

### Mathematical Operations

| Operation            | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| -------------------- | --------: | ----------: | ----------------: | -----------: |
| **Abs Positive**     |   4.84 ns |     8.49 ns |           4.65 ns |      12.3 ns |
| **Abs Negative**     |   1.30 ns |     1.26 ns |           1.84 ns |      1.97 ns |
| **Sqrt**             |    403 ns |      504 ns |            405 ns |      1367 ns |
| **Sqrt Perfect Sqr** |   25.9 ns |     35.1 ns |           49.7 ns |      58.6 ns |
| **Sqrt Large**       |   2658 ns |     4179 ns |           2727 ns |     10742 ns |
| **Truncate**         |   18.2 ns |     29.8 ns |           32.1 ns |      60.0 ns |
| **Floor**            |   18.6 ns |     31.4 ns |           32.1 ns |      61.4 ns |
| **Ceiling**          |   20.6 ns |     34.4 ns |           39.2 ns |      62.8 ns |
| **Round**            |   19.6 ns |     34.3 ns |           31.4 ns |      62.8 ns |

### Free Functions

| Operation          | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ------------------ | --------: | ----------: | ----------------: | -----------: |
| **abs() Positive** |   4.78 ns |     9.87 ns |           4.65 ns |      12.7 ns |
| **abs() Negative** |   1.32 ns |     1.39 ns |           1.90 ns |      1.99 ns |
| **sqrt()**         |    402 ns |      548 ns |            426 ns |      1339 ns |
| **round()**        |   20.7 ns |     31.7 ns |           33.0 ns |      60.9 ns |
| **floor()**        |   18.4 ns |     29.1 ns |           32.1 ns |      55.8 ns |
| **ceil( )**        |   20.5 ns |     31.5 ns |           40.1 ns |      64.5 ns |
| **trunc( )**       |   18.6 ns |     28.4 ns |           32.1 ns |      57.2 ns |

### Formatting Operations

| Operation           | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ------------------- | --------: | ----------: | ----------------: | -----------: |
| **Format**          |   67.4 ns |     61.1 ns |           88.9 ns |       112 ns |
| **Format Negative** |   67.3 ns |     60.0 ns |           90.0 ns |       112 ns |

### Property Accessors & Constants

| Operation              | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ---------------------- | --------: | ----------: | ----------------: | -----------: |
| **GetScale**           |  0.238 ns |    0.240 ns |          0.262 ns |     0.753 ns |
| **DecimalPlacesCount** |   5.67 ns |     2.65 ns |           5.47 ns |      3.00 ns |
| **Constant MinValue**  |  0.224 ns |    0.324 ns |          0.229 ns |     0.928 ns |
| **Constant MaxValue**  |  0.225 ns |    0.229 ns |          0.225 ns |     0.941 ns |

---

## Int128 Operations

### Construction Benchmarks

| Operation                   | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| --------------------------- | --------: | ----------: | ----------------: | -----------: |
| **Construct Default**       |  0.224 ns |    0.217 ns |          0.240 ns |     0.732 ns |
| **Construct from Int32**    |  0.222 ns |    0.220 ns |          0.235 ns |     0.715 ns |
| **Construct from Int64**    |  0.222 ns |    0.221 ns |          0.234 ns |     0.753 ns |
| **Construct from Uint64**   |  0.229 ns |    0.218 ns |          0.230 ns |     0.732 ns |
| **Construct from TwoWords** |  0.314 ns |    0.222 ns |          0.234 ns |     0.750 ns |
| **Construct from Float**    |   2.46 ns |     1.54 ns |           2.61 ns |      9.84 ns |
| **Construct from Double**   |    366 ns |      368 ns |            558 ns |       802 ns |
| **Construct from Decimal**  |   8.00 ns |     16.4 ns |           8.37 ns |      23.5 ns |
| **Copy Construct**          |  0.218 ns |    0.221 ns |          0.229 ns |     0.750 ns |

### Arithmetic Operations

| Operation                | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ------------------------ | --------: | ----------: | ----------------: | -----------: |
| **Addition**             |  0.224 ns |    0.223 ns |          0.230 ns |     0.732 ns |
| **Subtraction**          |  0.222 ns |    0.222 ns |          0.241 ns |     0.725 ns |
| **Multiplication**       |  0.220 ns |    0.226 ns |          0.230 ns |      2.65 ns |
| **Multiplication Large** |  0.253 ns |    0.224 ns |          0.234 ns |      2.76 ns |
| **Division**             |  0.218 ns |    0.222 ns |          0.225 ns |      7.11 ns |
| **Division Large**       |  0.227 ns |    0.244 ns |          0.234 ns |       184 ns |
| **Modulo**               |  0.228 ns |    0.223 ns |          0.229 ns |      8.72 ns |
| **Unary Minus**          |  0.227 ns |    0.225 ns |          0.225 ns |     0.750 ns |

### Parsing Operations

| Operation                 | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ------------------------- | --------: | ----------: | ----------------: | -----------: |
| **Parse Small Number**    |   3.21 ns |     3.28 ns |           3.30 ns |      3.84 ns |
| **Parse Medium Number**   |   13.8 ns |     15.1 ns |           16.0 ns |      14.8 ns |
| **Parse Large Number**    |   61.0 ns |     66.3 ns |           73.2 ns |      99.4 ns |
| **Parse Negative Number** |   48.0 ns |     46.4 ns |           55.8 ns |      73.2 ns |
| **FromString Valid**      |   14.6 ns |     14.4 ns |           16.4 ns |      16.5 ns |
| **FromString ValidLong**  |   48.3 ns |     46.0 ns |           55.8 ns |      82.3 ns |
| **FromString Invalid**    |   2.23 ns |     2.93 ns |           2.39 ns |      3.61 ns |

### Conversion & Formatting

| Operation             | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| --------------------- | --------: | ----------: | ----------------: | -----------: |
| **ToLow**             |  0.106 ns |    0.234 ns |          0.110 ns |     0.767 ns |
| **ToHigh**            |  0.107 ns |    0.226 ns |          0.110 ns |     0.715 ns |
| **ToBits**            |  0.219 ns |    0.223 ns |          0.225 ns |     0.547 ns |
| **ToString Small**    |   8.85 ns |     7.76 ns |           9.21 ns |      21.0 ns |
| **ToString Medium**   |   43.1 ns |     45.5 ns |           68.0 ns |      81.6 ns |
| **ToString Large**    |    293 ns |      150 ns |            328 ns |       562 ns |
| **ToString Negative** |    294 ns |      139 ns |            330 ns |       502 ns |

### Comparison Operations

| Operation              | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ---------------------- | --------: | ----------: | ----------------: | -----------: |
| **Equality Int128**    |  0.106 ns |    0.238 ns |          0.110 ns |     0.750 ns |
| **LessThan Int128**    |  0.111 ns |    0.233 ns |          0.112 ns |     0.767 ns |
| **GreaterThan Int128** |  0.108 ns |    0.230 ns |          0.109 ns |     0.750 ns |
| **Equality Int64**     |  0.112 ns |    0.229 ns |          0.110 ns |     0.732 ns |
| **LessThan Int64**     |  0.109 ns |    0.228 ns |          0.112 ns |     0.802 ns |
| **Equality Uint64**    |  0.108 ns |    0.227 ns |          0.110 ns |     0.830 ns |
| **Equality Double**    |  0.107 ns |    0.227 ns |          0.110 ns |      3.22 ns |
| **LessThan Double**    |  0.107 ns |    0.227 ns |          0.112 ns |      3.22 ns |
| **Equality Float**     |  0.107 ns |    0.227 ns |          0.112 ns |      3.35 ns |
| **Equality Decimal**   |   5.32 ns |     8.04 ns |           5.72 ns |      13.1 ns |
| **LessThan Decimal**   |   4.97 ns |     6.99 ns |           5.44 ns |      12.6 ns |

### State Checking Operations

| Operation                 | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ------------------------- | --------: | ----------: | ----------------: | -----------: |
| **IsZero (Zero)**         |  0.108 ns |    0.228 ns |          0.113 ns |     0.715 ns |
| **IsZero (NonZero)**      |  0.108 ns |    0.225 ns |          0.106 ns |     0.715 ns |
| **IsNegative (Negative)** |  0.107 ns |    0.226 ns |          0.112 ns |     0.715 ns |
| **IsNegative (Positive)** |  0.106 ns |    0.219 ns |          0.110 ns |     0.802 ns |

### Mathematical Operations

| Operation                | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ------------------------ | --------: | ----------: | ----------------: | -----------: |
| **Abs Positive**         |  0.222 ns |    0.223 ns |          0.230 ns |     0.730 ns |
| **Abs Negative**         |  0.224 ns |    0.249 ns |          0.229 ns |     0.732 ns |
| **isqrt**                |   16.7 ns |     18.4 ns |           46.0 ns |      49.9 ns |
| **isqrt Large**          |   17.6 ns |     18.3 ns |           47.1 ns |      64.3 ns |
| **isqrt Perfect Square** |   13.1 ns |     14.7 ns |           35.3 ns |      38.3 ns |

### Free Functions

| Operation   | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ----------- | --------: | ----------: | ----------------: | -----------: |
| **abs()**   |  0.234 ns |    0.231 ns |          0.230 ns |     0.715 ns |
| **isqrt()** |   16.3 ns |     18.4 ns |           47.1 ns |      50.6 ns |
| **sqrt()**  |   25.2 ns |     27.3 ns |           57.8 ns |      61.3 ns |

### Formatting Operations

| Operation           | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ------------------- | --------: | ----------: | ----------------: | -----------: |
| **Format**          |    279 ns |      153 ns |            337 ns |       388 ns |
| **Format Negative** |    280 ns |      154 ns |            345 ns |       389 ns |

---

_Updated on February 03, 2026_
