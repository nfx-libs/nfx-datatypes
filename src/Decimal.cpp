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
 * @file Decimal.cpp
 * @brief Implementation of cross-platform 128-bit decimal arithmetic
 * @details Provides exact decimal arithmetic with portable 128-bit operations
 */

#include <charconv>
#include <cmath>
#include <iomanip>
#include <istream>
#include <ostream>
#include <sstream>

#include "nfx/datatypes/Decimal.h"

#include "nfx/datatypes/Int128.h"
#include "nfx/detail/datatypes/Constants.h"

namespace nfx::datatypes
{
	namespace internal
	{
		//=====================================================================
		// Internal helper functions
		//=====================================================================

		/**
		 * @brief Multiply a 96-bit mantissa by 10 and add a digit (in-place)
		 * @param mantissa Pointer to 3-element mantissa array
		 * @param digit Digit to add (0-9)
		 */
		static void multiplyMantissaBy10AndAdd( std::uint32_t* mantissa, std::uint32_t digit ) noexcept
		{
			// Multiply 96-bit number by 10 using: result = mantissa * 8 + mantissa * 2
			std::uint64_t carry = 0;

			// Multiply by 10 (shift left 3 bits + shift left 1 bit = multiply by 8 + 2 = 10)
			for ( int i = 0; i < constants::DECIMAL_MANTISSA_ARRAY_SIZE; ++i )
			{
				std::uint64_t temp = static_cast<std::uint64_t>( mantissa[i] ) * constants::DECIMAL_BASE + carry;
				mantissa[i] = static_cast<std::uint32_t>( temp );
				carry = temp >> constants::BITS_PER_UINT32;
			}

			// Add the digit
			carry = digit;
			for ( int i = 0; i < constants::DECIMAL_MANTISSA_ARRAY_SIZE && carry > 0; ++i )
			{
				std::uint64_t temp = static_cast<std::uint64_t>( mantissa[i] ) + carry;
				mantissa[i] = static_cast<std::uint32_t>( temp );
				carry = temp >> constants::BITS_PER_UINT32;
			}
		}

		/**
		 * @brief Get power of 10 as Int128 for any scale 0-28
		 * @param power The power (0-28)
		 * @return Int128 representing 10^power
		 */
		static Int128 powerOf10( std::uint8_t power ) noexcept
		{
			if ( power < constants::DECIMAL_POWER_TABLE_SIZE && constants::DECIMAL_POWERS_OF_10[power] != 0 )
			{
				// Use 64-bit lookup table for powers 0-19
				return Int128{ constants::DECIMAL_POWERS_OF_10[power] };
			}
			else if ( power >= constants::DECIMAL_EXTENDED_POWER_MIN && power <= constants::DECIMAL_EXTENDED_POWER_MAX )
			{
				// Use pre-computed 128-bit values for powers 20-28
				const auto& extended{ constants::DECIMAL_EXTENDED_POWERS_OF_10[power - constants::DECIMAL_EXTENDED_POWER_MIN] };
				return Int128{ extended.first, extended.second };
			}
			else
			{
				// Fallback to iterative computation for invalid powers (shouldn't happen)
				Int128 result{ 1 };
				for ( std::uint8_t i{ 0 }; i < power; ++i )
				{
					result = result * Int128{ constants::DECIMAL_BASE };
				}
				return result;
			}
		}

		/**
		 * @brief Extract 128-bit mantissa value from Decimal
		 * @param decimal The decimal value to extract mantissa from
		 * @return Int128 representation of the mantissa
		 */
		static Int128 mantissaAsInt128( const Decimal& decimal ) noexcept
		{
#if NFX_DATATYPES_HAS_NATIVE_INT128
			const auto& mantissaArray{ decimal.mantissa() };
			NFX_DATATYPES_NATIVE_INT128 value{ static_cast<NFX_DATATYPES_NATIVE_INT128>( mantissaArray[2] ) << constants::BITS_PER_UINT64 |
											   static_cast<NFX_DATATYPES_NATIVE_INT128>( mantissaArray[1] ) << constants::BITS_PER_UINT32 |
											   static_cast<NFX_DATATYPES_NATIVE_INT128>( mantissaArray[0] ) };

			return Int128{ value };
#else
			const auto& mantissaArray{ decimal.mantissa() };
			std::uint64_t low{ static_cast<std::uint64_t>( mantissaArray[1] ) << constants::BITS_PER_UINT32 | mantissaArray[0] };
			std::uint64_t high{ mantissaArray[2] };

			return Int128{ low, high };
#endif
		}

		/**
		 * @brief Align scales of two decimals for arithmetic operations
		 * @param decimal First decimal value
		 * @param other Second decimal value
		 * @return Pair of Int128 mantissas with aligned scales
		 */
		static std::pair<Int128, Int128> alignScale( const Decimal& decimal, const Decimal& other )
		{
			Int128 left{ mantissaAsInt128( decimal ) };
			Int128 right{ mantissaAsInt128( other ) };

			std::uint8_t leftScale{ decimal.scale() };
			std::uint8_t rightScale{ other.scale() };

			// Optimized scaling using enhanced power-of-10 lookup with 128-bit support
			if ( leftScale < rightScale )
			{
				std::uint8_t scaleDiff{ static_cast<std::uint8_t>( rightScale - leftScale ) };
				left = left * powerOf10( scaleDiff );
			}
			else if ( rightScale < leftScale )
			{
				std::uint8_t scaleDiff{ static_cast<std::uint8_t>( leftScale - rightScale ) };
				right = right * powerOf10( scaleDiff );
			}

			return { std::move( left ), std::move( right ) };
		}

		/**
		 * @brief Set mantissa value in Decimal from Int128
		 * @param decimal The decimal to modify
		 * @param value The Int128 mantissa value to set
		 */
		static void setMantissa( Decimal& decimal, const Int128& value ) noexcept
		{
#if NFX_DATATYPES_HAS_NATIVE_INT128
			auto nativeValue{ value.toNative() };
			auto& mantissa{ decimal.mantissa() };
			mantissa[0] = static_cast<std::uint32_t>( nativeValue );
			mantissa[1] = static_cast<std::uint32_t>( nativeValue >> constants::BITS_PER_UINT32 );
			mantissa[2] = static_cast<std::uint32_t>( nativeValue >> constants::BITS_PER_UINT64 );
#else
			auto& mantissa{ decimal.mantissa() };
			std::uint64_t low{ value.toLow() };
			std::uint64_t high{ value.toHigh() };

			mantissa[0] = static_cast<std::uint32_t>( low );
			mantissa[1] = static_cast<std::uint32_t>( low >> constants::BITS_PER_UINT32 );
			mantissa[2] = static_cast<std::uint32_t>( high );
#endif
		}

