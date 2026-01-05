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
 * @file Decimal.inl
 * @brief Inline implementations for cross-platform Decimal class
 */

#include <format>
#include <limits>

#include "Constants.h"

namespace nfx::datatypes
{
	//=====================================================================
	// Decimal class
	//=====================================================================

	//----------------------------------------------
	// Construction
	//----------------------------------------------

	inline constexpr Decimal::Decimal() noexcept
		: m_layout{ 0, { { 0, 0, 0 } } }
	{
	}

	inline Decimal::Decimal( std::int32_t value ) noexcept
		: m_layout{ 0, { { 0, 0, 0 } } }
	{
		if ( value < 0 )
		{
			m_layout.flags |= constants::DECIMAL_SIGN_MASK;
			value = -value;
		}

		m_layout.mantissa[0] = static_cast<std::uint32_t>( value );
	}

	inline Decimal::Decimal( std::int64_t value ) noexcept
		: m_layout{ 0, { { 0, 0, 0 } } }
	{
		if ( value < 0 )
		{
			m_layout.flags |= constants::DECIMAL_SIGN_MASK;
			value = -value;
		}

		m_layout.mantissa[0] = static_cast<std::uint32_t>( value );
		m_layout.mantissa[1] = static_cast<std::uint32_t>( value >> constants::BITS_PER_UINT32 );
	}

	inline Decimal::Decimal( std::uint32_t value ) noexcept
		: m_layout{ 0, { { 0, 0, 0 } } }
	{
		m_layout.mantissa[0] = value;
	}

	inline Decimal::Decimal( std::uint64_t value ) noexcept
		: m_layout{ 0, { { 0, 0, 0 } } }
	{
		m_layout.mantissa[0] = static_cast<std::uint32_t>( value );
		m_layout.mantissa[1] = static_cast<std::uint32_t>( value >> constants::BITS_PER_UINT32 );
	}

	inline Decimal::Decimal( std::string_view str )
		: m_layout{ 0, { { 0, 0, 0 } } }
	{
		if ( !fromString( str, *this ) )
		{
			throw std::invalid_argument{ "Invalid decimal string format" };
		}
	}

	//----------------------------------------------
	// Comparison with built-in floating point types
	//----------------------------------------------

	inline bool Decimal::operator==( float val ) const noexcept
	{
		return *this == static_cast<double>( val );
	}

	inline bool Decimal::operator!=( float val ) const noexcept
	{
		return *this != static_cast<double>( val );
	}

	inline bool Decimal::operator<( float val ) const noexcept
	{
		return *this < static_cast<double>( val );
	}

	inline bool Decimal::operator<=( float val ) const noexcept
	{
		return *this <= static_cast<double>( val );
	}

	inline bool Decimal::operator>( float val ) const noexcept
	{
		return *this > static_cast<double>( val );
	}

	inline bool Decimal::operator>=( float val ) const noexcept
	{
		return *this >= static_cast<double>( val );
	}

	inline bool Decimal::operator==( double val ) const noexcept
	{
		if ( std::isnan( val ) || std::isinf( val ) )
		{
			return false; // Decimal has no NaN/Infinity representation
		}

		// Convert double to Decimal for comparison
		Decimal temp{ val };
		return *this == temp;
	}

	inline bool Decimal::operator!=( double val ) const noexcept
	{
		return !( *this == val );
	}

	inline bool Decimal::operator<( double val ) const noexcept
	{
		if ( std::isnan( val ) )
		{
			return false; // No ordering with NaN
		}
		if ( std::isinf( val ) )
		{
			return val > 0.0; // Any finite value < +infinity, any finite value > -infinity
		}

		Decimal temp{ val };
		return *this < temp;
	}

	inline bool Decimal::operator<=( double val ) const noexcept
	{
		return *this < val || *this == val;
	}

	inline bool Decimal::operator>( double val ) const noexcept
	{
		if ( std::isnan( val ) )
		{
			return false; // No ordering with NaN
		}
		if ( std::isinf( val ) )
		{
			return val < 0.0; // Any finite value > -infinity, any finite value < +infinity
		}

		Decimal temp{ val };
		return *this > temp;
	}

	inline bool Decimal::operator>=( double val ) const noexcept
	{
		return *this > val || *this == val;
	}

	//----------------------------------------------
	// Comparison with built-in integer types
	//----------------------------------------------

	inline bool Decimal::operator==( std::int64_t val ) const noexcept
	{
		// For integer comparison, we need exact equality
		if ( scale() > 0 )
		{
			// If this has fractional part, it can't equal an integer
			return false;
		}

		Decimal temp{ val };
		return *this == temp;
	}

	inline bool Decimal::operator!=( std::int64_t val ) const noexcept
	{
		return !( *this == val );
	}

	inline bool Decimal::operator<( std::int64_t val ) const noexcept
	{
		Decimal temp{ val };
		return *this < temp;
	}

	inline bool Decimal::operator<=( std::int64_t val ) const noexcept
	{
		return *this < val || *this == val;
	}

