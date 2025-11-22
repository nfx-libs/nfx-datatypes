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

| Platform    | OS         | Benchmark Framework     | C++ Compiler              | nfx-datatypes Version |
| ----------- | ---------- | ----------------------- | ------------------------- | --------------------- |
| **Linux**   | LMDE 7     | Google Benchmark v1.9.4 | GCC 14.2.0-x64            | v0.1.0                |
| **Linux**   | LMDE 7     | Google Benchmark v1.9.4 | Clang 19.1.7-x64          | v0.1.0                |
| **Windows** | Windows 10 | Google Benchmark v1.9.4 | MinGW GCC 14.2.0-x64      | v0.1.0                |
| **Windows** | Windows 10 | Google Benchmark v1.9.4 | Clang-GNU-CLI 19.1.5-x64  | v0.1.0                |
| **Windows** | Windows 10 | Google Benchmark v1.9.4 | Clang-MSVC-CLI 19.1.5-x64 | v0.1.0                |
| **Windows** | Windows 10 | Google Benchmark v1.9.4 | MSVC 19.44.35217.0-x64    | v0.1.0                |

---

# Performance Results

## Decimal Operations

### Construction Benchmarks

| Operation                 | Linux GCC    | Linux Clang  | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------- | ------------ | ------------ | ----------------- | --------------------- | ---------------------- | ------------ |
| **Construct Default**     | **0.210 ns** | 0.212 ns     | 0.292 ns          | 0.460 ns              | 0.283 ns               | 0.715 ns     |
| **Construct from Int32**  | **0.210 ns** | **0.210 ns** | 0.291 ns          | 0.471 ns              | 0.457 ns               | 0.921 ns     |
| **Construct from Int64**  | 0.211 ns     | **0.209 ns** | 0.285 ns          | 0.474 ns              | 0.276 ns               | 0.921 ns     |
| **Construct from Uint32** | 0.212 ns     | **0.209 ns** | 0.292 ns          | 0.500 ns              | 0.435 ns               | 0.924 ns     |
| **Construct from Uint64** | 0.212 ns     | **0.209 ns** | 0.289 ns          | 0.476 ns              | 0.283 ns               | 0.921 ns     |
| **Construct from Float**  | 66.4 ns      | **65.8 ns**  | 176 ns            | 157 ns                | 92.1 ns                | 77.4 ns      |
| **Construct from Double** | 61.3 ns      | **58.9 ns**  | 180 ns            | 146 ns                | 87.2 ns                | 73.2 ns      |
| **Construct from Int128** | **1.17 ns**  | 1.61 ns      | 1.37 ns           | 8.37 ns               | 5.02 ns                | 1.88 ns      |
| **Copy Construct**        | 0.223 ns     | **0.210 ns** | 0.301 ns          | 0.476 ns              | 0.264 ns               | 0.711 ns     |

### Arithmetic Operations

| Operation                     | Linux GCC   | Linux Clang  | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ----------------------------- | ----------- | ------------ | ----------------- | --------------------- | ---------------------- | ------------ |
| **Addition**                  | **26.3 ns** | 44.3 ns      | 27.9 ns           | 195 ns                | 112 ns                 | 69.8 ns      |
| **Subtraction**               | **22.1 ns** | 30.2 ns      | 22.9 ns           | 144 ns                | 75.3 ns                | 51.6 ns      |
| **Multiplication**            | 61.1 ns     | **43.9 ns**  | 151 ns            | 802 ns                | 435 ns                 | 338 ns       |
| **Multiplication Large**      | 68.1 ns     | **43.8 ns**  | 73.2 ns           | 578 ns                | 345 ns                 | 235 ns       |
| **Division**                  | **41.9 ns** | 45.8 ns      | 62.8 ns           | 645 ns                | 375 ns                 | 471 ns       |
| **Division High Precision**   | **37.7 ns** | 42.8 ns      | 53.1 ns           | 298 ns                | 169 ns                 | 215 ns       |
| **Unary Minus**               | 0.479 ns    | **0.229 ns** | 0.384 ns          | 0.670 ns              | 0.345 ns               | 0.732 ns     |
| **Addition Assignment**       | **26.2 ns** | 44.6 ns      | 27.3 ns           | 197 ns                | 114 ns                 | 71.5 ns      |
| **Subtraction Assignment**    | **22.4 ns** | 30.8 ns      | 22.9 ns           | 143 ns                | 77.4 ns                | 51.6 ns      |
| **Multiplication Assignment** | 61.5 ns     | **53.6 ns**  | 150 ns            | 816 ns                | 449 ns                 | 345 ns       |
| **Division Assignment**       | **41.9 ns** | 46.2 ns      | 59.4 ns           | 670 ns                | 392 ns                 | 500 ns       |