		/**
		 * @brief Divide decimal mantissa by power of 10
		 * @param decimal The decimal to modify
		 * @param power The power of 10 to divide by (0-28)
		 */
		static void divideByPowerOf10( Decimal& decimal, std::uint8_t power )
		{
			Int128 mantissa{ mantissaAsInt128( decimal ) };

			// Use enhanced power-of-10 lookup with full 128-bit support
			mantissa = mantissa / powerOf10( power );

			setMantissa( decimal, mantissa );
		}

		/**
		 * @brief Normalize decimal by removing trailing zeros and reducing scale
		 * @param decimal The decimal to normalize
		 */
		static void normalize( Decimal& decimal ) noexcept
		{
			// Remove trailing zeros and reduce scale
			while ( decimal.scale() > 0 && ( mantissaAsInt128( decimal ) % Int128{ constants::DECIMAL_BASE } ) == Int128{ 0 } )
			{
				divideByPowerOf10( decimal, 1U );
				std::uint8_t currentScale{ decimal.scale() };
				std::uint32_t currentFlags = decimal.flags();
				std::uint32_t newFlags = ( currentFlags & ~constants::DECIMAL_SCALE_MASK ) |
										 ( static_cast<std::uint32_t>( currentScale - 1U )
											 << constants::DECIMAL_SCALE_SHIFT );
				decimal.flags() = newFlags;
			}
		}

		/**
		 * @brief Determine if rounding up is needed for ToNearest mode (Banker's rounding)
		 */

		static bool shouldRoundUpToNearest( const Int128& roundingDigit, const Int128& mantissa,
			const Int128& divisor, std::uint8_t digitsToRemove,
			const Decimal& result ) noexcept
		{
			if ( roundingDigit.toLow() > constants::DECIMAL_ROUNDING_THRESHOLD )
			{
				return true; // > 5: always round away from zero
			}
			else if ( roundingDigit.toLow() == constants::DECIMAL_ROUNDING_THRESHOLD )
			{
				// == 5: check if there are any non-zero digits after this one
				bool hasRemainingFraction{ false };
				if ( digitsToRemove > 1U )
				{
					Int128 remainderDivisor{ divisor };
					Int128 remainder{ mantissa % remainderDivisor };
					Int128 roundingDigitContribution{ roundingDigit * ( divisor / Int128{ constants::DECIMAL_BASE } ) };
					hasRemainingFraction = ( remainder != roundingDigitContribution );
				}

				if ( hasRemainingFraction )
				{
					return true; // Ties away from zero when there's additional fractional part
				}
				else
				{
					// Exact tie: round to even
					Int128 resultMantissa{ mantissaAsInt128( result ) };
					bool isEven{ ( resultMantissa % Int128{ 2 } ) == Int128{ 0 } };
					return !isEven; // Round up if currently odd
				}
			}
			return false;
		}

		/**
		 * @brief Determine if rounding up is needed for ToNearestTiesAway mode
		 */
		static bool shouldRoundUpToNearestTiesAway( const Int128& roundingDigit ) noexcept
		{
			return ( roundingDigit.toLow() >= constants::DECIMAL_ROUNDING_THRESHOLD );
		}

		/**
		 * @brief Determine if rounding up is needed for ToPositiveInfinity mode (Ceiling)
		 */
		static bool shouldRoundUpToPositiveInfinity( const Int128& mantissa, std::uint8_t digitsToRemove,
			bool isNegative ) noexcept
		{
			if ( isNegative )
			{
				return false; // Negative numbers round toward zero for ceiling
			}

			// Check if ANY fractional digits exist
			if ( digitsToRemove > 0 )
			{
				Int128 fractionalDivisor{ powerOf10( digitsToRemove ) };
				Int128 fractionalPart{ mantissa % fractionalDivisor };
				return fractionalPart != Int128{};
			}
			return false;
		}

		/**
		 * @brief Determine if rounding up is needed for ToNegativeInfinity mode (Floor)
		 */
		static bool shouldRoundUpToNegativeInfinity( const Int128& mantissa, std::uint8_t digitsToRemove,
			bool isNegative ) noexcept
		{
			if ( !isNegative )
			{
				return false; // Positive numbers round toward zero for floor
			}

			// Check if ANY fractional digits exist
			if ( digitsToRemove > 0 )
			{
				Int128 fractionalDivisor{ powerOf10( digitsToRemove ) };
				Int128 fractionalPart{ mantissa % fractionalDivisor };
				return fractionalPart != Int128{};
			}
			return false;
		}
	} // namespace internal

	//=====================================================================
	// Decimal class
	//=====================================================================

	//----------------------------------------------
	// Construction
	//----------------------------------------------

	Decimal::Decimal( float value ) noexcept
		: m_layout{ 0, { { 0, 0, 0 } } }
	{
		if ( std::isnan( value ) || std::isinf( value ) || value == 0.0f )
		{
			return;
		}

		char buffer[64];

		float absValue = value < 0 ? -value : value;
		int precision = ( absValue > 0 && absValue < 1e-6f ) ? 10 : 4;

		auto [ptr, ec] = std::to_chars( buffer, buffer + sizeof( buffer ), value,
			std::chars_format::fixed, precision );

		if ( ec != std::errc{} )
		{
			return; // Zero on error
		}

		// Parse the string directly (inline, avoiding fromString overhead)
		const char* p = buffer;
		const char* end = ptr;

		// Handle sign
		bool negative = false;
		if ( *p == '-' )
		{
			negative = true;
			++p;
		}
		else if ( *p == '+' )
		{
			++p;
		}

		// Parse digits and track decimal point
		const char* decimalPoint = nullptr;
		while ( p < end )
		{
			if ( *p == '.' )
			{
				decimalPoint = p;
				++p;
				continue;
			}

			if ( *p >= '0' && *p <= '9' )
			{
				internal::multiplyMantissaBy10AndAdd( m_layout.mantissa.data(),
					static_cast<std::uint32_t>( *p - '0' ) );
			}
			++p;
		}

		// Calculate scale (digits after decimal point)
		std::uint8_t scale = 0;
		if ( decimalPoint )
		{
			scale = static_cast<std::uint8_t>( end - decimalPoint - 1 );

			// Remove trailing zeros from the string representation
			while ( scale > 0 && buffer[end - p - 1] == '0' )
			{
				--scale;
				--end;
			}
		}

		m_layout.flags = ( scale << constants::DECIMAL_SCALE_SHIFT );
		if ( negative )
		{
			m_layout.flags |= constants::DECIMAL_SIGN_MASK;
		}

		internal::normalize( *this );
	}

