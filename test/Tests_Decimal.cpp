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
 * @file Tests_Decimal.cpp
 * @brief Comprehensive tests for cross-platform Decimal implementation
 * @details Validates Decimal compatibility and cross-platform behavior
 */

#include <limits>

#include <gtest/gtest.h>

#include <nfx/datatypes/Decimal.h>

namespace nfx::datatypes::test
{
    //=====================================================================
    // Decimal type tests
    //=====================================================================

    //----------------------------------------------
    // Construction
    //----------------------------------------------

    TEST( DecimalConstruction, DefaultConstruction )
    {
        datatypes::Decimal d;
        EXPECT_TRUE( d == 0 );
        EXPECT_FALSE( d < 0 );
        EXPECT_EQ( d.scale(), 0 );
        EXPECT_EQ( d.toString(), "0" );
    }

    TEST( DecimalConstruction, IntegerConstruction )
    {
        // Positive integers
        datatypes::Decimal d1{ 42 };
        EXPECT_FALSE( d1 == 0 );
        EXPECT_FALSE( d1 < 0 );
        EXPECT_EQ( d1.toString(), "42" );

        // Negative integers
        datatypes::Decimal d2{ -123 };
        EXPECT_FALSE( d2 == 0 );
        EXPECT_TRUE( d2 < 0 );
        EXPECT_EQ( d2.toString(), "-123" );

        // Zero
        datatypes::Decimal d3{ 0 };
        EXPECT_TRUE( d3 == 0 );
        EXPECT_FALSE( d3 < 0 );
        EXPECT_EQ( d3.toString(), "0" );

        // Large integers
        datatypes::Decimal d4{ std::numeric_limits<std::int64_t>::max() };
        EXPECT_FALSE( d4 == 0 );
        EXPECT_FALSE( d4 < 0 );
    }

    TEST( DecimalConstruction, FloatConstruction )
    {
        // Simple positive float
        datatypes::Decimal d1{ 123.456f };
        EXPECT_FALSE( d1 == 0 );
        EXPECT_FALSE( d1 < 0 );
        // Note: Due to float precision, exact comparison may not work
        // We verify it's approximately correct
        EXPECT_TRUE( d1 > 123.0 );
        EXPECT_TRUE( d1 < 124.0 );

        // Negative float
        datatypes::Decimal d2{ -123.456f };
        EXPECT_FALSE( d2 == 0 );
        EXPECT_TRUE( d2 < 0 );
        EXPECT_TRUE( d2 < -123.0 );
        EXPECT_TRUE( d2 > -124.0 );

        // Zero float
        datatypes::Decimal d3{ 0.0f };
        EXPECT_TRUE( d3 == 0 );
        EXPECT_FALSE( d3 < 0 );

        // Small positive float
        datatypes::Decimal d4{ 0.001f };
        EXPECT_FALSE( d4 == 0 );
        EXPECT_FALSE( d4 < 0 );
        EXPECT_TRUE( d4 > 0.0 );
        EXPECT_TRUE( d4 < 0.01 );

        // Large float
        datatypes::Decimal d5{ 123456789.0f };
        EXPECT_FALSE( d5 == 0 );
        EXPECT_FALSE( d5 < 0 );

        // Integer float (exact representation)
        datatypes::Decimal d6{ 42.0f };
        EXPECT_FALSE( d6 == 0 );
        EXPECT_FALSE( d6 < 0 );
        EXPECT_EQ( d6.toString(), "42" ); // Should be exact for integer values

        // NaN should be handled (likely by setting to zero or throwing)
        datatypes::Decimal d9{ std::numeric_limits<float>::quiet_NaN() };

        // Very small float (near zero)
        datatypes::Decimal d10{ 1e-10f };
        EXPECT_FALSE( d10 == 0 );
        EXPECT_FALSE( d10 < 0 );

        // Float precision edge case - values that can't be represented exactly
        float imprecise = 0.1f; // 0.1 cannot be represented exactly in float
        datatypes::Decimal d11{ imprecise };
        EXPECT_FALSE( d11 == 0 );
        EXPECT_FALSE( d11 < 0 );
        // The result should be close to 0.1 but may have precision artifacts
    }

    TEST( DecimalConstruction, FloatIEEE754ArtifactPrevention )
    {
        // Test that float constructor avoids IEEE 754 binary representation artifacts
        // These values have problematic binary representations that should be cleaned

        // 273.15f - temperature conversion constant (Celsius to Kelvin)
        // Binary representation causes "273.149994..." without fix
        datatypes::Decimal d1{ 273.15f };
        std::string str1 = d1.toString();
        EXPECT_EQ( str1, "273.15" );

        // 380.4 - another value with binary artifacts
        datatypes::Decimal d2{ 380.4f };
        std::string str2 = d2.toString();
        // Should be close to 380.4, not "380.399..." (exact match depends on float precision)
        EXPECT_TRUE( str2.substr( 0, 5 ) == "380.4" );

        // Test integer values stored as float - should be exact
        datatypes::Decimal d3{ 42.0f };
        EXPECT_EQ( d3.toString(), "42" );

        datatypes::Decimal d4{ 100.0f };
        EXPECT_EQ( d4.toString(), "100" );

        // Test large float values that might trigger scientific notation in to_chars
        datatypes::Decimal d5{ 100000000.0f }; // 1e8
        std::string str5 = d5.toString();
        EXPECT_TRUE( str5.find( 'e' ) == std::string::npos ); // No scientific notation
        EXPECT_TRUE( str5.find( 'E' ) == std::string::npos );
        EXPECT_TRUE( d5 > 99999999.0 && d5 < 100000001.0 );

        // Test small floats that are still representable with precision 4-6
        datatypes::Decimal d6{ 0.001f };
        EXPECT_TRUE( d6 > 0.0 );
        EXPECT_TRUE( d6 < 0.01 );

        // Test negative values with binary artifacts
        datatypes::Decimal d7{ -273.15f };
        std::string str7 = d7.toString();
        EXPECT_EQ( str7, "-273.15" );

        // Test common financial values
        datatypes::Decimal d8{ 99.99f };
        std::string str8 = d8.toString();
        EXPECT_TRUE( str8.substr( 0, 5 ) == "99.99" );

        // Test zero and negative zero
        datatypes::Decimal d9{ 0.0f };
        EXPECT_EQ( d9.toString(), "0" );

        datatypes::Decimal d10{ -0.0f };
        EXPECT_EQ( d10.toString(), "0" );
    }

    TEST( DecimalConstruction, DoubleConstruction )
    {
        // Simple double
        datatypes::Decimal d1{ 123.456 };
        EXPECT_FALSE( d1 == 0 );
        EXPECT_FALSE( d1 < 0 );

        // Negative double
        datatypes::Decimal d2{ -123.456 };
        EXPECT_FALSE( d2 == 0 );
        EXPECT_TRUE( d2 < 0 );

        // Zero double
        datatypes::Decimal d3{ 0.0 };
        EXPECT_TRUE( d3 == 0 );
        EXPECT_FALSE( d3 < 0 );

        // Small Decimal
        datatypes::Decimal d4{ 0.001 };
        EXPECT_FALSE( d4 == 0 );
        EXPECT_FALSE( d4 < 0 );
    }

    TEST( DecimalConstruction, DoubleIEEE754ArtifactPrevention )
    {
        // Test that double constructor avoids IEEE 754 binary representation artifacts

        // 380.4 - the original bug report value
        // Binary representation causes "380.399999999999977" without fix
        datatypes::Decimal d1{ 380.4 };
        std::string str1 = d1.toString();
        EXPECT_EQ( str1, "380.4" );

        // Test integer values stored as double - should be exact
        datatypes::Decimal d2{ 42.0 };
        EXPECT_EQ( d2.toString(), "42" );

        datatypes::Decimal d3{ 1000.0 };
        EXPECT_EQ( d3.toString(), "1000" );

        // Test large double values that might trigger scientific notation
        datatypes::Decimal d4{ 100000000.0 }; // 1e8
        std::string str4 = d4.toString();
        EXPECT_TRUE( str4.find( 'e' ) == std::string::npos ); // No scientific notation
        EXPECT_TRUE( str4.find( 'E' ) == std::string::npos );
        EXPECT_EQ( str4, "100000000" );

        datatypes::Decimal d5{ 1e15 }; // Very large value
        std::string str5 = d5.toString();
        EXPECT_TRUE( str5.find( 'e' ) == std::string::npos );
        EXPECT_TRUE( str5.find( 'E' ) == std::string::npos );

        // Test very small doubles
        datatypes::Decimal d6{ 1e-10 };
        EXPECT_TRUE( d6 > 0.0 );
        EXPECT_TRUE( d6 < 0.001 );

        // Test negative values
        datatypes::Decimal d7{ -380.4 };
        std::string str7 = d7.toString();
        EXPECT_EQ( str7, "-380.4" );

        // Test common decimal values
        datatypes::Decimal d8{ 0.5 };
        EXPECT_EQ( d8.toString(), "0.5" );

        datatypes::Decimal d9{ 0.25 };
        EXPECT_EQ( d9.toString(), "0.25" );

        datatypes::Decimal d10{ 0.125 };
        EXPECT_EQ( d10.toString(), "0.125" );

        // Test zero
        datatypes::Decimal d11{ 0.0 };
        EXPECT_EQ( d11.toString(), "0" );

        datatypes::Decimal d12{ -0.0 };
        EXPECT_EQ( d12.toString(), "0" );
    }

    TEST( DecimalConstruction, Int128Construction )
    {
        // Test construction from positive Int128
        datatypes::Int128 positive{ 42 };
        datatypes::Decimal d1{ positive };
        EXPECT_FALSE( d1 == 0 );
        EXPECT_FALSE( d1 < 0 );
        EXPECT_EQ( d1.toString(), "42" );
        EXPECT_EQ( d1.scale(), 0 ); // Integer values should have scale 0

        // Test construction from negative Int128
        datatypes::Int128 negative{ -123 };
        datatypes::Decimal d2{ negative };
        EXPECT_FALSE( d2 == 0 );
        EXPECT_TRUE( d2 < 0 );
        EXPECT_EQ( d2.toString(), "-123" );
        EXPECT_EQ( d2.scale(), 0 );

        // Test construction from zero Int128
        datatypes::Int128 zero{ 0 };
        datatypes::Decimal d3{ zero };
        EXPECT_TRUE( d3 == 0 );
        EXPECT_FALSE( d3 < 0 );
        EXPECT_EQ( d3.toString(), "0" );
        EXPECT_EQ( d3.scale(), 0 );

        // Test construction from large positive Int128
        datatypes::Int128 large{ "1234567890123456789" };
        datatypes::Decimal d4{ large };
        EXPECT_FALSE( d4 == 0 );
        EXPECT_FALSE( d4 < 0 );
        EXPECT_EQ( d4.toString(), "1234567890123456789" );

        // Test construction from large negative Int128
        datatypes::Int128 largenegative{ "-9876543210987654321" };
        datatypes::Decimal d5{ largenegative };
        EXPECT_FALSE( d5 == 0 );
        EXPECT_TRUE( d5 < 0 );
        EXPECT_EQ( d5.toString(), "-9876543210987654321" );

        // Test construction from maximum positive Int128 value - should throw overflow_error
        datatypes::Int128 maxInt128{ "170141183460469231731687303715884105727" }; // 2^127 - 1
        EXPECT_THROW( datatypes::Decimal{ maxInt128 }, std::overflow_error );

        // Test construction from minimum negative Int128 value - should throw overflow_error
        datatypes::Int128 minInt128{ "-170141183460469231731687303715884105728" }; // -2^127
        EXPECT_THROW( datatypes::Decimal{ minInt128 }, std::overflow_error );

        // Test that the resulting Decimal can be converted back to Int128 for smaller values
        datatypes::Int128 original{ 98765 };
        datatypes::Decimal converted{ original };
        datatypes::Int128 backConverted{ converted }; // This should work for integer Decimals
        EXPECT_TRUE( original == backConverted );

        // Test bidirectional conversion consistency for zero
        datatypes::Int128 zeroOriginal{ 0 };
        datatypes::Decimal zeroConverted{ zeroOriginal };
        datatypes::Int128 zeroBackConverted{ zeroConverted };
        EXPECT_TRUE( zeroOriginal == zeroBackConverted );
        EXPECT_TRUE( zeroConverted == 0 );

        // Test bidirectional conversion consistency for negative values
        datatypes::Int128 negativeOriginal{ -54321 };
        datatypes::Decimal negativeConverted{ negativeOriginal };
        datatypes::Int128 negativeBackConverted{ negativeConverted };
        EXPECT_TRUE( negativeOriginal == negativeBackConverted );
        EXPECT_TRUE( negativeConverted < 0 );

        // Test edge case: values at the boundary of Decimal's capacity
        // Maximum value that fits exactly in Decimal (2^96 - 1)
        datatypes::Int128 decimalMax{ "79228162514264337593543950335" };
        datatypes::Decimal d8{ decimalMax };
        EXPECT_FALSE( d8 == 0 );
        EXPECT_FALSE( d8 < 0 );
        EXPECT_EQ( d8.toString(), "79228162514264337593543950335" );

        // Minimum value that fits exactly in Decimal (-(2^96 - 1))
        datatypes::Int128 decimalMin{ "-79228162514264337593543950335" };
        datatypes::Decimal d9{ decimalMin };
        EXPECT_FALSE( d9 == 0 );
        EXPECT_TRUE( d9 < 0 );
        EXPECT_EQ( d9.toString(), "-79228162514264337593543950335" );

        // Value just over Decimal's capacity - should throw overflow_error
        datatypes::Int128 justOverMax{ "79228162514264337593543950336" }; // 2^96
        EXPECT_THROW( datatypes::Decimal{ justOverMax }, std::overflow_error );
    }

    //----------------------------------------------
    // Assignment
    //----------------------------------------------

    TEST( DecimalAssignment, CompoundAssignment )
    {
        datatypes::Decimal d1{ "100" };
        datatypes::Decimal d2{ "25" };

        // Addition assignment
        d1 += d2;
        EXPECT_FALSE( d1 == 0 );
        // Should be 125

        // Subtraction assignment
        d1 -= datatypes::Decimal{ "25" };
        // Should be back to 100

        // Multiplication assignment
        d1 *= datatypes::Decimal{ "2" };
        // Should be 200

        // Division assignment
        d1 /= datatypes::Decimal{ "4" };

        // Should be 50
        EXPECT_EQ( d1.toString(), "50" );
        EXPECT_FALSE( d1 == 0 );
    }

    TEST( DecimalAssignment, UnaryMinus )
    {
        datatypes::Decimal d1{ "123.45" };
        datatypes::Decimal d2{ -d1 };

        EXPECT_FALSE( d1 < 0 );
        EXPECT_TRUE( d2 < 0 );
        EXPECT_EQ( d1.toString(), "123.45" );

        // Double negation
        datatypes::Decimal d3{ -d2 };
        EXPECT_FALSE( d3 < 0 );
        EXPECT_EQ( d3.toString(), "123.45" );
    }

    //----------------------------------------------
    // Conversion
    //----------------------------------------------