### Parsing Operations

| Operation                | Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------ | --------- | ----------- | ----------------- | --------------------- | ---------------------- | ------------ |
| **Parse Integer**        | 10.5 ns   | **9.81 ns** | 13.2 ns           | 110 ns                | 61.4 ns                | 26.1 ns      |
| **Parse Small Decimal**  | 18.0 ns   | **17.3 ns** | 22.9 ns           | 138 ns                | 75.0 ns                | 33.0 ns      |
| **Parse Large Decimal**  | 50.8 ns   | **43.4 ns** | 64.2 ns           | 628 ns                | 345 ns                 | 126 ns       |
| **Parse High Precision** | 53.3 ns   | **45.0 ns** | 67.0 ns           | 656 ns                | 360 ns                 | 144 ns       |
| **Parse Negative**       | 36.3 ns   | **29.0 ns** | 45.5 ns           | 381 ns                | 225 ns                 | 88.9 ns      |
| **FromString Valid**     | 24.2 ns   | **19.7 ns** | 28.9 ns           | 209 ns                | 120 ns                 | 44.9 ns      |
| **FromString Invalid**   | 5.09 ns   | **4.90 ns** | 6.63 ns           | 12.0 ns               | 6.63 ns                | 12.6 ns      |

### Conversion & Formatting

| Operation             | Linux GCC | Linux Clang  | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| --------------------- | --------- | ------------ | ----------------- | --------------------- | ---------------------- | ------------ |
| **ToDouble**          | 13.6 ns   | **12.6 ns**  | 18.0 ns           | 110 ns                | 60.0 ns                | 14.4 ns      |
| **ToBits**            | 0.607 ns  | **0.467 ns** | 0.600 ns          | 0.952 ns              | 0.547 ns               | 1.43 ns      |
| **ToString Integer**  | 19.1 ns   | **16.4 ns**  | 46.0 ns           | 100 ns                | 54.4 ns                | 44.5 ns      |
| **ToString Small**    | 35.1 ns   | **34.2 ns**  | 69.8 ns           | 148 ns                | 80.2 ns                | 64.2 ns      |
| **ToString Large**    | 35.6 ns   | **40.4 ns**  | 69.8 ns           | 148 ns                | 80.2 ns                | 61.4 ns      |
| **ToString Negative** | 177 ns    | **128 ns**   | 267 ns            | 408 ns                | 215 ns                 | 209 ns       |

### Comparison Operations

| Operation               | Linux GCC   | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ----------------------- | ----------- | ----------- | ----------------- | --------------------- | ---------------------- | ------------ |
| **Equality Decimal**    | **2.42 ns** | 6.08 ns     | 3.28 ns           | 15.7 ns               | 8.89 ns                | 8.20 ns      |
| **LessThan Decimal**    | 2.41 ns     | **2.33 ns** | 3.05 ns           | 18.4 ns               | 10.0 ns                | 8.02 ns      |
| **GreaterThan Decimal** | 2.62 ns     | **2.56 ns** | 3.37 ns           | 20.9 ns               | 11.7 ns                | 9.00 ns      |
| **Equality Int32**      | **2.73 ns** | 9.00 ns     | 3.60 ns           | 21.5 ns               | 12.0 ns                | 8.20 ns      |
| **Equality Int64**      | **2.82 ns** | 6.28 ns     | 3.61 ns           | 16.5 ns               | 9.42 ns                | 8.37 ns      |
| **Equality Uint64**     | **5.52 ns** | 11.7 ns     | 6.56 ns           | 43.0 ns               | 24.6 ns                | 18.4 ns      |
| **Equality Double**     | 66.8 ns     | **58.8 ns** | 181 ns            | 105 ns                | 58.6 ns                | 68.0 ns      |
| **LessThan Double**     | 72.7 ns     | **71.0 ns** | 180 ns            | 164 ns                | 92.1 ns                | 87.9 ns      |
| **Equality Float**      | 71.8 ns     | **67.6 ns** | 180 ns            | 153 ns                | 87.9 ns                | 81.6 ns      |
| **Equality Int128**     | **4.55 ns** | 5.88 ns     | 2.10 ns           | 30.7 ns               | 17.3 ns                | 10.3 ns      |
| **LessThan Int128**     | 10.7 ns     | 15.9 ns     | **9.00 ns**       | 87.2 ns               | 48.7 ns                | 31.5 ns      |