	Decimal::Decimal( double value ) noexcept
		: m_layout{ 0, { { 0, 0, 0 } } }
	{
		if ( std::isnan( value ) || std::isinf( value ) || value == 0.0 )
		{
			return;
		}

		// Convert double to string using std::to_chars.
		// Use fixed format to avoid scientific notation which would require
		// additional parsing logic. This ensures clean decimal representation
		// while avoiding binary representation artifacts
		char buffer[64];

		auto [ptr, ec] = std::to_chars( buffer, buffer + sizeof( buffer ), value,
			std::chars_format::fixed );

		if ( ec != std::errc{} )
		{
			return; // Zero on error
		}

		// Parse the string directly (inline, avoiding fromString overhead)
		const char* p = buffer;
		const char* end = ptr;

		// Handle sign
		bool negative = false;
		if ( *p == '-' )
		{
			negative = true;
			++p;
		}
		else if ( *p == '+' )
		{
			++p;
		}

		// Parse digits and track decimal point
		const char* decimalPoint = nullptr;
		while ( p < end )
		{
			if ( *p == '.' )
			{
				decimalPoint = p;
				++p;
				continue;
			}

			if ( *p >= '0' && *p <= '9' )
			{
				internal::multiplyMantissaBy10AndAdd( m_layout.mantissa.data(),
					static_cast<std::uint32_t>( *p - '0' ) );
			}
			++p;
		}

		// Calculate scale (digits after decimal point)
		std::uint8_t scale = 0;
		if ( decimalPoint )
		{
			scale = static_cast<std::uint8_t>( end - decimalPoint - 1 );
		}

		m_layout.flags = ( scale << constants::DECIMAL_SCALE_SHIFT );
		if ( negative )
		{
			m_layout.flags |= constants::DECIMAL_SIGN_MASK;
		}

		internal::normalize( *this );
	}

	Decimal::Decimal( const Int128& val )
		: m_layout{ 0, { { 0, 0, 0 } } }
	{
		if ( val == Int128{} )
		{
			return;
		}

		// Extract sign and get absolute value
		bool isNegative = val < Int128{};

		// Handle the special case of minimum Int128 value (-2^127)
		// This value cannot be represented positively in 128-bit signed integer
		Int128 absoluteValue;
		if ( val == std::numeric_limits<Int128>::min() )
		{
			// Minimum Int128 value exceeds Decimal's 96-bit capacity
			throw std::overflow_error( "Int128 value exceeds Decimal range (±79228162514264337593543950335)" );
		}

		absoluteValue = val.abs();
		if ( isNegative )
		{
			m_layout.flags |= constants::DECIMAL_SIGN_MASK;
		}

		// Check if the absolute value fits in Decimal's 96-bit mantissa capacity
		// Maximum 96-bit unsigned value: 2^96 - 1 = 79,228,162,514,264,337,593,543,950,335
		// This is much smaller than Int128 max value: 2^127 - 1 = 170,141,183,460,469,231,731,687,303,715,884,105,727

		// Check if the high 64 bits contain anything beyond what fits in 32 bits (mantissa[2])
		std::uint64_t high64 = absoluteValue.toHigh();
		if ( high64 > constants::UINT32_MAX_VALUE )
		{
			// Value exceeds Decimal's 96-bit mantissa capacity
			throw std::overflow_error( "Int128 value exceeds Decimal range (±79228162514264337593543950335)" );
		}

		// Value fits in 96 bits - store it directly
		internal::setMantissa( *this, absoluteValue );
	}

	//----------------------------------------------
	// Comparison operators
	//----------------------------------------------

	std::strong_ordering Decimal::operator<=>( const Decimal& other ) const noexcept
	{
		// Compare signs first by checking flags directly (avoid recursion)
		bool thisNeg = ( m_layout.flags & constants::DECIMAL_SIGN_MASK ) != 0;
		bool otherNeg = ( other.m_layout.flags & constants::DECIMAL_SIGN_MASK ) != 0;

		if ( thisNeg != otherNeg )
		{
			// Different signs: negative < positive
			return thisNeg ? std::strong_ordering::less : std::strong_ordering::greater;
		}

		// Same sign: compare aligned mantissas
		auto [leftMantissa, rightMantissa] = internal::alignScale( *this, other );

		// For both negative, reverse the comparison
		if ( thisNeg )
		{
			if ( leftMantissa > rightMantissa )
			{
				return std::strong_ordering::less;
			}
			if ( leftMantissa < rightMantissa )
			{
				return std::strong_ordering::greater;
			}
			return std::strong_ordering::equal;
		}

		// Both positive or both zero
		if ( leftMantissa < rightMantissa )
		{
			return std::strong_ordering::less;
		}
		if ( leftMantissa > rightMantissa )
		{
			return std::strong_ordering::greater;
		}
		return std::strong_ordering::equal;
	}

	bool Decimal::operator==( const Decimal& other ) const noexcept
	{
		// Check both zero by inspecting mantissa directly
		bool thisZero = m_layout.mantissa[0] == 0 && m_layout.mantissa[1] == 0 && m_layout.mantissa[2] == 0;
		bool otherZero = other.m_layout.mantissa[0] == 0 && other.m_layout.mantissa[1] == 0 && other.m_layout.mantissa[2] == 0;

		if ( thisZero && otherZero )
		{
			return true;
		}

		// Check sign mismatch by inspecting flags directly
		bool thisNeg = ( m_layout.flags & constants::DECIMAL_SIGN_MASK ) != 0;
		bool otherNeg = ( other.m_layout.flags & constants::DECIMAL_SIGN_MASK ) != 0;

		if ( thisNeg != otherNeg )
		{
			return false;
		}

		auto [left, right] = internal::alignScale( *this, other );

		return left == right;
	}

	//----------------------------------------------
	// Comparison with nfx::datatypes::Int128
	//----------------------------------------------