    TEST( DecimalConversion, ToDouble )
    {
        datatypes::Decimal d1{ 123.456 };
        double converted{ d1.toDouble() };
        EXPECT_NEAR( converted, 123.456, 0.001 );

        datatypes::Decimal d2{ -789.123 };
        converted = d2.toDouble();
        EXPECT_NEAR( converted, -789.123, 0.001 );

        datatypes::Decimal d3{ 0 };
        converted = d3.toDouble();
        EXPECT_EQ( converted, 0.0 );
    }

    TEST( DecimalConversion, ToString )
    {
        // Basic cases
        EXPECT_EQ( datatypes::Decimal{ 123 }.toString(), "123" );
        EXPECT_EQ( datatypes::Decimal{ -456 }.toString(), "-456" );
        EXPECT_EQ( datatypes::Decimal{ 0 }.toString(), "0" );

        // Decimal cases
        datatypes::Decimal d1{ "123.456" };
        EXPECT_EQ( d1.toString(), "123.456" );

        datatypes::Decimal d2{ "-0.001" };
        EXPECT_EQ( d2.toString(), "-0.001" );
    }

    TEST( DecimalConversion, ToBits )
    {
        // Test zero value
        datatypes::Decimal zero{ 0 };
        auto zeroBits{ zero.toBits() };
        EXPECT_EQ( zeroBits.size(), 4 );
        EXPECT_EQ( zeroBits[0], 0 ); // Low mantissa
        EXPECT_EQ( zeroBits[1], 0 ); // Mid mantissa
        EXPECT_EQ( zeroBits[2], 0 ); // High mantissa
        EXPECT_EQ( zeroBits[3], 0 ); // Flags (scale=0, positive)

        // Test simple positive integer
        datatypes::Decimal simpleInt{ 123 };
        auto intBits{ simpleInt.toBits() };
        EXPECT_EQ( intBits[0], 123 ); // Low mantissa should contain 123
        EXPECT_EQ( intBits[1], 0 );   // Mid mantissa should be 0
        EXPECT_EQ( intBits[2], 0 );   // High mantissa should be 0
        EXPECT_EQ( intBits[3], 0 );   // Flags: scale=0, positive

        // Test positive decimal with scale
        datatypes::Decimal decimal{ "123.45" };
        auto decimalBits{ decimal.toBits() };
        EXPECT_EQ( decimalBits[0], 12345 ); // Mantissa should be 12345 (123.45 * 10^2)
        EXPECT_EQ( decimalBits[1], 0 );     // Mid mantissa should be 0
        EXPECT_EQ( decimalBits[2], 0 );     // High mantissa should be 0
        // Flags should contain scale=2, positive (scale in bits 16-23)
        EXPECT_EQ( ( decimalBits[3] >> 16 ) & 0xFF, 2 ); // Scale should be 2

        // Test negative value
        datatypes::Decimal negative{ "-456.789" };
        auto negativeBits{ negative.toBits() };
        EXPECT_EQ( negativeBits[0], 456789 ); // Mantissa should be 456789
        EXPECT_EQ( negativeBits[1], 0 );      // Mid mantissa should be 0
        EXPECT_EQ( negativeBits[2], 0 );      // High mantissa should be 0
        // Sign bit (bit 31) should be set in flags
        EXPECT_NE( static_cast<std::uint32_t>( negativeBits[3] ) & constants::DECIMAL_SIGN_MASK, 0u );

        // Scale should be 3
        EXPECT_EQ( ( negativeBits[3] >> 16 ) & 0xFF, 3u );

        // Test large value requiring multiple mantissa words
        datatypes::Decimal large{ "123456789012345.678" };
        auto largeBits{ large.toBits() };
        EXPECT_NE( largeBits[0], 0 ); // Low mantissa should be non-zero
        // For large numbers, we might use higher mantissa words
        // The exact values depend on internal representation

        // Test very small decimal
        datatypes::Decimal small{ "0.001" };
        auto smallBits{ small.toBits() };
        EXPECT_EQ( smallBits[0], 1 ); // Mantissa should be 1 (0.001 * 10^3)
        EXPECT_EQ( smallBits[1], 0 ); // Mid mantissa should be 0
        EXPECT_EQ( smallBits[2], 0 ); // High mantissa should be 0
        // Scale should be 3
        EXPECT_EQ( ( smallBits[3] >> 16 ) & 0xFF, 3u );
        // Should be positive
        EXPECT_EQ( static_cast<std::uint32_t>( smallBits[3] ) & constants::DECIMAL_SIGN_MASK, 0u );
    }

    //----------------------------------------------
    // Arithmetic
    //----------------------------------------------

    TEST( DecimalArithmetic, Addition )
    {
        datatypes::Decimal d1{ "123.45" };
        datatypes::Decimal d2{ "67.89" };
        auto result{ d1 + d2 };

        // Should be close to 191.34
        EXPECT_FALSE( result == 0 );
        EXPECT_FALSE( result < 0 );

        // Test with different signs
        datatypes::Decimal d3{ "100" };
        datatypes::Decimal d4{ "-50" };
        result = d3 + d4;
        // Should be 50
        EXPECT_FALSE( result == 0 );
        EXPECT_FALSE( result < 0 );

        // Test adding zero
        result = d1 + datatypes::Decimal{ 0 };
        EXPECT_EQ( result.toString(), d1.toString() );
    }

    TEST( DecimalArithmetic, Subtraction )
    {
        datatypes::Decimal d1{ "100" };
        datatypes::Decimal d2{ "30" };
        datatypes::Decimal result{ d1 - d2 };

        // Should be 70
        EXPECT_FALSE( result == 0 );
        EXPECT_FALSE( result < 0 );

        // Test negative result
        result = d2 - d1;
        // Should be -70
        EXPECT_FALSE( result == 0 );
        EXPECT_TRUE( result < 0 );

        // Test subtracting self
        result = d1 - d1;
        EXPECT_TRUE( result == 0 );
    }

    TEST( DecimalArithmetic, Multiplication )
    {
        datatypes::Decimal d1{ "12.5" };
        datatypes::Decimal d2{ "8" };
        datatypes::Decimal result{ d1 * d2 };

        // Should be 100
        EXPECT_FALSE( result == 0 );
        EXPECT_FALSE( result < 0 );

        // Test with negative
        datatypes::Decimal d3{ "-5" };
        result = d1 * d3;
        // Should be -62.5
        EXPECT_FALSE( result == 0 );
        EXPECT_TRUE( result < 0 );

        // Test multiply by zero
        result = d1 * datatypes::Decimal{ 0 };
        EXPECT_TRUE( result == 0 );
    }

    TEST( DecimalArithmetic, Division )
    {
        datatypes::Decimal d1{ "100" };
        datatypes::Decimal d2{ "4" };
        datatypes::Decimal result{ d1 / d2 };

        // Should be 25
        EXPECT_FALSE( result == 0 );
        EXPECT_FALSE( result < 0 );
        EXPECT_EQ( result.toString(), "25" );

        // Test with negative
        datatypes::Decimal d3{ "-20" };
        result = d1 / d3;
        // Should be -5
        EXPECT_FALSE( result == 0 );
        EXPECT_TRUE( result < 0 );
        EXPECT_EQ( result.toString(), "-5" );

        // Test division with fractional results
        datatypes::Decimal d4{ "10" };
        datatypes::Decimal d5{ "3" };
        result = d4 / d5;
        EXPECT_TRUE( result > datatypes::Decimal{ "3.3" } );
        EXPECT_TRUE( result < datatypes::Decimal{ "3.4" } );

        // Test division that was failing in sqrt
        datatypes::Decimal d6{ "2" };
        datatypes::Decimal d7{ "1.5" };
        result = d6 / d7;
        EXPECT_TRUE( result > datatypes::Decimal{ "1.3" } );
        EXPECT_TRUE( result < datatypes::Decimal{ "1.4" } );

        // Test division with more precision
        datatypes::Decimal d8{ "2" };
        datatypes::Decimal d9{ "1.416666666666666665" };
        result = d8 / d9;
        EXPECT_TRUE( result > datatypes::Decimal{ "1.4" } );
        EXPECT_TRUE( result < datatypes::Decimal{ "1.5" } );

        // Test division by zero
        EXPECT_THROW( d1 / datatypes::Decimal{ 0 }, std::overflow_error );
    }

    //----------------------------------------------
    // Arithmetic -free functions
    //----------------------------------------------

    TEST( DecimalFreeFunctions, AbsFunction )
    {
        using namespace nfx;

        // Test positive value
        datatypes::Decimal pos{ "123.45" };
        EXPECT_EQ( abs( pos ), datatypes::Decimal{ "123.45" } );

        // Test negative value
        datatypes::Decimal neg{ "-123.45" };
        EXPECT_EQ( abs( neg ), datatypes::Decimal{ "123.45" } );

        // Test zero
        datatypes::Decimal zero{ "0" };
        EXPECT_EQ( abs( zero ), datatypes::Decimal{ "0" } );

        // Test very small negative
        datatypes::Decimal smallNeg{ "-0.0001" };
        EXPECT_EQ( abs( smallNeg ), datatypes::Decimal{ "0.0001" } );
    }

    TEST( DecimalFreeFunctions, SqrtFunction )
    {
        using namespace nfx;

        // Test perfect squares
        EXPECT_EQ( sqrt( datatypes::Decimal{ "4" } ), datatypes::Decimal{ "2" } );
        EXPECT_EQ( sqrt( datatypes::Decimal{ "9" } ), datatypes::Decimal{ "3" } );
        EXPECT_EQ( sqrt( datatypes::Decimal{ "100" } ), datatypes::Decimal{ "10" } );

        // Test non-perfect square
        datatypes::Decimal sqrtTwo = sqrt( datatypes::Decimal{ "2" } );
        EXPECT_TRUE( sqrtTwo > datatypes::Decimal{ "1.414213" } );
        EXPECT_TRUE( sqrtTwo < datatypes::Decimal{ "1.414214" } );

        // Test zero
        EXPECT_EQ( sqrt( datatypes::Decimal{ "0" } ), datatypes::Decimal{ "0" } );

        // Test negative throws
        EXPECT_THROW( [[maybe_unused]] auto result = sqrt( datatypes::Decimal{ "-1" } ), std::domain_error );
    }

    TEST( DecimalFreeFunctions, RoundFunction )
    {
        using namespace nfx;

        // Test basic rounding
        EXPECT_EQ( round( datatypes::Decimal{ "123.456" }, 2 ), datatypes::Decimal{ "123.46" } );
        EXPECT_EQ( round( datatypes::Decimal{ "123.456" }, 1 ), datatypes::Decimal{ "123.5" } );
        EXPECT_EQ( round( datatypes::Decimal{ "123.456" }, 0 ), datatypes::Decimal{ "123" } );

        // Test with rounding modes
        datatypes::Decimal value{ "2.5" };
        EXPECT_EQ( round( value, 0, datatypes::Decimal::RoundingMode::ToNearest ), datatypes::Decimal{ "2" } ); // Banker's rounding
        EXPECT_EQ( round( value, 0, datatypes::Decimal::RoundingMode::ToNearestTiesAway ), datatypes::Decimal{ "3" } );
    }

    TEST( DecimalFreeFunctions, FloorFunction )
    {
        using namespace nfx;

        EXPECT_EQ( floor( datatypes::Decimal{ "123.9" } ), datatypes::Decimal{ "123" } );
        EXPECT_EQ( floor( datatypes::Decimal{ "123.1" } ), datatypes::Decimal{ "123" } );
        EXPECT_EQ( floor( datatypes::Decimal{ "-123.1" } ), datatypes::Decimal{ "-124" } );
        EXPECT_EQ( floor( datatypes::Decimal{ "-123.9" } ), datatypes::Decimal{ "-124" } );
        EXPECT_EQ( floor( datatypes::Decimal{ "123" } ), datatypes::Decimal{ "123" } );
    }

    TEST( DecimalFreeFunctions, CeilingFunction )
    {
        using namespace nfx;

        EXPECT_EQ( ceil( datatypes::Decimal{ "123.1" } ), datatypes::Decimal{ "124" } );
        EXPECT_EQ( ceil( datatypes::Decimal{ "123.9" } ), datatypes::Decimal{ "124" } );
        EXPECT_EQ( ceil( datatypes::Decimal{ "-123.9" } ), datatypes::Decimal{ "-123" } );
        EXPECT_EQ( ceil( datatypes::Decimal{ "-123.1" } ), datatypes::Decimal{ "-123" } );
        EXPECT_EQ( ceil( datatypes::Decimal{ "123" } ), datatypes::Decimal{ "123" } );
    }

    TEST( DecimalFreeFunctions, TruncateFunction )
    {
        using namespace nfx;

        EXPECT_EQ( trunc( datatypes::Decimal{ "123.456" } ), datatypes::Decimal{ "123" } );
        EXPECT_EQ( trunc( datatypes::Decimal{ "123.999" } ), datatypes::Decimal{ "123" } );
        EXPECT_EQ( trunc( datatypes::Decimal{ "-123.456" } ), datatypes::Decimal{ "-123" } );
        EXPECT_EQ( trunc( datatypes::Decimal{ "-123.999" } ), datatypes::Decimal{ "-123" } );
        EXPECT_EQ( trunc( datatypes::Decimal{ "123" } ), datatypes::Decimal{ "123" } );
    }

    TEST( DecimalFreeFunctions, ADLSupport )
    {
        using namespace nfx::datatypes;

        // Test that ADL (Argument-Dependent Lookup) works
        // These calls should find the free functions via ADL
        Decimal value{ "123.456" };
        Decimal negative{ "-123.456" };

        // abs via ADL
        auto absResult = abs( negative );
        EXPECT_EQ( absResult, Decimal{ "123.456" } );

        // sqrt via ADL
        auto sqrtResult = sqrt( Decimal{ "4" } );
        EXPECT_EQ( sqrtResult, Decimal{ "2" } );

        // round via ADL
        auto roundResult = round( value, 2 );
        EXPECT_EQ( roundResult, Decimal{ "123.46" } );

        // floor via ADL
        auto floorResult = floor( value );
        EXPECT_EQ( floorResult, Decimal{ "123" } );

        // ceiling via ADL
        auto ceilingResult = ceil( value );
        EXPECT_EQ( ceilingResult, Decimal{ "124" } );

        // truncate via ADL
        auto truncResult = trunc( value );
        EXPECT_EQ( truncResult, Decimal{ "123" } );
    }

    //----------------------------------------------
    // Comparison
    //----------------------------------------------

    TEST( DecimalComparison, Equality )
    {
        datatypes::Decimal d1{ "123.45" };
        datatypes::Decimal d2{ "123.45" };
        datatypes::Decimal d3{ "123.46" };

        EXPECT_TRUE( d1 == d2 );
        EXPECT_FALSE( d1 == d3 );
        EXPECT_FALSE( d1 != d2 );
        EXPECT_TRUE( d1 != d3 );

        // Test with zero
        datatypes::Decimal zero1{ 0 };
        datatypes::Decimal zero2{ "0.0" };
        EXPECT_TRUE( zero1 == zero2 );
    }

