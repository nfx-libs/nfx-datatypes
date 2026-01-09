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
 * @file Int128.inl
 * @brief Inline implementations for cross-platform Int128 class
 */

#include <cmath>
#include <format>
#include <stdexcept>
#include <string_view>

#include "Constants.h"

namespace nfx::datatypes
{
    //=====================================================================
    // Int128 class
    //=====================================================================

#if NFX_DATATYPES_HAS_NATIVE_INT128

    //----------------------------------------------
    // Construction
    //----------------------------------------------

    inline constexpr Int128::Int128() noexcept
        : m_value{ 0 }
    {
    }

    inline constexpr Int128::Int128( int val ) noexcept
        : m_value{ val }
    {
    }

    inline constexpr Int128::Int128( std::int64_t val ) noexcept
        : m_value{ val }
    {
    }

    inline constexpr Int128::Int128( std::uint32_t val ) noexcept
        : m_value{ val }
    {
    }

    inline constexpr Int128::Int128( std::uint64_t val ) noexcept
        : m_value{ val }
    {
    }

    inline constexpr Int128::Int128( NFX_DATATYPES_NATIVE_INT128 val ) noexcept
        : m_value{ val }
    {
    }

    inline Int128::Int128( std::string_view str )
    {
        if ( !fromString( str, *this ) )
        {
            throw std::invalid_argument{ "Invalid Int128 string format" };
        }
    }

    inline constexpr Int128::Int128( std::uint64_t low, std::uint64_t high ) noexcept
        : m_value{ static_cast<NFX_DATATYPES_NATIVE_INT128>( high ) << constants::BITS_PER_UINT64 | low }
    {
    }

    //----------------------------------------------
    // Comparison operations
    //----------------------------------------------

    inline std::strong_ordering Int128::operator<=>( const Int128& other ) const noexcept
    {
        if ( m_value < other.m_value )
        {
            return std::strong_ordering::less;
        }
        if ( m_value > other.m_value )
        {
            return std::strong_ordering::greater;
        }
        return std::strong_ordering::equal;
    }

    inline bool Int128::operator==( const Int128& other ) const noexcept
    {
        return m_value == other.m_value;
    }

    //----------------------------------------------
    // Comparison with built-in integer types
    //----------------------------------------------

    inline bool Int128::operator==( int val ) const noexcept
    {
        return m_value == val;
    }

    inline bool Int128::operator!=( int val ) const noexcept
    {
        return !( *this == val );
    }

    inline bool Int128::operator<( int val ) const noexcept
    {
        return m_value < val;
    }

    inline bool Int128::operator<=( int val ) const noexcept
    {
        return *this < val || *this == val;
    }

    inline bool Int128::operator>( int val ) const noexcept
    {
        return m_value > val;
    }

    inline bool Int128::operator>=( int val ) const noexcept
    {
        return *this > val || *this == val;
    }

    inline bool Int128::operator==( std::int64_t val ) const noexcept
    {
        return m_value == val;
    }

    inline bool Int128::operator!=( std::int64_t val ) const noexcept
    {
        return !( *this == val );
    }

    inline bool Int128::operator<( std::int64_t val ) const noexcept
    {
        return m_value < val;
    }

    inline bool Int128::operator<=( std::int64_t val ) const noexcept
    {
        return *this < val || *this == val;
    }

    inline bool Int128::operator>( std::int64_t val ) const noexcept
    {
        return m_value > val;
    }

    inline bool Int128::operator>=( std::int64_t val ) const noexcept
    {
        return *this > val || *this == val;
    }

    inline bool Int128::operator==( std::uint64_t val ) const noexcept
    {
        return m_value >= 0 && static_cast<std::uint64_t>( m_value ) == val;
    }

    inline bool Int128::operator!=( std::uint64_t val ) const noexcept
    {
        return !( *this == val );
    }

    inline bool Int128::operator<( std::uint64_t val ) const noexcept
    {
        return m_value < 0 || static_cast<std::uint64_t>( m_value ) < val;
    }

    inline bool Int128::operator<=( std::uint64_t val ) const noexcept
    {
        return *this < val || *this == val;
    }