	bool Decimal::operator==( const Int128& val ) const noexcept
	{
		// For integer comparison, we need exact equality
		if ( scale() > 0 )
		{
			// If this has fractional part, it can't equal an integer
			return false;
		}

		// Convert this decimal's mantissa to Int128 and compare directly
		Int128 mantissa{ internal::mantissaAsInt128( *this ) };

		// Handle signs
		if ( *this < Decimal{} )
		{
			if ( val >= Int128{ 0 } )
			{
				return false; // Different signs
			}
			// Both negative - compare absolute values (negate mantissa for comparison)
			return mantissa == -val;
		}
		else
		{
			if ( val < Int128{ 0 } )
			{
				return false; // Different signs
			}
			// Both positive
			return mantissa == val;
		}
	}

	bool Decimal::operator<( const Int128& val ) const noexcept
	{
		// Handle different signs
		if ( *this < Decimal{} && val >= Int128{ 0 } )
		{
			return true; // Negative < Non-negative
		}
		if ( !( *this < Decimal{} ) && val < Int128{ 0 } )
		{
			return false; // Non-negative > Negative
		}

		// Same signs - convert decimal to comparable form
		Int128 mantissa{ internal::mantissaAsInt128( *this ) };

		if ( scale() > 0 )
		{
			// This decimal has fractional part - scale up the integer for comparison
			Int128 scaledVal{ val * internal::powerOf10( scale() ) };

			if ( *this < Decimal{} )
			{
				// Both negative - compare absolute values with flipped result
				return mantissa > scaledVal.abs();
			}
			else
			{
				return mantissa < scaledVal;
			}
		}
		else
		{
			// No fractional part - direct comparison
			if ( *this < Decimal{} )
			{
				// Both negative - compare absolute values with flipped result
				return mantissa > val.abs();
			}
			else
			{
				return mantissa < val;
			}
		}
	}

	//----------------------------------------------
	// Arithmetic operators
	//----------------------------------------------

	Decimal Decimal::operator+( const Decimal& other )
	{
		if ( *this == Decimal{} )
		{
			return other;
		}
		if ( other == Decimal{} )
		{
			return *this;
		}

		Decimal result;
		auto [left, right]{ internal::alignScale( *this, other ) };

		internal::setMantissa( result, left + right );
		result.m_layout.flags = ( m_layout.flags & ~constants::DECIMAL_SCALE_MASK ) |
								( std::max( scale(), other.scale() ) << constants::DECIMAL_SCALE_SHIFT );

		// Handle sign
		if ( ( *this < Decimal{} ) == ( other < Decimal{} ) )
		{
			if ( *this < Decimal{} )
			{
				result.m_layout.flags |= constants::DECIMAL_SIGN_MASK;
			}
		}
		else
		{
			// Different signs - need subtraction logic
			if ( left > right )
			{
				internal::setMantissa( result, left - right );
				if ( *this < Decimal{} )
				{
					result.m_layout.flags |= constants::DECIMAL_SIGN_MASK;
				}
			}
			else
			{
				internal::setMantissa( result, right - left );
				if ( other < Decimal{} )
				{
					result.m_layout.flags |= constants::DECIMAL_SIGN_MASK;
				}
			}
		}

		internal::normalize( result );

		return result;
	}

	Decimal Decimal::operator*( const Decimal& other ) const
	{
		if ( *this == Decimal{} || other == Decimal{} )
		{
			return Decimal{};
		}

		Decimal result;
		Int128 left{ internal::mantissaAsInt128( *this ) };
		Int128 right{ internal::mantissaAsInt128( other ) };

		// Combine scales
		std::uint8_t newScale{ static_cast<std::uint8_t>( scale() + other.scale() ) };

		// If combined scale would exceed maximum, pre-scale operands to prevent Int128 overflow
		// Example: pi(scale=28) × e(scale=28) = scale 56, we need to reduce by 28
		if ( newScale > constants::DECIMAL_MAXIMUM_PLACES )
		{
			std::uint8_t excessScale = newScale - constants::DECIMAL_MAXIMUM_PLACES;

			// Divide the excess scale evenly between operands to minimize precision loss
			std::uint8_t leftReduction = excessScale / 2;
			std::uint8_t rightReduction = excessScale - leftReduction;

			if ( leftReduction > 0 )
			{
				Int128 leftDivisor = internal::powerOf10( leftReduction );
				Int128 leftHalf = leftDivisor / Int128{ 2 };
				left = ( left + leftHalf ) / leftDivisor;
			}

			if ( rightReduction > 0 )
			{
				Int128 rightDivisor = internal::powerOf10( rightReduction );
				Int128 rightHalf = rightDivisor / Int128{ 2 };
				right = ( right + rightHalf ) / rightDivisor;
			}

			newScale = constants::DECIMAL_MAXIMUM_PLACES;
		} // Calculate the product mantissa (now safe from overflow)
		Int128 productMantissa{ left * right };

		// Check if the mantissa fits in 96 bits (max value: 2^96 - 1)
		const Int128 max96bit{ constants::DECIMAL_96BIT_MAX_LOW, constants::DECIMAL_96BIT_MAX_HIGH };

		// If mantissa still exceeds 96 bits, reduce precision iteratively
		while ( productMantissa > max96bit && newScale > 0 )
		{
			// Use rounding when dividing to minimize precision loss
			productMantissa = ( productMantissa + Int128{ 5 } ) / Int128{ constants::DECIMAL_BASE };
			newScale--;
		}

		// Safety check
		if ( productMantissa > max96bit )
		{
			// Number is too large for Decimal representation even at scale 0
			// Keep the most significant digits
			while ( productMantissa > max96bit )
			{
				productMantissa = productMantissa / Int128{ constants::DECIMAL_BASE };
			}
		}

		// Now store the properly scaled mantissa
		internal::setMantissa( result, productMantissa );

		result.m_layout.flags = ( static_cast<std::uint32_t>( newScale ) << constants::DECIMAL_SCALE_SHIFT );

		// Combine signs
		if ( ( *this < Decimal{} ) != ( other < Decimal{} ) )
		{
			result.m_layout.flags |= constants::DECIMAL_SIGN_MASK;
		}

		internal::normalize( result );

		return result;
	}

#ifdef __clang__
	__attribute__( ( optnone ) ) // Clang aggressively optimizes the mantissa/scale setting operations causing loss of decimal precision.
#endif
	Decimal
	Decimal::operator/( const Decimal& other ) const
	{
		if ( other == Decimal{} )
		{
			throw std::overflow_error{ "Division by zero" };
		}

		if ( *this == Decimal{} )
		{
			return Decimal{};
		}

		Decimal result;
		Int128 dividend{ internal::mantissaAsInt128( *this ) };
		Int128 divisor{ internal::mantissaAsInt128( other ) };

		// Scale adjustment for division:
		// If dividend has scale d and divisor has scale s,
		// we want result with enough precision
		//
		// The mathematical identity is:
		// (dividend / 10^d) / (divisor / 10^s) = (dividend * 10^s) / (divisor * 10^d)
		//
		// To get precision p in the result, we compute:
		// (dividend * 10^(s+p)) / divisor, with result scale = d + p

		std::int32_t dividendScale = static_cast<std::int32_t>( scale() );
		std::int32_t divisorScale = static_cast<std::int32_t>( other.scale() );

		// Target precision: match the maximum input precision plus extra
		std::int32_t targetPrecision = std::max( dividendScale, divisorScale ) + constants::DECIMAL_DIVISION_EXTRA_PRECISION;

		// Limit to maximum Decimal places
		if ( targetPrecision > constants::DECIMAL_MAXIMUM_PLACES )
		{
			targetPrecision = constants::DECIMAL_MAXIMUM_PLACES;
		}

		// Scale up dividend to achieve target precision
		// To get result with scale result_scale, we need:
		// result_mantissa / 10^result_scale = (dividend_mantissa / 10^dividend_scale) / (divisor_mantissa / 10^divisor_scale)
		// Which means: result_mantissa = (dividend_mantissa * 10^(divisor_scale + result_scale - dividend_scale)) / divisor_mantissa
		std::int32_t scaleUpBy = divisorScale + targetPrecision - dividendScale;

		if ( scaleUpBy > 0 )
		{
			for ( std::int32_t i = 0; i < scaleUpBy; ++i )
			{
				// Check if scaling would cause overflow BEFORE we multiply
				if ( dividend.toHigh() > constants::INT128_MUL10_OVERFLOW_THRESHOLD )
				{
					// Can't scale more without overflow
					targetPrecision = i + dividendScale - divisorScale;
					if ( targetPrecision < 0 )
					{
						targetPrecision = 0;
					}
					break;
				}
				Int128 newDividend = dividend * Int128{ constants::DECIMAL_BASE };
				// Double-check we didn't overflow (result should be positive and bigger)
				if ( newDividend < dividend )
				{
					targetPrecision = i + dividendScale - divisorScale;
					if ( targetPrecision < 0 )
					{
						targetPrecision = 0;
					}
					break;
				}
				dividend = newDividend;
			}
		}
		else if ( scaleUpBy < 0 )
		{
			// Need to scale down - shouldn't happen with our target precision logic
			// Just use the dividend as-is
			targetPrecision = dividendScale - divisorScale;
			if ( targetPrecision < 0 )
			{
				targetPrecision = 0;
			}
		}

		Int128 quotientMantissa = dividend / divisor;

		// Check if quotient fits in 96 bits
		// A 96-bit value has toHigh() <= 0xFFFFFFFF (only lower 32 bits of high word can be set)
		while ( quotientMantissa.toHigh() > 0xFFFFFFFFULL && targetPrecision > 0 )
		{
			quotientMantissa = quotientMantissa / Int128{ constants::DECIMAL_BASE };
			targetPrecision--;
		}

		internal::setMantissa( result, quotientMantissa );
		result.m_layout.flags = ( static_cast<std::uint32_t>( targetPrecision ) << constants::DECIMAL_SCALE_SHIFT );

		// Combine signs
		if ( ( *this < Decimal{} ) != ( other < Decimal{} ) )
		{
			result.m_layout.flags |= constants::DECIMAL_SIGN_MASK;
		}

		internal::normalize( result );

		return result;
	}