### State Checking Operations

| Operation                 | Linux GCC    | Linux Clang  | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------- | ------------ | ------------ | ----------------- | --------------------- | ---------------------- | ------------ |
| **IsZero (Zero)**         | 1.19 ns      | **1.05 ns**  | 2.01 ns           | 2.29 ns               | 1.28 ns                | 1.93 ns      |
| **IsZero (NonZero)**      | 0.237 ns     | **0.233 ns** | 0.285 ns          | 0.439 ns              | 0.251 ns               | 0.698 ns     |
| **IsNegative (Negative)** | **0.722 ns** | 0.860 ns     | 1.35 ns           | 2.20 ns               | 1.26 ns                | 1.76 ns      |
| **IsNegative (Positive)** | **3.58 ns**  | 7.27 ns      | 4.08 ns           | 35.3 ns               | 19.0 ns                | 12.0 ns      |

### Mathematical Operations

| Operation            | Linux GCC   | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| -------------------- | ----------- | ----------- | ----------------- | --------------------- | ---------------------- | ------------ |
| **Abs Positive**     | **4.06 ns** | 7.69 ns     | 4.17 ns           | 33.0 ns               | 18.0 ns                | 11.5 ns      |
| **Abs Negative**     | 1.20 ns     | **1.13 ns** | 1.69 ns           | 2.41 ns               | 1.38 ns                | 1.99 ns      |
| **Truncate**         | 94.0 ns     | **90.2 ns** | 129 ns            | 879 ns                | 544 ns                 | 586 ns       |
| **Floor**            | 92.0 ns     | **90.5 ns** | 127 ns            | 854 ns                | 547 ns                 | 594 ns       |
| **Ceiling**          | 99.5 ns     | **92.6 ns** | 129 ns            | 1067 ns               | 698 ns                 | 872 ns       |
| **Round**            | 96.2 ns     | **92.6 ns** | 129 ns            | 858 ns                | 562 ns                 | 586 ns       |
| **Sqrt**             | 384 ns      | **381 ns**  | 600 ns            | 3149 ns               | 2040 ns                | 2511 ns      |
| **Sqrt Perfect Sqr** | **23.5 ns** | 33.3 ns     | 57.2 ns           | 107 ns                | 65.6 ns                | 60.9 ns      |
| **Sqrt Large**       | **2138 ns** | 2674 ns     | 2762 ns           | 16113 ns              | 10010 ns               | 12277 ns     |

### Free Functions

| Operation          | Linux GCC   | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------ | ----------- | ----------- | ----------------- | --------------------- | ---------------------- | ------------ |
| **abs() Positive** | **4.14 ns** | 7.70 ns     | 4.17 ns           | 30.0 ns               | 18.8 ns                | 11.7 ns      |
| **abs() Negative** | 1.23 ns     | **1.15 ns** | 1.73 ns           | 2.30 ns               | 1.40 ns                | 2.05 ns      |
| **sqrt()**         | 389 ns      | **379 ns**  | 572 ns            | 2982 ns               | 1995 ns                | 2550 ns      |
| **round()**        | 96.0 ns     | **92.5 ns** | 123 ns            | 820 ns                | 562 ns                 | 600 ns       |
| **floor()**        | 94.1 ns     | **90.2 ns** | 123 ns            | 785 ns                | 544 ns                 | 586 ns       |
| **ceil( )**        | 100 ns      | **92.6 ns** | 129 ns            | 977 ns                | 684 ns                 | 858 ns       |
| **trunc( )**       | 94.5 ns     | **90.1 ns** | 120 ns            | 767 ns                | 547 ns                 | 586 ns       |