    inline bool Int128::operator>( std::uint64_t val ) const noexcept
    {
        return m_value >= 0 && static_cast<std::uint64_t>( m_value ) > val;
    }

    inline bool Int128::operator>=( std::uint64_t val ) const noexcept
    {
        return *this > val || *this == val;
    }

    //----------------------------------------------
    // Comparison with built-in floating point types
    //----------------------------------------------

    inline bool Int128::operator==( float val ) const noexcept
    {
        return *this == static_cast<double>( val );
    }

    inline bool Int128::operator!=( float val ) const noexcept
    {
        return *this != static_cast<double>( val );
    }

    inline bool Int128::operator<( float val ) const noexcept
    {
        return *this < static_cast<double>( val );
    }

    inline bool Int128::operator<=( float val ) const noexcept
    {
        return *this <= static_cast<double>( val );
    }

    inline bool Int128::operator>( float val ) const noexcept
    {
        return *this > static_cast<double>( val );
    }

    inline bool Int128::operator>=( float val ) const noexcept
    {
        return *this >= static_cast<double>( val );
    }

    inline bool Int128::operator==( double val ) const noexcept
    {
        constexpr long double EPSILON = std::numeric_limits<long double>::epsilon();
        return std::fabs( static_cast<long double>( m_value ) - static_cast<long double>( val ) ) <= EPSILON;
    }

    inline bool Int128::operator!=( double val ) const noexcept
    {
        return !( *this == val );
    }

    inline bool Int128::operator<( double val ) const noexcept
    {
        return static_cast<long double>( m_value ) < static_cast<long double>( val );
    }

    inline bool Int128::operator<=( double val ) const noexcept
    {
        return *this < val || *this == val;
    }

    inline bool Int128::operator>( double val ) const noexcept
    {
        return static_cast<long double>( m_value ) > static_cast<long double>( val );
    }

    inline bool Int128::operator>=( double val ) const noexcept
    {
        return *this > val || *this == val;
    }

    //----------------------------------------------
    // Arithmetic operators
    //----------------------------------------------

    inline Int128 Int128::operator+( const Int128& other ) const noexcept
    {
        return Int128{ m_value + other.m_value };
    }

    inline Int128 Int128::operator-( const Int128& other ) const noexcept
    {
        return Int128{ m_value - other.m_value };
    }

    inline Int128 Int128::operator*( const Int128& other ) const noexcept
    {
        return Int128{ m_value * other.m_value };
    }

    inline Int128 Int128::operator/( const Int128& other ) const
    {
        if ( other.m_value == 0 )
        {
            throw std::overflow_error{ "Division by zero" };
        }
        return Int128{ m_value / other.m_value };
    }

    inline Int128 Int128::operator%( const Int128& other ) const
    {
        if ( other.m_value == 0 )
        {
            throw std::overflow_error{ "Division by zero" };
        }

        return Int128{ m_value % other.m_value };
    }

    inline Int128 Int128::operator-() const noexcept
    {
        return Int128{ -m_value };
    }

    //----------------------------------------------
    // Mathematical operations
    //----------------------------------------------

    inline Int128 Int128::abs() const noexcept
    {
        return Int128{ m_value < 0
                           ? -m_value
                           : m_value };
    }

    //----------------------------------------------
    // Property accessors
    //----------------------------------------------

    inline std::uint64_t Int128::toLow() const noexcept
    {
        return static_cast<std::uint64_t>( m_value );
    }

    inline std::uint64_t Int128::toHigh() const noexcept
    {
        return static_cast<std::uint64_t>( m_value >> constants::BITS_PER_UINT64 );
    }

    inline NFX_DATATYPES_NATIVE_INT128 Int128::toNative() const noexcept
    {
        return m_value;
    }

    inline std::array<std::int32_t, 4> Int128::toBits() const noexcept
    {
        std::array<std::int32_t, 4> bits{};
        std::uint64_t low = toLow();
        std::uint64_t high = toHigh();
        bits[0] = static_cast<std::int32_t>( low );
        bits[1] = static_cast<std::int32_t>( low >> constants::BITS_PER_UINT32 );
        bits[2] = static_cast<std::int32_t>( high );
        bits[3] = static_cast<std::int32_t>( high >> constants::BITS_PER_UINT32 );
        return bits;
    }

#else