    TEST( DecimalComparison, Comparison )
    {
        datatypes::Decimal d1{ "100" };
        datatypes::Decimal d2{ "200" };
        datatypes::Decimal d3{ "-50" };

        // Less than
        EXPECT_TRUE( d1 < d2 );
        EXPECT_FALSE( d2 < d1 );
        EXPECT_TRUE( d3 < d1 );

        // Greater than
        EXPECT_TRUE( d2 > d1 );
        EXPECT_FALSE( d1 > d2 );
        EXPECT_TRUE( d1 > d3 );

        // Less than or equal
        EXPECT_TRUE( d1 <= d2 );
        EXPECT_TRUE( d1 <= datatypes::Decimal{ "100" } );
        EXPECT_FALSE( d2 <= d1 );

        // Greater than or equal
        EXPECT_TRUE( d2 >= d1 );
        EXPECT_TRUE( d1 >= datatypes::Decimal{ "100" } );
        EXPECT_FALSE( d1 >= d2 );
    }

    //----------------------------------------------
    // Comparison with built-in floating-point types
    //----------------------------------------------

    TEST( DecimalBuiltinComparison, FloatingPointComparison )
    {
        datatypes::Decimal d1{ 123.456 };
        datatypes::Decimal d2{ -123.456 };
        datatypes::Decimal zero{};

        // Test equality with double
        EXPECT_TRUE( d1 == 123.456 );
        EXPECT_TRUE( d2 == -123.456 );
        EXPECT_TRUE( zero == 0.0 );
        EXPECT_FALSE( d1 == 123.457 );

        // Test inequality with double
        EXPECT_FALSE( d1 != 123.456 );
        EXPECT_TRUE( d1 != 123.457 );
        EXPECT_TRUE( d2 != 123.456 );

        // Test less than with double
        EXPECT_TRUE( d1 < 123.457 );
        EXPECT_FALSE( d1 < 123.456 );
        EXPECT_FALSE( d1 < 123.455 );
        EXPECT_TRUE( d2 < 0.0 );

        // Test less than or equal with double
        EXPECT_TRUE( d1 <= 123.457 );
        EXPECT_TRUE( d1 <= 123.456 );
        EXPECT_FALSE( d1 <= 123.455 );

        // Test greater than with double
        EXPECT_FALSE( d1 > 123.457 );
        EXPECT_FALSE( d1 > 123.456 );
        EXPECT_TRUE( d1 > 123.455 );
        EXPECT_FALSE( d2 > 0.0 );

        // Test greater than or equal with double
        EXPECT_FALSE( d1 >= 123.457 );
        EXPECT_TRUE( d1 >= 123.456 );
        EXPECT_TRUE( d1 >= 123.455 );

        // Test with float - demonstrates IEEE 754 precision limitations
        // 123.456 cannot be represented exactly in either float or double
        float test_float = 123.456f; // Stored as ~123.45600128... (float precision)
        // double is ~123.45600000... (double precision) - different from float!

        // When Decimal is constructed from the float's double-cast value,
        // it should compare equal to that same float value
        datatypes::Decimal d_from_float{ static_cast<double>( test_float ) };
        EXPECT_TRUE( d_from_float == test_float ); // Same precision source

        // But a Decimal from exact double won't equal the float due to different precision errors
        datatypes::Decimal d_from_double{ 123.456 }; // From double literal
        EXPECT_FALSE( d_from_double == test_float ); // Different precision errors

        // Test ordering with float values
        EXPECT_TRUE( d1 < 123.457f );
        EXPECT_TRUE( d1 > 123.455f );
    }

    TEST( DecimalBuiltinComparison, FloatingPointSpecialValues )
    {
        datatypes::Decimal d{ 123.456 };

        // Test with NaN - should always return false for equality, no ordering
        double nan = std::numeric_limits<double>::quiet_NaN();
        EXPECT_FALSE( d == nan );
        EXPECT_TRUE( d != nan );
        EXPECT_FALSE( d < nan );
        EXPECT_FALSE( d <= nan );
        EXPECT_FALSE( d > nan );
        EXPECT_FALSE( d >= nan );
    }

    TEST( DecimalBuiltinComparison, FloatingPointPrecisionBehavior )
    {
        // Test that demonstrates IEEE 754 floating-point precision limitations

        // Values that can be represented exactly in float
        datatypes::Decimal d_exact{ 123.5 }; // 123.5 is exactly representable
        EXPECT_TRUE( d_exact == 123.5f );
        EXPECT_TRUE( d_exact == 123.5 );

        // Values that cannot be represented exactly in float
        float imprecise_float = 123.456f;
        double imprecise_double = 123.456;

        // The float and double have different precision errors
        // Use explicit floating point comparison to demonstrate precision differences
        EXPECT_FALSE( std::equal_to<double>{}( static_cast<double>( imprecise_float ), imprecise_double ) );

        // But each Decimal compares equal to its source type due to consistent precision handling
        datatypes::Decimal d_from_float{ static_cast<double>( imprecise_float ) };
        datatypes::Decimal d_from_double{ imprecise_double };

        EXPECT_TRUE( d_from_float == imprecise_float );   // Consistent with float precision
        EXPECT_TRUE( d_from_double == imprecise_double ); // Consistent with double precision

        // For 123.456, float and double typically have different precision errors,
        // but we verify this rather than assume it
        if ( !std::equal_to<double>{}( static_cast<double>( imprecise_float ), imprecise_double ) )
        {
            EXPECT_FALSE( d_from_float == d_from_double ); // Different precision sources
        }

        // This behavior matches IEEE 754 semantics and is mathematically correct
    }

    //----------------------------------------------
    // Comparison with built-in integer types
    //----------------------------------------------

    TEST( DecimalBuiltinComparison, SignedIntegerComparison )
    {
        datatypes::Decimal d1{ 42 };
        datatypes::Decimal d2{ -42 };
        datatypes::Decimal d3{ 42.5 };
        datatypes::Decimal zero{};

        // Test equality with int64_t
        EXPECT_TRUE( d1 == std::int64_t{ 42 } );
        EXPECT_TRUE( d2 == std::int64_t{ -42 } );
        EXPECT_TRUE( zero == std::int64_t{ 0 } );
        EXPECT_FALSE( d1 == std::int64_t{ 43 } );
        EXPECT_FALSE( d3 == std::int64_t{ 42 } ); // Fractional part means not equal to integer

        // Test inequality with int64_t
        EXPECT_FALSE( d1 != std::int64_t{ 42 } );
        EXPECT_TRUE( d1 != std::int64_t{ 43 } );
        EXPECT_TRUE( d3 != std::int64_t{ 42 } );

        // Test less than with int64_t
        EXPECT_TRUE( d1 < std::int64_t{ 43 } );
        EXPECT_FALSE( d1 < std::int64_t{ 42 } );
        EXPECT_FALSE( d1 < std::int64_t{ 41 } );
        EXPECT_TRUE( d2 < std::int64_t{ 0 } );

        // Test less than or equal with int64_t
        EXPECT_TRUE( d1 <= std::int64_t{ 43 } );
        EXPECT_TRUE( d1 <= std::int64_t{ 42 } );
        EXPECT_FALSE( d1 <= std::int64_t{ 41 } );

        // Test greater than with int64_t
        EXPECT_FALSE( d1 > std::int64_t{ 43 } );
        EXPECT_FALSE( d1 > std::int64_t{ 42 } );
        EXPECT_TRUE( d1 > std::int64_t{ 41 } );
        EXPECT_FALSE( d2 > std::int64_t{ 0 } );

        // Test greater than or equal with int64_t
        EXPECT_FALSE( d1 >= std::int64_t{ 43 } );
        EXPECT_TRUE( d1 >= std::int64_t{ 42 } );
        EXPECT_TRUE( d1 >= std::int64_t{ 41 } );

        // Test with int32_t (should delegate to int64_t)
        EXPECT_TRUE( d1 == 42 );
        EXPECT_TRUE( d1 < 43 );
        EXPECT_TRUE( d1 > 41 );
    }

    TEST( DecimalBuiltinComparison, UnsignedIntegerComparison )
    {
        datatypes::Decimal d1{ 42 };
        datatypes::Decimal d2{ -42 };
        datatypes::Decimal d3{ 42.5 };
        datatypes::Decimal zero{};

        // Test equality with uint64_t
        EXPECT_TRUE( d1 == std::uint64_t{ 42 } );
        EXPECT_TRUE( zero == std::uint64_t{ 0 } );
        EXPECT_FALSE( d1 == std::uint64_t{ 43 } );
        EXPECT_FALSE( d2 == std::uint64_t{ 42 } ); // Negative decimal can't equal positive uint64_t
        EXPECT_FALSE( d3 == std::uint64_t{ 42 } ); // Fractional part means not equal to integer

        // Test inequality with uint64_t
        EXPECT_FALSE( d1 != std::uint64_t{ 42 } );
        EXPECT_TRUE( d1 != std::uint64_t{ 43 } );
        EXPECT_TRUE( d2 != std::uint64_t{ 42 } );
        EXPECT_TRUE( d3 != std::uint64_t{ 42 } );

        // Test less than with uint64_t
        EXPECT_TRUE( d1 < std::uint64_t{ 43 } );
        EXPECT_FALSE( d1 < std::uint64_t{ 42 } );
        EXPECT_FALSE( d1 < std::uint64_t{ 41 } );
        EXPECT_TRUE( d2 < std::uint64_t{ 42 } ); // Negative is always less than positive uint64_t

        // Test less than or equal with uint64_t
        EXPECT_TRUE( d1 <= std::uint64_t{ 43 } );
        EXPECT_TRUE( d1 <= std::uint64_t{ 42 } );
        EXPECT_FALSE( d1 <= std::uint64_t{ 41 } );
        EXPECT_TRUE( d2 <= std::uint64_t{ 42 } );

        // Test greater than with uint64_t
        EXPECT_FALSE( d1 > std::uint64_t{ 43 } );
        EXPECT_FALSE( d1 > std::uint64_t{ 42 } );
        EXPECT_TRUE( d1 > std::uint64_t{ 41 } );
        EXPECT_FALSE( d2 > std::uint64_t{ 42 } ); // Negative is never greater than positive uint64_t

        // Test greater than or equal with uint64_t
        EXPECT_FALSE( d1 >= std::uint64_t{ 43 } );
        EXPECT_TRUE( d1 >= std::uint64_t{ 42 } );
        EXPECT_TRUE( d1 >= std::uint64_t{ 41 } );
        EXPECT_FALSE( d2 >= std::uint64_t{ 42 } );
    }

    TEST( DecimalBuiltinComparison, ComparisonEdgeCases )
    {
        datatypes::Decimal large{ "999999999999999999999999" };
        datatypes::Decimal small{ "0.000000000001" };
        datatypes::Decimal zero{};

        // Test with maximum values
        EXPECT_FALSE( large == std::numeric_limits<std::int64_t>::max() );
        EXPECT_TRUE( large > std::numeric_limits<std::int64_t>::max() );

        // Test with minimum values
        datatypes::Decimal negative_large{ "-999999999999999999999999" };
        EXPECT_FALSE( negative_large == std::numeric_limits<std::int64_t>::min() );
        EXPECT_TRUE( negative_large < std::numeric_limits<std::int64_t>::min() );

        // Test very small values
        EXPECT_FALSE( small == 0 );
        EXPECT_TRUE( small > 0 );
        EXPECT_TRUE( small != std::uint64_t{ 0 } );
        EXPECT_FALSE( small == std::uint64_t{ 0 } );

        // Test zero comparisons
        EXPECT_TRUE( zero == 0 );
        EXPECT_TRUE( zero == std::int64_t{ 0 } );
        EXPECT_TRUE( zero == std::uint64_t{ 0 } );
        EXPECT_TRUE( zero == 0.0 );
        EXPECT_TRUE( zero == 0.0f );
    }

    TEST( DecimalBuiltinComparison, ComparisonSymmetry )
    {
        datatypes::Decimal d{ 42.5 };

        // Test that comparison operators work consistently
        // For floating-point comparisons
        EXPECT_EQ( d == 42.5, true );
        EXPECT_EQ( d != 42.6, true );
        EXPECT_EQ( d < 42.6, true );
        EXPECT_EQ( d <= 42.5, true );
        EXPECT_EQ( d > 42.4, true );
        EXPECT_EQ( d >= 42.5, true );

        // For integer comparisons with fractional decimal
        EXPECT_EQ( d == 42, false ); // Has fractional part
        EXPECT_EQ( d != 42, true );
        EXPECT_EQ( d > 42, true );
        EXPECT_EQ( d >= 42, true );
        EXPECT_EQ( d < 43, true );
        EXPECT_EQ( d <= 43, true );
    }

    //----------------------------------------------
    // Comparison with nfx::datatypes::Int128
    //----------------------------------------------

    TEST( DecimalInt128Comparison, EqualityComparison )
    {
        // Test equality with positive values
        datatypes::Decimal d1{ 42 };
        datatypes::Int128 i1{ 42 };
        EXPECT_TRUE( d1 == i1 );
        EXPECT_FALSE( d1 != i1 );

        // Test equality with negative values
        datatypes::Decimal d2{ -123 };
        datatypes::Int128 i2{ -123 };
        EXPECT_TRUE( d2 == i2 );
        EXPECT_FALSE( d2 != i2 );

        // Test equality with zero
        datatypes::Decimal d3{ 0 };
        datatypes::Int128 i3{ 0 };
        EXPECT_TRUE( d3 == i3 );
        EXPECT_FALSE( d3 != i3 );

        // Test inequality with different values
        datatypes::Decimal d4{ 100 };
        datatypes::Int128 i4{ 200 };
        EXPECT_FALSE( d4 == i4 );
        EXPECT_TRUE( d4 != i4 );

        // Test inequality when Decimal has fractional part
        datatypes::Decimal d5{ 42.5 };
        datatypes::Int128 i5{ 42 };
        EXPECT_FALSE( d5 == i5 );
        EXPECT_TRUE( d5 != i5 );

        // Test inequality with different signs
        datatypes::Decimal d6{ 42 };
        datatypes::Int128 i6{ -42 };
        EXPECT_FALSE( d6 == i6 );
        EXPECT_TRUE( d6 != i6 );
    }

    TEST( DecimalInt128Comparison, OrderingComparison )
    {
        // Test less than with positive values
        datatypes::Decimal d1{ 100 };
        datatypes::Int128 i1{ 200 };
        EXPECT_TRUE( d1 < i1 );
        EXPECT_TRUE( d1 <= i1 );
        EXPECT_FALSE( d1 > i1 );
        EXPECT_FALSE( d1 >= i1 );

        // Test greater than with positive values
        datatypes::Decimal d2{ 300 };
        datatypes::Int128 i2{ 200 };
        EXPECT_FALSE( d2 < i2 );
        EXPECT_FALSE( d2 <= i2 );
        EXPECT_TRUE( d2 > i2 );
        EXPECT_TRUE( d2 >= i2 );

        // Test equal values
        datatypes::Decimal d3{ 150 };
        datatypes::Int128 i3{ 150 };
        EXPECT_FALSE( d3 < i3 );
        EXPECT_TRUE( d3 <= i3 );
        EXPECT_FALSE( d3 > i3 );
        EXPECT_TRUE( d3 >= i3 );

        // Test with negative vs positive
        datatypes::Decimal d4{ -50 };
        datatypes::Int128 i4{ 50 };
        EXPECT_TRUE( d4 < i4 );
        EXPECT_TRUE( d4 <= i4 );
        EXPECT_FALSE( d4 > i4 );
        EXPECT_FALSE( d4 >= i4 );

        // Test with positive vs negative
        datatypes::Decimal d5{ 50 };
        datatypes::Int128 i5{ -50 };
        EXPECT_FALSE( d5 < i5 );
        EXPECT_FALSE( d5 <= i5 );
        EXPECT_TRUE( d5 > i5 );
        EXPECT_TRUE( d5 >= i5 );
    }

