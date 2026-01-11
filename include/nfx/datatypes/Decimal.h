/*
 * MIT License
 *
 * Copyright (c) 2025 nfx
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file Decimal.h
 * @brief Cross-platform high-precision decimal arithmetic type
 * @details Implements the Decimal data type with exact decimal arithmetic and no floating-point rounding errors.
 *
 *          Range and Precision:
 *          - Values from -79,228,162,514,264,337,593,543,950,335 to +79,228,162,514,264,337,593,543,950,335
 *          - 28 significant decimal digits maximum
 *          - Finite set of values of the form m / 10^e where:
 *            * m is an integer such that -2^96 < m < 2^96
 *            * e is an integer between 0 and 28 inclusive
 *
 *          Key Benefits:
 *          - Exact representation of decimal fractions (e.g., 0.1 is represented exactly)
 *          - No round-off errors in financial calculations
 *          - Ideally suited for monetary and high-precision decimal arithmetic
 *
 *          Storage Format:
 *          - 96-bit mantissa + 32-bit scale/sign = 128-bit total storage
 *
 *          Memory Layout of Decimal (128 bits / 16 bytes):
 *          ==============================================
 *
 *          1. Flags (32 bits):
 *          ┌───────────┬─────────────────────────────────────┬───────────────────────────────────────────────────┐
 *          │    Bits   │             Description             │                       Notes                       │
 *          ├───────────┼─────────────────────────────────────┼───────────────────────────────────────────────────┤
 *          │   0 - 15  │  Unused (must be zero)              │  Reserved - Required to be zero for valid format  │
 *          │  16 - 23  │  Scale (0-28)                       │  Number of decimal digits after decimal point     │
 *          │  24 - 30  │  Unused (must be zero)              │  Reserved - Required to be zero for valid format  │
 *          │  31       │  Sign (0 = positive, 1 = negative)  │  Sign bit                                         │
 *          └───────────┴─────────────────────────────────────┴───────────────────────────────────────────────────┘
 *
 *          2. Mantissa (96 bits total):
 *          ┌───────────────┬───────────┬─────────────────────────────────┐
 *          │ Mantissa Part │   Bits    │           Description           │
 *          ├───────────────┼───────────┼─────────────────────────────────┤
 *          │  mantissa[0]  │   0 - 31  │  Lower 32 bits of the mantissa  │
 *          │  mantissa[1]  │  32 - 63  │  Middle 32 bits of the mantissa │
 *          │  mantissa[2]  │  64 - 95  │  Upper 32 bits of the mantissa  │
 *          └───────────────┴───────────┴─────────────────────────────────┘
 *
 *          Complete Memory Layout (128 bits / 16 bytes):
 *          =============================================
 *
 *          ┌─────────────────────────────────┬─────────────────────────────────┬─────────────────────────────────┬─────────────────────────────────┐
 *          │         mantissa[2]             │         mantissa[1]             │          mantissa[0]            │            flags                │
 *          │       (upper 32 bits)           │      (middle 32 bits)           │       (lower 32 bits)           │        (scale + sign)           │
 *          │          32 bits                │          32 bits                │           32 bits               │            32 bit               │
 *          └─────────────────────────────────┴─────────────────────────────────┴─────────────────────────────────┴─────────────────────────────────┘
 *          Bit 127                     Bit 96 Bit 95                     Bit 64 Bit 63                     Bit 32 Bit 31                       Bit 0
 *
 *          Where the 96-bit mantissa represents an unsigned integer from 0 to 2^96-1
 *          and the sign is stored separately in bit 31 of the flags word.
 *
 *          Summary:
 *          =======
 *
 *          - Total storage: 128 bits (16 bytes)
 *          - Value formula: decimal_value = mantissa / 10^scale × (sign ? -1 : 1)
 *
 *          Examples with Memory Layout:
 *          ============================
 *
 *          Example 1 - Value 123.45:
 *          - mantissa: 12345 (stored across mantissa[0-2])
 *          - scale   : 2 (bits 16-23 of flags, 2 decimal places)
 *          - sign    : 0 (bit 31 of flags, positive)
 *          - result  : 12345 / 10² = 123.45
 *
 *          Example 2 - Value -12,345,678,901,234,567,890.123456789:
 *          - mantissa: 12345678901234567890123456789 (96-bit value across mantissa[0-2])
 *                      mantissa[0] = 0x15CD5B07  - 365,072,135   (lower 32 bits)
 *                      mantissa[1] = 0x9CE5A30A  - 2,632,713,994 (middle 32 bits)
 *                      mantissa[2] = 0x27B95E997 - 669,260,439   (upper 32 bits)
 *          - scale   : 9 (bits 16-23 of flags, 9 decimal places)
 *          - sign    : 1 (bit 31 of flags, negative)
 *          - result  : 12345678901234567890123456789 / 10⁹ × (-1) = -12,345,678,901,234,567,890.123456789
 *
 *          - Original = (mantissa[2] × 2^64) + (mantissa[1] × 2^32) + mantissa[0]
 *          - Original = (669,260,439 × 18,446,744,073,709,551,616) + (2,632,713,994 × 4,294,967,296) + 365,072,135
 *          - Original = 12345678901234567890123456789
 *
 *          IEEE 754-2008 binary64 Input Compatibility:
 *          - Construction from double uses IEEE 754-2008 std::isnan and std::isinf functions
 *          - Preserves IEEE 754 binary64 precision limits (~15-17 digits)
 *          - NaN and Infinity from double are converted to zero
 *          - String construction provides exact decimal precision (up to 28 digits)
 *
 * @note This is NOT IEEE 754 decimal128 arithmetic - it implements
 *       exact fixed-point decimal arithmetic without floating-point rounding errors.
 *
 * @note Design inspired by .NET System.Decimal semantics (96-bit mantissa + scale, 28–29 digits, banker's rounding).
 */