    //----------------------------------------------
    // Construction
    //----------------------------------------------

    inline constexpr Int128::Int128() noexcept
        : m_layout{ 0, 0 }
    {
    }

    inline constexpr Int128::Int128( int val ) noexcept
        : m_layout{ static_cast<std::uint64_t>( val ),
              ( val < 0 ) ? static_cast<std::uint64_t>( -1 )
                          : 0 }
    {
    }

    inline constexpr Int128::Int128( std::int64_t val ) noexcept
        : m_layout{ static_cast<std::uint64_t>( val ),
              ( val < 0 ) ? static_cast<std::uint64_t>( -1 )
                          : 0 }
    {
    }

    inline constexpr Int128::Int128( std::uint32_t val ) noexcept
        : m_layout{ val, 0 }
    {
    }

    inline constexpr Int128::Int128( std::uint64_t val ) noexcept
        : m_layout{ val, 0 }
    {
    }

    inline Int128::Int128( std::string_view str )
    {
        if ( !fromString( str, *this ) )
        {
            throw std::invalid_argument{ "Invalid Int128 string format" };
        }
    }

    inline constexpr Int128::Int128( std::uint64_t low, std::uint64_t high ) noexcept
        : m_layout{ low, high }
    {
    }

    //----------------------------------------------
    // Comparison operators
    //----------------------------------------------

    inline std::strong_ordering Int128::operator<=>( const Int128& other ) const noexcept
    {
        bool thisNeg = static_cast<std::int64_t>( m_layout.upper64bits ) < 0;
        bool otherNeg = static_cast<std::int64_t>( other.m_layout.upper64bits ) < 0;

        if ( thisNeg && !otherNeg )
        {
            return std::strong_ordering::less;
        }
        if ( !thisNeg && otherNeg )
        {
            return std::strong_ordering::greater;
        }

        if ( m_layout.upper64bits < other.m_layout.upper64bits )
        {
            return std::strong_ordering::less;
        }
        if ( m_layout.upper64bits > other.m_layout.upper64bits )
        {
            return std::strong_ordering::greater;
        }

        if ( m_layout.lower64bits < other.m_layout.lower64bits )
        {
            return std::strong_ordering::less;
        }
        if ( m_layout.lower64bits > other.m_layout.lower64bits )
        {
            return std::strong_ordering::greater;
        }

        return std::strong_ordering::equal;
    }

#endif

    //----------------------------------------------
    // Comparison with nfx::datatypes::Decimal
    //----------------------------------------------

    inline bool Int128::operator!=( const Decimal& val ) const noexcept
    {
        return !( *this == val );
    }

    inline bool Int128::operator<=( const Decimal& val ) const noexcept
    {
        return *this < val || *this == val;
    }

    inline bool Int128::operator>( const Decimal& val ) const noexcept
    {
        return !( *this <= val );
    }

    inline bool Int128::operator>=( const Decimal& val ) const noexcept
    {
        return !( *this < val );
    }

    //----------------------------------------------
    // Arithmetic operators
    //----------------------------------------------

#if NFX_DATATYPES_HAS_NATIVE_INT128

    inline Int128& Int128::operator+=( const Int128& other ) noexcept
    {
        m_value += other.m_value;
        return *this;
    }

    inline Int128& Int128::operator-=( const Int128& other ) noexcept
    {
        m_value -= other.m_value;
        return *this;
    }

    inline Int128& Int128::operator*=( const Int128& other ) noexcept
    {
        m_value *= other.m_value;
        return *this;
    }

    inline Int128& Int128::operator/=( const Int128& other )
    {
        if ( other.m_value == 0 )
        {
            throw std::overflow_error{ "Division by zero" };
        }
        m_value /= other.m_value;
        return *this;
    }