    TEST( DecimalInt128Comparison, NegativeValueComparison )
    {
        // Test negative vs negative - less negative is greater
        datatypes::Decimal d1{ -100 };
        datatypes::Int128 i1{ -200 };
        EXPECT_FALSE( d1 < i1 );
        EXPECT_FALSE( d1 <= i1 );
        EXPECT_TRUE( d1 > i1 );
        EXPECT_TRUE( d1 >= i1 );

        // Test negative vs negative - more negative is smaller
        datatypes::Decimal d2{ -300 };
        datatypes::Int128 i2{ -200 };
        EXPECT_TRUE( d2 < i2 );
        EXPECT_TRUE( d2 <= i2 );
        EXPECT_FALSE( d2 > i2 );
        EXPECT_FALSE( d2 >= i2 );

        // Test equal negative values
        datatypes::Decimal d3{ -150 };
        datatypes::Int128 i3{ -150 };
        EXPECT_FALSE( d3 < i3 );
        EXPECT_TRUE( d3 <= i3 );
        EXPECT_FALSE( d3 > i3 );
        EXPECT_TRUE( d3 >= i3 );
    }

    TEST( DecimalInt128Comparison, FractionalPartHandling )
    {
        // Decimal with fractional part vs integer
        datatypes::Decimal d1{ 42.7 };
        datatypes::Int128 i1{ 42 };
        EXPECT_FALSE( d1 == i1 ); // Not equal due to fractional part
        EXPECT_TRUE( d1 != i1 );
        EXPECT_FALSE( d1 < i1 );
        EXPECT_FALSE( d1 <= i1 );
        EXPECT_TRUE( d1 > i1 );
        EXPECT_TRUE( d1 >= i1 );

        // Decimal with fractional part vs next integer
        datatypes::Decimal d2{ 42.3 };
        datatypes::Int128 i2{ 43 };
        EXPECT_FALSE( d2 == i2 );
        EXPECT_TRUE( d2 != i2 );
        EXPECT_TRUE( d2 < i2 );
        EXPECT_TRUE( d2 <= i2 );
        EXPECT_FALSE( d2 > i2 );
        EXPECT_FALSE( d2 >= i2 );

        // Negative decimal with fractional part
        datatypes::Decimal d3{ -42.3 };
        datatypes::Int128 i3{ -42 };
        EXPECT_FALSE( d3 == i3 );
        EXPECT_TRUE( d3 != i3 );
        EXPECT_TRUE( d3 < i3 ); // -42.3 < -42
        EXPECT_TRUE( d3 <= i3 );
        EXPECT_FALSE( d3 > i3 );
        EXPECT_FALSE( d3 >= i3 );

        // Negative decimal with fractional part vs more negative integer
        datatypes::Decimal d4{ -42.7 };
        datatypes::Int128 i4{ -43 };
        EXPECT_FALSE( d4 == i4 );
        EXPECT_TRUE( d4 != i4 );
        EXPECT_FALSE( d4 < i4 ); // -42.7 > -43
        EXPECT_FALSE( d4 <= i4 );
        EXPECT_TRUE( d4 > i4 );
        EXPECT_TRUE( d4 >= i4 );
    }

    TEST( DecimalInt128Comparison, ZeroComparison )
    {
        // Positive decimal vs zero
        datatypes::Decimal d1{ 0.001 };
        datatypes::Int128 i1{ 0 };
        EXPECT_FALSE( d1 == i1 );
        EXPECT_TRUE( d1 != i1 );
        EXPECT_FALSE( d1 < i1 );
        EXPECT_FALSE( d1 <= i1 );
        EXPECT_TRUE( d1 > i1 );
        EXPECT_TRUE( d1 >= i1 );

        // Negative decimal vs zero
        datatypes::Decimal d2{ -0.001 };
        datatypes::Int128 i2{ 0 };
        EXPECT_FALSE( d2 == i2 );
        EXPECT_TRUE( d2 != i2 );
        EXPECT_TRUE( d2 < i2 );
        EXPECT_TRUE( d2 <= i2 );
        EXPECT_FALSE( d2 > i2 );
        EXPECT_FALSE( d2 >= i2 );

        // Zero decimal vs positive integer
        datatypes::Decimal d3{ 0 };
        datatypes::Int128 i3{ 1 };
        EXPECT_FALSE( d3 == i3 );
        EXPECT_TRUE( d3 != i3 );
        EXPECT_TRUE( d3 < i3 );
        EXPECT_TRUE( d3 <= i3 );
        EXPECT_FALSE( d3 > i3 );
        EXPECT_FALSE( d3 >= i3 );

        // Zero decimal vs negative integer
        datatypes::Decimal d4{ 0 };
        datatypes::Int128 i4{ -1 };
        EXPECT_FALSE( d4 == i4 );
        EXPECT_TRUE( d4 != i4 );
        EXPECT_FALSE( d4 < i4 );
        EXPECT_FALSE( d4 <= i4 );
        EXPECT_TRUE( d4 > i4 );
        EXPECT_TRUE( d4 >= i4 );
    }

    TEST( DecimalInt128Comparison, LargeValueComparison )
    {
        // Test with large positive values (within Decimal's 28-digit precision)
        datatypes::Decimal d1{ "1234567890123456789012345678" }; // 28 digits max
        datatypes::Int128 i1{ "1234567890123456789012345678" };
        EXPECT_TRUE( d1 == i1 );
        EXPECT_FALSE( d1 != i1 );

        // Test with large negative values (within Decimal's 28-digit precision)
        datatypes::Decimal d2{ "-1234567890123456789012345678" };
        datatypes::Int128 i2{ "-1234567890123456789012345678" };
        EXPECT_TRUE( d2 == i2 );
        EXPECT_FALSE( d2 != i2 );

        // Test ordering with large values
        datatypes::Decimal d3{ "1234567890123456789012345677" };
        datatypes::Int128 i3{ "1234567890123456789012345678" };
        EXPECT_FALSE( d3 == i3 );
        EXPECT_TRUE( d3 != i3 );
        EXPECT_TRUE( d3 < i3 );
        EXPECT_TRUE( d3 <= i3 );
        EXPECT_FALSE( d3 > i3 );
        EXPECT_FALSE( d3 >= i3 );

        // Test with values approaching but within Decimal precision limits
        datatypes::Decimal d4{ "9876543210987654321098765432" }; // 28 digits, different pattern
        datatypes::Int128 i4{ "9876543210987654321098765432" };
        EXPECT_TRUE( d4 == i4 );
    }

    TEST( DecimalInt128Comparison, LargeValueWithFractionalComparison )
    {
        // Test with decimal having fractional part vs integer (should not be equal)
        datatypes::Decimal d1{ "12345678901234567890.8" }; // Has fractional part
        datatypes::Int128 i1{ "12345678901234567890" };    // Integer part only
        EXPECT_FALSE( d1 == i1 );                          // Cannot be equal due to fractional part
        EXPECT_TRUE( d1 != i1 );
        EXPECT_FALSE( d1 < i1 ); // d1 > i1 because d1 has additional 0.8
        EXPECT_FALSE( d1 <= i1 );
        EXPECT_TRUE( d1 > i1 );
        EXPECT_TRUE( d1 >= i1 );

        // Test with negative decimal having fractional part vs integer
        datatypes::Decimal d2{ "-12345678901234567890.8" }; // More negative
        datatypes::Int128 i2{ "-12345678901234567890" };    // Less negative
        EXPECT_FALSE( d2 == i2 );
        EXPECT_TRUE( d2 != i2 );
        EXPECT_TRUE( d2 < i2 ); // d2 < i2 because d2 is more negative
        EXPECT_TRUE( d2 <= i2 );
        EXPECT_FALSE( d2 > i2 );
        EXPECT_FALSE( d2 >= i2 );

        // Test ordering with large values having small fractional differences
        datatypes::Decimal d3{ "12345678901234567890.1" };
        datatypes::Int128 i3{ "12345678901234567890" }; // Same integer part
        EXPECT_FALSE( d3 == i3 );
        EXPECT_TRUE( d3 != i3 );
        EXPECT_FALSE( d3 < i3 ); // d3 > i3 because d3 has fractional part 0.1
        EXPECT_FALSE( d3 <= i3 );
        EXPECT_TRUE( d3 > i3 );
        EXPECT_TRUE( d3 >= i3 );

        // Test with maximum precision decimal vs large integer (within Int128 limits)
        datatypes::Decimal d4{ "12345678901234567890.123456" }; // 26 total digits with fractional part
        datatypes::Int128 i4{ "12345678901234567890" };         // Same integer part
        EXPECT_FALSE( d4 == i4 );                               // Cannot be equal due to fractional part
        EXPECT_TRUE( d4 != i4 );
        EXPECT_FALSE( d4 < i4 ); // d4 > i4 because of fractional part
        EXPECT_FALSE( d4 <= i4 );
        EXPECT_TRUE( d4 > i4 );
        EXPECT_TRUE( d4 >= i4 );

        // Test edge case: decimal close to but less than next integer
        datatypes::Decimal d5{ "999999999999999999.9" };
        datatypes::Int128 i5{ "1000000000000000000" }; // Next integer
        EXPECT_FALSE( d5 == i5 );
        EXPECT_TRUE( d5 != i5 );
        EXPECT_TRUE( d5 < i5 ); // d5 is still less than i5
        EXPECT_TRUE( d5 <= i5 );
        EXPECT_FALSE( d5 > i5 );
        EXPECT_FALSE( d5 >= i5 );
    }

    TEST( DecimalInt128Comparison, PrecisionEdgeCases )
    {
        // Test decimal with significant fractional part vs integer
        datatypes::Decimal d1{ "42.1" }; // Clear fractional part
        datatypes::Int128 i1{ 42 };
        EXPECT_FALSE( d1 == i1 ); // Fractional part makes them not equal
        EXPECT_TRUE( d1 != i1 );
        EXPECT_FALSE( d1 < i1 );
        EXPECT_FALSE( d1 <= i1 );
        EXPECT_TRUE( d1 > i1 );
        EXPECT_TRUE( d1 >= i1 );

        // Test decimal that normalizes to exact integer vs integer
        datatypes::Decimal d2{ "42.0000000000000000000000000000" };
        datatypes::Int128 i2{ 42 };
        // This should be exactly equal after normalization
        EXPECT_TRUE( d2 == i2 );
        EXPECT_FALSE( d2 != i2 );

        // Test very small decimal vs zero (within precision)
        datatypes::Decimal d3{ "0.0000000000000000000000000001" };
        datatypes::Int128 i3{ 0 };
        EXPECT_FALSE( d3 == i3 );
        EXPECT_TRUE( d3 != i3 );
        EXPECT_FALSE( d3 < i3 );
        EXPECT_FALSE( d3 <= i3 );
        EXPECT_TRUE( d3 > i3 );
        EXPECT_TRUE( d3 >= i3 );
    }

    TEST( DecimalInt128Comparison, ConsistencyWithReversedOperands )
    {
        // Test that Int128 vs Decimal gives consistent results
        // Note: This requires the Int128 operators to be implemented correctly

        datatypes::Decimal d1{ 100 };
        datatypes::Int128 i1{ 200 };

        // Test decimal < int128 vs int128 > decimal
        EXPECT_TRUE( d1 < i1 );
        EXPECT_TRUE( i1 > d1 );

        // Test decimal > int128 vs int128 < decimal
        datatypes::Decimal d2{ 300 };
        datatypes::Int128 i2{ 200 };
        EXPECT_TRUE( d2 > i2 );
        EXPECT_TRUE( i2 < d2 );

        // Test decimal == int128 vs int128 == decimal
        datatypes::Decimal d3{ 150 };
        datatypes::Int128 i3{ 150 };
        EXPECT_TRUE( d3 == i3 );
        EXPECT_TRUE( i3 == d3 );

        // Test with fractional decimal (should not equal integer)
        datatypes::Decimal d4{ 42.5 };
        datatypes::Int128 i4{ 42 };
        EXPECT_FALSE( d4 == i4 );
        EXPECT_FALSE( i4 == d4 );
        EXPECT_TRUE( d4 > i4 );
        EXPECT_TRUE( i4 < d4 );
    }

    //----------------------------------------------
    // Precision
    //----------------------------------------------

    TEST( DecimalPrecision, MaxPrecision )
    {
        // Test with maximum Decimal places (28)
        std::string maxPrecision{ "1.2345678901234567890123456789" };
        datatypes::Decimal d1;
        EXPECT_TRUE( datatypes::Decimal::fromString( maxPrecision, d1 ) );
        EXPECT_FALSE( d1 == 0 );

        // Test exceeding string to be truncated
        // 29 Decimal places -> 28
        std::string tooPrecise{ "1.23456789012345678901234567890" };
        EXPECT_TRUE( datatypes::Decimal::fromString( tooPrecise, d1 ) );
    }

    TEST( DecimalPrecision, LargeNumbers )
    {
        // Test with large integers
        datatypes::Decimal d1{ std::numeric_limits<std::int64_t>::max() };
        EXPECT_FALSE( d1 == 0 );
        EXPECT_FALSE( d1 < 0 );

        datatypes::Decimal d2{ std::numeric_limits<std::int64_t>::min() };
        EXPECT_FALSE( d2 == 0 );
        EXPECT_TRUE( d2 < 0 );
    }

    TEST( DecimalPrecision, VerySmallNumbers )
    {
        // Test very small Decimal values
        // 28 Decimal places
        datatypes::Decimal d1{ "0.0000000000000000000000000001" };
        EXPECT_FALSE( d1 == 0 );
        EXPECT_FALSE( d1 < 0 );
        EXPECT_EQ( d1.scale(), 28 );
    }

    //----------------------------------------------
    // Performance
    //----------------------------------------------

    TEST( DecimalPerformance, StressTestLargeOperations )
    {
        // Perform many operations to test stability
        datatypes::Decimal accumulator{ "0" };
        datatypes::Decimal increment{ "0.001" };

        for ( int i{ 0 }; i < 1000; ++i )
        {
            accumulator += increment;
        }

        // Should be close to 1.0
        EXPECT_FALSE( accumulator == 0 );
        EXPECT_FALSE( accumulator < 0 );
    }

    //----------------------------------------------
    // Utilities
    //----------------------------------------------