#pragma once

#include <array>
#include <compare>
#include <cstdint>
#include <limits>
#include <optional>
#include <string>
#include <string_view>

#include "Int128.h"

namespace nfx::datatypes
{
    //=====================================================================
    // Decimal class
    //=====================================================================

    /**
     * @brief Cross-platform high-precision decimal type
     * @details Implements exact decimal arithmetic with:
     *          - Range: ±79,228,162,514,264,337,593,543,950,335
     *          - 28 significant decimal digits maximum
     *          - 96-bit mantissa + 32-bit scale/sign = 128-bit total storage
     *          - Exact decimal arithmetic (no floating-point rounding errors)
     *
     *          Value Representation:
     *          - All values are of the form: mantissa / 10^scale
     *          - Mantissa: 96-bit unsigned integer (0 ≤ m < 2^96)
     *          - Sign: Stored separately in bit 31 of flags word
     *          - Scale: 0 to 28 inclusive (number of decimal places)
     *
     *          Financial Calculation Benefits:
     *          - Decimal fractions like 0.1 are represented exactly (no 0.099999... issues)
     *          - No round-off errors in monetary calculations
     *          - Predictable precision for financial applications
     *
     *          IEEE 754-2008 binary64 Input Compatibility:
     *          - Double constructor uses IEEE 754-2008 std::isnan and std::isinf functions
     *          - Respects IEEE 754 binary64 precision limits (~15-17 digits)
     *          - NaN and Infinity from double input are converted to zero
     *          - No internal NaN/Infinity states (division by zero throws std::overflow_error)
     *          - Use string constructor for values requiring exact decimal precision
     *
     * @note Note: This implements exact fixed-point decimal arithmetic, NOT IEEE 754 decimal128.
     */
    class Decimal final
    {
        friend struct std::numeric_limits<Decimal>;

    public:
        //----------------------------------------------
        // Rounding modes
        //----------------------------------------------