	inline bool Decimal::operator>( std::int64_t val ) const noexcept
	{
		Decimal temp{ val };
		return *this > temp;
	}

	inline bool Decimal::operator>=( std::int64_t val ) const noexcept
	{
		return *this > val || *this == val;
	}

	inline bool Decimal::operator==( std::uint64_t val ) const noexcept
	{
		if ( scale() > 0 )
		{
			return false;
		}

		if ( *this < Decimal{} )
		{
			return false;
		}

		Decimal temp{ val };
		return *this == temp;
	}

	inline bool Decimal::operator!=( std::uint64_t val ) const noexcept
	{
		return !( *this == val );
	}

	inline bool Decimal::operator<( std::uint64_t val ) const noexcept
	{
		if ( *this < Decimal{} )
		{
			return true;
		}

		Decimal temp{ val };
		return *this < temp;
	}

	inline bool Decimal::operator<=( std::uint64_t val ) const noexcept
	{
		return *this < val || *this == val;
	}

	inline bool Decimal::operator>( std::uint64_t val ) const noexcept
	{
		if ( *this < Decimal{} )
		{
			return false;
		}

		Decimal temp{ val };
		return *this > temp;
	}

	inline bool Decimal::operator>=( std::uint64_t val ) const noexcept
	{
		return *this > val || *this == val;
	}

	inline bool Decimal::operator==( std::int32_t val ) const noexcept
	{
		return *this == static_cast<std::int64_t>( val );
	}

	inline bool Decimal::operator!=( std::int32_t val ) const noexcept
	{
		return *this != static_cast<std::int64_t>( val );
	}

	inline bool Decimal::operator<( std::int32_t val ) const noexcept
	{
		return *this < static_cast<std::int64_t>( val );
	}

	inline bool Decimal::operator<=( std::int32_t val ) const noexcept
	{
		return *this <= static_cast<std::int64_t>( val );
	}

	inline bool Decimal::operator>( std::int32_t val ) const noexcept
	{
		return *this > static_cast<std::int64_t>( val );
	}

	inline bool Decimal::operator>=( std::int32_t val ) const noexcept
	{
		return *this >= static_cast<std::int64_t>( val );
	}

	//----------------------------------------------
	// Comparison with nfx::datatypes::Int128
	//----------------------------------------------

	inline bool Decimal::operator!=( const Int128& val ) const noexcept
	{
		return !( *this == val );
	}

	inline bool Decimal::operator<=( const Int128& val ) const noexcept
	{
		return *this < val || *this == val;
	}

	inline bool Decimal::operator>( const Int128& val ) const noexcept
	{
		return !( *this <= val );
	}

	inline bool Decimal::operator>=( const Int128& val ) const noexcept
	{
		return !( *this < val );
	}

	//----------------------------------------------
	// Arithmetic operators
	//----------------------------------------------

	inline Decimal& Decimal::operator+=( const Decimal& other )
	{
		*this = *this + other;
		return *this;
	}

	inline Decimal& Decimal::operator-=( const Decimal& other )
	{
		*this = *this - other;
		return *this;
	}

	inline Decimal& Decimal::operator*=( const Decimal& other )
	{
		*this = *this * other;
		return *this;
	}

	inline Decimal& Decimal::operator/=( const Decimal& other )
	{
		*this = *this / other;
		return *this;
	}

	inline Decimal Decimal::operator-( const Decimal& other )
	{
		Decimal negatedOther{ other };

		negatedOther.m_layout.flags ^= constants::DECIMAL_SIGN_MASK;

		return *this + negatedOther;
	}

	inline Decimal Decimal::operator-() const noexcept
	{
		Decimal result{ *this };

		result.m_layout.flags ^= constants::DECIMAL_SIGN_MASK;
		return result;
	}

	//----------------------------------------------
	// Property accessors
	//----------------------------------------------

	inline std::uint8_t Decimal::scale() const noexcept
	{
		return static_cast<std::uint8_t>( ( m_layout.flags & constants::DECIMAL_SCALE_MASK ) >> constants::DECIMAL_SCALE_SHIFT );
	}

	inline const std::uint32_t& Decimal::flags() const noexcept
	{
		return m_layout.flags;
	}

	inline std::uint32_t& Decimal::flags() noexcept
	{
		return m_layout.flags;
	}

	inline const std::array<std::uint32_t, 3>& Decimal::mantissa() const noexcept
	{
		return m_layout.mantissa;
	}

	inline std::array<std::uint32_t, 3>& Decimal::mantissa() noexcept
	{
		return m_layout.mantissa;
	}

	//----------------------------------------------
	// Mathematical operations
	//----------------------------------------------

	inline Decimal Decimal::abs() const noexcept
	{
		if ( *this < Decimal{} )
		{
			return -*this;
		}

		return *this;
	}

	inline Decimal Decimal::ceil() const noexcept
	{
		return round( 0, RoundingMode::ToPositiveInfinity );
	}

	inline Decimal Decimal::floor() const noexcept
	{
		return round( 0, RoundingMode::ToNegativeInfinity );
	}