	//----------------------------------------------
	// Mathematical operations
	//----------------------------------------------

	Decimal Decimal::round( std::int32_t decimalsPlacesCount, RoundingMode mode ) const noexcept
	{
		if ( decimalsPlacesCount < 0 )
		{
			decimalsPlacesCount = 0;
		}

		bool thisZero = m_layout.mantissa[0] == 0 && m_layout.mantissa[1] == 0 && m_layout.mantissa[2] == 0;
		if ( decimalsPlacesCount >= static_cast<std::int32_t>( scale() ) || thisZero )
		{
			return *this;
		}

		Decimal result{ *this };
		std::uint8_t currentScale{ scale() };
		std::uint8_t targetScale{ static_cast<std::uint8_t>( decimalsPlacesCount ) };
		std::uint8_t digitsToRemove{ static_cast<std::uint8_t>( currentScale - targetScale ) };

		// Get the digit that determines rounding direction
		Int128 mantissa{ internal::mantissaAsInt128( *this ) };
		Int128 divisor{ 1 };
		if ( digitsToRemove > 1U )
		{
			std::uint8_t divisorPowers = static_cast<std::uint8_t>( digitsToRemove - 1U );
			for ( std::uint8_t i{ 0 }; i < divisorPowers; ++i )
			{
				divisor = divisor * Int128{ constants::DECIMAL_BASE };
			}
		}

		Int128 roundingDigit{ ( mantissa / divisor ) % Int128{ constants::DECIMAL_BASE } };

		// Perform truncation to target scale
		for ( std::uint8_t i = 0; i < digitsToRemove; ++i )
		{
			internal::divideByPowerOf10( result, 1U );
		}

		result.m_layout.flags =
			( result.m_layout.flags & ~constants::DECIMAL_SCALE_MASK ) |
			( static_cast<std::uint32_t>( targetScale ) << constants::DECIMAL_SCALE_SHIFT );

		// Determine if we should round up based on the rounding mode
		bool shouldRoundUp{ false };

		switch ( mode )
		{
			case RoundingMode::ToNearest:
			{
				shouldRoundUp = internal::shouldRoundUpToNearest( roundingDigit, mantissa, divisor, digitsToRemove, result );
				break;
			}
			case RoundingMode::ToNearestTiesAway:
			{
				shouldRoundUp = internal::shouldRoundUpToNearestTiesAway( roundingDigit );
				break;
			}
			case RoundingMode::ToZero:
			{
				shouldRoundUp = false; // Truncate (never round up)
				break;
			}
			case RoundingMode::ToPositiveInfinity:
			{
				bool thisNeg = ( m_layout.flags & constants::DECIMAL_SIGN_MASK ) != 0;
				shouldRoundUp = internal::shouldRoundUpToPositiveInfinity( mantissa, digitsToRemove, thisNeg );
				break;
			}
			case RoundingMode::ToNegativeInfinity:
			{
				bool thisNeg = ( m_layout.flags & constants::DECIMAL_SIGN_MASK ) != 0;
				shouldRoundUp = internal::shouldRoundUpToNegativeInfinity( mantissa, digitsToRemove, thisNeg );
				break;
			}
		}

		// Apply rounding adjustment
		if ( shouldRoundUp )
		{
			Int128 resultMantissa{ internal::mantissaAsInt128( result ) };
			bool thisNeg = ( m_layout.flags & constants::DECIMAL_SIGN_MASK ) != 0;
			if ( thisNeg )
			{
				// For negative numbers, "rounding up" means increasing the absolute value (magnitude)
				// Since mantissa is unsigned, we ADD to make the number more negative
				// Example: -123 → -124 means mantissa goes from 123 to 124
				resultMantissa = resultMantissa + Int128{ 1 };
			}
			else
			{
				// For positive numbers, rounding up means adding to the mantissa
				resultMantissa = resultMantissa + Int128{ 1 };
			}
			internal::setMantissa( result, resultMantissa );
		}

		return result;
	}