        /**
         * @brief Rounding modes for decimal arithmetic operations
         * @details Defines how rounding should be performed when reducing decimal precision.
         *          Each mode follows industry-standard rounding semantics:
         *
         *          - ToNearest (banker's rounding): Round to nearest value, ties round to even digit
         *            Examples: 2.5→2, 3.5→4, 4.5→4, 5.5→6 (minimizes cumulative rounding bias)
         *
         *          - ToNearestTiesAway (standard rounding): Round to nearest value, ties away from zero
         *            Examples: 2.5→3, 3.5→4, -2.5→-3, -3.5→-4 (traditional "round half up" for positives)
         *
         *          - ToZero (truncation): Round towards zero, discarding fractional part
         *            Examples: 2.9→2, -2.9→-2, 3.1→3, -3.1→-3
         *
         *          - ToPositiveInfinity (ceiling): Round towards positive infinity
         *            Examples: 2.1→3, -2.9→-2, 3.0→3
         *
         *          - ToNegativeInfinity (floor): Round towards negative infinity
         *            Examples: 2.9→2, -2.1→-3, 3.0→3
         */
        enum class RoundingMode : std::uint8_t
        {
            ToNearest = 0,      ///< Round to nearest, ties to even (banker's rounding)
            ToNearestTiesAway,  ///< Round to nearest, ties away from zero (standard rounding)
            ToZero,             ///< Round towards zero (truncate)
            ToPositiveInfinity, ///< Round towards +∞ (ceiling)
            ToNegativeInfinity  ///< Round towards -∞ (floor)
        };

        //----------------------------------------------
        // Construction
        //----------------------------------------------

        /**
         * @brief Default constructor (zero value)
         */
        inline constexpr Decimal() noexcept;

        /**
         * @brief Construct from single-precision floating-point value with IEEE 754-2008 input compatibility
         * @param value Float value to convert
         * @details Delegates to the double constructor for consistent behavior.
         *          Limited to IEEE 754 binary32 precision (~6-7 significant digits).
         *          Uses IEEE 754-2008 std::isnan and std::isinf for special value detection.
         *          NaN and Infinity values are converted to zero.
         * @note For exact precision with fractional values, use string constructor instead.
         *       Float precision is inherently limited and may introduce rounding artifacts.
         */
        explicit Decimal( float value ) noexcept;

        /**
         * @brief Construct from double with IEEE 754-2008 input compatibility
         * @param value Double value to convert
         * @note Limited to IEEE 754 binary64 precision (~15-17 significant digits).
         *       Uses IEEE 754-2008 std::isnan and std::isinf for special value detection.
         *       NaN and Infinity values are converted to zero.
         *       For exact precision, use string constructor instead.
         */
        explicit Decimal( double value ) noexcept;

        /**
         * @brief Construct from 32-bit integer
         * @param value Integer value to convert
         */
        inline explicit Decimal( std::int32_t value ) noexcept;

        /**
         * @brief Construct from 64-bit integer
         * @param value Integer value to convert
         */
        inline explicit Decimal( std::int64_t value ) noexcept;

        /**
         * @brief Construct from 32-bit unsigned integer
         * @param value Unsigned integer value to convert
         */
        inline explicit Decimal( std::uint32_t value ) noexcept;

        /**
         * @brief Construct from 64-bit unsigned integer
         * @param value Unsigned integer value to convert
         */
        inline explicit Decimal( std::uint64_t value ) noexcept;

        /**
         * @brief Construct from string (exact parsing)
         * @param str String representation (e.g., "123.456")
         * @throws std::invalid_argument if string is not a valid decimal
         * @see fromString() for non-throwing parsing
         */
        inline explicit Decimal( std::string_view str );

        /**
         * @brief Construct from C-string (exact parsing)
         * @param scStr Null-terminated C-string representation (e.g., "123.456")
         * @throws std::invalid_argument if string is not a valid decimal
         * @see fromString() for non-throwing parsing
         */
        inline explicit Decimal( const char* scStr );

        /**
         * @brief Construct from 128-bit integer with overflow detection
         * @param val Int128 value to convert
         * @throws std::overflow_error if Int128 value exceeds Decimal's 96-bit mantissa capacity
         * @details Converts Int128 to Decimal integer representation (scale = 0).
         *          Range Handling:
         *          - Int128 range: ±170,141,183,460,469,231,731,687,303,715,884,105,727/728 (2^127 - 1 / -2^127)
         *          - Decimal range: ±79,228,162,514,264,337,593,543,950,335 (2^96 - 1)
         *
         *          Overflow Behavior:
         *          - Values within Decimal's range are preserved exactly
         *          - Values exceeding Decimal's capacity throw std::overflow_error
         *          - Zero values are handled efficiently
         *
         *          Examples:
         *          - Int128(42)     → Decimal("42")
         *          - Int128("-123") → Decimal("-123")
         *          - Int128("79228162514264337593543950335")  → Decimal("79228162514264337593543950335") (max valid)
         *          - Int128("170141183460469231731687303715884105727")  → throws std::overflow_error
         *          - Int128("-170141183460469231731687303715884105728") → throws std::overflow_error
         */
        explicit Decimal( const Int128& val );

