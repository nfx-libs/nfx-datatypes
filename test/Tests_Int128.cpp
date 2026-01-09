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
 * @file Tests_Int128.cpp
 * @brief Comprehensive tests for Int128 128-bit signed integer class
 * @details Tests covering construction, arithmetic, comparison, and edge cases
 */

#include <limits>

#include <gtest/gtest.h>

#include <nfx/datatypes/Decimal.h>
#include <nfx/datatypes/Int128.h>

namespace nfx::datatypes::test
{
    //=====================================================================
    // Int128 type tests
    //=====================================================================

    //----------------------------------------------
    // Construction
    //----------------------------------------------

    TEST( Int128Construction, DefaultConstruction )
    {
        datatypes::Int128 zero;
        EXPECT_TRUE( zero == 0 );
        EXPECT_FALSE( zero < 0 );
        EXPECT_EQ( 0ULL, zero.toLow() );
        EXPECT_EQ( 0ULL, zero.toHigh() );
    }

    TEST( Int128Construction, ConstructionFromUint64 )
    {
        const std::uint64_t value{ 0x123456789ABCDEFULL };
        datatypes::Int128 num{ value };

        EXPECT_FALSE( num == 0 );
        EXPECT_FALSE( num < 0 );
        EXPECT_EQ( value, num.toLow() );
        EXPECT_EQ( 0ULL, num.toHigh() );
    }

    TEST( Int128Construction, ConstructionFromInt64Positive )
    {
        const std::int64_t value{ 0x123456789ABCDEFLL };
        datatypes::Int128 num{ value };

        EXPECT_FALSE( num == 0 );
        EXPECT_FALSE( num < 0 );
        EXPECT_EQ( static_cast<std::uint64_t>( value ), num.toLow() );
        EXPECT_EQ( 0ULL, num.toHigh() );
    }

    TEST( Int128Construction, ConstructionFromInt64Negative )
    {
        const std::int64_t value{ -0x123456789ABCDEFLL };
        datatypes::Int128 num{ value };

        EXPECT_FALSE( num == 0 );
        EXPECT_TRUE( num < 0 );
        EXPECT_EQ( static_cast<std::uint64_t>( value ), num.toLow() );
        EXPECT_EQ( constants::INT128_MAX_POSITIVE_LOW, num.toHigh() ); // Sign extension
    }

    TEST( Int128Construction, ConstructionFromUint32 )
    {
        const std::uint32_t value{ 0x12345678U };
        datatypes::Int128 num{ value };

        EXPECT_FALSE( num == 0 );
        EXPECT_FALSE( num < 0 );
        EXPECT_EQ( static_cast<std::uint64_t>( value ), num.toLow() );
        EXPECT_EQ( 0ULL, num.toHigh() );
    }

    TEST( Int128Construction, ConstructionFromIntPositive )
    {
        const int value{ 0x12345678 };
        datatypes::Int128 num{ value };

        EXPECT_FALSE( num == 0 );
        EXPECT_FALSE( num < 0 );
        EXPECT_EQ( static_cast<std::uint64_t>( value ), num.toLow() );
        EXPECT_EQ( 0ULL, num.toHigh() );
    }

    TEST( Int128Construction, ConstructionFromIntNegative )
    {
        const int value{ -0x12345678 };
        datatypes::Int128 num{ value };

        EXPECT_FALSE( num == 0 );
        EXPECT_TRUE( num < 0 );
        EXPECT_EQ( static_cast<std::uint64_t>( value ), num.toLow() );
        EXPECT_EQ( constants::INT128_MAX_POSITIVE_LOW, num.toHigh() ); // Sign extension
    }

    TEST( Int128Construction, ConstructionFromLowHigh )
    {
        const std::uint64_t low{ 0x123456789ABCDEFULL };
        const std::uint64_t high{ 0xFEDCBA9876543210ULL };
        datatypes::Int128 num{ low, high };

        EXPECT_FALSE( num == 0 );
        EXPECT_TRUE( num < 0 );
        EXPECT_EQ( low, num.toLow() );
        EXPECT_EQ( high, num.toHigh() );
    }

    TEST( Int128Construction, ConstructionFromFloat )
    {
        // Test basic truncation behavior (matches C++ static_cast<int>(float))
        float f1 = 42.9f;
        datatypes::Int128 i1{ f1 };
        EXPECT_EQ( i1.toString(), "42" ); // Truncated toward zero
        EXPECT_FALSE( i1 < 0 );
        EXPECT_FALSE( i1 == 0 );

        // Test negative truncation
        float f2 = -42.9f;
        datatypes::Int128 i2{ f2 };
        EXPECT_EQ( i2.toString(), "-42" ); // Truncated toward zero
        EXPECT_TRUE( i2 < 0 );
        EXPECT_FALSE( i2 == 0 );

        // Test exact integer values
        float f3 = 123.0f;
        datatypes::Int128 i3{ f3 };
        EXPECT_EQ( i3.toString(), "123" );
        EXPECT_FALSE( i3 < 0 );

        // Test zero
        float f4 = 0.0f;
        datatypes::Int128 i4{ f4 };
        EXPECT_TRUE( i4 == 0 );
        EXPECT_FALSE( i4 < 0 );

        // Test negative zero
        float f5 = -0.0f;
        datatypes::Int128 i5{ f5 };
        EXPECT_TRUE( i5 == 0 );
        EXPECT_FALSE( i5 < 0 );

        // Test large positive float
        float f6 = 1234567.8f;
        datatypes::Int128 i6{ f6 };
        EXPECT_EQ( i6.toString(), "1234567" ); // Truncated
        EXPECT_FALSE( i6 < 0 );

        // Test large negative float
        float f7 = -9876543.2f;
        datatypes::Int128 i7{ f7 };
        EXPECT_EQ( i7.toString(), "-9876543" ); // Truncated
        EXPECT_TRUE( i7 < 0 );

        // Test special values - NaN should become zero
        float f8 = std::numeric_limits<float>::quiet_NaN();
        datatypes::Int128 i8{ f8 };
        EXPECT_TRUE( i8 == 0 );

        // Test small fractional values
        float f11 = 0.9f;
        datatypes::Int128 i11{ f11 };
        EXPECT_TRUE( i11 == 0 ); // Truncated to zero

        float f12 = -0.9f;
        datatypes::Int128 i12{ f12 };
        EXPECT_TRUE( i12 == 0 ); // Truncated to zero
    }

    TEST( Int128Construction, ConstructionFromDouble )
    {
        // Test basic truncation behavior (matches C++ static_cast<int>(double))
        double d1 = 42.7;
        datatypes::Int128 i1{ d1 };
        EXPECT_EQ( i1.toString(), "42" ); // Truncated toward zero
        EXPECT_FALSE( i1 < 0 );
        EXPECT_FALSE( i1 == 0 );

        // Test negative truncation
        double d2 = -42.7;
        datatypes::Int128 i2{ d2 };
        EXPECT_EQ( i2.toString(), "-42" ); // Truncated toward zero
        EXPECT_TRUE( i2 < 0 );
        EXPECT_FALSE( i2 == 0 );

        // Test exact integer values
        double d3 = 123456.0;
        datatypes::Int128 i3{ d3 };
        EXPECT_EQ( i3.toString(), "123456" );
        EXPECT_FALSE( i3 < 0 );

        // Test zero
        double d4 = 0.0;
        datatypes::Int128 i4{ d4 };
        EXPECT_TRUE( i4 == 0 );
        EXPECT_FALSE( i4 < 0 );

        // Test negative zero
        double d5 = -0.0;
        datatypes::Int128 i5{ d5 };
        EXPECT_TRUE( i5 == 0 );
        EXPECT_FALSE( i5 < 0 );

        // Test large positive double within range
        double d6 = 123456789012345.6;
        datatypes::Int128 i6{ d6 };
        EXPECT_EQ( i6.toString(), "123456789012345" ); // Truncated
        EXPECT_FALSE( i6 < 0 );

        // Test large negative double within range
        double d7 = -987654321098765.4;
        datatypes::Int128 i7{ d7 };
        EXPECT_EQ( i7.toString(), "-987654321098765" ); // Truncated
        EXPECT_TRUE( i7 < 0 );

        // Test very large double (should use string conversion for accuracy)
        double d8 = 1.23456789012345e20; // 20 digits
        datatypes::Int128 i8{ d8 };
        EXPECT_FALSE( i8 == 0 );
        EXPECT_FALSE( i8 < 0 );
        // Result should be close to 123456789012345000000 (truncated)

        // Test special values - NaN should become zero
        double d9 = std::numeric_limits<double>::quiet_NaN();
        datatypes::Int128 i9{ d9 };
        EXPECT_TRUE( i9 == 0 );

        // Test small fractional values
        double d12 = 0.9999;
        datatypes::Int128 i12{ d12 };
        EXPECT_TRUE( i12 == 0 ); // Truncated to zero

        double d13 = -0.9999;
        datatypes::Int128 i13{ d13 };
        EXPECT_TRUE( i13 == 0 ); // Truncated to zero

        // Test edge case: exactly 1.0 - epsilon
        double d14 = 1.0 - std::numeric_limits<double>::epsilon();
        datatypes::Int128 i14{ d14 };
        EXPECT_TRUE( i14 == 0 ); // Should truncate to 0

        // Test edge case: exactly 1.0
        double d15 = 1.0;
        datatypes::Int128 i15{ d15 };
        EXPECT_EQ( i15.toString(), "1" );
        EXPECT_FALSE( i15 < 0 );

        // Test consistency with C++ behavior
        double testValue = 12.6;
        int cpp_result = static_cast<int>( testValue );
        datatypes::Int128 int128_result{ testValue };
        EXPECT_EQ( int128_result.toString(), std::to_string( cpp_result ) );
    }

    TEST( Int128Construction, ConstructionFromDecimal )
    {
        // Test construction from simple positive Decimal
        datatypes::Decimal d1{ 42 };
        datatypes::Int128 i1{ d1 };
        EXPECT_EQ( 42ULL, i1.toLow() );
        EXPECT_EQ( 0ULL, i1.toHigh() );
        EXPECT_FALSE( i1 < 0 );
        EXPECT_FALSE( i1 == 0 );

        // Test construction from simple negative Decimal
        datatypes::Decimal d2{ -123 };
        datatypes::Int128 i2{ d2 };
        EXPECT_TRUE( i2 < 0 );
        EXPECT_FALSE( i2 == 0 );
        EXPECT_EQ( i2.toString(), "-123" );

        // Test construction from zero Decimal
        datatypes::Decimal d3{ 0 };
        datatypes::Int128 i3{ d3 };
        EXPECT_TRUE( i3 == 0 );
        EXPECT_FALSE( i3 < 0 );
        EXPECT_EQ( 0ULL, i3.toLow() );
        EXPECT_EQ( 0ULL, i3.toHigh() );

        // Test construction from large positive Decimal (within Int128 range)
        datatypes::Decimal d4{ "12345678901234567890" };
        datatypes::Int128 i4{ d4 };
        EXPECT_FALSE( i4 == 0 );
        EXPECT_FALSE( i4 < 0 );
        EXPECT_EQ( i4.toString(), "12345678901234567890" );

        // Test construction from large negative Decimal
        datatypes::Decimal d5{ "-9876543210987654321" };
        datatypes::Int128 i5{ d5 };
        EXPECT_FALSE( i5 == 0 );
        EXPECT_TRUE( i5 < 0 );
        EXPECT_EQ( i5.toString(), "-9876543210987654321" );

        // Test construction from Decimal with trailing zeros (should work)
        datatypes::Decimal d6{ "42.0000" }; // Will be normalized to integer
        datatypes::Int128 i6{ d6 };
        EXPECT_FALSE( i6 == 0 );
        EXPECT_FALSE( i6 < 0 );
        EXPECT_EQ( 42ULL, i6.toLow() );

        // Test that construction from Decimal with fractional part truncates (like C++ cast)
        datatypes::Decimal d7{ "42.5" };
        datatypes::Int128 i7{ d7 };
        EXPECT_EQ( i7.toString(), "42" ); // Truncated toward zero

        // Test that construction from Decimal with small fractional part truncates
        datatypes::Decimal d8{ "123.001" };
        datatypes::Int128 i8{ d8 };
        EXPECT_EQ( i8.toString(), "123" ); // Truncated toward zero

        // Test that construction from negative Decimal with fractional part truncates
        datatypes::Decimal d9{ "-456.789" };
        datatypes::Int128 i9{ d9 };
        EXPECT_EQ( i9.toString(), "-456" ); // Truncated toward zero

        // Test construction from very large Decimal (within both types' limits)
        datatypes::Decimal d10{ "1234567890123456789012345678" }; // 28 digits (Decimal limit)
        datatypes::Int128 i10{ d10 };
        EXPECT_FALSE( i10 == 0 );
        EXPECT_FALSE( i10 < 0 );
        EXPECT_EQ( i10.toString(), "1234567890123456789012345678" );

        // Test roundtrip conversion consistency
        datatypes::Decimal original{ "987654321098765432109876" };
        datatypes::Int128 converted{ original };
        datatypes::Decimal backToDecimal{ converted.toString() };
        EXPECT_TRUE( original == backToDecimal );
    }