    inline Int128& Int128::operator%=( const Int128& other )
    {
        if ( other.m_value == 0 )
        {
            throw std::overflow_error{ "Division by zero" };
        }
        m_value %= other.m_value;
        return *this;
    }
#else
    Int128 Int128::operator+( const Int128& other ) const noexcept
    {
        // 128-bit addition with carry propagation
        std::uint64_t resultLow{ m_layout.lower64bits + other.m_layout.lower64bits };
        std::uint64_t carry{ ( resultLow < m_layout.lower64bits ) ? constants::BIT_MASK_ONE : constants::BIT_MASK_ZERO };
        std::uint64_t resultHigh{ m_layout.upper64bits + other.m_layout.upper64bits + carry };
        return Int128{ resultLow, resultHigh };
    }

    Int128 Int128::operator-( const Int128& other ) const noexcept
    {
        // 128-bit subtraction with borrow propagation
        std::uint64_t resultLow{ m_layout.lower64bits - other.m_layout.lower64bits };
        std::uint64_t borrow{ ( m_layout.lower64bits < other.m_layout.lower64bits ) ? constants::BIT_MASK_ONE : constants::BIT_MASK_ZERO };
        std::uint64_t resultHigh{ m_layout.upper64bits - other.m_layout.upper64bits - borrow };
        return Int128{ resultLow, resultHigh };
    }

    inline Int128& Int128::operator+=( const Int128& other ) noexcept
    {
        *this = *this + other;
        return *this;
    }

    inline Int128& Int128::operator-=( const Int128& other ) noexcept
    {
        *this = *this - other;
        return *this;
    }

    inline Int128& Int128::operator*=( const Int128& other ) noexcept
    {
        *this = *this * other;
        return *this;
    }

    inline Int128& Int128::operator/=( const Int128& other )
    {
        *this = *this / other;
        return *this;
    }

    inline Int128& Int128::operator%=( const Int128& other )
    {
        if ( other == Int128{} )
        {
            throw std::overflow_error{ "Division by zero" };
        }
        *this = *this % other;
        return *this;
    }

    inline Int128 Int128::operator%( const Int128& other ) const
    {
        if ( other == Int128{} )
        {
            throw std::overflow_error{ "Division by zero" };
        }

        // Performance optimization: Fast modulo for 64-bit values
        if ( m_layout.upper64bits == 0 && other.m_layout.upper64bits == 0 )
        {
            // Both fit in 64-bit - use native modulo
            return Int128{ m_layout.lower64bits % other.m_layout.lower64bits, 0 };
        }

        Int128 quotient{ *this / other };

        return *this - ( quotient * other );
    }

    inline Int128 Int128::operator-() const noexcept
    {
        // Two's complement negation
        Int128 result{ Int128{ ~m_layout.lower64bits, ~m_layout.upper64bits } };

        return Int128{ result + Int128{ 1 } };
    }

    //----------------------------------------------
    // Comparison operations
    //----------------------------------------------

    inline bool Int128::operator==( const Int128& other ) const noexcept
    {
        return m_layout.lower64bits == other.m_layout.lower64bits && m_layout.upper64bits == other.m_layout.upper64bits;
    }

    //----------------------------------------------
    // Comparison with built-in integer types
    //----------------------------------------------

    inline bool Int128::operator==( int val ) const noexcept
    {
        return *this == static_cast<std::int64_t>( val );
    }

    inline bool Int128::operator!=( int val ) const noexcept
    {
        return *this != static_cast<std::int64_t>( val );
    }

    inline bool Int128::operator<( int val ) const noexcept
    {
        return *this < static_cast<std::int64_t>( val );
    }

    inline bool Int128::operator<=( int val ) const noexcept
    {
        return *this <= static_cast<std::int64_t>( val );
    }

    inline bool Int128::operator>( int val ) const noexcept
    {
        return *this > static_cast<std::int64_t>( val );
    }

    inline bool Int128::operator>=( int val ) const noexcept
    {
        return *this >= static_cast<std::int64_t>( val );
    }