### Formatting Operations

| Operation           | Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------- | --------- | ----------- | ----------------- | --------------------- | ---------------------- | ------------ |
| **Format**          | 205 ns    | **164 ns**  | 305 ns            | 283 ns                | 188 ns                 | 225 ns       |
| **Format Negative** | 206 ns    | **166 ns**  | 305 ns            | 276 ns                | 186 ns                 | 215 ns       |

### Property Accessors & Constants

| Operation              | Linux GCC    | Linux Clang  | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ---------------------- | ------------ | ------------ | ----------------- | --------------------- | ---------------------- | ------------ |
| **GetScale**           | **0.212 ns** | 0.213 ns     | 0.283 ns          | 0.361 ns              | 0.251 ns               | 0.750 ns     |
| **DecimalPlacesCount** | 4.95 ns      | **2.54 ns**  | 5.78 ns           | 4.50 ns               | 3.15 ns                | 3.05 ns      |
| **Constant MinValue**  | 0.213 ns     | **0.210 ns** | 0.283 ns          | 0.353 ns              | 0.246 ns               | 1.00 ns      |
| **Constant MaxValue**  | 0.212 ns     | **0.209 ns** | 0.276 ns          | 0.345 ns              | 0.251 ns               | 1.00 ns      |

---

## Int128 Operations

### Construction Benchmarks

| Operation                   | Linux GCC    | Linux Clang  | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| --------------------------- | ------------ | ------------ | ----------------- | --------------------- | ---------------------- | ------------ |
| **Construct Default**       | **0.210 ns** | 0.211 ns     | 0.225 ns          | 0.246 ns              | 0.239 ns               | 0.670 ns     |
| **Construct from Int32**    | **0.210 ns** | 0.346 ns     | 0.220 ns          | 0.256 ns              | 0.246 ns               | 0.670 ns     |
| **Construct from Int64**    | **0.209 ns** | 0.210 ns     | 0.220 ns          | 0.240 ns              | 0.240 ns               | 0.670 ns     |
| **Construct from Uint64**   | **0.209 ns** | 0.217 ns     | 0.220 ns          | 0.246 ns              | 0.246 ns               | 0.663 ns     |
| **Construct from TwoWords** | **0.209 ns** | 0.210 ns     | 0.220 ns          | 0.235 ns              | 0.241 ns               | 0.670 ns     |
| **Construct from Float**    | 2.49 ns      | ** 1.62 ns** | 2.83 ns           | 8.20 ns               | 8.37 ns                | 9.63 ns      |
| **Construct from Double**   | 323 ns       | **322 ns**   | 516 ns            | 711 ns                | 715 ns                 | 753 ns       |
| **Construct from Decimal**  | **6.36 ns**  | 14.9 ns      | 7.15 ns           | 24.0 ns               | 25.1 ns                | 21.9 ns      |
| **Copy Construct**          | 0.210 ns     | **0.209 ns** | 0.225 ns          | 0.225 ns              | 0.241 ns               | 0.663 ns     |

### Arithmetic Operations

