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
 * @file Int128.h
 * @brief Cross-platform 128-bit integer arithmetic type
 * @details Provides portable 128-bit signed integer operations with
 *          native __int128 on GCC/Clang and manual implementation on MSVC
 *
 *          Integration with Decimal:
 *          - Optimized for mantissa storage and manipulation in Decimal class
 *          - Efficient conversion between 96-bit decimal mantissa and 128-bit integer
 *
 *          Memory Layout and Internal Representation:
 *          =========================================
 *
 *          1. GCC/Clang with native __int128 support (NFX_DATATYPES_HAS_NATIVE_INT128=1):
 *          @code
 *          ┌─────────────────────────────────────────────────────────────────┐
 *          │                     Native __int128                             │
 *          │                        (16 bytes)                               │
 *          └─────────────────────────────────────────────────────────────────┘
 *          Bit 127                                                       Bit 0
 *          @endcode
 *
 *          2. MSVC and other compilers (NFX_DATATYPES_HAS_NATIVE_INT128=0):
 *          @code
 *          ┌─────────────────────────────────┬─────────────────────────────────┐
 *          │          upper64bits            │          lower64bits            │
 *          │       (most significant)        │       (least significant)       │
 *          │           8 bytes               │           8 bytes               │
 *          └─────────────────────────────────┴─────────────────────────────────┘
 *          Bit 127                     Bit 64 Bit 63                       Bit 0
 *          @endcode
 *
 *          Bit Layout and Sign Representation:
 *          ===================================
 *
 *          The 128-bit signed integer uses two's complement representation:
 *          @code
 *          ┌─┬───────────────────────────────────────────────────────────────────┐
 *          │S│                    Value Bits (127 bits)                          │
 *          └─┴───────────────────────────────────────────────────────────────────┘
 *          Bit 127                                                           Bit 0
 *          @endcode
 *
 *          Where:
 *          - S (bit 127): Sign bit (0 = positive, 1 = negative)
 *          - Bits 126-0: Magnitude in two's complement form
 *
 *          Value Range:
 *          - Minimum: -2^127 = -170,141,183,460,469,231,731,687,303,715,884,105,728
 *          - Maximum:  2^127-1 = 170,141,183,460,469,231,731,687,303,715,884,105,727
 *
 *          Examples with Memory Layout:
 *          ============================
 *
 *          Example 1: Small positive number (42)
 *          - Decimal: 42
 *          - Hex: 0x0000000000000000000000000000002A
 *          - Memory layout (little-endian on x86-64):
 *            * upper64bits: 0x0000000000000000
 *            * lower64bits: 0x000000000000002A
 *          - Bit pattern: 0000...00101010 (127 zeros followed by 101010)
 *
 *          Example 2: Large positive number (12,345,678,901,234,567,890)
 *          - Decimal: 12,345,678,901,234,567,890
 *          - Hex: 0x00000000000000000AB54A98CEB1F0D2
 *          - Memory layout:
 *            * upper64bits: 0x0000000000000000
 *            * lower64bits: 0x0AB54A98CEB1F0D2
 *          - Bit breakdown:
 *            * Bits 127-64: All zeros (positive number, high bits unused)
 *            * Bits 63-0: 0x0AB54A98CEB1F0D2 = 12,345,678,901,234,567,890
 *
 *          Example 3: Very large number requiring full 128 bits
 *          - Decimal: 123,456,789,012,345,678,901,234,567,890,123,456,789
 *          - Hex: 0x0173DC35270122E8EBC2CE4F3C95D6F5
 *          - Memory layout:
 *            * upper64bits: 0x0173DC35270122E8
 *            * lower64bits: 0xEBC2CE4F3C95D6F5
 *          - Bit breakdown:
 *            * Bits 127-64: 0x0173DC35270122E8 = 1,662,554,368,463,341,288
 *            * Bits 63-0:   0xEBC2CE4F3C95D6F5 = 17,034,473,836,310,554,357
 *            * Full value: (1,662,554,368,463,341,288 << 64) + 17,034,473,836,310,554,357
 *
 *          Example 4: Negative number (-42)
 *          - Decimal: -42
 *          - Two's complement hex: 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD6
 *          - Memory layout:
 *            * upper64bits: 0xFFFFFFFFFFFFFFFF
 *            * lower64bits: 0xFFFFFFFFFFFFFFD6
 *          - Bit pattern: 1111...11010110 (sign extension with two's complement)
 */

#pragma once

#include <array>
#include <compare>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

#include "nfx/detail/datatypes/CompilerSupport.h"

namespace nfx::datatypes
{
	class Decimal;

	//=====================================================================
	// Int128 class
	//=====================================================================

	/**
	 * @brief Cross-platform 128-bit signed integer type
	 * @details Implements portable 128-bit signed integer arithmetic with:
	 *          - Native __int128 support on GCC/Clang for maximum performance
	 *          - Manual implementation on MSVC using two 64-bit words
	 *          - Full arithmetic and comparison operator support
	 *          - Optimized for decimal arithmetic operations
	 *          - Cross-platform value consistency and API compatibility
	 */
	class Int128 final
	{
	public:
		//----------------------------------------------
		// Construction
		//----------------------------------------------

		/**
		 * @brief Default constructor
		 */
		inline constexpr Int128() noexcept;

		/**
		 * @brief Construct from signed 32-bit integer
		 * @param val 32-bit signed value
		 */
		inline explicit constexpr Int128( int val ) noexcept;

		/**
		 * @brief Construct from signed 64-bit integer
		 * @param val 64-bit signed value
		 */
		inline explicit constexpr Int128( std::int64_t val ) noexcept;

		/**
		 * @brief Construct from unsigned 32-bit integer
		 * @param val 32-bit unsigned value
		 */
		inline explicit constexpr Int128( std::uint32_t val ) noexcept;

		/**
		 * @brief Construct from unsigned 64-bit integer
		 * @param val 64-bit unsigned value
		 */
		inline explicit constexpr Int128( std::uint64_t val ) noexcept;

		/**
		 * @brief Construct from two 64-bit words
		 * @param low Lower 64 bits
		 * @param high Upper 64 bits
		 */
		constexpr Int128( std::uint64_t low, std::uint64_t high ) noexcept;

#if NFX_DATATYPES_HAS_NATIVE_INT128
		/**
		 * @brief Construct from native __int128 (GCC/Clang only)
		 * @param val Native 128-bit value
		 */
		explicit constexpr Int128( NFX_DATATYPES_NATIVE_INT128 val ) noexcept;
#endif

		/**
		 * @brief Construct from string (exact parsing)
		 * @param str String representation (e.g., "123", "-456789")
		 * @throws std::invalid_argument if string is not a valid integer
		 */
		inline explicit Int128( std::string_view str );

		/**
		 * @brief Construct from single-precision floating-point value
		 * @param val Float value to convert
		 * @details Follows C++ standard truncation behavior (like static_cast<int>(float)):
		 *          - Fractional parts are truncated toward zero (42.9f → 42, -42.9f → -42)
		 *          - NaN and infinity values are converted to zero
		 *          - Values exceeding Int128 range are clamped to Int128 limits (±2^127)
		 * @note This matches the behavior of static_cast<int>(float) for consistency
		 */
		explicit Int128( float val );

		/**
		 * @brief Construct from double-precision floating-point value
		 * @param val Double value to convert
		 * @details Follows C++ standard truncation behavior (like static_cast<int>(double)):
		 *          - Fractional parts are truncated toward zero (42.7 → 42, -42.7 → -42)
		 *          - NaN and infinity values are converted to zero
		 *          - Values exceeding Int128 range are clamped to Int128 limits (±2^127)
		 *          - Uses string conversion to preserve full double precision (53-bit mantissa)
		 * @note This matches the behavior of static_cast<int>(double) for consistency
		 */
		explicit Int128( double val );

		/**
		 * @brief Construct from Decimal value
		 * @param decimal Decimal value to convert
		 * @details Follows C++ standard truncation behavior (like static_cast<int>(double)):
		 *          - Fractional parts are truncated toward zero (42.7 → 42, -42.7 → -42)
		 *          - Integer part is extracted by dividing mantissa by 10^scale
		 *          - Zero values are handled efficiently
		 *          - Large values within Int128 range are supported
		 * @note This matches the behavior of static_cast<int>(double) for consistency,
		 *       rather than throwing exceptions for fractional parts
		 */
		explicit Int128( const Decimal& decimal );

		/**
		 * @brief Copy constructor
		 * @param other The Int128 object to copy from
		 */
		constexpr Int128( const Int128& other ) noexcept = default;

		/**
		 * @brief Move constructor
		 * @param other The Int128 object to move from
		 */
		constexpr Int128( Int128&& other ) noexcept = default;

		//----------------------------------------------
		// Destruction
		//----------------------------------------------

		/** @brief Destructor */
		~Int128() = default;

		//----------------------------------------------
		// Assignment
		//----------------------------------------------

		/**
		 * @brief Copy assignment operator
		 * @param other The Int128 object to copy from
		 * @return Reference to this Int128 object after assignment
		 */
		constexpr Int128& operator=( const Int128& other ) noexcept = default;

		/**
		 * @brief Move assignment operator
		 * @param other The Int128 object to move from
		 * @return Reference to this Int128 object after assignment
		 */
		constexpr Int128& operator=( Int128&& other ) noexcept = default;

		//----------------------------------------------
		// Comparison operators
		//----------------------------------------------

		/**
		 * @brief Three-way comparison operator (C++20)
		 * @param other Right operand to compare with
		 * @return std::strong_ordering indicating relative order (less, equal, or greater)
		 * @note Compiler auto-generates operator!=, <, <=, >, >= from this operator
		 */
		inline std::strong_ordering operator<=>( const Int128& other ) const noexcept;

		/**
		 * @brief Equality operator
		 * @param other Right operand
		 * @return true if values are equal
		 * @note Required alongside operator<=> for complete C++20 comparison support
		 */
		inline bool operator==( const Int128& other ) const noexcept;

		//----------------------------------------------
		// Comparison with built-in integer types
		//----------------------------------------------

		/**
		 * @brief Equality comparison with signed 32-bit integer
		 * @param val Right operand
		 * @return true if values are equal
		 */
		inline bool operator==( int val ) const noexcept;

		/**
		 * @brief Inequality comparison with signed 32-bit integer
		 * @param val Right operand
		 * @return true if values are not equal
		 */
		inline bool operator!=( int val ) const noexcept;

		/**
		 * @brief Less than comparison with signed 32-bit integer
		 * @param val Right operand
		 * @return true if this is less than val
		 */
		inline bool operator<( int val ) const noexcept;

		/**
		 * @brief Less than or equal comparison with signed 32-bit integer
		 * @param val Right operand
		 * @return true if this is less than or equal to val
		 */
		inline bool operator<=( int val ) const noexcept;

		/**
		 * @brief Greater than comparison with signed 32-bit integer
		 * @param val Right operand
		 * @return true if this is greater than val
		 */
		inline bool operator>( int val ) const noexcept;

		/**
		 * @brief Greater than or equal comparison with signed 32-bit integer
		 * @param val Right operand
		 * @return true if this is greater than or equal to val
		 */
		inline bool operator>=( int val ) const noexcept;

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
		bool operator!=( std::int64_t val ) const noexcept;

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

		//----------------------------------------------
		// Comparison with built-in floating point types
		//----------------------------------------------

		/**
		 * @brief Equality comparison with single-precision floating-point
		 * @param val Right operand
		 * @return true if values are equal (within floating-point precision)
		 */
		inline bool operator==( float val ) const noexcept;

		/**
		 * @brief Inequality comparison with single-precision floating-point
		 * @param val Right operand
		 * @return true if values are not equal
		 */
		inline bool operator!=( float val ) const noexcept;

		/**
		 * @brief Less than comparison with single-precision floating-point
		 * @param val Right operand
		 * @return true if this is less than val
		 */
		inline bool operator<( float val ) const noexcept;

		/**
		 * @brief Less than or equal comparison with single-precision floating-point
		 * @param val Right operand
		 * @return true if this is less than or equal to val
		 */
		inline bool operator<=( float val ) const noexcept;

		/**
		 * @brief Greater than comparison with single-precision floating-point
		 * @param val Right operand
		 * @return true if this is greater than val
		 */
		inline bool operator>( float val ) const noexcept;

		/**
		 * @brief Greater than or equal comparison with single-precision floating-point
		 * @param val Right operand
		 * @return true if this is greater than or equal to val
		 */
		inline bool operator>=( float val ) const noexcept;

		/**
		 * @brief Equality comparison with double-precision floating-point
		 * @param val Right operand
		 * @return true if values are equal (within floating-point precision)
		 */
		bool operator==( double val ) const noexcept;

		/**
		 * @brief Inequality comparison with double-precision floating-point
		 * @param val Right operand
		 * @return true if values are not equal
		 */
		inline bool operator!=( double val ) const noexcept;

		/**
		 * @brief Less than comparison with double-precision floating-point
		 * @param val Right operand
		 * @return true if this is less than val
		 */
		bool operator<( double val ) const noexcept;

		/**
		 * @brief Less than or equal comparison with double-precision floating-point
		 * @param val Right operand
		 * @return true if this is less than or equal to val
		 */
		inline bool operator<=( double val ) const noexcept;

		/**
		 * @brief Greater than comparison with double-precision floating-point
		 * @param val Right operand
		 * @return true if this is greater than val
		 */
		bool operator>( double val ) const noexcept;

		/**
		 * @brief Greater than or equal comparison with double-precision floating-point
		 * @param val Right operand
		 * @return true if this is greater than or equal to val
		 */
		inline bool operator>=( double val ) const noexcept;

		//----------------------------------------------
		// Comparison with nfx::datatypes::Decimal
		//----------------------------------------------

		/**
		 * @brief Equality comparison with Decimal
		 * @param val Right operand
		 * @return true if values are equal
		 * @note For equality, the Decimal must have no fractional part and represent the same integer value
		 */
		bool operator==( const Decimal& val ) const noexcept;

		/**
		 * @brief Inequality comparison with Decimal
		 * @param val Right operand
		 * @return true if values are not equal
		 */
		inline bool operator!=( const Decimal& val ) const noexcept;

		/**
		 * @brief Less than comparison with Decimal
		 * @param val Right operand
		 * @return true if this is less than val
		 */
		bool operator<( const Decimal& val ) const noexcept;

		/**
		 * @brief Less than or equal comparison with Decimal
		 * @param val Right operand
		 * @return true if this is less than or equal to val
		 */
		inline bool operator<=( const Decimal& val ) const noexcept;

		/**
		 * @brief Greater than comparison with Decimal
		 * @param val Right operand
		 * @return true if this is greater than val
		 */
		inline bool operator>( const Decimal& val ) const noexcept;

		/**
		 * @brief Greater than or equal comparison with Decimal
		 * @param val Right operand
		 * @return true if this is greater than or equal to val
		 */
		inline bool operator>=( const Decimal& val ) const noexcept;

		//----------------------------------------------
		// Arithmetic operators
		//----------------------------------------------

		/**
		 * @brief Addition operator
		 * @param other Right operand
		 * @return Sum of this and other
		 */
		inline Int128 operator+( const Int128& other ) const noexcept;

		/**
		 * @brief Subtraction operator
		 * @param other Right operand
		 * @return Difference of this and other
		 */
		inline Int128 operator-( const Int128& other ) const noexcept;

		/**
		 * @brief Multiplication operator
		 * @param other Right operand
		 * @return Product of this and other
		 */
		Int128 operator*( const Int128& other ) const noexcept;

		/**
		 * @brief Division operator
		 * @param other Divisor
		 * @return Result of division
		 * @throws std::overflow_error if divisor is zero
		 */
		Int128 operator/( const Int128& other ) const;

		/**
		 * @brief Addition assignment operator
		 * @param other Right operand to add
		 * @return Reference to this after addition
		 */
		inline Int128& operator+=( const Int128& other ) noexcept;

		/**
		 * @brief Subtraction assignment operator
		 * @param other Right operand to subtract
		 * @return Reference to this after subtraction
		 */
		inline Int128& operator-=( const Int128& other ) noexcept;

		/**
		 * @brief Multiplication assignment operator
		 * @param other Right operand to multiply by
		 * @return Reference to this after multiplication
		 */
		inline Int128& operator*=( const Int128& other ) noexcept;

		/**
		 * @brief Division assignment operator
		 * @param other Divisor
		 * @return Reference to this after division
		 * @throws std::overflow_error if divisor is zero
		 */
		inline Int128& operator/=( const Int128& other );

		/**
		 * @brief Modulo assignment operator
		 * @param other Divisor
		 * @return Reference to this after modulo operation
		 * @throws std::overflow_error if divisor is zero
		 */
		inline Int128& operator%=( const Int128& other );

		/**
		 * @brief Modulo operator
		 * @param other Divisor
		 * @return Remainder of division
		 * @throws std::overflow_error if divisor is zero
		 */
		inline Int128 operator%( const Int128& other ) const;

		/**
		 * @brief Unary minus operator
		 * @return Negated value
		 */
		inline Int128 operator-() const noexcept;

		//----------------------------------------------
		// Property accessors
		//----------------------------------------------

		/**
		 * @brief Get lower 64 bits
		 * @return Lower 64 bits as unsigned integer
		 * @note This function is marked [[nodiscard]] - the return value should not be ignored
		 */
		[[nodiscard]] std::uint64_t toLow() const noexcept;

		/**
		 * @brief Get upper 64 bits
		 * @return Upper 64 bits as unsigned integer
		 * @note This function is marked [[nodiscard]] - the return value should not be ignored
		 */
		[[nodiscard]] std::uint64_t toHigh() const noexcept;

#if NFX_DATATYPES_HAS_NATIVE_INT128
		/**
		 * @brief Get native 128-bit integer value (GCC/Clang only)
		 * @return Native __int128 value for direct use with compiler intrinsics
		 * @details This method provides access to the underlying native 128-bit integer
		 *          when compiled with GCC or Clang. Useful for interfacing with APIs
		 *          that expect native __int128 types or for maximum performance operations.
		 *
		 *          Performance benefits:
		 *          - Zero-cost conversion to native type
		 *          - Direct hardware instruction utilization
		 *          - Seamless integration with compiler intrinsics
		 *          - Optimal performance for bulk arithmetic operations
		 * @note This function is marked [[nodiscard]] - the return value should not be ignored
		 */
		NFX_DATATYPES_NATIVE_INT128 toNative() const noexcept;
#endif

		//----------------------------------------------
		// Mathematical operations
		//----------------------------------------------

		/**
		 * @brief Get absolute value
		 * @return Absolute value of the integer
		 * @note This function is marked [[nodiscard]] - the return value should not be ignored
		 */
		[[nodiscard]] inline Int128 abs() const noexcept;

		/**
		 * @brief Calculate integer square root (floor of exact square root)
		 * @return Largest integer i such that i*i <= this
		 * @throws std::domain_error if this is negative
		 * @note Returns floor(sqrt(this)) - always rounds down
		 * @note For exact precision, convert to Decimal and use Decimal::sqrt()
		 * @note This function is marked [[nodiscard]] - the return value should not be ignored
		 */
		[[nodiscard]] Int128 isqrt() const;

		//----------------------------------------------
		// String parsing
		//----------------------------------------------

		/**
		 * @brief Try to parse 128-bit integer from string without throwing
		 * @param str String representation of integer (e.g., "12345", "-9876543210")
		 * @param result Output parameter to store parsed value on success
		 * @return true if parsing succeeded, false if string format is invalid or value is outside Int128 range
		 * @details Parses string representations of 128-bit signed integers with comprehensive format support.
		 *          Supported input formats:
		 *          - Positive integers: "123", "9876543210"
		 *          - Negative integers: "-123", "-9876543210"
		 *          - Very large integers: "170141183460469231731687303715884105727"
		 *          - Leading/trailing whitespace is not supported (will cause parsing failure)
		 *          - Scientific notation is not supported
		 *          - Only decimal base is supported (no hex, octal, or binary)
		 * @note This function is marked [[nodiscard]] - the return value should not be ignored
		 * @note Range: -2^127 to 2^127-1 (±170,141,183,460,469,231,731,687,303,715,884,105,72x)
		 * @note For convenience wrapper with std::optional return, use the single-parameter overload
		 */
		[[nodiscard]] static bool fromString( std::string_view str, Int128& result ) noexcept;

		/**
		 * @brief Parse 128-bit integer from string with optional return
		 * @param str String representation of integer (e.g., "12345", "-9876543210")
		 * @return std::optional<Int128> containing parsed value if successful, std::nullopt if parsing failed
		 * @details This is a convenience wrapper around the two-parameter fromString method.
		 *          Supports the same input formats:
		 *          - Positive integers: "123", "9876543210"
		 *          - Negative integers: "-123", "-9876543210"
		 *          - Very large integers: "170141183460469231731687303715884105727"
		 *          - Leading/trailing whitespace is not supported (will cause parsing failure)
		 *          - Scientific notation is not supported
		 *          - Only decimal base is supported (no hex, octal, or binary)
		 * @note This function is marked [[nodiscard]] - the return value should not be ignored
		 * @note For error details, use the two-parameter overload with explicit error handling
		 * @note Range: -2^127 to 2^127-1 (±170,141,183,460,469,231,731,687,303,715,884,105,72x)
		 */
		[[nodiscard]] static std::optional<Int128> fromString( std::string_view str ) noexcept;

		//----------------------------------------------
		// Type conversion
		//----------------------------------------------

		/**
		 * @brief Convert to string with exact precision
		 * @return String representation
		 * @note This function is marked [[nodiscard]] - the return value should not be ignored
		 */
		[[nodiscard]] std::string toString() const;

		/**
		 * @brief Get internal 32-bit representation
		 * @return Array of 4 32-bit integers representing the 128-bit integer
		 * @note This function is marked [[nodiscard]] - the return value should not be ignored
		 */
		[[nodiscard]] std::array<std::int32_t, 4> toBits() const noexcept;

	private:
		//----------------------------------------------
		// Internal representation
		//----------------------------------------------

#if NFX_DATATYPES_HAS_NATIVE_INT128
		/** @brief Native 128-bit integer storage (GCC/Clang) */
		NFX_DATATYPES_NATIVE_INT128 m_value;
#else
		struct Layout
		{
			/** @brief Manual 128-bit storage using two 64-bit words (MSVC) */
			std::uint64_t lower64bits;
			std::uint64_t upper64bits;
		} m_layout;
#endif
	};

	//=====================================================================
	// Free functions
	//=====================================================================

	/**
	 * @brief Get absolute value of Int128 (free function)
	 * @param value Int128 to get absolute value of
	 * @return Absolute value of the integer
	 * @details Free function wrapper for generic programming and ADL (Argument-Dependent Lookup).
	 *          Enables usage in generic algorithms: `abs(value)` works via ADL.
	 * @note This function is marked [[nodiscard]] - the return value should not be ignored
	 */
	[[nodiscard]] inline Int128 abs( const Int128& value ) noexcept
	{
		return value.abs();
	}

	/**
	 * @brief Calculate integer square root (free function)
	 * @param value Int128 to compute square root of
	 * @return Largest integer i such that i*i <= value
	 * @throws std::domain_error if value is negative
	 * @details Free function wrapper for generic programming and ADL (Argument-Dependent Lookup).
	 *          Returns floor(sqrt(value)) - always rounds down.
	 * @note This function is marked [[nodiscard]] - the return value should not be ignored
	 */
	[[nodiscard]] inline Int128 isqrt( const Int128& value )
	{
		return value.isqrt();
	}

	/**
	 * @brief Calculate square root of Int128 value with high precision
	 * @param value Integer value to compute square root of
	 * @return Decimal value representing the square root with up to 28 digits of precision
	 * @throws std::domain_error if value is negative
	 * @details Converts Int128 to Decimal and computes high-precision square root using Newton-Raphson method.
	 *          This provides exact decimal precision (up to 28 digits) compared to isqrt() which returns floor(sqrt).
	 *
	 *          Examples:
	 *          - sqrt(Int128{4}) → Decimal{"2.0"} (exact)
	 *          - sqrt(Int128{2}) → Decimal{"1.4142135623730950488..."} (28-digit precision)
	 *          - sqrt(Int128{100}) → Decimal{"10.0"} (exact)
	 *
	 * @note This function is marked [[nodiscard]] - the return value should not be ignored
	 * @note For integer-only results, use Int128::isqrt() or isqrt(Int128) instead
	 * @see Int128::isqrt() for integer square root (floor)
	 * @see isqrt(Int128) for free function variant
	 * @see Decimal::sqrt() for the underlying high-precision implementation
	 */
	[[nodiscard]] Decimal sqrt( const Int128& value );
} // namespace nfx::datatypes

#include "nfx/detail/datatypes/Int128.inl"