    //----------------------------------------------
    // Assignment
    //----------------------------------------------

    TEST( Int128Assignment, CompoundAssignment )
    {
        // Addition assignment
        datatypes::Int128 a{ 100 };
        a += datatypes::Int128{ 50 };
        EXPECT_EQ( a, datatypes::Int128{ 150 } );

        a += datatypes::Int128{ -30 };
        EXPECT_EQ( a, datatypes::Int128{ 120 } );

        // Test with large values
        datatypes::Int128 large1{ "123456789012345678901234567890" };
        datatypes::Int128 large2{ "987654321098765432109876543210" };
        large1 += large2;
        EXPECT_EQ( large1.toString(), "1111111110111111111011111111100" );

        // Subtraction assignment
        datatypes::Int128 b{ 200 };
        b -= datatypes::Int128{ 75 };
        EXPECT_EQ( b, datatypes::Int128{ 125 } );

        b -= datatypes::Int128{ -25 };
        EXPECT_EQ( b, datatypes::Int128{ 150 } );

        // Multiplication assignment
        datatypes::Int128 c{ 12 };
        c *= datatypes::Int128{ 5 };
        EXPECT_EQ( c, datatypes::Int128{ 60 } );

        c *= datatypes::Int128{ -2 };
        EXPECT_EQ( c, datatypes::Int128{ -120 } );

        // Test with larger values
        datatypes::Int128 large3{ 1000000000 };
        large3 *= datatypes::Int128{ 1000000000 };
        EXPECT_EQ( large3, datatypes::Int128{ static_cast<std::int64_t>( 1000000000000000000LL ) } );

        // Division assignment
        datatypes::Int128 d{ 100 };
        d /= datatypes::Int128{ 5 };
        EXPECT_EQ( d, datatypes::Int128{ 20 } );

        d /= datatypes::Int128{ -4 };
        EXPECT_EQ( d, datatypes::Int128{ -5 } );

        // Division by zero should throw
        datatypes::Int128 e{ 42 };
        EXPECT_THROW( e /= datatypes::Int128{ 0 }, std::overflow_error );

        // Modulo assignment
        datatypes::Int128 f{ 100 };
        f %= datatypes::Int128{ 7 };
        EXPECT_EQ( f, datatypes::Int128{ 2 } );

        f = datatypes::Int128{ 50 };
        f %= datatypes::Int128{ 8 };
        EXPECT_EQ( f, datatypes::Int128{ 2 } );

        // Modulo by zero should throw
        datatypes::Int128 g{ 42 };
        EXPECT_THROW( g %= datatypes::Int128{ 0 }, std::overflow_error );

        // Chained compound assignments
        datatypes::Int128 h{ 10 };
        h += datatypes::Int128{ 5 };
        h *= datatypes::Int128{ 2 };
        h -= datatypes::Int128{ 10 };
        h /= datatypes::Int128{ 2 };
        EXPECT_EQ( h, datatypes::Int128{ 10 } ); // ((10 + 5) * 2 - 10) / 2 = 10
    }

    TEST( Int128Assignment, UnaryMinus )
    {
        // Basic positive to negative
        datatypes::Int128 a{ 42 };
        datatypes::Int128 negA = -a;
        EXPECT_EQ( negA, datatypes::Int128{ -42 } );
        EXPECT_TRUE( negA < 0 );

        // Negative to positive
        datatypes::Int128 b{ -123 };
        datatypes::Int128 negB = -b;
        EXPECT_EQ( negB, datatypes::Int128{ 123 } );
        EXPECT_FALSE( negB < 0 );

        // Zero remains zero
        datatypes::Int128 zero{ 0 };
        datatypes::Int128 negZero = -zero;
        EXPECT_EQ( negZero, datatypes::Int128{ 0 } );
        EXPECT_TRUE( negZero == 0 );

        // Large positive value
        datatypes::Int128 largePos{ "123456789012345678901234567890" };
        datatypes::Int128 largeNeg = -largePos;
        EXPECT_EQ( largeNeg.toString(), "-123456789012345678901234567890" );
        EXPECT_TRUE( largeNeg < 0 );

        // Double negation
        datatypes::Int128 original{ 777 };
        datatypes::Int128 doubleNeg = -( -original );
        EXPECT_EQ( doubleNeg, original );
    }

    //----------------------------------------------
    // Conversion
    //----------------------------------------------

    TEST( Int128Conversion, ToString )
    {
        // Basic cases
        EXPECT_EQ( datatypes::Int128{ 123 }.toString(), "123" );
        EXPECT_EQ( datatypes::Int128{ -456 }.toString(), "-456" );
        EXPECT_EQ( datatypes::Int128{ 0 }.toString(), "0" );

        // String constructor with valid integer strings
        datatypes::Int128 i1{ "123" };
        EXPECT_EQ( i1.toString(), "123" );

        datatypes::Int128 i2{ "-456" };
        EXPECT_EQ( i2.toString(), "-456" );

        // Large integer string
        datatypes::Int128 i3{ "123456789012345678901234567890" };
        EXPECT_FALSE( i3 == 0 );

        // Decimal strings should throw (Int128 is integer type, not decimal)
        EXPECT_THROW( datatypes::Int128{ "123.456" }, std::invalid_argument );
        EXPECT_THROW( datatypes::Int128{ "-0.001" }, std::invalid_argument );
    }

    TEST( Int128Conversion, ToBits )
    {
        // Test zero
        datatypes::Int128 zero{};
        auto zeroBits{ zero.toBits() };
        EXPECT_EQ( zeroBits.size(), 4 );
        EXPECT_EQ( zeroBits[0], 0 );
        EXPECT_EQ( zeroBits[1], 0 );
        EXPECT_EQ( zeroBits[2], 0 );
        EXPECT_EQ( zeroBits[3], 0 );

        // Test simple positive value
        datatypes::Int128 simple{ 123456 };
        auto simpleBits{ simple.toBits() };
        EXPECT_EQ( simpleBits[0], 123456 );
        EXPECT_EQ( simpleBits[1], 0 );
        EXPECT_EQ( simpleBits[2], 0 );
        EXPECT_EQ( simpleBits[3], 0 );

        // Test negative value (sign extension)
        datatypes::Int128 negative{ -1 };
        auto negativeBits{ negative.toBits() };
        EXPECT_EQ( negativeBits[0], -1 );
        EXPECT_EQ( negativeBits[1], -1 );
        EXPECT_EQ( negativeBits[2], -1 );
        EXPECT_EQ( negativeBits[3], -1 );

        // Test specific negative value
        datatypes::Int128 negativeValue{ -123456 };
        auto negBits{ negativeValue.toBits() };
        EXPECT_EQ( negBits[0], -123456 );
        EXPECT_EQ( negBits[1], -1 ); // Sign extension
        EXPECT_EQ( negBits[2], -1 ); // Sign extension
        EXPECT_EQ( negBits[3], -1 ); // Sign extension

        // Test that toBits() returns 4 elements for any value
        datatypes::Int128 large{ constants::UINT32_MAX_VALUE };
        auto largeBits{ large.toBits() };
        EXPECT_EQ( largeBits.size(), 4 );
        // First element should contain the lower 32 bits
        EXPECT_EQ( largeBits[0], static_cast<std::int32_t>( 0xFFFFFFFF ) );
        EXPECT_EQ( largeBits[1], 0 ); // Should be zero for values fitting in 32 bits
        EXPECT_EQ( largeBits[2], 0 );
        EXPECT_EQ( largeBits[3], 0 );
    }

    //----------------------------------------------
    // Arithmetic
    //----------------------------------------------

    TEST( Int128Arithmetic, Addition )
    {
        datatypes::Int128 a{ 100 };
        datatypes::Int128 b{ 200 };
        datatypes::Int128 result{ a + b };

        EXPECT_EQ( 300ULL, result.toLow() );
        EXPECT_EQ( 0ULL, result.toHigh() );
    }

    TEST( Int128Arithmetic, AdditionWithCarry )
    {
        // Test overflow from low to high word
        datatypes::Int128 a{ constants::INT128_MAX_POSITIVE_LOW, 0 };
        datatypes::Int128 b{ 1 };
        datatypes::Int128 result{ a + b };

        EXPECT_EQ( 0ULL, result.toLow() );
        EXPECT_EQ( 1ULL, result.toHigh() );
    }

    TEST( Int128Arithmetic, Subtraction )
    {
        datatypes::Int128 a{ 300 };
        datatypes::Int128 b{ 100 };
        datatypes::Int128 result{ a - b };

        EXPECT_EQ( 200ULL, result.toLow() );
        EXPECT_EQ( 0ULL, result.toHigh() );
    }

    TEST( Int128Arithmetic, SubtractionWithBorrow )
    {
        // Test borrow from high to low word
        datatypes::Int128 a{ 0, 1 };
        datatypes::Int128 b{ 1 };
        datatypes::Int128 result{ a - b };

        EXPECT_EQ( constants::INT128_MAX_POSITIVE_LOW, result.toLow() );
        EXPECT_EQ( 0ULL, result.toHigh() );
    }

    TEST( Int128Arithmetic, Multiplication )
    {
        datatypes::Int128 a{ 123 };
        datatypes::Int128 b{ 456 };
        datatypes::Int128 result{ a * b };

        EXPECT_EQ( 123ULL * 456ULL, result.toLow() );
        EXPECT_EQ( 0ULL, result.toHigh() );
    }

    TEST( Int128Arithmetic, MultiplicationLarge )
    {
        // Test multiplication that requires high word
        const std::uint64_t a_val{ 0x123456789ABCDEFULL };
        const std::uint64_t b_val{ 0x100000000ULL }; // 2^32

        datatypes::Int128 a{ a_val };
        datatypes::Int128 b{ b_val };
        datatypes::Int128 result{ a * b };

        // Result should be a_val shifted left by 32 bits
        EXPECT_EQ( ( a_val << 32 ) & constants::INT128_MAX_POSITIVE_LOW, result.toLow() );
        EXPECT_EQ( a_val >> 32, result.toHigh() );
    }

    TEST( Int128Arithmetic, Division )
    {
        // Simple 64-bit division
        datatypes::Int128 a{ 456 };
        datatypes::Int128 b{ 123 };
        datatypes::Int128 result{ a / b };

        EXPECT_EQ( 3ULL, result.toLow() );
        EXPECT_EQ( 0ULL, result.toHigh() );

        // Large 64-bit division
        datatypes::Int128 large1{ static_cast<uint64_t>( 1000000000000ULL ) };
        datatypes::Int128 large2{ static_cast<uint64_t>( 3ULL ) };
        datatypes::Int128 result2{ large1 / large2 };
        EXPECT_EQ( 333333333333ULL, result2.toLow() );
        EXPECT_EQ( 0ULL, result2.toHigh() );
        // Division with 128-bit dividend, 64-bit divisor
        // This is the CRITICAL test case that's failing in Decimal division!
        // dividend = 2 * 10^37 = 20000000000000000000000000000000000000
        // divisor = 1416666666666666665
        // Expected quotient ≈ 14117647058823529428
        datatypes::Int128 bigDividend{ "20000000000000000000000000000000000000" };
        datatypes::Int128 divisor{ static_cast<uint64_t>( 1416666666666666665ULL ) };
        datatypes::Int128 quotient{ bigDividend / divisor };
        datatypes::Int128 expected{ "14117647058823529428" };
        EXPECT_EQ( expected, quotient ) << "128-bit dividend / 64-bit divisor failed!";

        // Another 128-bit test case
        datatypes::Int128 big1{ "1000000000000000000000" }; // 10^21
        datatypes::Int128 big2{ static_cast<uint64_t>( 7ULL ) };
        datatypes::Int128 result3{ big1 / big2 };
        datatypes::Int128 expected2{ "142857142857142857142" }; // 10^21 / 7
        EXPECT_EQ( expected2, result3 );
    }

    TEST( Int128Arithmetic, DivisionByZero )
    {
        datatypes::Int128 a{ 123 };
        datatypes::Int128 zero{};

        EXPECT_THROW( a / zero, std::overflow_error );
    }