    TEST( DecimalUtlities, DecimalPlacesCount )
    {
        // Test zero values
        datatypes::Decimal d1{ "0" };
        EXPECT_EQ( d1.decimalPlacesCount(), 0 );

        datatypes::Decimal d2{ "0.0" };
        EXPECT_EQ( d2.decimalPlacesCount(), 0 );

        datatypes::Decimal d3{ "0.000" };
        EXPECT_EQ( d3.decimalPlacesCount(), 0 );

        // Test integers (no decimal places)
        datatypes::Decimal d4{ "123" };
        EXPECT_EQ( d4.decimalPlacesCount(), 0 );

        datatypes::Decimal d5{ "-456" };
        EXPECT_EQ( d5.decimalPlacesCount(), 0 );

        // Test decimals without trailing zeros
        datatypes::Decimal d6{ "123.456" };
        EXPECT_EQ( d6.decimalPlacesCount(), 3 );

        datatypes::Decimal d7{ "-789.123" };
        EXPECT_EQ( d7.decimalPlacesCount(), 3 );

        datatypes::Decimal d8{ "0.001" };
        EXPECT_EQ( d8.decimalPlacesCount(), 3 );

        datatypes::Decimal d9{ "0.5" };
        EXPECT_EQ( d9.decimalPlacesCount(), 1 );

        // Test decimals with trailing zeros (should ignore them)
        datatypes::Decimal d10{ "123.4500" };
        EXPECT_EQ( d10.decimalPlacesCount(), 2 ); // 123.45, trailing zeros ignored

        datatypes::Decimal d11{ "123.000" };
        EXPECT_EQ( d11.decimalPlacesCount(), 0 ); // Integer value

        datatypes::Decimal d12{ "456.7800" };
        EXPECT_EQ( d12.decimalPlacesCount(), 2 ); // 456.78, trailing zeros ignored

        datatypes::Decimal d13{ "0.1000" };
        EXPECT_EQ( d13.decimalPlacesCount(), 1 ); // 0.1, trailing zeros ignored

        // Test edge cases with various trailing zero patterns
        datatypes::Decimal d14{ "123.4560" };
        EXPECT_EQ( d14.decimalPlacesCount(), 3 ); // 123.456, one trailing zero ignored

        datatypes::Decimal d15{ "123.4000" };
        EXPECT_EQ( d15.decimalPlacesCount(), 1 ); // 123.4, three trailing zeros ignored

        datatypes::Decimal d16{ "999.9990" };
        EXPECT_EQ( d16.decimalPlacesCount(), 3 ); // 999.999, one trailing zero ignored

        // Test small decimal values
        datatypes::Decimal d17{ "0.01" };
        EXPECT_EQ( d17.decimalPlacesCount(), 2 );

        datatypes::Decimal d18{ "0.001" };
        EXPECT_EQ( d18.decimalPlacesCount(), 3 );

        datatypes::Decimal d19{ "0.0001" };
        EXPECT_EQ( d19.decimalPlacesCount(), 4 );

        // Test high precision values
        datatypes::Decimal d20{ "123.123456789" };
        EXPECT_EQ( d20.decimalPlacesCount(), 9 );

        datatypes::Decimal d21{ "0.123456789012345678901234567" };
        EXPECT_EQ( d21.decimalPlacesCount(), 27 );

        // Test maximum precision with trailing zeros
        datatypes::Decimal d22{ "1.1234567890123456789012345000" };
        EXPECT_EQ( d22.decimalPlacesCount(), 25 ); // Should remove 3 trailing zeros

        // Test very small values with high precision
        datatypes::Decimal d23{ "0.0000000000000000000000000001" };
        EXPECT_EQ( d23.decimalPlacesCount(), 28 ); // Maximum precision

        // Test consistency with scale() method
        datatypes::Decimal d24{ "123.4500" };
        EXPECT_EQ( d24.scale(), 2 );              // Internal scale includes trailing zeros
        EXPECT_EQ( d24.decimalPlacesCount(), 2 ); // Actual precision excludes trailing zeros

        datatypes::Decimal d25{ "789.12300" };
        EXPECT_EQ( d25.scale(), 3 );              // Internal scale includes trailing zeros
        EXPECT_EQ( d25.decimalPlacesCount(), 3 ); // Actual precision excludes trailing zeros

        // Test with constructed from integer (should have scale 0 and decimalPlacesCount 0)
        datatypes::Decimal d26{ 42 };
        EXPECT_EQ( d26.scale(), 0 );
        EXPECT_EQ( d26.decimalPlacesCount(), 0 );

        // Test with constructed from double
        datatypes::Decimal d27{ 123.456 };
        EXPECT_TRUE( d27.decimalPlacesCount() >= 3 ); // At least 3, could be more due to double precision

        // Test negative values (sign shouldn't affect decimal places count)
        datatypes::Decimal d28{ "-123.456" };
        EXPECT_EQ( d28.decimalPlacesCount(), 3 );

        datatypes::Decimal d29{ "-0.001" };
        EXPECT_EQ( d29.decimalPlacesCount(), 3 );

        datatypes::Decimal d30{ "-123.4500" };
        EXPECT_EQ( d30.decimalPlacesCount(), 2 ); // Trailing zeros ignored
    }

    //----------------------------------------------
    // Maths
    //----------------------------------------------

    TEST( DecimalMaths, BinaryRepresentation )
    {
        // Test toBits() method that returns Decimal binary representation
        datatypes::Decimal d1{ 123.456 };
        auto bits{ d1.toBits() };

        // Should return 4 elements: [low, mid, high, flags]
        EXPECT_EQ( bits.size(), 4 );

        // Test zero
        datatypes::Decimal zero{ 0 };
        auto zeroBits{ zero.toBits() };
        EXPECT_EQ( zeroBits[0], 0 );
        EXPECT_EQ( zeroBits[1], 0 );
        EXPECT_EQ( zeroBits[2], 0 );
        // Flags should be zero for positive zero
        EXPECT_EQ( zeroBits[3], 0u );

        // Test negative value
        datatypes::Decimal negative{ -123.456 };
        auto negativeBits{ negative.toBits() };
        // Sign bit should be set in flags
        EXPECT_NE( static_cast<std::uint32_t>( negativeBits[3] ) & constants::DECIMAL_SIGN_MASK, 0u );
    }

    TEST( DecimalMaths, TruncateMethod )
    {
        // Test instance method
        datatypes::Decimal d1{ "123.789" };
        auto truncated{ d1.trunc() };
        EXPECT_EQ( truncated.toString(), "123" );

        // Test negative truncation
        datatypes::Decimal d2{ "-123.789" };
        truncated = d2.trunc();
        EXPECT_EQ( truncated.toString(), "-123" );

        // Test zero
        datatypes::Decimal d3{ "0.123" };
        truncated = d3.trunc();
        EXPECT_EQ( truncated.toString(), "0" );

        // Test integer (no change)
        datatypes::Decimal d4{ "123" };
        truncated = d4.trunc();
        EXPECT_EQ( truncated.toString(), "123" );

        // Test instance method (alternative call style)
        datatypes::Decimal d5{ "456.789" };
        auto instanceTruncated{ d5.trunc() };
        EXPECT_EQ( instanceTruncated.toString(), "456" );
    }

    TEST( DecimalMaths, FloorMethod )
    {
        // Test positive values
        datatypes::Decimal d1{ "123.789" };
        auto floored{ d1.floor() };
        EXPECT_EQ( floored.toString(), "123" );

        // Test negative values (should round down)
        datatypes::Decimal d2{ "-123.789" };
        floored = d2.floor();
        EXPECT_EQ( floored.toString(), "-124" );

        // Test positive value close to integer
        datatypes::Decimal d3{ "123.001" };
        floored = d3.floor();
        EXPECT_EQ( floored.toString(), "123" );

        // Test negative value close to integer
        datatypes::Decimal d4{ "-123.001" };
        floored = d4.floor();
        EXPECT_EQ( floored.toString(), "-124" );

        // Test integer (no change)
        datatypes::Decimal d5{ "123" };
        floored = d5.floor();
        EXPECT_EQ( floored.toString(), "123" );
    }

    TEST( DecimalMaths, CeilingMethod )
    {
        // Test positive values (should round up)
        datatypes::Decimal d1{ "123.123" };
        auto ceiled{ d1.ceil() };
        EXPECT_EQ( ceiled.toString(), "124" );

        // Test negative values
        datatypes::Decimal d2{ "-123.123" };
        ceiled = d2.ceil();
        EXPECT_EQ( ceiled.toString(), "-123" );

        // Test positive value close to integer
        datatypes::Decimal d3{ "123.001" };
        ceiled = d3.ceil();
        EXPECT_EQ( ceiled.toString(), "124" );

        // Test negative value close to integer
        datatypes::Decimal d4{ "-123.001" };
        ceiled = d4.ceil();
        EXPECT_EQ( ceiled.toString(), "-123" );

        // Test integer (no change)
        datatypes::Decimal d5{ "123" };
        ceiled = d5.ceil();
        EXPECT_EQ( ceiled.toString(), "123" );
    }

    TEST( DecimalMaths, RoundMethod )
    {
        // Test basic rounding to nearest integer
        datatypes::Decimal d1{ "123.4" };
        auto rounded{ d1.round() };
        EXPECT_EQ( rounded.toString(), "123" );

        datatypes::Decimal d2{ "123.6" };
        rounded = d2.round();
        EXPECT_EQ( rounded.toString(), "124" );

        // Test exact half - should round to nearest even (banker's rounding)
        datatypes::Decimal d3{ "123.5" };
        rounded = d3.round();
        EXPECT_EQ( rounded.toString(), "124" );

        // Test negative rounding
        datatypes::Decimal d4{ "-123.4" };
        rounded = d4.round();
        EXPECT_EQ( rounded.toString(), "-123" );

        datatypes::Decimal d5{ "-123.6" };
        rounded = d5.round();
        EXPECT_EQ( rounded.toString(), "-124" );
    }

    TEST( DecimalMaths, RoundWithDecimalPlaces )
    {
        // Test rounding to specific Decimal places
        datatypes::Decimal d1{ "123.4567" };

        // Round to 2 Decimal places
        auto rounded{ d1.round( 2 ) };
        EXPECT_EQ( rounded.toString(), "123.46" );

        // Round to 1 Decimal place
        rounded = d1.round( 1 );
        EXPECT_EQ( rounded.toString(), "123.5" );

        // Round to 0 Decimal places (same as round())
        rounded = d1.round( 0 );
        EXPECT_EQ( rounded.toString(), "123" );

        // Test negative values
        datatypes::Decimal d2{ "-123.4567" };
        rounded = d2.round( 2 );
        EXPECT_EQ( rounded.toString(), "-123.46" );

        // Test rounding to more places than available (should return unchanged)
        datatypes::Decimal d4{ "123.45" };
        rounded = d4.round( 5 );
        EXPECT_EQ( rounded.toString(), "123.45" );
    }

    TEST( DecimalMaths, AbsMethod )
    {
        // Test positive value (should remain unchanged)
        datatypes::Decimal d1{ "123.456" };
        auto absValue{ d1.abs() };
        EXPECT_EQ( absValue.toString(), "123.456" );
        EXPECT_FALSE( absValue < 0 );

        // Test negative value (should become positive)
        datatypes::Decimal d2{ "-123.456" };
        absValue = d2.abs();
        EXPECT_EQ( absValue.toString(), "123.456" );
        EXPECT_FALSE( absValue < 0 );

        // Test zero (should remain zero)
        datatypes::Decimal d3{ "0" };
        absValue = d3.abs();
        EXPECT_EQ( absValue.toString(), "0" );
        EXPECT_TRUE( absValue == 0 );

        // Test negative zero (should become positive zero)
        datatypes::Decimal d4{ "-0.0" };
        absValue = d4.abs();
        EXPECT_TRUE( absValue == 0 );
        EXPECT_FALSE( absValue < 0 );

        // Test very small negative value
        datatypes::Decimal d6{ "-0.000000000000000000000000001" };
        absValue = d6.abs();
        EXPECT_FALSE( absValue < 0 );
        EXPECT_FALSE( absValue == 0 );
    }