	inline Decimal Decimal::trunc() const noexcept
	{
		return round( 0, RoundingMode::ToZero );
	}

	//=====================================================================
	// Stream operators
	//=====================================================================

	std::ostream& operator<<( std::ostream& os, const Decimal& decimal );

	std::istream& operator>>( std::istream& is, Decimal& decimal );
} // namespace nfx::datatypes

namespace std
{
	//=====================================================================
	// std::formatter specialization
	//=====================================================================

	template <>
	struct formatter<nfx::datatypes::Decimal>
	{
		constexpr auto parse( std::format_parse_context& ctx )
		{
			return ctx.begin();
		}

		auto format( const nfx::datatypes::Decimal& d, std::format_context& ctx ) const
		{
			return format_to( ctx.out(), "{}", d.toString() );
		}
	};

	//=====================================================================
	// std::numeric_limits specialization
	//=====================================================================

	template <>
	struct numeric_limits<nfx::datatypes::Decimal>
	{
		static constexpr bool is_specialized = true;
		static constexpr bool is_signed = true;
		static constexpr bool is_integer = false;
		static constexpr bool is_exact = true;
		static constexpr bool has_infinity = false;
		static constexpr bool has_quiet_NaN = false;
		static constexpr bool has_signaling_NaN = false;
		static constexpr bool has_denorm = false;
		static constexpr bool has_denorm_loss = false;
		static constexpr bool is_bounded = true;
		static constexpr bool is_modulo = false;
		static constexpr bool traps = false;
		static constexpr bool tinyness_before = false;
		static constexpr std::float_round_style round_style = std::round_toward_zero;

		static nfx::datatypes::Decimal min() noexcept
		{
			// Smallest positive value: 1 × 10^-28
			nfx::datatypes::Decimal result{};
			result.m_layout.mantissa[0] = nfx::datatypes::constants::DECIMAL_MIN_MANTISSA_0;
			result.m_layout.mantissa[1] = nfx::datatypes::constants::DECIMAL_MIN_MANTISSA_1;
			result.m_layout.mantissa[2] = nfx::datatypes::constants::DECIMAL_MIN_MANTISSA_2;
			result.m_layout.flags = ( nfx::datatypes::constants::DECIMAL_MAXIMUM_PLACES << nfx::datatypes::constants::DECIMAL_SCALE_SHIFT );
			return result;
		}

		static nfx::datatypes::Decimal max() noexcept
		{
			// Maximum value: 79,228,162,514,264,337,593,543,950,335 (2^96 - 1)
			nfx::datatypes::Decimal result{};
			result.m_layout.mantissa[0] = nfx::datatypes::constants::DECIMAL_MAX_MANTISSA_0;
			result.m_layout.mantissa[1] = nfx::datatypes::constants::DECIMAL_MAX_MANTISSA_1;
			result.m_layout.mantissa[2] = nfx::datatypes::constants::DECIMAL_MAX_MANTISSA_2;
			return result;
		}

		static nfx::datatypes::Decimal lowest() noexcept
		{
			// Most negative value: -79,228,162,514,264,337,593,543,950,335
			nfx::datatypes::Decimal result{};
			result.m_layout.mantissa[0] = nfx::datatypes::constants::DECIMAL_MAX_MANTISSA_0;
			result.m_layout.mantissa[1] = nfx::datatypes::constants::DECIMAL_MAX_MANTISSA_1;
			result.m_layout.mantissa[2] = nfx::datatypes::constants::DECIMAL_MAX_MANTISSA_2;
			result.m_layout.flags = nfx::datatypes::constants::DECIMAL_SIGN_MASK;
			return result;
		}

		static nfx::datatypes::Decimal epsilon() noexcept
		{
			// Epsilon for 28 decimal places: 1 × 10^-28
			nfx::datatypes::Decimal result{};
			result.m_layout.mantissa[0] = nfx::datatypes::constants::DECIMAL_MIN_MANTISSA_0;
			result.m_layout.mantissa[1] = nfx::datatypes::constants::DECIMAL_MIN_MANTISSA_1;
			result.m_layout.mantissa[2] = nfx::datatypes::constants::DECIMAL_MIN_MANTISSA_2;
			result.m_layout.flags = ( nfx::datatypes::constants::DECIMAL_MAXIMUM_PLACES << nfx::datatypes::constants::DECIMAL_SCALE_SHIFT );
			return result;
		}

		static constexpr int digits = nfx::datatypes::constants::DECIMAL_MANTISSA_BITS;
		static constexpr int digits10 = nfx::datatypes::constants::DECIMAL_DIGITS10;
		static constexpr int max_digits10 = nfx::datatypes::constants::DECIMAL_DIGITS10;
		static constexpr int radix = nfx::datatypes::constants::DECIMAL_RADIX;
		static constexpr int min_exponent = 0;
		static constexpr int min_exponent10 = 0;
		static constexpr int max_exponent = 0;
		static constexpr int max_exponent10 = 0;
	};
} // namespace std