| Operation                | Linux GCC    | Linux Clang  | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------ | ------------ | ------------ | ----------------- | --------------------- | ---------------------- | ------------ |
| **Addition**             | 0.210 ns     | **0.209 ns** | 0.220 ns          | 0.544 ns              | 0.572 ns               | 0.698 ns     |
| **Subtraction**          | 0.210 ns     | **0.209 ns** | 0.220 ns          | 0.575 ns              | 0.572 ns               | 0.645 ns     |
| **Multiplication**       | **0.209 ns** | **0.209 ns** | 0.220 ns          | 1.95 ns               | 2.20 ns                | 2.51 ns      |
| **Multiplication Large** | **0.209 ns** | 0.210 ns     | 0.225 ns          | 1.97 ns               | 2.29 ns                | 2.49 ns      |
| **Division**             | **0.209 ns** | 0.235 ns     | 0.220 ns          | 5.58 ns               | 6.28 ns                | 6.42 ns      |
| **Division Large**       | 0.214 ns     | **0.209 ns** | 0.220 ns          | 123 ns                | 157 ns                 | 195 ns       |
| **Modulo**               | 0.243 ns     | **0.210 ns** | 0.220 ns          | 7.53 ns               | 7.85 ns                | 8.02 ns      |
| **Unary Minus**          | **0.210 ns** | 0.265 ns     | 0.215 ns          | 0.942 ns              | 0.977 ns               | 0.684 ns     |

### Parsing Operations

| Operation                 | Linux GCC   | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------- | ----------- | ----------- | ----------------- | --------------------- | ---------------------- | ------------ |
| **Parse Small Number**    | 3.25 ns     | **2.77 ns** | 3.30 ns           | 4.24 ns               | 4.30 ns                | 5.31 ns      |
| **Parse Medium Number**   | 27.6 ns     | **23.2 ns** | 29.6 ns           | 40.1 ns               | 42.4 ns                | 33.0 ns      |
| **Parse Large Number**    | **47.5 ns** | 55.6 ns     | 76.7 ns           | 90.0 ns               | 103 ns                 | 75.0 ns      |
| **Parse Negative Number** | 49.0 ns     | **40.7 ns** | 53.1 ns           | 68.0 ns               | 78.5 ns                | 54.4 ns      |
| **FromString Valid**      | 48.3 ns     | **40.4 ns** | 50.0 ns           | 68.4 ns               | 76.7 ns                | 54.4 ns      |
| **FromString Invalid**    | **1.39 ns** | 1.62 ns     | 1.61 ns           | 2.18 ns               | 2.41 ns                | 2.83 ns      |

### Conversion & Formatting

| Operation             | Linux GCC    | Linux Clang  | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| --------------------- | ------------ | ------------ | ----------------- | --------------------- | ---------------------- | ------------ |
| **ToLow**             | **0.105 ns** | 0.210 ns     | 0.112 ns          | 0.215 ns              | 0.240 ns               | 0.684 ns     |
| **ToHigh**            | **0.105 ns** | 0.209 ns     | 0.106 ns          | 0.218 ns              | 0.246 ns               | 0.663 ns     |
| **ToBits**            | **0.209 ns** | **0.209 ns** | 0.215 ns          | 0.222 ns              | 0.241 ns               | 0.698 ns     |
| **ToString Small**    | **20.9 ns**  | 22.6 ns      | 48.7 ns           | 50.8 ns               | 53.1 ns                | 47.6 ns      |
| **ToString Medium**   | **188 ns**   | 226 ns       | 314 ns            | 345 ns                | 345 ns                 | 314 ns       |
| **ToString Large**    | **432 ns**   | 556 ns       | 1046 ns           | 1200 ns               | 1294 ns                | 1123 ns      |
| **ToString Negative** | **438 ns**   | 585 ns       | 1088 ns           | 1283 ns               | 1283 ns                | 1172 ns      |

### Comparison Operations