    inline bool Int128::operator==( std::int64_t val ) const noexcept
    {
        // For negative values, upper64bits should be all 1s (sign extension)
        // For positive values, upper64bits should be 0
        std::uint64_t expected_upper = ( val < 0 ) ? static_cast<std::uint64_t>( -1 ) : 0;

        return m_layout.upper64bits == expected_upper &&
               m_layout.lower64bits == static_cast<std::uint64_t>( val );
    }

    inline bool Int128::operator!=( std::int64_t val ) const noexcept
    {
        return !( *this == val );
    }

    inline bool Int128::operator<( std::int64_t val ) const noexcept
    {
        // For negative values, upper64bits should be all 1s (sign extension)
        // For positive values, upper64bits should be 0
        std::uint64_t expected_upper = ( val < 0 ) ? static_cast<std::uint64_t>( -1 ) : 0;

        // Compare as signed values for correct ordering
        if ( m_layout.upper64bits != expected_upper )
        {
            return static_cast<std::int64_t>( m_layout.upper64bits ) < static_cast<std::int64_t>( expected_upper );
        }
        return m_layout.lower64bits < static_cast<std::uint64_t>( val );
    }

    inline bool Int128::operator<=( std::int64_t val ) const noexcept
    {
        return *this < val || *this == val;
    }

    inline bool Int128::operator>( std::int64_t val ) const noexcept
    {
        // For negative values, upper64bits should be all 1s (sign extension)
        // For positive values, upper64bits should be 0
        std::uint64_t expected_upper = ( val < 0 ) ? static_cast<std::uint64_t>( -1 ) : 0;

        // Compare as signed values for correct ordering
        if ( m_layout.upper64bits != expected_upper )
        {
            return static_cast<std::int64_t>( m_layout.upper64bits ) > static_cast<std::int64_t>( expected_upper );
        }
        return m_layout.lower64bits > static_cast<std::uint64_t>( val );
    }

    inline bool Int128::operator>=( std::int64_t val ) const noexcept
    {
        return *this > val || *this == val;
    }

    inline bool Int128::operator==( std::uint64_t val ) const noexcept
    {
        // For unsigned comparison, this Int128 must be non-negative
        return m_layout.upper64bits == 0 && m_layout.lower64bits == val;
    }

    inline bool Int128::operator!=( std::uint64_t val ) const noexcept
    {
        return !( *this == val );
    }

    inline bool Int128::operator<( std::uint64_t val ) const noexcept
    {
        // If this is negative, it's always less than any positive uint64_t
        if ( *this < Int128{} )
        {
            return true;
        }

        // If upper bits are non-zero, this is definitely >= 2^64, so greater than any uint64_t
        if ( m_layout.upper64bits != 0 )
        {
            return false;
        }

        // Compare lower bits
        return m_layout.lower64bits < val;
    }

    inline bool Int128::operator<=( std::uint64_t val ) const noexcept
    {
        return *this < val || *this == val;
    }

    inline bool Int128::operator>( std::uint64_t val ) const noexcept
    {
        // If this is negative, it's never greater than any positive uint64_t
        if ( *this < Int128{} )
        {
            return false;
        }

        // If upper bits are non-zero, this is definitely >= 2^64, so greater than any uint64_t
        if ( m_layout.upper64bits != 0 )
        {
            return true;
        }

        // Compare lower bits
        return m_layout.lower64bits > val;
    }

    inline bool Int128::operator>=( std::uint64_t val ) const noexcept
    {
        return *this > val || *this == val;
    }

    //----------------------------------------------
    // Comparison with built-in floating point types
    //----------------------------------------------

    inline bool Int128::operator==( float val ) const noexcept
    {
        return *this == static_cast<double>( val );
    }

    inline bool Int128::operator!=( float val ) const noexcept
    {
        return *this != static_cast<double>( val );
    }

    inline bool Int128::operator<( float val ) const noexcept
    {
        return *this < static_cast<double>( val );
    }

    inline bool Int128::operator<=( float val ) const noexcept
    {
        return *this <= static_cast<double>( val );
    }

    inline bool Int128::operator>( float val ) const noexcept
    {
        return *this > static_cast<double>( val );
    }