        /**
         * @brief Copy constructor
         * @param other The Decimal object to copy from
         */
        Decimal( const Decimal& other ) noexcept = default;

        /**
         * @brief Move constructor
         * @param other The Decimal object to move from
         */
        Decimal( Decimal&& other ) noexcept = default;

        //----------------------------------------------
        // Destruction
        //----------------------------------------------

        /** @brief Destructor */
        ~Decimal() = default;

        //----------------------------------------------
        // Assignment
        //----------------------------------------------

        /**
         * @brief Copy assignment operator
         * @param other The Decimal object to copy from
         * @return Reference to this Decimal object after assignment
         */
        Decimal& operator=( const Decimal& other ) noexcept = default;

        /**
         * @brief Move assignment operator
         * @param other The Decimal object to move from
         * @return Reference to this Decimal object after assignment
         */
        Decimal& operator=( Decimal&& other ) noexcept = default;

        //----------------------------------------------
        // Comparison operators
        //----------------------------------------------

        /**
         * @brief Three-way comparison operator (C++20)
         * @param other The Decimal value to compare with
         * @return std::strong_ordering indicating relative order (less, equal, or greater)
         * @note Compiler auto-generates operator!=, <, <=, >, >= from this operator
         */
        std::strong_ordering operator<=>( const Decimal& other ) const noexcept;

        /**
         * @brief Equality comparison operator
         * @param other The Decimal value to compare with
         * @return true if values are equal, false otherwise
         * @note Required alongside operator<=> for complete C++20 comparison support
         */
        bool operator==( const Decimal& other ) const noexcept;

        //----------------------------------------------
        // Comparison with built-in floating point types
        //----------------------------------------------

        /**
         * @brief Equality comparison with float
         * @param val Right operand
         * @return true if values are equal (subject to floating-point precision)
         * @note Comparison may have precision limitations due to float's ~6-7 digit precision
         */
        inline bool operator==( float val ) const noexcept;

        /**
         * @brief Inequality comparison with float
         * @param val Right operand
         * @return true if values are not equal (subject to floating-point precision)
         * @note Comparison may have precision limitations due to float's ~6-7 digit precision
         */
        inline bool operator!=( float val ) const noexcept;

        /**
         * @brief Less than comparison with float
         * @param val Right operand
         * @return true if this is less than val (subject to floating-point precision)
         * @note Comparison may have precision limitations due to float's ~6-7 digit precision
         */
        inline bool operator<( float val ) const noexcept;

        /**
         * @brief Less than or equal comparison with float
         * @param val Right operand
         * @return true if this is less than or equal to val (subject to floating-point precision)
         * @note Comparison may have precision limitations due to float's ~6-7 digit precision
         */
        inline bool operator<=( float val ) const noexcept;

        /**
         * @brief Greater than comparison with float
         * @param val Right operand
         * @return true if this is greater than val (subject to floating-point precision)
         * @note Comparison may have precision limitations due to float's ~6-7 digit precision
         */
        inline bool operator>( float val ) const noexcept;

        /**
         * @brief Greater than or equal comparison with float
         * @param val Right operand
         * @return true if this is greater than or equal to val (subject to floating-point precision)
         * @note Comparison may have precision limitations due to float's ~6-7 digit precision
         */
        inline bool operator>=( float val ) const noexcept;

        /**
         * @brief Equality comparison with double
         * @param val Right operand
         * @return true if values are equal (subject to floating-point precision)
         * @note Comparison may have precision limitations due to double's ~15-17 digit precision
         */
        inline bool operator==( double val ) const noexcept;

        /**
         * @brief Inequality comparison with double
         * @param val Right operand
         * @return true if values are not equal (subject to floating-point precision)
         * @note Comparison may have precision limitations due to double's ~15-17 digit precision
         */
        inline bool operator!=( double val ) const noexcept;