| Operation              | Linux GCC    | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ---------------------- | ------------ | ----------- | ----------------- | --------------------- | ---------------------- | ------------ |
| **Equality Int128**    | **0.105 ns** | 0.210 ns    | 0.115 ns          | 0.215 ns              | 0.240 ns               | 0.684 ns     |
| **LessThan Int128**    | **0.105 ns** | 0.211 ns    | 0.117 ns          | 1.14 ns               | 1.20 ns                | 0.684 ns     |
| **GreaterThan Int128** | **0.105 ns** | 0.210 ns    | 0.117 ns          | 1.38 ns               | 1.44 ns                | 0.663 ns     |
| **Equality Int64**     | **0.105 ns** | 0.209 ns    | 0.112 ns          | 0.215 ns              | 0.241 ns               | 0.670 ns     |
| **LessThan Int64**     | **0.105 ns** | 0.209 ns    | 0.115 ns          | 0.215 ns              | 0.240 ns               | 0.684 ns     |
| **Equality Uint64**    | **0.105 ns** | 0.210 ns    | 0.112 ns          | 0.214 ns              | 0.235 ns               | 0.684 ns     |
| **Equality Double**    | **0.105 ns** | 0.210 ns    | 0.112 ns          | 4.24 ns               | 4.55 ns                | 2.89 ns      |
| **LessThan Double**    | **0.105 ns** | 0.210 ns    | 0.115 ns          | 3.52 ns               | 3.77 ns                | 2.83 ns      |
| **Equality Float**     | **0.105 ns** | 0.209 ns    | 0.115 ns          | 4.24 ns               | 4.50 ns                | 2.83 ns      |
| **Equality Decimal**   | **4.50 ns**  | 7.28 ns     | 4.65 ns           | 14.4 ns               | 15.7 ns                | 11.0 ns      |
| **LessThan Decimal**   | **4.10 ns**  | 6.32 ns     | 4.76 ns           | 14.1 ns               | 15.4 ns                | 11.1 ns      |

### State Checking Operations

| Operation                 | Linux GCC    | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------- | ------------ | ----------- | ----------------- | --------------------- | ---------------------- | ------------ |
| **IsZero (Zero)**         | **0.105 ns** | 0.209 ns    | 0.114 ns          | 0.215 ns              | 0.234 ns               | 0.680 ns     |
| **IsZero (NonZero)**      | **0.105 ns** | 0.210 ns    | 0.114 ns          | 0.210 ns              | 0.240 ns               | 0.684 ns     |
| **IsNegative (Negative)** | **0.105 ns** | 0.209 ns    | 0.115 ns          | 0.215 ns              | 0.244 ns               | 0.684 ns     |
| **IsNegative (Positive)** | **0.105 ns** | 0.209 ns    | 0.114 ns          | 0.213 ns              | 0.241 ns               | 0.684 ns     |

### Mathematical Operations

| Operation        | Linux GCC    | Linux Clang  | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ---------------- | ------------ | ------------ | ----------------- | --------------------- | ---------------------- | ------------ |
| **Abs Positive** | **0.209 ns** | **0.209 ns** | 0.235 ns          | 1.29 ns               | 1.34 ns                | 0.684 ns     |
| **Abs Negative** | **0.209 ns** | 0.210 ns     | 0.235 ns          | 1.57 ns               | 1.60 ns                | 0.684 ns     |
| **isqrt Small**  | **14.1 ns**  | 16.3 ns      | 46.0 ns           | 42.4 ns               | 51.6 ns                | 51.6 ns      |
| **isqrt Large**  | **15.7 ns**  | 16.4 ns      | 47.1 ns           | 600 ns                | 670 ns                 | 984 ns       |
| **sqrt Perfect** | **12.0 ns**  | 13.0 ns      | 36.1 ns           | 31.4 ns               | 35.3 ns                | 36.8 ns      |
| **sqrt NonPerf** | **20.0 ns**  | 24.8 ns      | 46.0 ns           | 42.4 ns               | 51.6 ns                | 51.6 ns      |

### Free Functions

| Operation   | Linux GCC   | Linux Clang  | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ----------- | ----------- | ------------ | ----------------- | --------------------- | ---------------------- | ------------ |
| **abs()**   | 0.210 ns    | **0.209 ns** | 0.240 ns          | 1.29 ns               | 1.34 ns                | 0.684 ns     |
| **isqrt()** | **14.5 ns** | 16.3 ns      | 51.6 ns           | 43.9 ns               | 51.6 ns                | 51.6 ns      |
| **sqrt()**  | **20.0 ns** | 24.8 ns      | 60.0 ns           | 55.8 ns               | 58.6 ns                | 61.4 ns      |

### Formatting Operations

| Operation  | Linux GCC  | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ---------- | ---------- | ----------- | ----------------- | --------------------- | ---------------------- | ------------ |
| **Format** | **358 ns** | 432 ns      | 750 ns            | 900 ns                | 872 ns                 | 837 ns       |

---

_Updated on November 22, 2025_