	Decimal Decimal::sqrt() const
	{
		// Check for negative input
		bool isNegative = ( m_layout.flags & constants::DECIMAL_SIGN_MASK ) != 0;
		if ( isNegative )
		{
			throw std::domain_error( "Cannot compute square root of negative number" );
		}

		// Handle zero
		if ( *this == 0 )
		{
			return Decimal{ 0 };
		}

		// Handle one
		Decimal one{ 1 };
		if ( *this == one )
		{
			return one;
		}

		// Check if this is a perfect square by converting to Int128 and using isqrt
		// This gives exact results for perfect squares
		// For scaled values (e.g., 2.25 = 225/100), scale up to integer, check if perfect square
		const auto& mantissaArray = mantissa();
		Int128 mantissaValue;

#if NFX_DATATYPES_HAS_NATIVE_INT128
		NFX_DATATYPES_NATIVE_INT128 nativeVal = static_cast<NFX_DATATYPES_NATIVE_INT128>( mantissaArray[2] ) << constants::BITS_PER_UINT64 |
												static_cast<NFX_DATATYPES_NATIVE_INT128>( mantissaArray[1] ) << constants::BITS_PER_UINT32 |
												static_cast<NFX_DATATYPES_NATIVE_INT128>( mantissaArray[0] );
		mantissaValue = Int128{ nativeVal };
#else
		std::uint64_t low = static_cast<std::uint64_t>( mantissaArray[1] ) << constants::BITS_PER_UINT32 | mantissaArray[0];
		std::uint64_t high = mantissaArray[2];
		mantissaValue = Int128{ low, high };
#endif

		// Check if mantissa is a perfect square
		Int128 intSqrt = mantissaValue.isqrt();
		Int128 squared = intSqrt * intSqrt;

		if ( squared == mantissaValue )
		{
			// Mantissa is a perfect square
			// sqrt(mantissa * 10^-scale) = sqrt(mantissa) * 10^(-scale/2)
			// If scale is even, we get an exact result
			std::uint8_t currentScale = scale();

			if ( currentScale % 2 == 0 )
			{
				// Even scale - exact result
				// Construct result directly with correct scale
				std::uint8_t targetScale = currentScale / 2;

				// Convert intSqrt to Decimal with target scale
				Decimal result{ intSqrt };

				// Decimal from Int128 has scale 0, we need to set it to targetScale
				// This means we divide by 10^targetScale
				if ( targetScale > 0 )
				{
					Decimal divisor{ internal::powerOf10( targetScale ) };
					result = result / divisor;
				}

				return result;
			}
		}

		// Newton-Raphson method: x_{n+1} = 0.5 * (x_n + value/x_n)
		// Use magnitude-based initial guess for much faster convergence
		// Convert to double to estimate magnitude, then use as starting point
		double valueApprox = toDouble();
		double guessApprox = std::sqrt( std::abs( valueApprox ) );
		Decimal x{ guessApprox };

		// Ensure initial guess is at least 1 for very small numbers
		if ( x < one )
		{
			x = one;
		}

		Decimal two{ 2 };
		// Use a more practical epsilon based on the precision we can actually achieve
		Decimal epsilon{ constants::DECIMAL_SQRT_EPSILON };

		// Iterate until convergence (just a few iterations to refine double precision to Decimal precision)
		Decimal xNew = x;
		Decimal xPrev = x;
		for ( int i = 0; i < constants::DECIMAL_SQRT_MAX_ITERATIONS; ++i )
		{
			Decimal quotient = *this / x;
			Decimal sum = x + quotient;
			xNew = sum / two; // Use division instead of multiplication

			// Check convergence: if difference is tiny, we're done
			Decimal diff = ( xNew > x ) ? ( xNew - x ) : ( x - xNew );

			if ( diff < epsilon )
			{
				return xNew;
			}

			// Check if xNew equals x (no progress being made)
			if ( xNew == x )
			{
				return xNew;
			}

			// Check if we're oscillating between two values
			if ( i > 0 && xNew == xPrev )
			{
				// Oscillating - return the value closest to the true sqrt
				// Since we're oscillating, both x and xNew are very close to the answer
				return xNew;
			}

			xPrev = x;
			x = xNew;
		}

		// Return the last computed value even if we didn't fully converge
		return xNew;
	}

	//----------------------------------------------
	// String parsing
	//----------------------------------------------