        /**
         * @brief Less than comparison with double
         * @param val Right operand
         * @return true if this is less than val (subject to floating-point precision)
         * @note Comparison may have precision limitations due to double's ~15-17 digit precision
         */
        inline bool operator<( double val ) const noexcept;

        /**
         * @brief Less than or equal comparison with double
         * @param val Right operand
         * @return true if this is less than or equal to val (subject to floating-point precision)
         * @note Comparison may have precision limitations due to double's ~15-17 digit precision
         */
        inline bool operator<=( double val ) const noexcept;

        /**
         * @brief Greater than comparison with double
         * @param val Right operand
         * @return true if this is greater than val (subject to floating-point precision)
         * @note Comparison may have precision limitations due to double's ~15-17 digit precision
         */
        inline bool operator>( double val ) const noexcept;

        /**
         * @brief Greater than or equal comparison with double
         * @param val Right operand
         * @return true if this is greater than or equal to val (subject to floating-point precision)
         * @note Comparison may have precision limitations due to double's ~15-17 digit precision
         */
        inline bool operator>=( double val ) const noexcept;

        //----------------------------------------------
        // Comparison with built-in integer types
        //----------------------------------------------

        /**
         * @brief Equality comparison with signed 64-bit integer
         * @param val Right operand
         * @return true if values are equal
         */
        inline bool operator==( std::int64_t val ) const noexcept;

        /**
         * @brief Inequality comparison with signed 64-bit integer
         * @param val Right operand
         * @return true if values are not equal
         */
        inline bool operator!=( std::int64_t val ) const noexcept;

        /**
         * @brief Less than comparison with signed 64-bit integer
         * @param val Right operand
         * @return true if this is less than val
         */
        inline bool operator<( std::int64_t val ) const noexcept;

        /**
         * @brief Less than or equal comparison with signed 64-bit integer
         * @param val Right operand
         * @return true if this is less than or equal to val
         */
        inline bool operator<=( std::int64_t val ) const noexcept;

        /**
         * @brief Greater than comparison with signed 64-bit integer
         * @param val Right operand
         * @return true if this is greater than val
         */
        inline bool operator>( std::int64_t val ) const noexcept;

        /**
         * @brief Greater than or equal comparison with signed 64-bit integer
         * @param val Right operand
         * @return true if this is greater than or equal to val
         */
        inline bool operator>=( std::int64_t val ) const noexcept;

        /**
         * @brief Equality comparison with unsigned 64-bit integer
         * @param val Right operand
         * @return true if values are equal
         */
        inline bool operator==( std::uint64_t val ) const noexcept;

        /**
         * @brief Inequality comparison with unsigned 64-bit integer
         * @param val Right operand
         * @return true if values are not equal
         */
        inline bool operator!=( std::uint64_t val ) const noexcept;

        /**
         * @brief Less than comparison with unsigned 64-bit integer
         * @param val Right operand
         * @return true if this is less than val
         */
        inline bool operator<( std::uint64_t val ) const noexcept;

        /**
         * @brief Less than or equal comparison with unsigned 64-bit integer
         * @param val Right operand
         * @return true if this is less than or equal to val
         */
        inline bool operator<=( std::uint64_t val ) const noexcept;

        /**
         * @brief Greater than comparison with unsigned 64-bit integer
         * @param val Right operand
         * @return true if this is greater than val
         */
        inline bool operator>( std::uint64_t val ) const noexcept;

        /**
         * @brief Greater than or equal comparison with unsigned 64-bit integer
         * @param val Right operand
         * @return true if this is greater than or equal to val
         */
        inline bool operator>=( std::uint64_t val ) const noexcept;

        /**
         * @brief Equality comparison with signed 32-bit integer
         * @param val Right operand
         * @return true if values are equal
         */
        inline bool operator==( std::int32_t val ) const noexcept;

        /**
         * @brief Inequality comparison with signed 32-bit integer
         * @param val Right operand
         * @return true if values are not equal
         */
        inline bool operator!=( std::int32_t val ) const noexcept;

        /**
         * @brief Less than comparison with signed 32-bit integer
         * @param val Right operand
         * @return true if this is less than val
         */
        inline bool operator<( std::int32_t val ) const noexcept;