    TEST( Int128Arithmetic, Modulo )
    {
        datatypes::Int128 a{ 456 };
        datatypes::Int128 b{ 123 };
        datatypes::Int128 result{ a % b };

        // 456 % 123 = 87
        EXPECT_EQ( 87ULL, result.toLow() );
        EXPECT_EQ( 0ULL, result.toHigh() );
    }

    TEST( Int128Arithmetic, ModuloByZero )
    {
        datatypes::Int128 a{ 123 };
        datatypes::Int128 zero{};

        EXPECT_THROW( a % zero, std::overflow_error );
    }

    TEST( Int128Arithmetic, UnaryMinus )
    {
        datatypes::Int128 positive{ 123 };
        datatypes::Int128 negative{ -positive };

        EXPECT_TRUE( negative < 0 );
        EXPECT_FALSE( negative == 0 );

        // Two's complement: ~123 + 1
        EXPECT_EQ( static_cast<std::uint64_t>( -123 ), negative.toLow() );
        EXPECT_EQ( constants::INT128_MAX_POSITIVE_LOW, negative.toHigh() );
    }

    TEST( Int128Arithmetic, UnaryMinusZero )
    {
        datatypes::Int128 zero{};
        datatypes::Int128 negated{ -zero };

        EXPECT_TRUE( negated == 0 );
        EXPECT_FALSE( negated < 0 );
    }

    TEST( Int128Arithmetic, AbsoluteValue )
    {
        datatypes::Int128 positive{ 123 };
        datatypes::Int128 negative{ -123 };
        datatypes::Int128 zero{};

        EXPECT_EQ( positive, positive.abs() );
        EXPECT_EQ( positive, negative.abs() );
        EXPECT_EQ( zero, zero.abs() );
    }

    TEST( Int128Arithmetic, IntegerSquareRoot )
    {
        // Test perfect squares
        datatypes::Int128 d1{ 4 };
        auto isqrtValue{ d1.isqrt() };
        EXPECT_EQ( isqrtValue, datatypes::Int128{ 2 } );

        datatypes::Int128 d2{ 9 };
        isqrtValue = d2.isqrt();
        EXPECT_EQ( isqrtValue, datatypes::Int128{ 3 } );

        datatypes::Int128 d3{ 25 };
        isqrtValue = d3.isqrt();
        EXPECT_EQ( isqrtValue, datatypes::Int128{ 5 } );

        datatypes::Int128 d4{ 100 };
        isqrtValue = d4.isqrt();
        EXPECT_EQ( isqrtValue, datatypes::Int128{ 10 } );

        // Test zero
        datatypes::Int128 d5{ 0 };
        isqrtValue = d5.isqrt();
        EXPECT_EQ( isqrtValue, datatypes::Int128{ 0 } );

        // Test one
        datatypes::Int128 d6{ 1 };
        isqrtValue = d6.isqrt();
        EXPECT_EQ( isqrtValue, datatypes::Int128{ 1 } );

        // Test non-perfect squares (floor behavior)
        datatypes::Int128 d7{ 5 };
        isqrtValue = d7.isqrt();
        EXPECT_EQ( isqrtValue, datatypes::Int128{ 2 } ); // floor(sqrt(5)) = 2

        datatypes::Int128 d8{ 10 };
        isqrtValue = d8.isqrt();
        EXPECT_EQ( isqrtValue, datatypes::Int128{ 3 } ); // floor(sqrt(10)) = 3

        datatypes::Int128 d9{ 99 };
        isqrtValue = d9.isqrt();
        EXPECT_EQ( isqrtValue, datatypes::Int128{ 9 } ); // floor(sqrt(99)) = 9

        datatypes::Int128 d10{ 101 };
        isqrtValue = d10.isqrt();
        EXPECT_EQ( isqrtValue, datatypes::Int128{ 10 } ); // floor(sqrt(101)) = 10

        // Test larger values
        datatypes::Int128 d11{ 10000 };
        isqrtValue = d11.isqrt();
        EXPECT_EQ( isqrtValue, datatypes::Int128{ 100 } );

        // Test that result squared is <= original
        datatypes::Int128 d12{ 50 };
        isqrtValue = d12.isqrt();
        datatypes::Int128 squared{ isqrtValue * isqrtValue };
        EXPECT_TRUE( squared <= d12 );
        // And (result+1)^2 > original
        datatypes::Int128 nextSquared{ ( isqrtValue + datatypes::Int128{ 1 } ) * ( isqrtValue + datatypes::Int128{ 1 } ) };
        EXPECT_TRUE( nextSquared > d12 );

        // Test that isqrt() throws for negative values
        datatypes::Int128 d13{ -4 };
        EXPECT_THROW( [[maybe_unused]] auto result = d13.isqrt(), std::domain_error );

        // Test very large value
        datatypes::Int128 d14{ "1000000000000000000" }; // 10^18
        isqrtValue = d14.isqrt();
        EXPECT_EQ( isqrtValue, datatypes::Int128{ 1000000000 } ); // 10^9
    }