	bool Decimal::fromString( std::string_view str, Decimal& result ) noexcept
	{
		try
		{
			result = Decimal{};

			if ( str.empty() )
			{
				return false;
			}

			// Handle sign
			bool negative{ false };
			size_t pos{ 0 };
			if ( str[0] == '-' )
			{
				negative = true;
				pos = 1;
			}
			else if ( str[0] == '+' )
			{
				pos = 1;
			}

			// Check if we have at least one character after sign
			if ( pos >= str.length() )
			{
				return false;
			}

			// Find decimal point and validate there's only one
			size_t decimalPos{ std::string_view::npos };
			std::uint8_t currentScale{ 0 };
			size_t decimalCount{ 0 };

			for ( size_t i{ pos }; i < str.length(); ++i )
			{
				if ( str[i] == '.' )
				{
					decimalCount++;
					if ( decimalCount > 1 )
					{
						return false;
					}

					decimalPos = i;
				}
			}

			if ( decimalPos != std::string_view::npos )
			{
				currentScale = static_cast<std::uint8_t>( str.length() - decimalPos - 1 );
				if ( currentScale > constants::DECIMAL_MAXIMUM_PLACES )
				{
					currentScale = constants::DECIMAL_MAXIMUM_PLACES;
				}
			}

			// Optimized digit accumulation
			Int128 mantissaValue;
			const Int128 ten{ constants::DECIMAL_BASE };
			bool hasDigits{ false };
			std::uint8_t significantDigits{ 0 };
			std::uint8_t decimalDigitsProcessed{ 0 };

			for ( size_t i{ pos }; i < str.length(); ++i )
			{
				if ( str[i] == '.' )
				{
					continue;
				}

				if ( str[i] < '0' || str[i] > '9' )
				{
					// Invalid character
					return false;
				}

				hasDigits = true;
				std::uint64_t digit{ static_cast<std::uint64_t>( str[i] - '0' ) };

				// Decimal specification: maximum 28 significant digits
				if ( significantDigits >= constants::DECIMAL_MAXIMUM_PLACES )
				{
					// Truncate excess digits - adjust scale based on actual decimal digits processed
					if ( decimalPos != std::string_view::npos )
					{
						currentScale = decimalDigitsProcessed;
					}
					break;
				}

				// Count significant digits (skip leading zeros only before decimal point)
				if ( digit != 0 || mantissaValue != Int128{ 0 } || ( decimalPos != std::string_view::npos && i > decimalPos ) )
				{
					significantDigits++;
				}

				// Count decimal digits processed
				if ( decimalPos != std::string_view::npos && i > decimalPos )
				{
					decimalDigitsProcessed++;
				}

				// Safe to accumulate this digit
				mantissaValue = mantissaValue * ten + Int128{ digit };
			}

			// Ensure we have at least one digit (prevents parsing ".", "+", "-", etc.)
			if ( !hasDigits )
			{
				return false;
			}

			// Check if mantissa fits in our 96-bit storage
			if ( mantissaValue.toHigh() > constants::UINT32_MAX_VALUE )
			{
				// Value too large - truncate excess precision to fit
				while ( mantissaValue.toHigh() > constants::UINT32_MAX_VALUE && currentScale > 0 )
				{
					mantissaValue = mantissaValue / Int128{ constants::DECIMAL_BASE };
					--currentScale;
				}

				// If still too large after removing all decimal places, truncate the integer part to fit
				while ( mantissaValue.toHigh() > constants::UINT32_MAX_VALUE )
				{
					mantissaValue = mantissaValue / Int128{ constants::DECIMAL_BASE };
				}
			}

			// Set result
			if ( negative )
			{
				result.m_layout.flags |= constants::DECIMAL_SIGN_MASK;
			}

			result.m_layout.flags |= ( static_cast<std::uint32_t>( currentScale ) << constants::DECIMAL_SCALE_SHIFT );

			// Store the 96-bit mantissa
			std::uint64_t low{ mantissaValue.toLow() };
			std::uint64_t high{ mantissaValue.toHigh() };

			result.m_layout.mantissa[0] = static_cast<std::uint32_t>( low );
			result.m_layout.mantissa[1] = static_cast<std::uint32_t>( low >> constants::BITS_PER_UINT32 );
			result.m_layout.mantissa[2] = static_cast<std::uint32_t>( high );

			// Normalize to remove trailing zeros
			internal::normalize( result );

			return true;
		}
		catch ( ... )
		{
			return false;
		}
	}

	std::optional<Decimal> Decimal::fromString( std::string_view str ) noexcept
	{
		Decimal result;
		if ( fromString( str, result ) )
		{
			return result;
		}
		return std::nullopt;
	}

	//----------------------------------------------
	// Type conversion
	//----------------------------------------------

	double Decimal::toDouble() const noexcept
	{
		Int128 mantissa{ internal::mantissaAsInt128( *this ) };

		double result;
#if NFX_DATATYPES_HAS_NATIVE_INT128
		result = static_cast<double>( mantissa.toNative() );
#else
		// Convert 128-bit to double (approximate)
		result = static_cast<double>(
					 mantissa.toHigh() ) *
					 ( constants::BIT_MASK_ONE << constants::BITS_PER_UINT32 ) *
					 ( constants::BIT_MASK_ONE << constants::BITS_PER_UINT32 ) +
				 static_cast<double>( mantissa.toLow() );
#endif
		// Apply scale (using single division to avoid cumulative rounding errors)
		std::uint8_t currentScale = scale();
		if ( currentScale > 0 )
		{
			Int128 divisor = internal::powerOf10( currentScale );
#if NFX_DATATYPES_HAS_NATIVE_INT128
			result /= static_cast<double>( divisor.toNative() );
#else
			double divisorDouble = static_cast<double>(
									   divisor.toHigh() ) *
									   ( constants::BIT_MASK_ONE << constants::BITS_PER_UINT32 ) *
									   ( constants::BIT_MASK_ONE << constants::BITS_PER_UINT32 ) +
								   static_cast<double>( divisor.toLow() );
			result /= divisorDouble;
#endif
		}

		// Apply sign
		if ( ( m_layout.flags & constants::DECIMAL_SIGN_MASK ) != 0 )
		{
			result = -result;
		}

		return result;
	}