        /**
         * @brief Less than or equal comparison with signed 32-bit integer
         * @param val Right operand
         * @return true if this is less than or equal to val
         */
        inline bool operator<=( std::int32_t val ) const noexcept;

        /**
         * @brief Greater than comparison with signed 32-bit integer
         * @param val Right operand
         * @return true if this is greater than val
         */
        inline bool operator>( std::int32_t val ) const noexcept;

        /**
         * @brief Greater than or equal comparison with signed 32-bit integer
         * @param val Right operand
         * @return true if this is greater than or equal to val
         */
        inline bool operator>=( std::int32_t val ) const noexcept;

        //----------------------------------------------
        // Comparison with nfx::datatypes::Int128
        //----------------------------------------------

        /**
         * @brief Equality comparison with Int128
         * @param val Right operand
         * @return true if values are equal
         * @note For equality, the Decimal must have no fractional part and represent the same integer value
         */
        bool operator==( const Int128& val ) const noexcept;

        /**
         * @brief Inequality comparison with Int128
         * @param val Right operand
         * @return true if values are not equal
         */
        inline bool operator!=( const Int128& val ) const noexcept;

        /**
         * @brief Less than comparison with Int128
         * @param val Right operand
         * @return true if this is less than val
         */
        bool operator<( const Int128& val ) const noexcept;

        /**
         * @brief Less than or equal comparison with Int128
         * @param val Right operand
         * @return true if this is less than or equal to val
         */
        inline bool operator<=( const Int128& val ) const noexcept;

        /**
         * @brief Greater than comparison with Int128
         * @param val Right operand
         * @return true if this is greater than val
         */
        inline bool operator>( const Int128& val ) const noexcept;

        /**
         * @brief Greater than or equal comparison with Int128
         * @param val Right operand
         * @return true if this is greater than or equal to val
         */
        inline bool operator>=( const Int128& val ) const noexcept;

        //----------------------------------------------
        // Arithmetic operators
        //----------------------------------------------

        /**
         * @brief Addition operator
         * @param other The Decimal value to add
         * @return Result of addition
         */
        Decimal operator+( const Decimal& other );

        /**
         * @brief Subtraction operator
         * @param other The Decimal value to subtract
         * @return Result of subtraction
         */
        inline Decimal operator-( const Decimal& other );

        /**
         * @brief Multiplication operator
         * @param other The Decimal value to multiply by
         * @return Result of multiplication
         */
        Decimal operator*( const Decimal& other ) const;

        /**
         * @brief Division operator
         * @param other Divisor
         * @return Result of division
         * @throws std::overflow_error if divisor is zero (no NaN/Infinity representation)
         */
        Decimal operator/( const Decimal& other ) const;

        /**
         * @brief Addition assignment operator
         * @param other The Decimal value to add
         * @return Reference to this Decimal after addition
         */
        inline Decimal& operator+=( const Decimal& other );

        /**
         * @brief Subtraction assignment operator
         * @param other The Decimal value to subtract
         * @return Reference to this Decimal after subtraction
         */
        inline Decimal& operator-=( const Decimal& other );

        /**
         * @brief Multiplication assignment operator
         * @param other The Decimal value to multiply by
         * @return Reference to this Decimal after multiplication
         */
        inline Decimal& operator*=( const Decimal& other );

        /**
         * @brief Division assignment operator
         * @param other Divisor
         * @return Reference to this after division
         * @throws std::overflow_error if divisor is zero (no NaN/Infinity representation)
         */
        inline Decimal& operator/=( const Decimal& other );

        /**
         * @brief Unary minus operator (negation)
         * @return Negated decimal value
         */
        inline Decimal operator-() const noexcept;

        //----------------------------------------------
        // Property accessors
        //----------------------------------------------