    inline bool Int128::operator>=( float val ) const noexcept
    {
        return *this >= static_cast<double>( val );
    }

    inline bool Int128::operator!=( double val ) const noexcept
    {
        return !( *this == val );
    }

    inline bool Int128::operator<=( double val ) const noexcept
    {
        return *this < val || *this == val;
    }

    inline bool Int128::operator>=( double val ) const noexcept
    {
        return *this > val || *this == val;
    }

    //----------------------------------------------
    // Mathematical operations
    //----------------------------------------------

    inline Int128 Int128::abs() const noexcept
    {
        if ( !( *this < Int128{} ) )
        {
            return *this;
        }
        return -*this;
    }

    //----------------------------------------------
    // Property accessors
    //----------------------------------------------

    inline std::uint64_t Int128::toLow() const noexcept
    {
        return m_layout.lower64bits;
    }

    inline std::uint64_t Int128::toHigh() const noexcept
    {
        return m_layout.upper64bits;
    }

    inline std::array<std::int32_t, 4> Int128::toBits() const noexcept
    {
        std::array<std::int32_t, 4> bits{};
        std::uint64_t low = toLow();
        std::uint64_t high = toHigh();
        bits[0] = static_cast<std::int32_t>( low );
        bits[1] = static_cast<std::int32_t>( low >> constants::BITS_PER_UINT32 );
        bits[2] = static_cast<std::int32_t>( high );
        bits[3] = static_cast<std::int32_t>( high >> constants::BITS_PER_UINT32 );
        return bits;
    }
#endif

    //=====================================================================
    // Stream operators
    //=====================================================================

    std::ostream& operator<<( std::ostream& os, const Int128& value );

    std::istream& operator>>( std::istream& is, Int128& value );
} // namespace nfx::datatypes

namespace std
{
    //=====================================================================
    // std::formatter specialization
    //=====================================================================

    template <>
    struct formatter<nfx::datatypes::Int128>
    {
        constexpr auto parse( format_parse_context& ctx )
        {
            return ctx.begin();
        }

        auto format( const nfx::datatypes::Int128& val, format_context& ctx ) const
        {
            return format_to( ctx.out(), "{}", val.toString() );
        }
    };

    //=====================================================================
    // std::numeric_limits specialization
    //=====================================================================

    template <>
    struct numeric_limits<nfx::datatypes::Int128>
    {
        static constexpr bool is_specialized = true;
        static constexpr bool is_signed = true;
        static constexpr bool is_integer = true;
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

        static nfx::datatypes::Int128 min() noexcept
        {
            // Minimum value: -2^127 = -170141183460469231731687303715884105728
            return nfx::datatypes::Int128{
                nfx::datatypes::constants::INT128_MIN_NEGATIVE_LOW,
                nfx::datatypes::constants::INT128_MIN_NEGATIVE_HIGH };
        }

        static nfx::datatypes::Int128 max() noexcept
        {
            // Maximum value: 2^127 - 1 = 170141183460469231731687303715884105727
            return nfx::datatypes::Int128{
                nfx::datatypes::constants::INT128_MAX_POSITIVE_LOW,
                nfx::datatypes::constants::INT128_MAX_POSITIVE_HIGH };
        }

        static nfx::datatypes::Int128 lowest() noexcept
        {
            return min();
        }

        static nfx::datatypes::Int128 epsilon() noexcept
        {
            return nfx::datatypes::Int128{ 0 };
        }

        static constexpr int digits = nfx::datatypes::constants::INT128_DIGITS;             // value bits (excluding sign bit)
        static constexpr int digits10 = nfx::datatypes::constants::INT128_DIGITS10;         // decimal digits (floor(127 * log10(2)))
        static constexpr int max_digits10 = nfx::datatypes::constants::INT128_MAX_DIGITS10; // not applicable for integers
        static constexpr int radix = nfx::datatypes::constants::INT128_RADIX;               // binary
        static constexpr int min_exponent = 0;
        static constexpr int min_exponent10 = 0;
        static constexpr int max_exponent = 0;
        static constexpr int max_exponent10 = 0;
    };
} // namespace std