	std::string Decimal::toString() const
	{
		if ( m_layout.mantissa[0] == 0 && m_layout.mantissa[1] == 0 && m_layout.mantissa[2] == 0 )
		{
			return "0";
		}

		std::string result;
		result.reserve( constants::DECIMAL_STRING_BUFFER_SIZE );

		Int128 mantissa{ internal::mantissaAsInt128( *this ).abs() };
		std::uint8_t currentScale{ scale() };

		// Optimized digit extraction with fast division
		std::array<char, constants::DECIMAL_MAX_STRING_LENGTH> digits;
		size_t digitCount{ 0 };

#if NFX_DATATYPES_HAS_NATIVE_INT128
		// Fast path for values that fit in 64-bit
		if ( mantissa.toNative() <= UINT64_MAX )
		{
			std::uint64_t value{ static_cast<std::uint64_t>( mantissa.toNative() ) };
			while ( value > 0 && digitCount < digits.size() )
			{
				digits[digitCount++] = static_cast<char>( '0' + ( value % constants::DECIMAL_BASE ) );
				value /= constants::DECIMAL_BASE;
			}
		}
		else
		{
			// Full 128-bit extraction
			while ( mantissa != 0 && digitCount < digits.size() )
			{
				digits[digitCount++] = static_cast<char>( '0' +
														  ( mantissa.toNative() % constants::DECIMAL_BASE ) );
				mantissa = Int128{ mantissa.toNative() / constants::DECIMAL_BASE };
			}
		}
#else
		if ( mantissa.toHigh() == 0 )
		{
			// Fast 64-bit path
			std::uint64_t value{ mantissa.toLow() };
			while ( value > 0 && digitCount < digits.size() )
			{
				digits[digitCount++] = static_cast<char>( '0' + ( value % constants::DECIMAL_BASE ) );
				value /= constants::DECIMAL_BASE;
			}
		}
		else
		{
			// Manual 128-bit extraction
			while ( mantissa != Int128{} && digitCount < digits.size() )
			{
				if ( mantissa.toHigh() == 0 )
				{
					// Switched to 64-bit range
					std::uint64_t value{ mantissa.toLow() };
					while ( value > 0 && digitCount < digits.size() )
					{
						digits[digitCount++] = static_cast<char>( '0' + ( value % constants::DECIMAL_BASE ) );
						value /= constants::DECIMAL_BASE;
					}
					break;
				}
				// Use full 128-bit value for modulo operation, not just low 64 bits
				Int128 remainder{ mantissa % Int128{ constants::DECIMAL_BASE } };
				digits[digitCount++] = static_cast<char>( '0' + remainder.toLow() );

				mantissa = mantissa / Int128{ constants::DECIMAL_BASE };
			}
		}
#endif

		if ( digitCount == 0 )
		{
			digitCount = 1;
			digits[0] = '0';
		}

		// Handle sign
		if ( ( m_layout.flags & constants::DECIMAL_SIGN_MASK ) != 0 )
		{
			result.push_back( '-' );
		}

		// Apply decimal point formatting
		if ( currentScale > 0 )
		{
			if ( currentScale >= digitCount )
			{
				// Need leading zeros: "0.00123"
				result.push_back( '0' );
				result.push_back( '.' );

				// Add leading zeros
				for ( size_t i = 0; i < currentScale - digitCount; ++i )
				{
					result.push_back( '0' );
				}

				// Add digits in reverse order
				for ( size_t i = digitCount; i > 0; --i )
				{
					result.push_back( digits[i - 1] );
				}
			}
			else
			{
				// Add integer part (reverse order)
				for ( size_t i = digitCount; i > currentScale; --i )
				{
					result.push_back( digits[i - 1] );
				}

				result.push_back( '.' );

				// Add fractional part (reverse order)
				for ( size_t i = currentScale; i > 0; --i )
				{
					result.push_back( digits[i - 1] );
				}
			}
		}
		else
		{
			// No decimal point, just add digits in reverse order
			for ( size_t i = digitCount; i > 0; --i )
			{
				result.push_back( digits[i - 1] );
			}
		}

		return result;
	}

	std::array<std::int32_t, 4> Decimal::toBits() const noexcept
	{
		std::array<std::int32_t, 4> bits{};

		// First three elements are the 96-bit mantissa
		bits[0] = static_cast<std::int32_t>( m_layout.mantissa[0] );
		bits[1] = static_cast<std::int32_t>( m_layout.mantissa[1] );
		bits[2] = static_cast<std::int32_t>( m_layout.mantissa[2] );

		// Fourth element contains scale and sign information
		bits[3] = static_cast<std::int32_t>( m_layout.flags );

		return bits;
	}

	//----------------------------------------------
	// Utilities
	//----------------------------------------------

	std::uint8_t Decimal::decimalPlacesCount() const noexcept
	{
		// If the value is zero, it has 0 decimal places
		if ( m_layout.mantissa[0] == 0 && m_layout.mantissa[1] == 0 && m_layout.mantissa[2] == 0 )
		{
			return 0;
		}

		// Get the current scale
		std::uint8_t currentScale = scale();

		// If scale is 0, it's an integer - no decimal places
		if ( currentScale == 0 )
		{
			return 0;
		}

		// Convert mantissa to Int128 for proper arithmetic
		const auto& mantissaArray = mantissa();
#if NFX_DATATYPES_HAS_NATIVE_INT128
		NFX_DATATYPES_NATIVE_INT128 mantissaValue{ static_cast<NFX_DATATYPES_NATIVE_INT128>( mantissaArray[2] ) << constants::BITS_PER_UINT64 |
												   static_cast<NFX_DATATYPES_NATIVE_INT128>( mantissaArray[1] ) << constants::BITS_PER_UINT32 |
												   static_cast<NFX_DATATYPES_NATIVE_INT128>( mantissaArray[0] ) };
		Int128 mantissa128{ mantissaValue };
#else
		std::uint64_t low{ static_cast<std::uint64_t>( mantissaArray[1] ) << constants::BITS_PER_UINT32 | mantissaArray[0] };
		std::uint64_t high{ mantissaArray[2] };
		Int128 mantissa128{ low, high };
#endif

		std::uint8_t trailingZeros = 0;
		Int128 ten{ constants::DECIMAL_BASE };

		// Count trailing zeros by testing divisibility by 10 iteratively
		while ( trailingZeros < currentScale )
		{
			// If there's a remainder, we can't divide evenly by 10
			if ( mantissa128 % ten != Int128{ 0 } )
			{
				break;
			}

			// Continue testing with the next power of 10
			mantissa128 = mantissa128 / ten;
			trailingZeros++;
		}

		return currentScale - trailingZeros;
	}

	//=====================================================================
	// Stream operators
	//=====================================================================

	std::ostream& operator<<( std::ostream& os, const Decimal& decimal )
	{
		// Check if std::fixed is set with specific precision
		if ( ( os.flags() & std::ios_base::fixed ) && os.precision() >= 0 )
		{
			// Format with minimum decimal places based on stream precision
			std::string str{ decimal.toString() };

			// Find decimal point
			size_t dot_pos{ str.find( '.' ) };

			if ( dot_pos == std::string::npos )
			{
				// No decimal point - add it with required precision
				str += '.';
				for ( std::streamsize i = 0; i < os.precision(); ++i )
				{
					str += '0';
				}
			}
			else
			{
				// Has decimal point - pad to required precision
				size_t current_decimals{ str.length() - dot_pos - 1 };
				if ( static_cast<std::streamsize>( current_decimals ) < os.precision() )
				{
					for ( std::streamsize i = static_cast<std::streamsize>( current_decimals ); i < os.precision(); ++i )
					{
						str += '0';
					}
				}
			}

			return os << str;
		}

		// Default behavior - use toString() as-is
		return os << decimal.toString();
	}

	std::istream& operator>>( std::istream& is, Decimal& decimal )
	{
		std::string str;
		is >> str;

		if ( !Decimal::fromString( str, decimal ) )
		{
			is.setstate( std::ios::failbit );
		}

		return is;
	}
} // namespace nfx::datatypes