        /**
         * @brief Get decimal scale (number of decimal places)
         * @return Scale value (0-28)
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] std::uint8_t scale() const noexcept;

        /**
         * @brief Get flags value
         * @return Reference to flags
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] inline const std::uint32_t& flags() const noexcept;

        /**
         * @brief Get mutable flags value
         * @return Mutable reference to flags
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] inline std::uint32_t& flags() noexcept;

        /**
         * @brief Get mantissa array
         * @return Reference to mantissa array
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] inline const std::array<std::uint32_t, 3>& mantissa() const noexcept;

        /**
         * @brief Get mutable mantissa array
         * @return Mutable reference to mantissa array
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] inline std::array<std::uint32_t, 3>& mantissa() noexcept;

        //----------------------------------------------
        // Mathematical operations
        //----------------------------------------------

        /**
         * @brief Get absolute value
         * @return Absolute value of the decimal
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] inline Decimal abs() const noexcept;

        /**
         * @brief Round up to nearest integer
         * @return Smallest integer greater than or equal to value
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] inline Decimal ceil() const noexcept;

        /**
         * @brief Round down to nearest integer
         * @return Largest integer less than or equal to value
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] inline Decimal floor() const noexcept;

        /**
         * @brief Round decimal to specified precision using configurable rounding mode
         * @param decimalsPlacesCount Number of decimal places to round to (default: 0 for integer rounding)
         * @param mode Rounding mode to apply (default: RoundingMode::ToNearest for banker's rounding)
         * @return Decimal value rounded to the specified precision
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] Decimal round( std::int32_t decimalsPlacesCount = 0, RoundingMode mode = RoundingMode::ToNearest ) const noexcept;

        /**
         * @brief Compute square root using Newton-Raphson method
         * @return Square root with full decimal precision (up to 28 digits)
         * @throws std::domain_error if called on negative value
         * @details Uses iterative Newton-Raphson algorithm: x_{n+1} = (x_n + value/x_n) / 2
         *          Converges quadratically for positive values.
         *          Returns exact result for perfect squares within precision limits.
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] Decimal sqrt() const;

        /**
         * @brief Remove fractional part
         * @return Decimal with fractional part removed
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] inline Decimal trunc() const noexcept;

        //----------------------------------------------
        // String parsing
        //----------------------------------------------

        /**
         * @brief Parse string to decimal with error handling
         * @param str String to parse (e.g., "123.45", "-0.001", "42")
         * @param result Output decimal value
         * @return true if parsing succeeded, false otherwise
         * @details Parses string representations of decimal numbers with comprehensive format support.
         *          Supported input formats:
         *          - Integers: "123", "-456"
         *          - Decimals: "123.45", "-0.001"
         *          - Scientific notation: "1.23e5", "-4.56E-3"
         *          - Leading/trailing whitespace is trimmed
         *          - Positive signs are optional: "+123.45" → 123.45
         *          - Maximum precision: 28 significant decimal digits
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         * @note Range: ±79,228,162,514,264,337,593,543,950,335 with up to 28 decimal places
         * @note For convenience wrapper with std::optional return, use the single-parameter overload
         */
        [[nodiscard]] static bool fromString( std::string_view str, Decimal& result ) noexcept;

        /**
         * @brief Parse string to decimal with optional return
         * @param str String to parse (e.g., "123.45", "-0.001", "42")
         * @return std::optional<Decimal> containing parsed value if successful, std::nullopt if parsing failed
         * @details This is a convenience wrapper around the two-parameter fromString method.
         *          Supports the same input formats:
         *          - Integers: "123", "-456"
         *          - Decimals: "123.45", "-0.001"
         *          - Scientific notation: "1.23e5", "-4.56E-3"
         *          - Leading/trailing whitespace is trimmed
         *          - Positive signs are optional: "+123.45" → 123.45
         *          - Maximum precision: 28 significant decimal digits
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         * @note For error details, use the two-parameter overload with explicit error handling
         * @note Range: ±79,228,162,514,264,337,593,543,950,335 with up to 28 decimal places
         */
        [[nodiscard]] static std::optional<Decimal> fromString( std::string_view str ) noexcept;

        //----------------------------------------------
        // Type conversion
        //----------------------------------------------