    TEST( DecimalMaths, SqrtMethod )
    {
        // Test perfect square (4)
        datatypes::Decimal d1{ "4" };
        auto sqrtValue{ d1.sqrt() };
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 2 } );

        // Test perfect square (9)
        datatypes::Decimal d2{ "9" };
        sqrtValue = d2.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 3 } );

        // Test perfect square (100)
        datatypes::Decimal d3{ "100" };
        sqrtValue = d3.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 10 } );

        // Test large perfect square (10000)
        datatypes::Decimal d10{ "10000" };
        sqrtValue = d10.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ 100 } );

        // Test zero
        datatypes::Decimal d4{ "0" };
        sqrtValue = d4.sqrt();
        EXPECT_EQ( sqrtValue.toString(), "0" );

        // Test one
        datatypes::Decimal d5{ "1" };
        sqrtValue = d5.sqrt();
        EXPECT_EQ( sqrtValue.toString(), "1" );

        // Test non-perfect square (sqrt(2))
        datatypes::Decimal d6{ "2" };
        sqrtValue = d6.sqrt();
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "1.414" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "1.415" } );

        // Test non-perfect square (sqrt(3))
        datatypes::Decimal d7{ "3" };
        sqrtValue = d7.sqrt();
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "1.732" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "1.733" } );

        // Test decimal value (sqrt(2.25) = 1.5)
        datatypes::Decimal d8{ "2.25" };
        sqrtValue = d8.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "1.5" } );

        // Test that sqrt() throws for negative values
        datatypes::Decimal d9{ "-4" };
        EXPECT_THROW( [[maybe_unused]] auto result = d9.sqrt(), std::domain_error );
    }

    TEST( DecimalMaths, SqrtEdgeCases )
    {
        // Test very small perfect squares
        datatypes::Decimal v1{ "0.01" };
        auto sqrtValue = v1.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "0.1" } );

        datatypes::Decimal v2{ "0.04" };
        sqrtValue = v2.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "0.2" } );

        datatypes::Decimal v3{ "0.09" };
        sqrtValue = v3.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "0.3" } );

        datatypes::Decimal v4{ "0.16" };
        sqrtValue = v4.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "0.4" } );

        datatypes::Decimal v5{ "0.25" };
        sqrtValue = v5.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "0.5" } );

        datatypes::Decimal v6{ "0.36" };
        sqrtValue = v6.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "0.6" } );

        datatypes::Decimal v7{ "0.49" };
        sqrtValue = v7.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "0.7" } );

        datatypes::Decimal v8{ "0.64" };
        sqrtValue = v8.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "0.8" } );

        datatypes::Decimal v9{ "0.81" };
        sqrtValue = v9.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "0.9" } );

        // Test medium perfect squares
        datatypes::Decimal v10{ "1.44" };
        sqrtValue = v10.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "1.2" } );

        datatypes::Decimal v11{ "1.69" };
        sqrtValue = v11.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "1.3" } );

        datatypes::Decimal v12{ "6.25" };
        sqrtValue = v12.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "2.5" } );

        datatypes::Decimal v13{ "12.25" };
        sqrtValue = v13.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "3.5" } );

        datatypes::Decimal v14{ "20.25" };
        sqrtValue = v14.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "4.5" } );

        // Test large perfect squares
        datatypes::Decimal v15{ "400" };
        sqrtValue = v15.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "20" } );

        datatypes::Decimal v16{ "625" };
        sqrtValue = v16.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "25" } );

        datatypes::Decimal v17{ "900" };
        sqrtValue = v17.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "30" } );

        datatypes::Decimal v18{ "1600" };
        sqrtValue = v18.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "40" } );

        datatypes::Decimal v19{ "2500" };
        sqrtValue = v19.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "50" } );

        datatypes::Decimal v20{ "10000" };
        sqrtValue = v20.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "100" } );

        datatypes::Decimal v21{ "1000000" };
        sqrtValue = v21.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "1000" } );
    }

    TEST( DecimalMaths, SqrtNonPerfectSquares )
    {
        // Test small non-perfect squares
        datatypes::Decimal n1{ "0.5" };
        auto sqrtValue = n1.sqrt();
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "0.707" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "0.708" } );

        datatypes::Decimal n2{ "1.5" };
        sqrtValue = n2.sqrt();
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "1.224" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "1.225" } );

        datatypes::Decimal n3{ "2" };
        sqrtValue = n3.sqrt();
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "1.414" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "1.415" } );

        datatypes::Decimal n4{ "3" };
        sqrtValue = n4.sqrt();
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "1.732" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "1.733" } );

        datatypes::Decimal n5{ "5" };
        sqrtValue = n5.sqrt();
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "2.236" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "2.237" } );

        datatypes::Decimal n6{ "6" };
        sqrtValue = n6.sqrt();
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "2.449" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "2.450" } );

        datatypes::Decimal n7{ "7" };
        sqrtValue = n7.sqrt();
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "2.645" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "2.647" } );

        datatypes::Decimal n8{ "8" };
        sqrtValue = n8.sqrt();
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "2.828" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "2.829" } );

        datatypes::Decimal n9{ "10" };
        sqrtValue = n9.sqrt();
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "3.162" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "3.163" } );

        // Test medium non-perfect squares
        datatypes::Decimal n10{ "50" };
        sqrtValue = n10.sqrt();
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "7.071" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "7.072" } );

        datatypes::Decimal n11{ "99" };
        sqrtValue = n11.sqrt();
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "9.949" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "9.950" } );

        datatypes::Decimal n12{ "101" };
        sqrtValue = n12.sqrt();
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "10.049" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "10.050" } );

        datatypes::Decimal n13{ "200" };
        sqrtValue = n13.sqrt();
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "14.142" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "14.143" } );

        datatypes::Decimal n14{ "500" };
        sqrtValue = n14.sqrt();
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "22.360" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "22.361" } );

        datatypes::Decimal n15{ "1000" };
        sqrtValue = n15.sqrt();
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "31.622" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "31.623" } );

        // Test large non-perfect squares
        datatypes::Decimal n16{ "9999" };
        sqrtValue = n16.sqrt();
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "99.99" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "100.00" } );

        datatypes::Decimal n17{ "10001" };
        sqrtValue = n17.sqrt();
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "100.00" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "100.01" } );

        datatypes::Decimal n18{ "999999" };
        sqrtValue = n18.sqrt();
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "999.99" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "1000.00" } );

        datatypes::Decimal n19{ "1000001" };
        sqrtValue = n19.sqrt();
        EXPECT_TRUE( sqrtValue > datatypes::Decimal{ "1000.00" } );
        EXPECT_TRUE( sqrtValue < datatypes::Decimal{ "1000.01" } );
    }

    TEST( DecimalMaths, SqrtDecimalPrecision )
    {
        // Test decimal values with high precision
        datatypes::Decimal d1{ "4.41" };
        auto sqrtValue = d1.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "2.1" } );

        datatypes::Decimal d2{ "5.29" };
        sqrtValue = d2.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "2.3" } );

        datatypes::Decimal d3{ "7.84" };
        sqrtValue = d3.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "2.8" } );

        datatypes::Decimal d4{ "9.61" };
        sqrtValue = d4.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "3.1" } );

        datatypes::Decimal d5{ "11.56" };
        sqrtValue = d5.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "3.4" } );

        datatypes::Decimal d6{ "14.44" };
        sqrtValue = d6.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "3.8" } );

        datatypes::Decimal d7{ "17.64" };
        sqrtValue = d7.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "4.2" } );

        // Test very small decimal values
        datatypes::Decimal d8{ "0.0001" };
        sqrtValue = d8.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "0.01" } );

        datatypes::Decimal d9{ "0.0004" };
        sqrtValue = d9.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "0.02" } );

        datatypes::Decimal d10{ "0.0009" };
        sqrtValue = d10.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "0.03" } );

        datatypes::Decimal d11{ "0.0016" };
        sqrtValue = d11.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "0.04" } );

        datatypes::Decimal d12{ "0.0025" };
        sqrtValue = d12.sqrt();
        EXPECT_EQ( sqrtValue, datatypes::Decimal{ "0.05" } );
    }

    TEST( DecimalMaths, SqrtDivisionClangRegressionTest )
    {
        datatypes::Decimal two{ "2" };
        auto sqrtTwo = two.sqrt();

        // Should be approximately 1.414...
        EXPECT_TRUE( sqrtTwo > datatypes::Decimal{ "1.4" } );
        EXPECT_TRUE( sqrtTwo < datatypes::Decimal{ "1.5" } );
        EXPECT_TRUE( sqrtTwo.scale() > 0 ); // Must have decimal places

        // More precise check
        EXPECT_TRUE( sqrtTwo > datatypes::Decimal{ "1.414" } );
        EXPECT_TRUE( sqrtTwo < datatypes::Decimal{ "1.415" } );

        // Test sqrt(3)
        datatypes::Decimal three{ "3" };
        auto sqrtThree = three.sqrt();
        EXPECT_TRUE( sqrtThree > datatypes::Decimal{ "1.732" } );
        EXPECT_TRUE( sqrtThree < datatypes::Decimal{ "1.733" } );
        EXPECT_TRUE( sqrtThree.scale() > 0 );

        // Test sqrt(5)
        datatypes::Decimal five{ "5" };
        auto sqrtFive = five.sqrt();
        EXPECT_TRUE( sqrtFive > datatypes::Decimal{ "2.236" } );
        EXPECT_TRUE( sqrtFive < datatypes::Decimal{ "2.237" } );
        EXPECT_TRUE( sqrtFive.scale() > 0 );

        // Test division directly - ensure it maintains proper scale
        datatypes::Decimal dividend{ "10" };
        datatypes::Decimal divisor{ "3" };
        auto quotient = dividend / divisor;

        // 10/3 = 3.333...
        EXPECT_TRUE( quotient > datatypes::Decimal{ "3.3" } );
        EXPECT_TRUE( quotient < datatypes::Decimal{ "3.4" } );
        EXPECT_TRUE( quotient.scale() > 0 ); // Must preserve decimal scale
    }

    TEST( DecimalMaths, HighScaleMultiplicationPiTimesE )
    {
        // Test multiplication with high-scale operands (maximum precision)
        // This verifies the multiplication algorithm handles extreme scale combinations correctly

        // π with 28 decimal places (maximum precision)
        datatypes::Decimal pi{ "3.1415926535897932384626433832" };

        // e with 28 decimal places (maximum precision)
        datatypes::Decimal e{ "2.7182818284590452353602874713" };

        // Expected result: π × e ≈ 8.5397342226735670654635508695
        auto product = pi * e;
        // Verify the product is in the expected range (8.5 to 8.6)
        EXPECT_TRUE( product > datatypes::Decimal{ "8.5" } );
        EXPECT_TRUE( product < datatypes::Decimal{ "8.6" } );

        // More precise check (π × e ≈ 8.539734...)
        EXPECT_TRUE( product > datatypes::Decimal{ "8.539" } );
        EXPECT_TRUE( product < datatypes::Decimal{ "8.540" } );

        // Even more precise
        EXPECT_TRUE( product > datatypes::Decimal{ "8.5397" } );
        EXPECT_TRUE( product < datatypes::Decimal{ "8.5398" } );

        // Test with slightly lower scale to verify algorithm doesn't degrade
        datatypes::Decimal pi16{ "3.1415926535897932" }; // 16 decimal places
        datatypes::Decimal e16{ "2.7182818284590452" };  // 16 decimal places
        auto product16 = pi16 * e16;

        EXPECT_TRUE( product16 > datatypes::Decimal{ "8.539" } );
        EXPECT_TRUE( product16 < datatypes::Decimal{ "8.540" } );

        // Test sum as well (this worked in the sample output)
        auto sum = pi + e;
        EXPECT_TRUE( sum > datatypes::Decimal{ "5.859" } );
        EXPECT_TRUE( sum < datatypes::Decimal{ "5.860" } );
    }

    TEST( DecimalMaths, MathematicalConsistency )
    {
        // Test that mathematical operations are consistent
        datatypes::Decimal a{ "123.45" };
        datatypes::Decimal b{ "67.89" };

        // Test that (a + b) - a == b
        datatypes::Decimal sum{ a + b };
        datatypes::Decimal diff{ sum - a };
        EXPECT_TRUE( diff == b );

        // Test that a * b / a == b (within precision limits)
        datatypes::Decimal product{ a * b };
        datatypes::Decimal quotient{ product / a };
        datatypes::Decimal difference{ quotient - b };

        // Due to precision, we test that they're very close
        datatypes::Decimal tolerance{ "0.00001" };
        EXPECT_TRUE( difference.abs() < tolerance );
    }

    TEST( DecimalMaths, RoundingConsistency )
    {
        // Test that different rounding methods work consistently
        datatypes::Decimal value{ "123.456789" };

        // Truncate should always round toward zero
        datatypes::Decimal truncated{ value.trunc() };
        EXPECT_EQ( truncated.toString(), "123" );

        // Floor should always round down
        datatypes::Decimal floored{ value.floor() };
        EXPECT_EQ( floored.toString(), "123" );

        // Ceiling should always round up
        datatypes::Decimal ceiled{ value.ceil() };
        EXPECT_EQ( ceiled.toString(), "124" );

        // Round should use banker's rounding
        datatypes::Decimal rounded{ value.round() };
        EXPECT_EQ( rounded.toString(), "123" );

        // Test with negative values
        datatypes::Decimal negValue{ "-123.456789" };

        truncated = negValue.trunc();
        EXPECT_EQ( truncated.toString(), "-123" );

        floored = negValue.floor();
        EXPECT_EQ( floored.toString(), "-124" );

        ceiled = negValue.ceil();
        EXPECT_EQ( ceiled.toString(), "-123" );

        rounded = negValue.round();
        EXPECT_EQ( rounded.toString(), "-123" );
    }

    TEST( DecimalMaths, PrecisionPreservation )
    {
        // Test that precision is preserved in operations
        datatypes::Decimal a{ "0.1" };
        datatypes::Decimal b{ "0.2" };
        datatypes::Decimal c{ "0.3" };

        // This should be exactly 0.3, not 0.30000000000000004 like with double
        datatypes::Decimal sum{ a + b };
        EXPECT_TRUE( sum == c );

        // Test with many Decimal places
        datatypes::Decimal precise1{ "0.1234567890123456789012345678" };
        datatypes::Decimal precise2{ "0.0000000000000000000000000001" };
        datatypes::Decimal preciseSum{ precise1 + precise2 };

        // Should maintain precision
        EXPECT_FALSE( preciseSum == precise1 );
        EXPECT_TRUE( preciseSum > precise1 );
    }

    //----------------------------------------------
    // Round with all RoundingModes
    //----------------------------------------------

    TEST( DecimalRounding, RoundWithToNearestMode )
    {
        using datatypes::Decimal;

        // Banker's rounding (round-half-to-even)
        // Test positive values with different fractional parts
        EXPECT_EQ( Decimal( "2.4" ).round( 0, Decimal::RoundingMode::ToNearest ).toString(), "2" );
        EXPECT_EQ( Decimal( "2.5" ).round( 0, Decimal::RoundingMode::ToNearest ).toString(), "2" ); // Tie: round to even
        EXPECT_EQ( Decimal( "2.6" ).round( 0, Decimal::RoundingMode::ToNearest ).toString(), "3" );
        EXPECT_EQ( Decimal( "3.5" ).round( 0, Decimal::RoundingMode::ToNearest ).toString(), "4" ); // Tie: round to even
        EXPECT_EQ( Decimal( "4.5" ).round( 0, Decimal::RoundingMode::ToNearest ).toString(), "4" ); // Tie: round to even
        EXPECT_EQ( Decimal( "5.5" ).round( 0, Decimal::RoundingMode::ToNearest ).toString(), "6" ); // Tie: round to even

        // Test negative values
        EXPECT_EQ( Decimal( "-2.4" ).round( 0, Decimal::RoundingMode::ToNearest ).toString(), "-2" );
        EXPECT_EQ( Decimal( "-2.5" ).round( 0, Decimal::RoundingMode::ToNearest ).toString(), "-2" ); // Tie: round to even
        EXPECT_EQ( Decimal( "-2.6" ).round( 0, Decimal::RoundingMode::ToNearest ).toString(), "-3" );
        EXPECT_EQ( Decimal( "-3.5" ).round( 0, Decimal::RoundingMode::ToNearest ).toString(), "-4" ); // Tie: round to even
        EXPECT_EQ( Decimal( "-4.5" ).round( 0, Decimal::RoundingMode::ToNearest ).toString(), "-4" ); // Tie: round to even

        // Test with decimal places
        EXPECT_EQ( Decimal( "123.456" ).round( 2, Decimal::RoundingMode::ToNearest ).toString(), "123.46" );
        EXPECT_EQ( Decimal( "123.455" ).round( 2, Decimal::RoundingMode::ToNearest ).toString(), "123.46" ); // Tie: round to even
        EXPECT_EQ( Decimal( "123.445" ).round( 2, Decimal::RoundingMode::ToNearest ).toString(), "123.44" ); // Tie: round to even
        EXPECT_EQ( Decimal( "123.454" ).round( 2, Decimal::RoundingMode::ToNearest ).toString(), "123.45" );

        // Test non-tie cases (should always round away from zero when > 0.5)
        EXPECT_EQ( Decimal( "2.51" ).round( 0, Decimal::RoundingMode::ToNearest ).toString(), "3" );
        EXPECT_EQ( Decimal( "-2.51" ).round( 0, Decimal::RoundingMode::ToNearest ).toString(), "-3" );
    }

    TEST( DecimalRounding, RoundWithToNearestTiesAwayMode )
    {
        using datatypes::Decimal;

        // Standard rounding (round-half-away-from-zero)
        // Test positive values
        EXPECT_EQ( Decimal( "2.4" ).round( 0, Decimal::RoundingMode::ToNearestTiesAway ).toString(), "2" );
        EXPECT_EQ( Decimal( "2.5" ).round( 0, Decimal::RoundingMode::ToNearestTiesAway ).toString(), "3" ); // Tie: away from zero
        EXPECT_EQ( Decimal( "2.6" ).round( 0, Decimal::RoundingMode::ToNearestTiesAway ).toString(), "3" );
        EXPECT_EQ( Decimal( "3.5" ).round( 0, Decimal::RoundingMode::ToNearestTiesAway ).toString(), "4" ); // Tie: away from zero
        EXPECT_EQ( Decimal( "4.5" ).round( 0, Decimal::RoundingMode::ToNearestTiesAway ).toString(), "5" ); // Tie: away from zero

        // Test negative values (should round away from zero)
        EXPECT_EQ( Decimal( "-2.4" ).round( 0, Decimal::RoundingMode::ToNearestTiesAway ).toString(), "-2" );
        EXPECT_EQ( Decimal( "-2.5" ).round( 0, Decimal::RoundingMode::ToNearestTiesAway ).toString(), "-3" ); // Tie: away from zero
        EXPECT_EQ( Decimal( "-2.6" ).round( 0, Decimal::RoundingMode::ToNearestTiesAway ).toString(), "-3" );
        EXPECT_EQ( Decimal( "-3.5" ).round( 0, Decimal::RoundingMode::ToNearestTiesAway ).toString(), "-4" ); // Tie: away from zero

        // Test with decimal places
        EXPECT_EQ( Decimal( "123.455" ).round( 2, Decimal::RoundingMode::ToNearestTiesAway ).toString(), "123.46" );
        EXPECT_EQ( Decimal( "123.445" ).round( 2, Decimal::RoundingMode::ToNearestTiesAway ).toString(), "123.45" );
        EXPECT_EQ( Decimal( "-123.455" ).round( 2, Decimal::RoundingMode::ToNearestTiesAway ).toString(), "-123.46" );
    }

    TEST( DecimalRounding, RoundWithToZeroMode )
    {
        using datatypes::Decimal;

        // Truncation (always round toward zero)
        // Test positive values
        EXPECT_EQ( Decimal( "2.1" ).round( 0, Decimal::RoundingMode::ToZero ).toString(), "2" );
        EXPECT_EQ( Decimal( "2.5" ).round( 0, Decimal::RoundingMode::ToZero ).toString(), "2" );
        EXPECT_EQ( Decimal( "2.9" ).round( 0, Decimal::RoundingMode::ToZero ).toString(), "2" );
        EXPECT_EQ( Decimal( "123.999" ).round( 0, Decimal::RoundingMode::ToZero ).toString(), "123" );

        // Test negative values (truncate toward zero)
        EXPECT_EQ( Decimal( "-2.1" ).round( 0, Decimal::RoundingMode::ToZero ).toString(), "-2" );
        EXPECT_EQ( Decimal( "-2.5" ).round( 0, Decimal::RoundingMode::ToZero ).toString(), "-2" );
        EXPECT_EQ( Decimal( "-2.9" ).round( 0, Decimal::RoundingMode::ToZero ).toString(), "-2" );
        EXPECT_EQ( Decimal( "-123.999" ).round( 0, Decimal::RoundingMode::ToZero ).toString(), "-123" );

        // Test with decimal places
        EXPECT_EQ( Decimal( "123.456" ).round( 2, Decimal::RoundingMode::ToZero ).toString(), "123.45" );
        EXPECT_EQ( Decimal( "123.459" ).round( 2, Decimal::RoundingMode::ToZero ).toString(), "123.45" );
        EXPECT_EQ( Decimal( "-123.456" ).round( 2, Decimal::RoundingMode::ToZero ).toString(), "-123.45" );
        EXPECT_EQ( Decimal( "-123.459" ).round( 2, Decimal::RoundingMode::ToZero ).toString(), "-123.45" );

        // Verify equivalence with trunc( ) for integer rounding
        EXPECT_EQ( Decimal( "123.789" ).round( 0, Decimal::RoundingMode::ToZero ).toString(),
            Decimal( "123.789" ).trunc().toString() );
        EXPECT_EQ( Decimal( "-123.789" ).round( 0, Decimal::RoundingMode::ToZero ).toString(),
            Decimal( "-123.789" ).trunc().toString() );
    }

    TEST( DecimalRounding, RoundWithToPositiveInfinityMode )
    {
        using datatypes::Decimal;

        // Ceiling (always round toward +∞)
        // Test positive values (round up)
        EXPECT_EQ( Decimal( "2.1" ).round( 0, Decimal::RoundingMode::ToPositiveInfinity ).toString(), "3" );
        EXPECT_EQ( Decimal( "2.5" ).round( 0, Decimal::RoundingMode::ToPositiveInfinity ).toString(), "3" );
        EXPECT_EQ( Decimal( "2.9" ).round( 0, Decimal::RoundingMode::ToPositiveInfinity ).toString(), "3" );
        EXPECT_EQ( Decimal( "2.0" ).round( 0, Decimal::RoundingMode::ToPositiveInfinity ).toString(), "2" ); // Exact integer

        // Test negative values (round toward zero, which is toward +∞)
        EXPECT_EQ( Decimal( "-2.1" ).round( 0, Decimal::RoundingMode::ToPositiveInfinity ).toString(), "-2" );
        EXPECT_EQ( Decimal( "-2.5" ).round( 0, Decimal::RoundingMode::ToPositiveInfinity ).toString(), "-2" );
        EXPECT_EQ( Decimal( "-2.9" ).round( 0, Decimal::RoundingMode::ToPositiveInfinity ).toString(), "-2" );
        EXPECT_EQ( Decimal( "-2.0" ).round( 0, Decimal::RoundingMode::ToPositiveInfinity ).toString(), "-2" ); // Exact integer

        // Test with decimal places
        EXPECT_EQ( Decimal( "123.451" ).round( 2, Decimal::RoundingMode::ToPositiveInfinity ).toString(), "123.46" );
        EXPECT_EQ( Decimal( "123.459" ).round( 2, Decimal::RoundingMode::ToPositiveInfinity ).toString(), "123.46" );
        EXPECT_EQ( Decimal( "-123.451" ).round( 2, Decimal::RoundingMode::ToPositiveInfinity ).toString(), "-123.45" );
        EXPECT_EQ( Decimal( "-123.459" ).round( 2, Decimal::RoundingMode::ToPositiveInfinity ).toString(), "-123.45" );

        // Verify equivalence with ceil( ) for integer rounding
        EXPECT_EQ( Decimal( "123.001" ).round( 0, Decimal::RoundingMode::ToPositiveInfinity ).toString(),
            Decimal( "123.001" ).ceil().toString() );
        EXPECT_EQ( Decimal( "-123.001" ).round( 0, Decimal::RoundingMode::ToPositiveInfinity ).toString(),
            Decimal( "-123.001" ).ceil().toString() );
    }

    TEST( DecimalRounding, RoundWithToNegativeInfinityMode )
    {
        using datatypes::Decimal;

        // Floor (always round toward -∞)
        // Test positive values (round toward zero, which is toward -∞)
        EXPECT_EQ( Decimal( "2.1" ).round( 0, Decimal::RoundingMode::ToNegativeInfinity ).toString(), "2" );
        EXPECT_EQ( Decimal( "2.5" ).round( 0, Decimal::RoundingMode::ToNegativeInfinity ).toString(), "2" );
        EXPECT_EQ( Decimal( "2.9" ).round( 0, Decimal::RoundingMode::ToNegativeInfinity ).toString(), "2" );
        EXPECT_EQ( Decimal( "2.0" ).round( 0, Decimal::RoundingMode::ToNegativeInfinity ).toString(), "2" ); // Exact integer

        // Test negative values (round down, away from zero)
        EXPECT_EQ( Decimal( "-2.1" ).round( 0, Decimal::RoundingMode::ToNegativeInfinity ).toString(), "-3" );
        EXPECT_EQ( Decimal( "-2.5" ).round( 0, Decimal::RoundingMode::ToNegativeInfinity ).toString(), "-3" );
        EXPECT_EQ( Decimal( "-2.9" ).round( 0, Decimal::RoundingMode::ToNegativeInfinity ).toString(), "-3" );
        EXPECT_EQ( Decimal( "-2.0" ).round( 0, Decimal::RoundingMode::ToNegativeInfinity ).toString(), "-2" ); // Exact integer

        // Test with decimal places
        EXPECT_EQ( Decimal( "123.451" ).round( 2, Decimal::RoundingMode::ToNegativeInfinity ).toString(), "123.45" );
        EXPECT_EQ( Decimal( "123.459" ).round( 2, Decimal::RoundingMode::ToNegativeInfinity ).toString(), "123.45" );
        EXPECT_EQ( Decimal( "-123.451" ).round( 2, Decimal::RoundingMode::ToNegativeInfinity ).toString(), "-123.46" );
        EXPECT_EQ( Decimal( "-123.459" ).round( 2, Decimal::RoundingMode::ToNegativeInfinity ).toString(), "-123.46" );

        // Verify equivalence with floor() for integer rounding
        EXPECT_EQ( Decimal( "123.789" ).round( 0, Decimal::RoundingMode::ToNegativeInfinity ).toString(),
            Decimal( "123.789" ).floor().toString() );
        EXPECT_EQ( Decimal( "-123.789" ).round( 0, Decimal::RoundingMode::ToNegativeInfinity ).toString(),
            Decimal( "-123.789" ).floor().toString() );
    }

    TEST( DecimalRounding, RoundWithDecimalPlaces )
    {
        using datatypes::Decimal;

        // Test rounding to various decimal places with different modes
        Decimal value{ "123.456789" };

        // ToNearest (banker's rounding)
        EXPECT_EQ( value.round( 0, Decimal::RoundingMode::ToNearest ).toString(), "123" );
        EXPECT_EQ( value.round( 1, Decimal::RoundingMode::ToNearest ).toString(), "123.5" );
        EXPECT_EQ( value.round( 2, Decimal::RoundingMode::ToNearest ).toString(), "123.46" );
        EXPECT_EQ( value.round( 3, Decimal::RoundingMode::ToNearest ).toString(), "123.457" );
        EXPECT_EQ( value.round( 4, Decimal::RoundingMode::ToNearest ).toString(), "123.4568" );

        // ToZero (truncation)
        EXPECT_EQ( value.round( 0, Decimal::RoundingMode::ToZero ).toString(), "123" );
        EXPECT_EQ( value.round( 1, Decimal::RoundingMode::ToZero ).toString(), "123.4" );
        EXPECT_EQ( value.round( 2, Decimal::RoundingMode::ToZero ).toString(), "123.45" );
        EXPECT_EQ( value.round( 3, Decimal::RoundingMode::ToZero ).toString(), "123.456" );

        // ToPositiveInfinity (ceiling)
        EXPECT_EQ( value.round( 0, Decimal::RoundingMode::ToPositiveInfinity ).toString(), "124" );
        EXPECT_EQ( value.round( 1, Decimal::RoundingMode::ToPositiveInfinity ).toString(), "123.5" );
        EXPECT_EQ( value.round( 2, Decimal::RoundingMode::ToPositiveInfinity ).toString(), "123.46" );

        // ToNegativeInfinity (floor)
        EXPECT_EQ( value.round( 0, Decimal::RoundingMode::ToNegativeInfinity ).toString(), "123" );
        EXPECT_EQ( value.round( 1, Decimal::RoundingMode::ToNegativeInfinity ).toString(), "123.4" );
        EXPECT_EQ( value.round( 2, Decimal::RoundingMode::ToNegativeInfinity ).toString(), "123.45" );
    }

    TEST( DecimalRounding, RoundEdgeCases )
    {
        using datatypes::Decimal;

        // Test zero
        EXPECT_EQ( Decimal( "0" ).round( 0, Decimal::RoundingMode::ToNearest ).toString(), "0" );
        EXPECT_EQ( Decimal( "0.0" ).round( 0, Decimal::RoundingMode::ToNearest ).toString(), "0" );

        // Test already integer values
        EXPECT_EQ( Decimal( "123" ).round( 0, Decimal::RoundingMode::ToNearest ).toString(), "123" );
        EXPECT_EQ( Decimal( "-123" ).round( 0, Decimal::RoundingMode::ToNearest ).toString(), "-123" );

        // Test negative decimal places (should treat as 0)
        EXPECT_EQ( Decimal( "123.456" ).round( -1, Decimal::RoundingMode::ToNearest ).toString(), "123" );

        // Test rounding to more places than exist (should return unchanged)
        EXPECT_EQ( Decimal( "123.45" ).round( 5, Decimal::RoundingMode::ToNearest ).toString(), "123.45" );

        // Test very small values
        EXPECT_EQ( Decimal( "0.001" ).round( 0, Decimal::RoundingMode::ToNearest ).toString(), "0" );
        EXPECT_EQ( Decimal( "0.001" ).round( 2, Decimal::RoundingMode::ToNearest ).toString(), "0" );
        EXPECT_EQ( Decimal( "0.001" ).round( 3, Decimal::RoundingMode::ToNearest ).toString(), "0.001" );
    }

    //----------------------------------------------
    // String parsing
    //----------------------------------------------

    TEST( DecimalStringParsing, StringConstruction )
    {
        // Valid strings
        datatypes::Decimal d1{ "123.456" };
        EXPECT_EQ( d1.toString(), "123.456" );

        datatypes::Decimal d2{ "-789.123" };
        EXPECT_TRUE( d2 < 0 );

        datatypes::Decimal d3{ "0" };
        EXPECT_TRUE( d3 == 0 );

        datatypes::Decimal d4{ "0.0001" };
        EXPECT_FALSE( d4 == 0 );

        // Test with many Decimal places
        datatypes::Decimal d5{ "123.1234567890123456789" };
        EXPECT_FALSE( d5 == 0 );
    }

    TEST( DecimalStringParsing, CStringConstruction )
    {
        // Valid C-strings - tests const char* constructor
        const char* str1 = "123.456";
        datatypes::Decimal d1{ str1 };
        EXPECT_EQ( d1.toString(), "123.456" );

        const char* str2 = "-789.123";
        datatypes::Decimal d2{ str2 };
        EXPECT_TRUE( d2 < 0 );

        const char* str3 = "0";
        datatypes::Decimal d3{ str3 };
        EXPECT_TRUE( d3 == 0 );

        const char* str4 = "0.0001";
        datatypes::Decimal d4{ str4 };
        EXPECT_FALSE( d4 == 0 );

        // Test with many Decimal places
        const char* str5 = "123.1234567890123456789";
        datatypes::Decimal d5{ str5 };
        EXPECT_FALSE( d5 == 0 );

        // Test invalid C-string throws exception
        const char* invalid = "invalid";
        EXPECT_THROW( datatypes::Decimal{ invalid }, std::invalid_argument );

        // Test empty string throws exception
        const char* empty = "";
        EXPECT_THROW( datatypes::Decimal{ empty }, std::invalid_argument );
    }

    TEST( DecimalStringParsing, FromStringMethod )
    {
        datatypes::Decimal result;

        // Valid positive number
        EXPECT_TRUE( datatypes::Decimal::fromString( "123.456", result ) );
        EXPECT_EQ( result.toString(), "123.456" );
        EXPECT_FALSE( result < 0 );
        EXPECT_FALSE( result == 0 );

        // Valid negative number
        EXPECT_TRUE( datatypes::Decimal::fromString( "-789.123", result ) );
        EXPECT_TRUE( result < 0 );
        EXPECT_FALSE( result == 0 );

        // Valid zero
        EXPECT_TRUE( datatypes::Decimal::fromString( "0", result ) );
        EXPECT_TRUE( result == 0 );
        EXPECT_FALSE( result < 0 );

        // Valid zero with decimal
        EXPECT_TRUE( datatypes::Decimal::fromString( "0.0", result ) );
        EXPECT_TRUE( result == 0 );

        // Valid number with positive sign
        EXPECT_TRUE( datatypes::Decimal::fromString( "+456.789", result ) );
        EXPECT_FALSE( result < 0 );
        EXPECT_FALSE( result == 0 );

        // Valid integer
        EXPECT_TRUE( datatypes::Decimal::fromString( "12345", result ) );
        EXPECT_FALSE( result == 0 );

        // Valid decimal with leading zero
        EXPECT_TRUE( datatypes::Decimal::fromString( "0.123", result ) );
        EXPECT_FALSE( result == 0 );

        // Valid high precision number (within storage limits)
        EXPECT_TRUE( datatypes::Decimal::fromString( "1.2345678901234567890123456789", result ) );
        EXPECT_FALSE( result == 0 );

        // Invalid: empty string
        EXPECT_FALSE( datatypes::Decimal::fromString( "", result ) );

        // Invalid: non-numeric characters
        EXPECT_FALSE( datatypes::Decimal::fromString( "abc", result ) );
        EXPECT_FALSE( datatypes::Decimal::fromString( "12a34", result ) );
        EXPECT_FALSE( datatypes::Decimal::fromString( "12.34abc", result ) );

        // Invalid: multiple decimal points
        EXPECT_FALSE( datatypes::Decimal::fromString( "12.34.56", result ) );
        EXPECT_FALSE( datatypes::Decimal::fromString( "1.2.3", result ) );

        // Invalid: multiple signs
        EXPECT_FALSE( datatypes::Decimal::fromString( "+-123", result ) );
        EXPECT_FALSE( datatypes::Decimal::fromString( "--123", result ) );
        EXPECT_FALSE( datatypes::Decimal::fromString( "++123", result ) );

        // Invalid: sign in wrong position
        EXPECT_FALSE( datatypes::Decimal::fromString( "12+34", result ) );
        EXPECT_FALSE( datatypes::Decimal::fromString( "12-34", result ) );

        // Invalid: only decimal point
        EXPECT_FALSE( datatypes::Decimal::fromString( ".", result ) );
        EXPECT_FALSE( datatypes::Decimal::fromString( "..", result ) );

        // Invalid: only sign
        EXPECT_FALSE( datatypes::Decimal::fromString( "+", result ) );
        EXPECT_FALSE( datatypes::Decimal::fromString( "-", result ) );

        // Invalid: whitespace
        EXPECT_FALSE( datatypes::Decimal::fromString( " 123", result ) );
        EXPECT_FALSE( datatypes::Decimal::fromString( "123 ", result ) );
        EXPECT_FALSE( datatypes::Decimal::fromString( "1 23", result ) );

        // Invalid: scientific notation
        EXPECT_FALSE( datatypes::Decimal::fromString( "1.23e10", result ) );
        EXPECT_FALSE( datatypes::Decimal::fromString( "1.23E-5", result ) );
    }

    TEST( DecimalStringParsing, ParseMethod )
    {
        // Valid positive number
        auto result{ datatypes::Decimal{ "123.456" } };
        EXPECT_EQ( result.toString(), "123.456" );
        EXPECT_FALSE( result < 0 );
        EXPECT_FALSE( result == 0 );

        // Valid negative number
        result = datatypes::Decimal{ "-789.123" };
        EXPECT_TRUE( result < 0 );
        EXPECT_FALSE( result == 0 );

        // Valid zero
        result = datatypes::Decimal{ "0" };
        EXPECT_TRUE( result == 0 );
        EXPECT_FALSE( result < 0 );

        // Valid number with positive sign
        result = datatypes::Decimal{ "+456.789" };
        EXPECT_FALSE( result < 0 );
        EXPECT_FALSE( result == 0 );

        // Valid high precision number (within storage limits)
        result = datatypes::Decimal{ "1.2345678901234567890123456789" };
        EXPECT_FALSE( result == 0 );

        // Invalid: empty string should throw
        EXPECT_THROW( datatypes::Decimal{ "" }, std::invalid_argument );

        // Invalid: non-numeric should throw
        EXPECT_THROW( datatypes::Decimal{ "abc" }, std::invalid_argument );
        EXPECT_THROW( datatypes::Decimal{ "12a34" }, std::invalid_argument );

        // Invalid: multiple decimal points should throw
        EXPECT_THROW( datatypes::Decimal{ "12.34.56" }, std::invalid_argument );

        // Invalid: multiple signs should throw
        EXPECT_THROW( datatypes::Decimal{ "+-123" }, std::invalid_argument );
        EXPECT_THROW( datatypes::Decimal{ "--123" }, std::invalid_argument );

        // Invalid: sign in wrong position should throw
        EXPECT_THROW( datatypes::Decimal{ "12+34" }, std::invalid_argument );
        EXPECT_THROW( datatypes::Decimal{ "12-34" }, std::invalid_argument );

        // Invalid: only decimal point should throw
        EXPECT_THROW( datatypes::Decimal{ "." }, std::invalid_argument );

        // Invalid: only sign should throw
        EXPECT_THROW( datatypes::Decimal{ "+" }, std::invalid_argument );
        EXPECT_THROW( datatypes::Decimal{ "-" }, std::invalid_argument );

        // Invalid: whitespace should throw
        EXPECT_THROW( datatypes::Decimal{ " 123" }, std::invalid_argument );
        EXPECT_THROW( datatypes::Decimal{ "123 " }, std::invalid_argument );
        EXPECT_THROW( datatypes::Decimal{ "1 23" }, std::invalid_argument );

        // Invalid: scientific notation should throw
        EXPECT_THROW( datatypes::Decimal{ "1.23e10" }, std::invalid_argument );
        EXPECT_THROW( datatypes::Decimal{ "1.23E-5" }, std::invalid_argument );
    }

    //----------------------------------------------
    // Compatibility
    //----------------------------------------------

    TEST( DecimalCompatibility, CrossPlatformConsistency )
    {
        // Test that results are consistent across platforms
        datatypes::Decimal d1{ "123456789.123456789" };
        datatypes::Decimal d2{ "987654321.987654321" };

        // Addition should work consistently
        datatypes::Decimal sum{ d1 + d2 };
        EXPECT_FALSE( sum == 0 );

        // Multiplication should work consistently
        datatypes::Decimal product{ datatypes::Decimal{ "123.456" } * datatypes::Decimal{ "789.123" } };
        EXPECT_FALSE( product == 0 );

        // Division should work consistently
        datatypes::Decimal quotient{ datatypes::Decimal{ "1000" } / datatypes::Decimal{ "3" } };
        EXPECT_FALSE( quotient == 0 );
    }

    TEST( DecimalCompatibility, StandardDecimalCompatibility )
    {
        // Test behaviors that should match standard Decimal arithmetic

        // Decimal has 28-29 significant digits
        datatypes::Decimal d1{ "1234567890123456789012345678.9" };
        EXPECT_FALSE( d1 == 0 );

        // Decimal preserves trailing zeros in scale
        datatypes::Decimal d2{ "123.4500" };
        EXPECT_EQ( d2.scale(), 2 ); // Should preserve 2 Decimal places

        // Decimal arithmetic should be exact (no floating-point errors)
        datatypes::Decimal d3{ "0.1" };
        datatypes::Decimal d4{ "0.2" };
        datatypes::Decimal sum{ d3 + d4 };

        // Should be exactly 0.3, not 0.30000000000000004 like double
        datatypes::Decimal expected{ "0.3" };
        EXPECT_TRUE( sum == expected );
    }

    TEST( DecimalCompatibility, NormalizationBehavior )
    {
        // Test that normalization removes unnecessary trailing zeros
        datatypes::Decimal d1{ "123.4500" };
        // Force normalization through conversion
        std::string normalized{ d1.toString() };

        // After normalization, trailing zeros should be removed
        // but scale should still be maintained for precision
        EXPECT_FALSE( d1 == 0 );

        // Test zero normalization
        datatypes::Decimal zero1{ "0.000" };
        datatypes::Decimal zero2{ 0 };
        EXPECT_TRUE( zero1 == zero2 );
    }

    //----------------------------------------------
    // Stream
    //----------------------------------------------

    TEST( DecimalStream, StreamOutput )
    {
        datatypes::Decimal d1{ "123.456" };
        std::ostringstream oss;
        oss << d1;
        EXPECT_EQ( oss.str(), "123.456" );

        datatypes::Decimal d2{ "-789.123" };
        oss.str( "" );
        oss << d2;
        EXPECT_EQ( oss.str(), "-789.123" );
    }

    TEST( DecimalStream, StreamInput )
    {
        datatypes::Decimal d1;
        std::istringstream iss( "456.789" );
        iss >> d1;
        EXPECT_FALSE( iss.fail() );
        EXPECT_EQ( d1.toString(), "456.789" );

        // Test invalid input
        datatypes::Decimal d2;
        std::istringstream iss2( "invalid" );
        iss2 >> d2;
        EXPECT_TRUE( iss2.fail() );
    }

    //----------------------------------------------
    // std::formatter support
    //----------------------------------------------

    TEST( DecimalFormatter, BasicFormatting )
    {
        // Simple integer
        datatypes::Decimal d1 = datatypes::Decimal{ "42" };
        auto str = std::format( "{}", d1 );
        EXPECT_EQ( "42", str );
    }

    TEST( DecimalFormatter, ZeroValue )
    {
        datatypes::Decimal zero = datatypes::Decimal{ "0" };
        auto str = std::format( "{}", zero );
        EXPECT_EQ( "0", str );
    }

    TEST( DecimalFormatter, DecimalPlaces )
    {
        // Exact decimal representation
        datatypes::Decimal d1 = datatypes::Decimal{ "123.456" };
        auto str = std::format( "{}", d1 );
        EXPECT_EQ( "123.456", str );
    }

    TEST( DecimalFormatter, NegativeValue )
    {
        datatypes::Decimal neg = datatypes::Decimal{ "-456.78" };
        auto str = std::format( "{}", neg );
        EXPECT_EQ( "-456.78", str );
    }

    TEST( DecimalFormatter, TrailingZeros )
    {
        // Trailing zeros should be preserved
        datatypes::Decimal d1 = datatypes::Decimal{ "100.50" };
        auto str = std::format( "{}", d1 );
        EXPECT_EQ( "100.5", str ); // Trailing zero removed by toString()
    }

    TEST( DecimalFormatter, HighPrecision )
    {
        // Maximum precision (28 digits)
        datatypes::Decimal precise = datatypes::Decimal{ "12345678901234567890.12345678" };
        auto str = std::format( "{}", precise );
        EXPECT_EQ( "12345678901234567890.12345678", str );
    }

    TEST( DecimalFormatter, SmallDecimal )
    {
        datatypes::Decimal small = datatypes::Decimal{ "0.000001" };
        auto str = std::format( "{}", small );
        EXPECT_EQ( "0.000001", str );
    }

    TEST( DecimalFormatter, FinancialCalculation )
    {
        datatypes::Decimal price = datatypes::Decimal{ "99.99" };
        datatypes::Decimal tax = datatypes::Decimal{ "8.50" };
        datatypes::Decimal total = price + tax;

        auto str = std::format( "Price: {}, Tax: {}, Total: {}", price, tax, total );
        EXPECT_EQ( "Price: 99.99, Tax: 8.5, Total: 108.49", str );
    }

    TEST( DecimalFormatter, MultipleValuesInFormat )
    {
        datatypes::Decimal a = datatypes::Decimal{ "1.23" };
        datatypes::Decimal b = datatypes::Decimal{ "-4.56" };
        datatypes::Decimal c = datatypes::Decimal{ "7.89" };

        auto str = std::format( "a={}, b={}, c={}", a, b, c );
        EXPECT_EQ( "a=1.23, b=-4.56, c=7.89", str );
    }

    TEST( DecimalFormatter, FormattingInContainer )
    {
        std::vector<datatypes::Decimal> values = {
            datatypes::Decimal{ "10.50" },
            datatypes::Decimal{ "20.75" },
            datatypes::Decimal{ "30.25" } };

        std::string result;
        for ( const auto& val : values )
        {
            result += std::format( "{} ", val );
        }
        EXPECT_EQ( "10.5 20.75 30.25 ", result );
    }

    TEST( DecimalFormatter, MaximumValue )
    {
        // Maximum decimal value
        datatypes::Decimal maxVal = std::numeric_limits<datatypes::Decimal>::max();
        auto str = std::format( "{}", maxVal );
        EXPECT_EQ( "79228162514264337593543950335", str );
    }

    TEST( DecimalFormatter, MinimumValue )
    {
        // Minimum positive decimal value (smallest non-zero)
        datatypes::Decimal minVal = std::numeric_limits<datatypes::Decimal>::min();
        auto str = std::format( "{}", minVal );
        // Should be 1E-28 (0.0000000000000000000000000001)
        EXPECT_EQ( "0.0000000000000000000000000001", str );
    }

    //----------------------------------------------
    // Edge case and overflow
    //----------------------------------------------

    TEST( DecimalEdgeCaseAndOverflow, DivisionByZeroHandling )
    {
        datatypes::Decimal dividend{ "123.45" };
        datatypes::Decimal zero{ "0" };

        // Division by zero should throw
        EXPECT_THROW( dividend / zero, std::overflow_error );
        EXPECT_THROW( dividend /= zero, std::overflow_error );
    }

    TEST( DecimalEdgeCaseAndOverflow, OverflowHandling )
    {
        // Test near maximum values
        try
        {
            // Create large Decimal values within limits
            datatypes::Decimal large1{ "99999999999999999999999999.99" };
            datatypes::Decimal large2{ "1.01" };

            // Operations should either succeed or throw appropriately
            datatypes::Decimal result{ large1 * large2 };
            // If we get here, the operation succeeded
            EXPECT_FALSE( result == 0 );
        }
        catch ( const std::exception& )
        {
            // Throwing on overflow is acceptable behavior
            SUCCEED();
        }
    }

    TEST( DecimalEdgeCaseAndOverflow, InvalidInputHandling )
    {
        datatypes::Decimal result;

        // Test various invalid string formats
        EXPECT_FALSE( datatypes::Decimal::fromString( "", result ) );
        EXPECT_FALSE( datatypes::Decimal::fromString( "abc", result ) );
        EXPECT_FALSE( datatypes::Decimal::fromString( "12.34.56", result ) );
        EXPECT_FALSE( datatypes::Decimal::fromString( "12a34", result ) );
        EXPECT_FALSE( datatypes::Decimal::fromString( "+-123", result ) );
        EXPECT_FALSE( datatypes::Decimal::fromString( "123..", result ) );
        EXPECT_FALSE( datatypes::Decimal::fromString( ".123.", result ) );

        // Test strings that are too long - should be truncated
        std::string tooLong{ "1." };
        for ( int i{ 0 }; i < 50; ++i )
        {
            tooLong += "1";
        }

        EXPECT_TRUE( datatypes::Decimal::fromString( tooLong, result ) );
    }
} // namespace nfx::datatypes::test