    TEST( Int128Arithmetic, SquareRootFunction )
    {
        // Test perfect square (4) - should be exact
        datatypes::Int128 d1{ 4 };
        auto sqrtValue = datatypes::sqrt( d1 );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 2 } );

        // Test perfect square (9) - should be exact
        datatypes::Int128 d2{ 9 };
        sqrtValue = datatypes::sqrt( d2 );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 3 } );

        // Test perfect square (100) - should be exact
        datatypes::Int128 d3{ 100 };
        sqrtValue = datatypes::sqrt( d3 );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 10 } );

        // Test zero
        datatypes::Int128 d4{ 0 };
        sqrtValue = datatypes::sqrt( d4 );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 0 } );

        // Test one
        datatypes::Int128 d5{ 1 };
        sqrtValue = datatypes::sqrt( d5 );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 1 } );

        // Test non-perfect square with high precision
        datatypes::Int128 d6{ 2 };
        sqrtValue = datatypes::sqrt( d6 );
        // sqrt(2) ≈ 1.414213562373095048...
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "1.414" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "1.415" } );

        // Test non-perfect square (sqrt(3))
        datatypes::Int128 d6b{ 3 };
        sqrtValue = datatypes::sqrt( d6b );
        // sqrt(3) ≈ 1.732050807568877...
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "1.732" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "1.733" } );

        // Test non-perfect square (sqrt(50))
        datatypes::Int128 d6c{ 50 };
        sqrtValue = datatypes::sqrt( d6c );
        // sqrt(50) ≈ 7.071067811865476...
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "7.07" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "7.08" } );

        // Test that sqrt() throws for negative values
        datatypes::Int128 d7{ -4 };
        EXPECT_THROW( [[maybe_unused]] auto result = datatypes::sqrt( d7 ), std::domain_error );

        // Test large perfect square (10^10)
        datatypes::Int128 d8{ "10000000000" }; // 10^10
        sqrtValue = datatypes::sqrt( d8 );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 100000 } );
    }

    TEST( Int128Arithmetic, SquareRootFunctionBigIntegers )
    {
        // Test large perfect square: 10^20 (sqrt = 10^10)
        datatypes::Int128 d1{ "100000000000000000000" }; // 10^20
        auto sqrtValue = datatypes::sqrt( d1 );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "10000000000" } ); // 10^10

        // Test very large perfect square: (10^18)^2 = 10^36
        datatypes::Int128 d2{ "1000000000000000000" }; // 10^18
        datatypes::Int128 d2Squared = d2 * d2;         // 10^36
        sqrtValue = datatypes::sqrt( d2Squared );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ d2 } );

        // Test another large perfect square: 999999999^2
        datatypes::Int128 d4{ 999999999 };
        datatypes::Int128 d4Squared = d4 * d4;
        sqrtValue = datatypes::sqrt( d4Squared );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 999999999 } );

        // Test large perfect square: 123456789^2
        datatypes::Int128 d5{ 123456789 };
        datatypes::Int128 d5Squared = d5 * d5; // 15241578750190521
        sqrtValue = datatypes::sqrt( d5Squared );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 123456789 } );
        sqrtValue = datatypes::sqrt( d4Squared );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 999999999 } );
    }

    TEST( Int128Arithmetic, SquareRootEdgeCases )
    {
        // Test very small perfect squares
        datatypes::Int128 v1{ 1 };
        auto sqrtValue = datatypes::sqrt( v1 );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 1 } );

        datatypes::Int128 v2{ 4 };
        sqrtValue = datatypes::sqrt( v2 );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 2 } );

        datatypes::Int128 v3{ 16 };
        sqrtValue = datatypes::sqrt( v3 );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 4 } );

        datatypes::Int128 v4{ 25 };
        sqrtValue = datatypes::sqrt( v4 );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 5 } );

        datatypes::Int128 v5{ 36 };
        sqrtValue = datatypes::sqrt( v5 );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 6 } );

        datatypes::Int128 v6{ 49 };
        sqrtValue = datatypes::sqrt( v6 );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 7 } );

        datatypes::Int128 v7{ 64 };
        sqrtValue = datatypes::sqrt( v7 );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 8 } );

        datatypes::Int128 v8{ 81 };
        sqrtValue = datatypes::sqrt( v8 );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 9 } );

        datatypes::Int128 v9{ 100 };
        sqrtValue = datatypes::sqrt( v9 );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 10 } );

        // Test medium perfect squares
        datatypes::Int128 v10{ 144 };
        sqrtValue = datatypes::sqrt( v10 );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 12 } );

        datatypes::Int128 v11{ 169 };
        sqrtValue = datatypes::sqrt( v11 );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 13 } );

        datatypes::Int128 v12{ 225 };
        sqrtValue = datatypes::sqrt( v12 );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 15 } );

        datatypes::Int128 v13{ 256 };
        sqrtValue = datatypes::sqrt( v13 );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 16 } );

        datatypes::Int128 v14{ 289 };
        sqrtValue = datatypes::sqrt( v14 );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 17 } );

        datatypes::Int128 v15{ 324 };
        sqrtValue = datatypes::sqrt( v15 );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 18 } );

        datatypes::Int128 v16{ 361 };
        sqrtValue = datatypes::sqrt( v16 );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 19 } );

        datatypes::Int128 v17{ 400 };
        sqrtValue = datatypes::sqrt( v17 );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 20 } );

        // Test large perfect squares
        datatypes::Int128 v18{ 10000 };
        sqrtValue = datatypes::sqrt( v18 );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 100 } );

        datatypes::Int128 v19{ 1000000 };
        sqrtValue = datatypes::sqrt( v19 );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 1000 } );

        datatypes::Int128 v20{ "1000000000000" }; // 10^12
        sqrtValue = datatypes::sqrt( v20 );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 1000000 } );

        // Test very large perfect squares
        datatypes::Int128 v21{ "1000000000000000000" }; // 10^18
        sqrtValue = datatypes::sqrt( v21 );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "1000000000" } ); // 10^9

        datatypes::Int128 v22{ "100000000000000000000" }; // 10^20
        sqrtValue = datatypes::sqrt( v22 );
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "10000000000" } ); // 10^10
    }

    TEST( Int128Arithmetic, SquareRootNonPerfectSquares )
    {
        // Test small non-perfect squares
        datatypes::Int128 n1{ 2 };
        auto sqrtValue = datatypes::sqrt( n1 );
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "1.41" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "1.42" } );

        datatypes::Int128 n2{ 3 };
        sqrtValue = datatypes::sqrt( n2 );
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "1.73" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "1.74" } );

        datatypes::Int128 n3{ 5 };
        sqrtValue = datatypes::sqrt( n3 );
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "2.23" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "2.24" } );

        datatypes::Int128 n4{ 6 };
        sqrtValue = datatypes::sqrt( n4 );
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "2.44" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "2.45" } );

        datatypes::Int128 n5{ 7 };
        sqrtValue = datatypes::sqrt( n5 );
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "2.64" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "2.65" } );

        datatypes::Int128 n6{ 8 };
        sqrtValue = datatypes::sqrt( n6 );
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "2.82" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "2.83" } );

        datatypes::Int128 n7{ 10 };
        sqrtValue = datatypes::sqrt( n7 );
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "3.16" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "3.17" } );

        // Test medium non-perfect squares
        datatypes::Int128 n8{ 50 };
        sqrtValue = datatypes::sqrt( n8 );
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "7.07" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "7.08" } );

        datatypes::Int128 n9{ 99 };
        sqrtValue = datatypes::sqrt( n9 );
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "9.94" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "9.95" } );

        datatypes::Int128 n10{ 101 };
        sqrtValue = datatypes::sqrt( n10 );
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "10.04" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "10.05" } );

        datatypes::Int128 n11{ 200 };
        sqrtValue = datatypes::sqrt( n11 );
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "14.14" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "14.15" } );

        datatypes::Int128 n12{ 500 };
        sqrtValue = datatypes::sqrt( n12 );
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "22.36" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "22.37" } );

        datatypes::Int128 n13{ 1000 };
        sqrtValue = datatypes::sqrt( n13 );
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "31.62" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "31.63" } );

        // Test large non-perfect squares
        datatypes::Int128 n14{ 9999 };
        sqrtValue = datatypes::sqrt( n14 );
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "99.99" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "100.00" } );

        datatypes::Int128 n15{ 10001 };
        sqrtValue = datatypes::sqrt( n15 );
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "100.00" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "100.01" } );

        datatypes::Int128 n16{ 999999 };
        sqrtValue = datatypes::sqrt( n16 );
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "999.99" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "1000.00" } );

        datatypes::Int128 n17{ 1000001 };
        sqrtValue = datatypes::sqrt( n17 );
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "1000.00" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "1000.01" } );

        // Test very large non-perfect squares
        datatypes::Int128 n18{ "9999999999999999" }; // Just under 10^16
        sqrtValue = datatypes::sqrt( n18 );
        datatypes::Decimal decTest{ n18 };
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "99999999.9" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "100000000.1" } );

        datatypes::Int128 n19{ "10000000000000001" }; // Just over 10^16
        sqrtValue = datatypes::sqrt( n19 );
        datatypes::Decimal decTest2{ n19 };
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "99999999.9" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "100000000.1" } );
    }

    TEST( Int128Arithmetic, IntegerSquareRootEdgeCases )
    {
        // Test isqrt with very small values
        datatypes::Int128 v1{ 0 };
        EXPECT_EQ( v1.isqrt(), datatypes::Int128{ 0 } );

        datatypes::Int128 v2{ 1 };
        EXPECT_EQ( v2.isqrt(), datatypes::Int128{ 1 } );

        datatypes::Int128 v3{ 2 };
        EXPECT_EQ( v3.isqrt(), datatypes::Int128{ 1 } );

        datatypes::Int128 v4{ 3 };
        EXPECT_EQ( v4.isqrt(), datatypes::Int128{ 1 } );

        datatypes::Int128 v5{ 4 };
        EXPECT_EQ( v5.isqrt(), datatypes::Int128{ 2 } );

        // Test isqrt with medium values
        datatypes::Int128 v6{ 15 };
        EXPECT_EQ( v6.isqrt(), datatypes::Int128{ 3 } );

        datatypes::Int128 v7{ 16 };
        EXPECT_EQ( v7.isqrt(), datatypes::Int128{ 4 } );

        datatypes::Int128 v8{ 17 };
        EXPECT_EQ( v8.isqrt(), datatypes::Int128{ 4 } );

        datatypes::Int128 v9{ 99 };
        EXPECT_EQ( v9.isqrt(), datatypes::Int128{ 9 } );

        datatypes::Int128 v10{ 100 };
        EXPECT_EQ( v10.isqrt(), datatypes::Int128{ 10 } );

        datatypes::Int128 v11{ 101 };
        EXPECT_EQ( v11.isqrt(), datatypes::Int128{ 10 } );

        // Test isqrt with large values
        datatypes::Int128 v12{ 9999 };
        EXPECT_EQ( v12.isqrt(), datatypes::Int128{ 99 } );

        datatypes::Int128 v13{ 10000 };
        EXPECT_EQ( v13.isqrt(), datatypes::Int128{ 100 } );

        datatypes::Int128 v14{ 10001 };
        EXPECT_EQ( v14.isqrt(), datatypes::Int128{ 100 } );

        datatypes::Int128 v15{ 999999 };
        EXPECT_EQ( v15.isqrt(), datatypes::Int128{ 999 } );

        datatypes::Int128 v16{ 1000000 };
        EXPECT_EQ( v16.isqrt(), datatypes::Int128{ 1000 } );

        datatypes::Int128 v17{ 1000001 };
        EXPECT_EQ( v17.isqrt(), datatypes::Int128{ 1000 } );

        // Test isqrt with very large values
        datatypes::Int128 v18{ "999999999999" };                          // Just under 10^12
        datatypes::Int128 expectedSqrt18 = datatypes::Int128{ "999999" }; // sqrt(10^12) ≈ 10^6
        EXPECT_EQ( v18.isqrt(), expectedSqrt18 );

        datatypes::Int128 v19{ "1000000000000" };               // 10^12
        EXPECT_EQ( v19.isqrt(), datatypes::Int128{ 1000000 } ); // 10^6

        datatypes::Int128 v20{ "1000000000001" }; // Just over 10^12
        EXPECT_EQ( v20.isqrt(), datatypes::Int128{ 1000000 } );

        // Test isqrt with maximum representable perfect squares
        datatypes::Int128 v21{ "10000000000000000" };               // 10^16
        EXPECT_EQ( v21.isqrt(), datatypes::Int128{ "100000000" } ); // 10^8

        datatypes::Int128 v22{ "1000000000000000000" };              // 10^18
        EXPECT_EQ( v22.isqrt(), datatypes::Int128{ "1000000000" } ); // 10^9

        // Test that isqrt throws for negative values
        datatypes::Int128 neg{ -1 };
        EXPECT_THROW( [[maybe_unused]] auto result = neg.isqrt(), std::domain_error );

        datatypes::Int128 neg2{ -100 };
        EXPECT_THROW( [[maybe_unused]] auto result = neg2.isqrt(), std::domain_error );
    }

    //----------------------------------------------
    // Arithmetic -free functions
    //----------------------------------------------

    TEST( Int128FreeFunctions, AbsFunction )
    {
        using namespace nfx;

        // Test positive value
        datatypes::Int128 pos{ 42 };
        EXPECT_EQ( abs( pos ), datatypes::Int128{ 42 } );

        // Test negative value
        datatypes::Int128 neg{ -42 };
        EXPECT_EQ( abs( neg ), datatypes::Int128{ 42 } );

        // Test zero
        datatypes::Int128 zero{ 0 };
        EXPECT_EQ( abs( zero ), datatypes::Int128{ 0 } );

        // Test large positive value
        datatypes::Int128 largePos{ "12345678901234567890" };
        EXPECT_EQ( abs( largePos ), largePos );

        // Test large negative value
        datatypes::Int128 largeNeg{ "-12345678901234567890" };
        EXPECT_EQ( abs( largeNeg ), datatypes::Int128{ "12345678901234567890" } );
    }

    TEST( Int128FreeFunctions, IsqrtFunction )
    {
        using namespace nfx;

        // Test perfect squares
        EXPECT_EQ( isqrt( datatypes::Int128{ 0 } ), datatypes::Int128{ 0 } );
        EXPECT_EQ( isqrt( datatypes::Int128{ 1 } ), datatypes::Int128{ 1 } );
        EXPECT_EQ( isqrt( datatypes::Int128{ 4 } ), datatypes::Int128{ 2 } );
        EXPECT_EQ( isqrt( datatypes::Int128{ 9 } ), datatypes::Int128{ 3 } );
        EXPECT_EQ( isqrt( datatypes::Int128{ 16 } ), datatypes::Int128{ 4 } );
        EXPECT_EQ( isqrt( datatypes::Int128{ 100 } ), datatypes::Int128{ 10 } );

        // Test non-perfect squares (should return floor)
        EXPECT_EQ( isqrt( datatypes::Int128{ 2 } ), datatypes::Int128{ 1 } );
        EXPECT_EQ( isqrt( datatypes::Int128{ 3 } ), datatypes::Int128{ 1 } );
        EXPECT_EQ( isqrt( datatypes::Int128{ 8 } ), datatypes::Int128{ 2 } );
        EXPECT_EQ( isqrt( datatypes::Int128{ 15 } ), datatypes::Int128{ 3 } );
        EXPECT_EQ( isqrt( datatypes::Int128{ 99 } ), datatypes::Int128{ 9 } );

        // Test large perfect square that doesn't overflow
        // Use 10^9 instead of 10^10 to avoid overflow: (10^9)^2 = 10^18 fits in Int128
        datatypes::Int128 largeSquare{ "1000000000" };              // 10^9
        datatypes::Int128 largeSquared = largeSquare * largeSquare; // 10^18
        EXPECT_EQ( isqrt( largeSquared ), largeSquare );

        // Test maximum Int128 value: sqrt(2^127 - 1) ≈ 13043817825332782212 (floor)
        datatypes::Int128 maxValue = std::numeric_limits<datatypes::Int128>::max();
        datatypes::Int128 maxSqrt = isqrt( maxValue );
        // Verify: maxSqrt^2 <= maxValue
        EXPECT_LE( maxSqrt * maxSqrt, maxValue );
        // Verify: (maxSqrt+1)^2 > maxValue by checking maxValue - maxSqrt^2 < 2*maxSqrt + 1
        // This avoids overflow in (maxSqrt+1)^2 calculation
        datatypes::Int128 remainder = maxValue - ( maxSqrt * maxSqrt );
        datatypes::Int128 nextIncrement = datatypes::Int128{ 2 } * maxSqrt + datatypes::Int128{ 1 };
        EXPECT_LT( remainder, nextIncrement );
        // Expected value for sqrt(2^127 - 1)
        datatypes::Int128 expectedMaxSqrt{ "13043817825332782212" };
        EXPECT_EQ( maxSqrt, expectedMaxSqrt );

        // Test negative value throws
        datatypes::Int128 negative{ -1 };
        EXPECT_THROW( [[maybe_unused]] auto result = isqrt( negative ), std::domain_error );
    }

    TEST( Int128FreeFunctions, SqrtFunction )
    {
        using namespace nfx;

        // Test perfect squares
        datatypes::Decimal result1 = sqrt( datatypes::Int128{ 4 } );
        EXPECT_EQ( result1, datatypes::Decimal{ "2" } );

        datatypes::Decimal result2 = sqrt( datatypes::Int128{ 100 } );
        EXPECT_EQ( result2, datatypes::Decimal{ "10" } );

        // Test non-perfect square (should return high-precision decimal)
        datatypes::Decimal result3 = sqrt( datatypes::Int128{ 2 } );
        EXPECT_TRUE( result3 > datatypes::Decimal{ "1.414213" } );
        EXPECT_TRUE( result3 < datatypes::Decimal{ "1.414214" } );

        // Test zero
        datatypes::Decimal result4 = sqrt( datatypes::Int128{ 0 } );
        EXPECT_EQ( result4, datatypes::Decimal{ "0" } );

        // Test negative value throws
        datatypes::Int128 negative{ -1 };
        EXPECT_THROW( [[maybe_unused]] auto result = sqrt( negative ), std::domain_error );
    }

    TEST( Int128FreeFunctions, ADLSupport )
    {
        Int128 negative{ -42 };

        // abs via ADL
        auto absResult = nfx::datatypes::abs( negative );
        EXPECT_EQ( absResult, Int128{ 42 } );

        // isqrt via ADL
        auto isqrtResult = nfx::datatypes::isqrt( Int128{ 16 } );
        EXPECT_EQ( isqrtResult, Int128{ 4 } );

        // sqrt via ADL (returns Decimal)
        auto sqrtResult = nfx::datatypes::sqrt( Int128{ 9 } );
        EXPECT_EQ( sqrtResult, Decimal{ "3" } );
    }

    //----------------------------------------------
    // Comparison
    //----------------------------------------------

    TEST( Int128Comparison, Equality )
    {
        datatypes::Int128 a{ 123 };
        datatypes::Int128 b{ 123 };
        datatypes::Int128 c{ 456 };

        EXPECT_TRUE( a == b );
        EXPECT_FALSE( a == c );
    }

    TEST( Int128Comparison, Inequality )
    {
        datatypes::Int128 a{ 123 };
        datatypes::Int128 b{ 123 };
        datatypes::Int128 c{ 456 };

        EXPECT_FALSE( a != b );
        EXPECT_TRUE( a != c );
    }

    TEST( Int128Comparison, LessThan )
    {
        datatypes::Int128 a{ 123 };
        datatypes::Int128 b{ 456 };

        EXPECT_TRUE( a < b );
        EXPECT_FALSE( b < a );
        EXPECT_FALSE( a < a );
    }

    TEST( Int128Comparison, LessThanSignedComparison )
    {
        datatypes::Int128 positive{ 123 };
        datatypes::Int128 negative{ -456 };

        EXPECT_TRUE( negative < positive );
        EXPECT_FALSE( positive < negative );
    }

    TEST( Int128Comparison, LessEqual )
    {
        datatypes::Int128 a{ 123 };
        datatypes::Int128 b{ 456 };
        datatypes::Int128 c{ 123 };

        EXPECT_TRUE( a <= b );
        EXPECT_TRUE( a <= c );
        EXPECT_FALSE( b <= a );
    }

    TEST( Int128Comparison, GreaterThan )
    {
        datatypes::Int128 a{ 123 };
        datatypes::Int128 b{ 456 };

        EXPECT_FALSE( a > b );
        EXPECT_TRUE( b > a );
        EXPECT_FALSE( a > a );
    }

    TEST( Int128Comparison, GreaterEqual )
    {
        datatypes::Int128 a{ 123 };
        datatypes::Int128 b{ 456 };
        datatypes::Int128 c{ 123 };

        EXPECT_FALSE( a >= b );
        EXPECT_TRUE( a >= c );
        EXPECT_TRUE( b >= a );
    }

    TEST( Int128Comparison, ComparisonHighWord )
    {
        // Test comparison when high words differ
        datatypes::Int128 a{ constants::INT128_MAX_POSITIVE_LOW, 0 };
        datatypes::Int128 b{ 0, 1 };

        EXPECT_TRUE( a < b );
        EXPECT_FALSE( b < a );
    }

    //----------------------------------------------
    // Comparison with built-in integer types
    //----------------------------------------------

    TEST( Int128ComparisonBuiltinTypes, ComparisonWithInt64 )
    {
        datatypes::Int128 int128_42{ 42 };
        datatypes::Int128 int128_negative{ -123 };
        datatypes::Int128 int128_zero{ 0 };

        std::int64_t int64_42 = 42;
        std::int64_t int64_100 = 100;
        std::int64_t int64_negative = -123;
        std::int64_t int64_zero = 0;

        // Equality tests
        EXPECT_TRUE( int128_42 == int64_42 );
        EXPECT_FALSE( int128_42 == int64_100 );
        EXPECT_TRUE( int128_negative == int64_negative );
        EXPECT_TRUE( int128_zero == int64_zero );

        // Inequality tests
        EXPECT_FALSE( int128_42 != int64_42 );
        EXPECT_TRUE( int128_42 != int64_100 );
        EXPECT_FALSE( int128_negative != int64_negative );

        // Less than tests
        EXPECT_TRUE( int128_42 < int64_100 );
        EXPECT_FALSE( int128_42 < int64_42 );
        EXPECT_TRUE( int128_negative < int64_42 );
        EXPECT_TRUE( int128_negative < int64_zero );

        // Less than or equal tests
        EXPECT_TRUE( int128_42 <= int64_42 );
        EXPECT_TRUE( int128_42 <= int64_100 );
        EXPECT_FALSE( int128_42 <= int64_negative );

        // Greater than tests
        EXPECT_FALSE( int128_42 > int64_100 );
        EXPECT_FALSE( int128_42 > int64_42 );
        EXPECT_TRUE( int128_42 > int64_negative );
        EXPECT_TRUE( int128_zero > int64_negative );

        // Greater than or equal tests
        EXPECT_TRUE( int128_42 >= int64_42 );
        EXPECT_FALSE( int128_42 >= int64_100 );
        EXPECT_TRUE( int128_42 >= int64_negative );
    }

    TEST( Int128ComparisonBuiltinTypes, ComparisonWithUint64 )
    {
        datatypes::Int128 int128_42{ 42 };
        datatypes::Int128 int128_large{ static_cast<std::uint64_t>( 0x123456789ABCDEFULL ) };
        datatypes::Int128 int128_negative{ -123 };
        datatypes::Int128 int128_zero{ 0 };

        std::uint64_t uint64_42 = 42;
        std::uint64_t uint64_100 = 100;
        std::uint64_t uint64_large = 0x123456789ABCDEFULL;
        std::uint64_t uint64_zero = 0;

        // Equality tests
        EXPECT_TRUE( int128_42 == uint64_42 );
        EXPECT_FALSE( int128_42 == uint64_100 );
        EXPECT_TRUE( int128_large == uint64_large );
        EXPECT_TRUE( int128_zero == uint64_zero );
        EXPECT_FALSE( int128_negative == uint64_42 ); // Negative Int128 never equals positive uint64_t

        // Inequality tests
        EXPECT_FALSE( int128_42 != uint64_42 );
        EXPECT_TRUE( int128_42 != uint64_100 );
        EXPECT_TRUE( int128_negative != uint64_42 );

        // Less than tests with negative Int128
        EXPECT_TRUE( int128_negative < uint64_zero ); // Negative is always less than any positive uint64_t
        EXPECT_TRUE( int128_negative < uint64_42 );
        EXPECT_TRUE( int128_negative < uint64_100 );

        // Less than tests with positive Int128
        EXPECT_TRUE( int128_42 < uint64_100 );
        EXPECT_FALSE( int128_42 < uint64_42 );
        EXPECT_FALSE( int128_large < uint64_42 );

        // Less than or equal tests
        EXPECT_TRUE( int128_42 <= uint64_42 );
        EXPECT_TRUE( int128_42 <= uint64_100 );
        EXPECT_TRUE( int128_negative <= uint64_zero );

        // Greater than tests with negative Int128
        EXPECT_FALSE( int128_negative > uint64_zero ); // Negative is never greater than positive uint64_t
        EXPECT_FALSE( int128_negative > uint64_42 );

        // Greater than tests with positive Int128
        EXPECT_FALSE( int128_42 > uint64_100 );
        EXPECT_FALSE( int128_42 > uint64_42 );
        EXPECT_TRUE( int128_large > uint64_42 );

        // Greater than or equal tests
        EXPECT_TRUE( int128_42 >= uint64_42 );
        EXPECT_FALSE( int128_42 >= uint64_100 );
        EXPECT_FALSE( int128_negative >= uint64_zero );
    }

    TEST( Int128ComparisonBuiltinTypes, ComparisonWithInt )
    {
        datatypes::Int128 int128_42{ 42 };
        datatypes::Int128 int128_negative{ -123 };
        datatypes::Int128 int128_zero{ 0 };

        int int_42 = 42;
        int int_100 = 100;
        int int_negative = -123;
        int int_zero = 0;

        // Equality tests
        EXPECT_TRUE( int128_42 == int_42 );
        EXPECT_FALSE( int128_42 == int_100 );
        EXPECT_TRUE( int128_negative == int_negative );
        EXPECT_TRUE( int128_zero == int_zero );

        // Inequality tests
        EXPECT_FALSE( int128_42 != int_42 );
        EXPECT_TRUE( int128_42 != int_100 );
        EXPECT_FALSE( int128_negative != int_negative );

        // Less than tests
        EXPECT_TRUE( int128_42 < int_100 );
        EXPECT_FALSE( int128_42 < int_42 );
        EXPECT_TRUE( int128_negative < int_42 );
        EXPECT_TRUE( int128_negative < int_zero );

        // Less than or equal tests
        EXPECT_TRUE( int128_42 <= int_42 );
        EXPECT_TRUE( int128_42 <= int_100 );
        EXPECT_FALSE( int128_42 <= int_negative );

        // Greater than tests
        EXPECT_FALSE( int128_42 > int_100 );
        EXPECT_FALSE( int128_42 > int_42 );
        EXPECT_TRUE( int128_42 > int_negative );
        EXPECT_TRUE( int128_zero > int_negative );

        // Greater than or equal tests
        EXPECT_TRUE( int128_42 >= int_42 );
        EXPECT_FALSE( int128_42 >= int_100 );
        EXPECT_TRUE( int128_42 >= int_negative );
    }

    TEST( Int128ComparisonBuiltinTypes, ComparisonWithLargeValues )
    {
        // Test Int128 values that exceed the range of smaller integer types
        datatypes::Int128 very_large{ static_cast<std::uint64_t>( 0xFFFFFFFFFFFFFFFFULL ) }; // 2^64-1
        datatypes::Int128 int128_max_int64{ std::numeric_limits<std::int64_t>::max() };
        datatypes::Int128 int128_min_int64{ std::numeric_limits<std::int64_t>::min() };

        std::int64_t max_int64 = std::numeric_limits<std::int64_t>::max();
        std::int64_t min_int64 = std::numeric_limits<std::int64_t>::min();
        std::uint64_t max_uint64 = std::numeric_limits<std::uint64_t>::max();

        // Test boundary values
        EXPECT_TRUE( int128_max_int64 == max_int64 );
        EXPECT_TRUE( int128_min_int64 == min_int64 );
        EXPECT_TRUE( very_large == max_uint64 );

        // Test that large Int128 is greater than smaller type maximums
        EXPECT_TRUE( very_large > max_int64 );
        EXPECT_FALSE( very_large < max_int64 );
    }

    TEST( Int128ComparisonBuiltinTypes, ComparisonEdgeCases )
    {
        // Test edge cases around zero and sign boundaries
        datatypes::Int128 positive_one{ 1 };
        datatypes::Int128 negative_one{ -1 };
        datatypes::Int128 zero{ 0 };

        // Test comparisons with zero
        EXPECT_TRUE( positive_one > 0 );
        EXPECT_TRUE( negative_one < 0 );
        EXPECT_TRUE( zero == 0 );
        EXPECT_FALSE( zero != 0 );

        // Test with different integer types representing the same value
        EXPECT_TRUE( positive_one == std::int64_t{ 1 } );
        EXPECT_TRUE( positive_one == std::int64_t{ 1 } );
        EXPECT_TRUE( positive_one == std::int64_t{ 1 } );

        // Test negative comparisons with unsigned types
        EXPECT_TRUE( negative_one < std::uint64_t{ 0 } );
        EXPECT_FALSE( negative_one > std::uint64_t{ 0 } );
        EXPECT_FALSE( negative_one == std::uint64_t{ 2 } ); // Should never be equal to positive uint64_t
    }

    TEST( Int128ComparisonBuiltinTypes, ComparisonSymmetry )
    {
        // Test that Int128 comparison with built-in types works correctly
        // Note: We're testing Int128 op builtin_type, not builtin_type op Int128
        datatypes::Int128 int128_val{ 42 };
        std::int64_t int64_val = 42;
        std::uint64_t uint64_val = 42;
        int int_val = 42;

        // All these should be true for equality
        EXPECT_TRUE( int128_val == int64_val );
        EXPECT_TRUE( int128_val == uint64_val );
        EXPECT_TRUE( int128_val == int_val );

        // Test inequality
        EXPECT_FALSE( int128_val != int64_val );
        EXPECT_FALSE( int128_val != uint64_val );
        EXPECT_FALSE( int128_val != int_val );

        // Test with different values
        datatypes::Int128 int128_different{ 99 };
        EXPECT_FALSE( int128_different == int64_val );
        EXPECT_FALSE( int128_different == uint64_val );
        EXPECT_FALSE( int128_different == int_val );
        EXPECT_TRUE( int128_different != int64_val );
        EXPECT_TRUE( int128_different != uint64_val );
        EXPECT_TRUE( int128_different != int_val );
    }

    //----------------------------------------------
    // Comparison with built-in floating point types
    //----------------------------------------------

    TEST( Int128FloatingPointComparison, FloatingPointComparison )
    {
        datatypes::Int128 int128_42{ 42 };
        datatypes::Int128 int128_negative{ -123 };
        datatypes::Int128 int128_zero{ 0 };
        datatypes::Int128 int128_large{ 1000000 };

        // Test equality with double
        EXPECT_TRUE( int128_42 == 42.0 );
        EXPECT_TRUE( int128_negative == -123.0 );
        EXPECT_TRUE( int128_zero == 0.0 );
        EXPECT_TRUE( int128_large == 1000000.0 );
        EXPECT_FALSE( int128_42 == 42.1 );
        EXPECT_FALSE( int128_42 == 43.0 );

        // Test inequality with double
        EXPECT_FALSE( int128_42 != 42.0 );
        EXPECT_TRUE( int128_42 != 42.1 );
        EXPECT_TRUE( int128_negative != 42.0 );

        // Test less than with double
        EXPECT_TRUE( int128_42 < 42.1 );
        EXPECT_FALSE( int128_42 < 42.0 );
        EXPECT_FALSE( int128_42 < 41.9 );
        EXPECT_TRUE( int128_negative < 0.0 );
        EXPECT_TRUE( int128_negative < -122.9 );

        // Test less than or equal with double
        EXPECT_TRUE( int128_42 <= 42.1 );
        EXPECT_TRUE( int128_42 <= 42.0 );
        EXPECT_FALSE( int128_42 <= 41.9 );
        EXPECT_TRUE( int128_negative <= -123.0 );

        // Test greater than with double
        EXPECT_FALSE( int128_42 > 42.1 );
        EXPECT_FALSE( int128_42 > 42.0 );
        EXPECT_TRUE( int128_42 > 41.9 );
        EXPECT_FALSE( int128_negative > 0.0 );
        EXPECT_TRUE( int128_negative > -123.1 );

        // Test greater than or equal with double
        EXPECT_FALSE( int128_42 >= 42.1 );
        EXPECT_TRUE( int128_42 >= 42.0 );
        EXPECT_TRUE( int128_42 >= 41.9 );
        EXPECT_TRUE( int128_negative >= -123.0 );

        // Test with float (should delegate to double)
        EXPECT_TRUE( int128_42 == 42.0f );
        EXPECT_TRUE( int128_42 < 42.1f );
        EXPECT_TRUE( int128_42 > 41.9f );
    }

    TEST( Int128FloatingPointComparison, FloatingPointSpecialValues )
    {
        datatypes::Int128 int128_val{ 123 };

        // Test with NaN - should always return false for equality, no ordering
        double nan = std::numeric_limits<double>::quiet_NaN();
        EXPECT_FALSE( int128_val == nan );
        EXPECT_TRUE( int128_val != nan );
        EXPECT_FALSE( int128_val < nan );
        EXPECT_FALSE( int128_val <= nan );
        EXPECT_FALSE( int128_val > nan );
        EXPECT_FALSE( int128_val >= nan );
    }

    TEST( Int128FloatingPointComparison, FloatingPointPrecisionBehavior )
    {
        // Test that demonstrates floating-point precision with integers

        // Small integers should compare exactly
        datatypes::Int128 smallInt{ 42 };
        EXPECT_TRUE( smallInt == 42.0 );
        EXPECT_TRUE( smallInt == 42.0f );

        // Large integers within double precision range
        datatypes::Int128 medium_int{ 1000000 };
        EXPECT_TRUE( medium_int == 1000000.0 );
        EXPECT_TRUE( medium_int == 1000000.0f );

        // Very large integers may have precision issues with float but not double
        datatypes::Int128 largeInt{ static_cast<std::int64_t>( 123456789012345LL ) };
        EXPECT_TRUE( largeInt == 123456789012345.0 ); // Double should handle this

        // Test fractional values - integers should never equal non-integer values
        EXPECT_FALSE( smallInt == 42.1 );
        EXPECT_FALSE( smallInt == 42.000001 );
        EXPECT_FALSE( smallInt == 41.999999 );

        // Test ordering with fractional values
        EXPECT_TRUE( smallInt > 41.9 );
        EXPECT_TRUE( smallInt < 42.1 );
        EXPECT_TRUE( smallInt >= 42.0 );
        EXPECT_TRUE( smallInt <= 42.0 );
    }

    TEST( Int128FloatingPointComparison, LargeNumberComparisons )
    {
        // Test comparisons with numbers near the limits of floating-point precision

        // Large positive Int128
        datatypes::Int128 large_positive{ static_cast<std::uint64_t>( 0x1FFFFFFFFFFFFFULL ) }; // Large but within double precision
        double large_double = static_cast<double>( 0x1FFFFFFFFFFFFFULL );
        EXPECT_TRUE( large_positive == large_double );

        // Large negative Int128
        datatypes::Int128 large_negative{ -static_cast<std::int64_t>( 0x1FFFFFFFFFFFFFLL ) };
        double large_negative_double = -static_cast<double>( 0x1FFFFFFFFFFFFFULL );
        EXPECT_TRUE( large_negative == large_negative_double );

        // Test ordering with large numbers
        EXPECT_TRUE( large_positive > large_negative_double );
        EXPECT_TRUE( large_negative < large_double );

        // Test boundary conditions
        datatypes::Int128 max_int64{ std::numeric_limits<std::int64_t>::max() };
        double max_int64_as_double = static_cast<double>( std::numeric_limits<std::int64_t>::max() );
        EXPECT_TRUE( max_int64 <= max_int64_as_double ); // May not be exactly equal due to precision
    }

    TEST( Int128FloatingPointComparison, EdgeCaseComparisons )
    {
        datatypes::Int128 positive_one{ 1 };
        datatypes::Int128 negative_one{ -1 };
        datatypes::Int128 zero{ 0 };

        // Test comparisons around zero
        EXPECT_TRUE( positive_one > 0.0 );
        EXPECT_TRUE( negative_one < 0.0 );
        EXPECT_TRUE( zero == 0.0 );
        EXPECT_TRUE( zero == -0.0 ); // IEEE 754: +0.0 == -0.0

        // Test with very small floating-point values
        EXPECT_TRUE( zero > -0.000001 );
        EXPECT_TRUE( zero < 0.000001 );
        EXPECT_TRUE( positive_one > 0.999999 );
        EXPECT_TRUE( positive_one < 1.000001 );

        // Test negative comparisons
        EXPECT_TRUE( negative_one < -0.999999 );
        EXPECT_TRUE( negative_one > -1.000001 );
    }

    TEST( Int128FloatingPointComparison, ComparisonSymmetry )
    {
        // Test that Int128 floating-point comparisons work consistently
        datatypes::Int128 int128_val{ 42 };
        double double_val = 42.0;
        float float_val = 42.0f;

        // Test equality consistency
        EXPECT_EQ( int128_val == double_val, true );
        EXPECT_EQ( int128_val == float_val, true );
        EXPECT_EQ( int128_val != double_val, false );
        EXPECT_EQ( int128_val != float_val, false );

        // Test ordering consistency
        datatypes::Int128 int128_larger{ 50 };
        EXPECT_EQ( int128_larger > double_val, true );
        EXPECT_EQ( int128_larger > float_val, true );
        EXPECT_EQ( int128_val < 50.0, true );
        EXPECT_EQ( int128_val < 50.0f, true );

        // Test with different values
        EXPECT_EQ( int128_val == 43.0, false );
        EXPECT_EQ( int128_val != 43.0, true );
        EXPECT_EQ( int128_val < 43.0, true );
        EXPECT_EQ( int128_val > 41.0, true );
    }

    //----------------------------------------------
    // Comparison with nfx::datatypes::Decimal
    //----------------------------------------------

    TEST( Int128DecimalComparison, EqualityComparison )
    {
        // Test equality with positive values
        datatypes::Int128 i1{ 42 };
        datatypes::Decimal d1{ 42 };
        EXPECT_TRUE( i1 == d1 );
        EXPECT_FALSE( i1 != d1 );

        // Test equality with negative values
        datatypes::Int128 i2{ -123 };
        datatypes::Decimal d2{ -123 };
        EXPECT_TRUE( i2 == d2 );
        EXPECT_FALSE( i2 != d2 );

        // Test equality with zero
        datatypes::Int128 i3{ 0 };
        datatypes::Decimal d3{ 0 };
        EXPECT_TRUE( i3 == d3 );
        EXPECT_FALSE( i3 != d3 );

        // Test inequality with different values
        datatypes::Int128 i4{ 100 };
        datatypes::Decimal d4{ 200 };
        EXPECT_FALSE( i4 == d4 );
        EXPECT_TRUE( i4 != d4 );

        // Test inequality when Decimal has fractional part
        datatypes::Int128 i5{ 42 };
        datatypes::Decimal d5{ 42.5 };
        EXPECT_FALSE( i5 == d5 );
        EXPECT_TRUE( i5 != d5 );

        // Test inequality with different signs
        datatypes::Int128 i6{ 42 };
        datatypes::Decimal d6{ -42 };
        EXPECT_FALSE( i6 == d6 );
        EXPECT_TRUE( i6 != d6 );
    }

    TEST( Int128DecimalComparison, OrderingComparison )
    {
        // Test less than with positive values
        datatypes::Int128 i1{ 100 };
        datatypes::Decimal d1{ 200 };
        EXPECT_TRUE( i1 < d1 );
        EXPECT_TRUE( i1 <= d1 );
        EXPECT_FALSE( i1 > d1 );
        EXPECT_FALSE( i1 >= d1 );

        // Test greater than with positive values
        datatypes::Int128 i2{ 300 };
        datatypes::Decimal d2{ 200 };
        EXPECT_FALSE( i2 < d2 );
        EXPECT_FALSE( i2 <= d2 );
        EXPECT_TRUE( i2 > d2 );
        EXPECT_TRUE( i2 >= d2 );

        // Test equal values
        datatypes::Int128 i3{ 150 };
        datatypes::Decimal d3{ 150 };
        EXPECT_FALSE( i3 < d3 );
        EXPECT_TRUE( i3 <= d3 );
        EXPECT_FALSE( i3 > d3 );
        EXPECT_TRUE( i3 >= d3 );

        // Test with negative vs positive
        datatypes::Int128 i4{ -50 };
        datatypes::Decimal d4{ 50 };
        EXPECT_TRUE( i4 < d4 );
        EXPECT_TRUE( i4 <= d4 );
        EXPECT_FALSE( i4 > d4 );
        EXPECT_FALSE( i4 >= d4 );

        // Test with positive vs negative
        datatypes::Int128 i5{ 50 };
        datatypes::Decimal d5{ -50 };
        EXPECT_FALSE( i5 < d5 );
        EXPECT_FALSE( i5 <= d5 );
        EXPECT_TRUE( i5 > d5 );
        EXPECT_TRUE( i5 >= d5 );
    }

    TEST( Int128DecimalComparison, NegativeValueComparison )
    {
        // Test negative vs negative - less negative is greater
        datatypes::Int128 i1{ -100 };
        datatypes::Decimal d1{ -200 };
        EXPECT_FALSE( i1 < d1 );
        EXPECT_FALSE( i1 <= d1 );
        EXPECT_TRUE( i1 > d1 );
        EXPECT_TRUE( i1 >= d1 );

        // Test negative vs negative - more negative is smaller
        datatypes::Int128 i2{ -300 };
        datatypes::Decimal d2{ -200 };
        EXPECT_TRUE( i2 < d2 );
        EXPECT_TRUE( i2 <= d2 );
        EXPECT_FALSE( i2 > d2 );
        EXPECT_FALSE( i2 >= d2 );

        // Test equal negative values
        datatypes::Int128 i3{ -150 };
        datatypes::Decimal d3{ -150 };
        EXPECT_FALSE( i3 < d3 );
        EXPECT_TRUE( i3 <= d3 );
        EXPECT_FALSE( i3 > d3 );
        EXPECT_TRUE( i3 >= d3 );
    }

    TEST( Int128DecimalComparison, FractionalPartHandling )
    {
        // Integer vs decimal with fractional part
        datatypes::Int128 i1{ 42 };
        datatypes::Decimal d1{ 42.7 };
        EXPECT_FALSE( i1 == d1 ); // Not equal due to fractional part
        EXPECT_TRUE( i1 != d1 );
        EXPECT_TRUE( i1 < d1 ); // 42 < 42.7
        EXPECT_TRUE( i1 <= d1 );
        EXPECT_FALSE( i1 > d1 );
        EXPECT_FALSE( i1 >= d1 );

        // Integer vs decimal with fractional part (next integer)
        datatypes::Int128 i2{ 43 };
        datatypes::Decimal d2{ 42.3 };
        EXPECT_FALSE( i2 == d2 );
        EXPECT_TRUE( i2 != d2 );
        EXPECT_FALSE( i2 < d2 ); // 43 > 42.3
        EXPECT_FALSE( i2 <= d2 );
        EXPECT_TRUE( i2 > d2 );
        EXPECT_TRUE( i2 >= d2 );

        // Negative integer vs negative decimal with fractional part
        datatypes::Int128 i3{ -42 };
        datatypes::Decimal d3{ -42.3 };
        EXPECT_FALSE( i3 == d3 );
        EXPECT_TRUE( i3 != d3 );
        EXPECT_FALSE( i3 < d3 ); // -42 > -42.3
        EXPECT_FALSE( i3 <= d3 );
        EXPECT_TRUE( i3 > d3 );
        EXPECT_TRUE( i3 >= d3 );

        // Negative integer vs more negative decimal with fractional part
        datatypes::Int128 i4{ -43 };
        datatypes::Decimal d4{ -42.7 };
        EXPECT_FALSE( i4 == d4 );
        EXPECT_TRUE( i4 != d4 );
        EXPECT_TRUE( i4 < d4 ); // -43 < -42.7
        EXPECT_TRUE( i4 <= d4 );
        EXPECT_FALSE( i4 > d4 );
        EXPECT_FALSE( i4 >= d4 );
    }

    TEST( Int128DecimalComparison, ZeroComparison )
    {
        // Zero integer vs positive decimal
        datatypes::Int128 i1{ 0 };
        datatypes::Decimal d1{ 0.001 };
        EXPECT_FALSE( i1 == d1 );
        EXPECT_TRUE( i1 != d1 );
        EXPECT_TRUE( i1 < d1 );
        EXPECT_TRUE( i1 <= d1 );
        EXPECT_FALSE( i1 > d1 );
        EXPECT_FALSE( i1 >= d1 );

        // Zero integer vs negative decimal
        datatypes::Int128 i2{ 0 };
        datatypes::Decimal d2{ -0.001 };
        EXPECT_FALSE( i2 == d2 );
        EXPECT_TRUE( i2 != d2 );
        EXPECT_FALSE( i2 < d2 );
        EXPECT_FALSE( i2 <= d2 );
        EXPECT_TRUE( i2 > d2 );
        EXPECT_TRUE( i2 >= d2 );

        // Positive integer vs zero decimal
        datatypes::Int128 i3{ 1 };
        datatypes::Decimal d3{ 0 };
        EXPECT_FALSE( i3 == d3 );
        EXPECT_TRUE( i3 != d3 );
        EXPECT_FALSE( i3 < d3 );
        EXPECT_FALSE( i3 <= d3 );
        EXPECT_TRUE( i3 > d3 );
        EXPECT_TRUE( i3 >= d3 );

        // Negative integer vs zero decimal
        datatypes::Int128 i4{ -1 };
        datatypes::Decimal d4{ 0 };
        EXPECT_FALSE( i4 == d4 );
        EXPECT_TRUE( i4 != d4 );
        EXPECT_TRUE( i4 < d4 );
        EXPECT_TRUE( i4 <= d4 );
        EXPECT_FALSE( i4 > d4 );
        EXPECT_FALSE( i4 >= d4 );
    }

    TEST( Int128DecimalComparison, LargeValueComparison )
    {
        // Test with large positive values (within both Int128 and Decimal limits)
        datatypes::Int128 i1{ "1234567890123456789012345678" };
        datatypes::Decimal d1{ "1234567890123456789012345678" };
        EXPECT_TRUE( i1 == d1 );
        EXPECT_FALSE( i1 != d1 );

        // Test with large negative values
        datatypes::Int128 i2{ "-1234567890123456789012345678" };
        datatypes::Decimal d2{ "-1234567890123456789012345678" };
        EXPECT_TRUE( i2 == d2 );
        EXPECT_FALSE( i2 != d2 );

        // Test ordering with large values
        datatypes::Int128 i3{ "1234567890123456789012345" };
        datatypes::Decimal d3{ "1234567890123456789012346" };
        EXPECT_FALSE( i3 == d3 );
        EXPECT_TRUE( i3 != d3 );
        EXPECT_TRUE( i3 < d3 );
        EXPECT_TRUE( i3 <= d3 );
        EXPECT_FALSE( i3 > d3 );
        EXPECT_FALSE( i3 >= d3 );
    }

    TEST( Int128DecimalComparison, PrecisionEdgeCases )
    {
        // Test integer vs decimal with meaningful fractional part (won't be normalized away)
        datatypes::Int128 i1{ 42 };
        datatypes::Decimal d1{ "42.1" };
        EXPECT_FALSE( i1 == d1 ); // Fractional part makes them not equal
        EXPECT_TRUE( i1 != d1 );
        EXPECT_TRUE( i1 < d1 );
        EXPECT_TRUE( i1 <= d1 );
        EXPECT_FALSE( i1 > d1 );
        EXPECT_FALSE( i1 >= d1 );

        // Test integer vs decimal that equals integer after normalization
        datatypes::Int128 i2{ 42 };
        datatypes::Decimal d2{ "42.0000" }; // Trailing zeros will be normalized away
        // This should be exactly equal after normalization
        EXPECT_TRUE( i2 == d2 );
        EXPECT_FALSE( i2 != d2 );

        // Test zero vs small but significant decimal
        datatypes::Int128 i3{ 0 };
        datatypes::Decimal d3{ "0.001" }; // Small but won't be normalized to zero
        EXPECT_FALSE( i3 == d3 );
        EXPECT_TRUE( i3 != d3 );
        EXPECT_TRUE( i3 < d3 );
        EXPECT_TRUE( i3 <= d3 );
        EXPECT_FALSE( i3 > d3 );
        EXPECT_FALSE( i3 >= d3 );
    }

    TEST( Int128DecimalComparison, ScalingComparison )
    {
        // Test that Int128 is properly scaled when comparing to Decimal with scale
        datatypes::Int128 i1{ 42 };
        datatypes::Decimal d1{ "420" }; // 10 times larger
        EXPECT_FALSE( i1 == d1 );
        EXPECT_TRUE( i1 != d1 );
        EXPECT_TRUE( i1 < d1 );
        EXPECT_TRUE( i1 <= d1 );
        EXPECT_FALSE( i1 > d1 );
        EXPECT_FALSE( i1 >= d1 );

        // Test with decimal having scale
        datatypes::Int128 i2{ 420 };
        datatypes::Decimal d2{ "42.0" }; // Same value, different representation
        EXPECT_FALSE( i2 == d2 );        // 420 != 42.0
        EXPECT_TRUE( i2 != d2 );
        EXPECT_FALSE( i2 < d2 );
        EXPECT_FALSE( i2 <= d2 );
        EXPECT_TRUE( i2 > d2 );
        EXPECT_TRUE( i2 >= d2 );

        // Test with matching scaled values
        datatypes::Int128 i3{ 42 };
        datatypes::Decimal d3{ "4.2" };
        EXPECT_FALSE( i3 == d3 ); // 42 != 4.2
        EXPECT_TRUE( i3 != d3 );
        EXPECT_FALSE( i3 < d3 );
        EXPECT_FALSE( i3 <= d3 );
        EXPECT_TRUE( i3 > d3 );
        EXPECT_TRUE( i3 >= d3 );
    }

    TEST( Int128DecimalComparison, ConsistencyWithReversedOperands )
    {
        // Test that Decimal vs Int128 gives consistent results with Int128 vs Decimal
        // This tests the symmetry of the comparison operators

        datatypes::Int128 i1{ 100 };
        datatypes::Decimal d1{ 200 };

        // Test int128 < decimal vs decimal > int128
        EXPECT_TRUE( i1 < d1 );
        EXPECT_TRUE( d1 > i1 );

        // Test int128 > decimal vs decimal < int128
        datatypes::Int128 i2{ 300 };
        datatypes::Decimal d2{ 200 };
        EXPECT_TRUE( i2 > d2 );
        EXPECT_TRUE( d2 < i2 );

        // Test int128 == decimal vs decimal == int128
        datatypes::Int128 i3{ 150 };
        datatypes::Decimal d3{ 150 };
        EXPECT_TRUE( i3 == d3 );
        EXPECT_TRUE( d3 == i3 );

        // Test with fractional decimal (should not equal integer)
        datatypes::Int128 i4{ 42 };
        datatypes::Decimal d4{ 42.5 };
        EXPECT_FALSE( i4 == d4 );
        EXPECT_FALSE( d4 == i4 );
        EXPECT_TRUE( i4 < d4 );
        EXPECT_TRUE( d4 > i4 );
    }

    TEST( Int128DecimalComparison, EdgeCases )
    {
        // Test boundary values and edge cases within practical limits

        // Test with large values within Decimal's 28-digit precision limit
        datatypes::Int128 largeInt128{ "1234567890123456789012345678" }; // 28 digits
        datatypes::Decimal largeDecimal{ "1234567890123456789012345678" };
        EXPECT_TRUE( largeInt128 == largeDecimal );

        // Test with negative large values
        datatypes::Int128 largeNegInt128{ "-1234567890123456789012345678" };
        datatypes::Decimal largeNegDecimal{ "-1234567890123456789012345678" };
        EXPECT_TRUE( largeNegInt128 == largeNegDecimal );

        // Test comparison near large value boundaries
        datatypes::Int128 nearLarge{ "1234567890123456789012345677" };
        datatypes::Decimal largePlusOne{ "1234567890123456789012345678" };
        EXPECT_TRUE( nearLarge < largePlusOne );
        EXPECT_FALSE( nearLarge >= largePlusOne );

        // Test with meaningful decimal differences (won't be normalized away)
        datatypes::Int128 exactInt{ 1000000000 };
        datatypes::Decimal slightlyLarger{ "1000000000.5" }; // Clear fractional difference
        EXPECT_FALSE( exactInt == slightlyLarger );
        EXPECT_TRUE( exactInt < slightlyLarger );
    }

    //----------------------------------------------
    // State checking
    //----------------------------------------------

    TEST( Int128StateChecking, IsZero )
    {
        datatypes::Int128 zero{};
        datatypes::Int128 nonZero{ 1 };

        EXPECT_TRUE( zero == 0 );
        EXPECT_FALSE( nonZero == 0 );
    }

    TEST( Int128StateChecking, IsNegative )
    {
        datatypes::Int128 positive{ 123 };
        datatypes::Int128 negative{ -123 };
        datatypes::Int128 zero{};

        EXPECT_FALSE( positive < 0 );
        EXPECT_TRUE( negative < 0 );
        EXPECT_FALSE( zero < 0 );
    }

    TEST( Int128StateChecking, IsNegativeHighBit )
    {
        // Test negativity based on high bit
        datatypes::Int128 negative{ 0, constants::INT128_MIN_NEGATIVE_HIGH };
        datatypes::Int128 positive{ 0, constants::INT128_MAX_POSITIVE_HIGH };

        EXPECT_TRUE( negative < 0 );
        EXPECT_FALSE( positive < 0 );
    }

    //----------------------------------------------
    // String parsing
    //----------------------------------------------

    TEST( Int128StringParsing, FromStringMethod )
    {
        datatypes::Int128 result;

        // Valid positive parsing
        EXPECT_TRUE( datatypes::Int128::fromString( "12345", result ) );
        EXPECT_EQ( 12345ULL, result.toLow() );
        EXPECT_EQ( 0ULL, result.toHigh() );
        EXPECT_FALSE( result < 0 );

        // Valid negative parsing
        EXPECT_TRUE( datatypes::Int128::fromString( "-9876543210", result ) );
        EXPECT_TRUE( result < 0 );

        // Zero parsing
        EXPECT_TRUE( datatypes::Int128::fromString( "0", result ) );
        EXPECT_TRUE( result == 0 );
        EXPECT_FALSE( result < 0 );

        // Large positive number
        EXPECT_TRUE( datatypes::Int128::fromString( "123456789012345678901234567890", result ) );
        EXPECT_FALSE( result == 0 );
        EXPECT_FALSE( result < 0 );

        // Large negative number
        EXPECT_TRUE( datatypes::Int128::fromString( "-123456789012345678901234567890", result ) );
        EXPECT_FALSE( result == 0 );
        EXPECT_TRUE( result < 0 );

        // Positive sign
        EXPECT_TRUE( datatypes::Int128::fromString( "+42", result ) );
        EXPECT_EQ( 42ULL, result.toLow() );
        EXPECT_FALSE( result < 0 );

        // Invalid strings
        EXPECT_FALSE( datatypes::Int128::fromString( "", result ) );
        EXPECT_FALSE( datatypes::Int128::fromString( "abc", result ) );
        EXPECT_FALSE( datatypes::Int128::fromString( "123abc", result ) );
        EXPECT_FALSE( datatypes::Int128::fromString( "12.34", result ) );
        EXPECT_FALSE( datatypes::Int128::fromString( "+", result ) );
        EXPECT_FALSE( datatypes::Int128::fromString( "-", result ) );
        EXPECT_FALSE( datatypes::Int128::fromString( "123 456", result ) );
        EXPECT_FALSE( datatypes::Int128::fromString( "1e10", result ) );
    }

    TEST( Int128StringParsing, ParseMethod )
    {
        // Valid positive parsing
        auto result1{ datatypes::Int128{ "12345" } };
        EXPECT_EQ( 12345ULL, result1.toLow() );
        EXPECT_FALSE( result1 < 0 );

        // Valid negative parsing
        auto result2{ datatypes::Int128{ "-9876543210" } };
        EXPECT_TRUE( result2 < 0 );

        // Zero parsing
        auto result3{ datatypes::Int128{ "0" } };
        EXPECT_TRUE( result3 == 0 );

        // Large number parsing
        auto result4{ datatypes::Int128{ "123456789012345678901234567890" } };
        EXPECT_FALSE( result4 == 0 );
        EXPECT_FALSE( result4 < 0 );

        // Positive sign
        auto result5{ datatypes::Int128{ "+42" } };
        EXPECT_EQ( 42ULL, result5.toLow() );
        EXPECT_FALSE( result5 < 0 );

        // Invalid parsing should throw
        EXPECT_THROW( datatypes::Int128{ "" }, std::invalid_argument );
        EXPECT_THROW( datatypes::Int128{ "abc" }, std::invalid_argument );
        EXPECT_THROW( datatypes::Int128{ "123abc" }, std::invalid_argument );
        EXPECT_THROW( datatypes::Int128{ "12.34" }, std::invalid_argument );
        EXPECT_THROW( datatypes::Int128{ "+" }, std::invalid_argument );
        EXPECT_THROW( datatypes::Int128{ "-" }, std::invalid_argument );
        EXPECT_THROW( datatypes::Int128{ "123 456" }, std::invalid_argument );
        EXPECT_THROW( datatypes::Int128{ "1e10" }, std::invalid_argument );
    }

    //----------------------------------------------
    // Stream
    //----------------------------------------------

    TEST( Int128Stream, StreamOutput )
    {
        datatypes::Int128 i1{ "123456" };
        std::ostringstream oss;
        oss << i1;
        EXPECT_EQ( oss.str(), "123456" );

        datatypes::Int128 i2{ "-789123" };
        oss.str( "" );
        oss << i2;
        EXPECT_EQ( oss.str(), "-789123" );
    }

    TEST( Int128Stream, StreamInput )
    {
        datatypes::Int128 i1;
        std::istringstream iss( "456789" );
        iss >> i1;
        EXPECT_FALSE( iss.fail() );
        EXPECT_EQ( i1.toString(), "456789" );

        // Test invalid input
        datatypes::Int128 i2;
        std::istringstream iss2( "invalid" );
        iss2 >> i2;
        EXPECT_TRUE( iss2.fail() );

        // Test decimal input (should fail for integer type)
        datatypes::Int128 i3;
        std::istringstream iss3( "123.456" );
        iss3 >> i3;
        EXPECT_TRUE( iss3.fail() );
    }

    //----------------------------------------------
    // std::formatter support
    //----------------------------------------------

    TEST( Int128Formatter, BasicFormatting )
    {
        // Small positive value
        datatypes::Int128 small( 42 );
        auto str = std::format( "{}", small );
        EXPECT_EQ( "42", str );
    }

    TEST( Int128Formatter, ZeroValue )
    {
        datatypes::Int128 zero( 0 );
        auto str = std::format( "{}", zero );
        EXPECT_EQ( "0", str );
    }

    TEST( Int128Formatter, NegativeValue )
    {
        datatypes::Int128 neg( -123 );
        auto str = std::format( "{}", neg );
        EXPECT_EQ( "-123", str );
    }

    TEST( Int128Formatter, LargePositiveValue )
    {
        datatypes::Int128 large = datatypes::Int128{ "12345678901234567890" };
        auto str = std::format( "{}", large );
        EXPECT_EQ( "12345678901234567890", str );
    }

    TEST( Int128Formatter, LargeNegativeValue )
    {
        datatypes::Int128 large = datatypes::Int128{ "-98765432109876543210" };
        auto str = std::format( "{}", large );
        EXPECT_EQ( "-98765432109876543210", str );
    }

    TEST( Int128Formatter, MaximumValue )
    {
        // 2^127 - 1
        datatypes::Int128 maxVal = datatypes::Int128{ "170141183460469231731687303715884105727" };
        auto str = std::format( "{}", maxVal );
        EXPECT_EQ( "170141183460469231731687303715884105727", str );
    }

    TEST( Int128Formatter, MinimumValue )
    {
        // -2^127
        datatypes::Int128 minVal = datatypes::Int128{ "-170141183460469231731687303715884105728" };
        auto str = std::format( "{}", minVal );
        EXPECT_EQ( "-170141183460469231731687303715884105728", str );
    }

    TEST( Int128Formatter, MultipleValuesInFormat )
    {
        datatypes::Int128 a( 100 );
        datatypes::Int128 b( -200 );
        datatypes::Int128 c = datatypes::Int128{ "999999999999999999" };

        auto str = std::format( "a={}, b={}, c={}", a, b, c );
        EXPECT_EQ( "a=100, b=-200, c=999999999999999999", str );
    }

    TEST( Int128Formatter, FormattingInContainer )
    {
        std::vector<datatypes::Int128> values = {
            datatypes::Int128( 1 ),
            datatypes::Int128( 10 ),
            datatypes::Int128( 100 ),
            datatypes::Int128{ "1000000000000" } };

        std::string result;
        for ( const auto& val : values )
        {
            result += std::format( "{} ", val );
        }
        EXPECT_EQ( "1 10 100 1000000000000 ", result );
    }

    //----------------------------------------------
    // Edge case and overflow
    //----------------------------------------------

    TEST( Int128EdgeCaseAndOverflow, MaxValues )
    {
        // Test with maximum possible values
        datatypes::Int128 maxVal{ constants::INT128_MAX_POSITIVE_LOW, constants::INT128_MAX_POSITIVE_HIGH };

        EXPECT_FALSE( maxVal == 0 );
        EXPECT_FALSE( maxVal < 0 );
        EXPECT_EQ( constants::INT128_MAX_POSITIVE_LOW, maxVal.toLow() );
        EXPECT_EQ( constants::INT128_MAX_POSITIVE_HIGH, maxVal.toHigh() );
    }

    TEST( Int128EdgeCaseAndOverflow, MinValue )
    {
        // Test with minimum possible value (most negative)
        datatypes::Int128 minVal{ 0, constants::INT128_MIN_NEGATIVE_HIGH };

        EXPECT_FALSE( minVal == 0 );
        EXPECT_TRUE( minVal < 0 );
        EXPECT_EQ( 0ULL, minVal.toLow() );
        EXPECT_EQ( constants::INT128_MIN_NEGATIVE_HIGH, minVal.toHigh() );
    }

    TEST( Int128EdgeCaseAndOverflow, OverflowAddition )
    {
        // Test addition overflow
        datatypes::Int128 maxPositive{ constants::INT128_MAX_POSITIVE_LOW, constants::INT128_MAX_POSITIVE_HIGH };
        datatypes::Int128 one{ 1 };
        datatypes::Int128 result{ maxPositive + one };

        // Should wrap to minimum negative value
        EXPECT_TRUE( result < 0 );
        EXPECT_EQ( 0ULL, result.toLow() );
        EXPECT_EQ( constants::INT128_MIN_NEGATIVE_HIGH, result.toHigh() );
    }

    TEST( Int128EdgeCaseAndOverflow, OverflowSubtraction )
    {
        // Test subtraction underflow
        datatypes::Int128 minNegative{ 0, constants::INT128_MIN_NEGATIVE_HIGH };
        datatypes::Int128 one{ 1 };
        datatypes::Int128 result{ minNegative - one };

        // Should wrap to maximum positive value
        EXPECT_FALSE( result < 0 );
        EXPECT_EQ( constants::INT128_MAX_POSITIVE_LOW, result.toLow() );
        EXPECT_EQ( constants::INT128_MAX_POSITIVE_HIGH, result.toHigh() );
    }

    TEST( Int128EdgeCaseAndOverflow, OverflowMultiplication )
    {
        // Test multiplication that overflows 128 bits
        datatypes::Int128 large1{ constants::INT128_MAX_POSITIVE_LOW, constants::INT128_MAX_POSITIVE_HIGH };
        datatypes::Int128 two{ 2 };
        datatypes::Int128 result{ large1 * two };

        // Should wrap around due to overflow
        EXPECT_EQ( 0xFFFFFFFFFFFFFFFEULL, result.toLow() );
        EXPECT_EQ( constants::INT128_MAX_POSITIVE_LOW, result.toHigh() );
    }

    TEST( Int128EdgeCaseAndOverflow, DivisionLargeNumbers )
    {
        // Test division with large 128-bit numbers
        datatypes::Int128 dividend{ 0, 1 }; // 2^64
        datatypes::Int128 divisor{ 2 };
        datatypes::Int128 result{ dividend / divisor };

        // Should be 2^63
        EXPECT_EQ( constants::INT128_MIN_NEGATIVE_HIGH, result.toLow() );
        EXPECT_EQ( 0ULL, result.toHigh() );
    }

    TEST( Int128EdgeCaseAndOverflow, ModuloLargeNumbers )
    {
        // Test modulo with large numbers
        datatypes::Int128 dividend{ constants::INT128_MAX_POSITIVE_LOW, 1 }; // 2^64 + (2^64-1)
        datatypes::Int128 divisor{ 0, 1 };                                   // 2^64 (constructed as {0, 1})
        datatypes::Int128 result{ dividend % divisor };

        // Should be 2^64 - 1
        EXPECT_EQ( constants::INT128_MAX_POSITIVE_LOW, result.toLow() );
        EXPECT_EQ( 0ULL, result.toHigh() );
    }

    TEST( Int128EdgeCaseAndOverflow, SignBitBoundary )
    {
        // Test numbers right at the sign bit boundary
        datatypes::Int128 justPositive{ constants::INT128_MAX_POSITIVE_LOW, constants::INT128_MAX_POSITIVE_HIGH };
        datatypes::Int128 justNegative{ 0, constants::INT128_MIN_NEGATIVE_HIGH };

        EXPECT_FALSE( justPositive < 0 );
        EXPECT_TRUE( justNegative < 0 );

        // Adding 1 to max positive should make it negative
        datatypes::Int128 wrapped{ justPositive + datatypes::Int128{ 1 } };
        EXPECT_TRUE( wrapped < 0 );
        EXPECT_EQ( wrapped, justNegative );
    }

    TEST( Int128EdgeCaseAndOverflow, ParseVeryLargeNumbers )
    {
        // Test parsing numbers near 128-bit limits
        datatypes::Int128 result;

        // Test maximum positive 128-bit signed integer
        EXPECT_TRUE( datatypes::Int128::fromString( "170141183460469231731687303715884105727", result ) );
        EXPECT_FALSE( result < 0 );
        EXPECT_EQ( constants::INT128_MAX_POSITIVE_LOW, result.toLow() );
        EXPECT_EQ( constants::INT128_MAX_POSITIVE_HIGH, result.toHigh() );

        // Test minimum negative 128-bit signed integer
        EXPECT_TRUE( datatypes::Int128::fromString( "-170141183460469231731687303715884105728", result ) );
        EXPECT_TRUE( result < 0 );
        EXPECT_EQ( 0ULL, result.toLow() );
        EXPECT_EQ( constants::INT128_MIN_NEGATIVE_HIGH, result.toHigh() );
    }

    TEST( Int128EdgeCaseAndOverflow, ParseOverflowNumbers )
    {
        // Test parsing numbers that exceed 128-bit range
        datatypes::Int128 result;

        // Should fail for numbers too large
        EXPECT_FALSE( datatypes::Int128::fromString( "170141183460469231731687303715884105728", result ) );
        EXPECT_FALSE( datatypes::Int128::fromString( "-170141183460469231731687303715884105729", result ) );
        EXPECT_FALSE( datatypes::Int128::fromString( "999999999999999999999999999999999999999", result ) );
    }

    TEST( Int128EdgeCaseAndOverflow, ComparisonHighWordEdges )
    {
        // Test comparison edge cases with high word differences
        datatypes::Int128 a{ constants::INT128_MAX_POSITIVE_LOW, constants::INT128_MAX_POSITIVE_HIGH };
        datatypes::Int128 b{ 0, constants::INT128_MIN_NEGATIVE_HIGH };

        // Max positive vs min negative
        EXPECT_TRUE( a > b );
        EXPECT_FALSE( a < b );
        EXPECT_TRUE( b < a );
        EXPECT_FALSE( b > a );
    }

    TEST( Int128EdgeCaseAndOverflow, NegationEdgeCases )
    {
        // Test negation of boundary values
        datatypes::Int128 maxPositive{ constants::INT128_MAX_POSITIVE_LOW, constants::INT128_MAX_POSITIVE_HIGH };
        datatypes::Int128 minNegative{ 0, constants::INT128_MIN_NEGATIVE_HIGH };

        // Negating max positive should give min negative + 1
        datatypes::Int128 negated{ -maxPositive };
        datatypes::Int128 expected{ minNegative + datatypes::Int128{ 1 } };
        EXPECT_EQ( expected, negated );

        // Negating min negative should overflow back to itself
        datatypes::Int128 negatedMin{ -minNegative };
        EXPECT_EQ( minNegative, negatedMin );
    }
} // namespace nfx::datatypes::test