        /**
         * @brief Convert to double (may lose precision)
         * @return Double representation
         * @note Conversion may lose precision beyond ~15-17 significant digits
         * @note For exact precision, use toString() method instead
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] double toDouble() const noexcept;

        /**
         * @brief Convert to string with exact precision
         * @return String representation
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] std::string toString() const;

        /**
         * @brief Get internal 32-bit representation
         * @return Array of 4 32-bit integers representing the decimal
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] std::array<std::int32_t, 4> toBits() const noexcept;

        //----------------------------------------------
        // Utilities
        //----------------------------------------------

        /**
         * @brief Count actual decimal places (excluding trailing zeros)
         * @return Number of significant decimal places (0-28)
         * @details Examples:
         *          - Decimal("123.4500") returns 2 (trailing zeros ignored)
         *          - Decimal("123.000") returns 0 (integer value)
         *          - Decimal("0.001") returns 3
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] std::uint8_t decimalPlacesCount() const noexcept;

    private:
        //----------------------------------------------
        // Internal representation
        //----------------------------------------------

        /** @brief Internal storage layout for 128-bit decimal representation */
        struct Layout
        {
            /** @brief Scale (bits 16-23) + Sign (bit 31) */
            std::uint32_t flags;

            /** @brief 96-bit mantissa (3 x 32-bit) */
            std::array<std::uint32_t, 3> mantissa;
        } m_layout;
    };

    //=====================================================================
    // Free functions
    //=====================================================================

    /**
     * @brief Get absolute value of decimal (free function)
     * @param value Decimal to get absolute value of
     * @return Absolute value of the decimal
     * @details Free function wrapper for generic programming and ADL (Argument-Dependent Lookup).
     *          Enables usage in generic algorithms: `sqrt(value)` works via ADL.
     * @note This function is marked [[nodiscard]] - the return value should not be ignored
     */
    [[nodiscard]] inline Decimal abs( const Decimal& value ) noexcept
    {
        return value.abs();
    }

    /**
     * @brief Compute square root using Newton-Raphson method (free function)
     * @param value Decimal to compute square root of
     * @return Square root with full decimal precision (up to 28 digits)
     * @throws std::domain_error if called on negative value
     * @details Free function wrapper for generic programming and ADL (Argument-Dependent Lookup).
     *          Enables usage in generic algorithms: `sqrt(value)` works via ADL.
     * @note This function is marked [[nodiscard]] - the return value should not be ignored
     */
    [[nodiscard]] inline Decimal sqrt( const Decimal& value )
    {
        return value.sqrt();
    }

    /**
     * @brief Round up to nearest integer (free function)
     * @param value Decimal to ceiling
     * @return Smallest integer greater than or equal to value
     * @details Free function wrapper for generic programming and ADL (Argument-Dependent Lookup).
     * @note This function is marked [[nodiscard]] - the return value should not be ignored
     */
    [[nodiscard]] inline Decimal ceil( const Decimal& value ) noexcept
    {
        return value.ceil();
    }

    /**
     * @brief Round down to nearest integer (free function)
     * @param value Decimal to floor
     * @return Largest integer less than or equal to value
     * @details Free function wrapper for generic programming and ADL (Argument-Dependent Lookup).
     * @note This function is marked [[nodiscard]] - the return value should not be ignored
     */
    [[nodiscard]] inline Decimal floor( const Decimal& value ) noexcept
    {
        return value.floor();
    }

    /**
     * @brief Round decimal to specified precision (free function)
     * @param value Decimal value to round
     * @param decimalsPlacesCount Number of decimal places to round to (default: 0)
     * @param mode Rounding mode to apply (default: RoundingMode::ToNearest)
     * @return Decimal value rounded to the specified precision
     * @details Free function wrapper for generic programming and ADL (Argument-Dependent Lookup).
     *          Enables usage in generic algorithms.
     * @note This function is marked [[nodiscard]] - the return value should not be ignored
     */
    [[nodiscard]] inline Decimal round( const Decimal& value, std::int32_t decimalsPlacesCount = 0,
        Decimal::RoundingMode mode = Decimal::RoundingMode::ToNearest ) noexcept
    {
        return value.round( decimalsPlacesCount, mode );
    }

    /**
     * @brief Remove fractional part (free function)
     * @param value Decimal to truncate
     * @return Decimal with fractional part removed
     * @details Free function wrapper for generic programming and ADL (Argument-Dependent Lookup).
     * @note This function is marked [[nodiscard]] - the return value should not be ignored
     */
    [[nodiscard]] inline Decimal trunc( const Decimal& value ) noexcept
    {
        return value.trunc();
    }
} // namespace nfx::datatypes

#include "nfx/detail/datatypes/Decimal.inl"
