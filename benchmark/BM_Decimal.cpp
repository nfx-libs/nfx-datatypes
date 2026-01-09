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
 * @file BM_Decimal.cpp
 * @brief Benchmark Decimal construction, arithmetic, parsing, formatting, and comparison operations
 */

#include <benchmark/benchmark.h>

#include <nfx/datatypes/Decimal.h>
#include <nfx/datatypes/Int128.h>

#include <limits>

namespace nfx::datatypes::benchmark
{
    //=====================================================================
    // Decimal benchmark suite
    //=====================================================================

    //----------------------------------------------
    // Construction
    //----------------------------------------------

    static void BM_DecimalConstructDefault( ::benchmark::State& state )
    {
        for ( auto _ : state )
        {
            Decimal value{};
            ::benchmark::DoNotOptimize( value );
        }
    }

    static void BM_DecimalConstructFromInt32( ::benchmark::State& state )
    {
        for ( auto _ : state )
        {
            Decimal value{ static_cast<std::int32_t>( 42 ) };
            ::benchmark::DoNotOptimize( value );
        }
    }

    static void BM_DecimalConstructFromInt64( ::benchmark::State& state )
    {
        for ( auto _ : state )
        {
            Decimal value{ static_cast<std::int64_t>( 1234567890123456789LL ) };
            ::benchmark::DoNotOptimize( value );
        }
    }

    static void BM_DecimalConstructFromUint32( ::benchmark::State& state )
    {
        for ( auto _ : state )
        {
            Decimal value{ static_cast<std::uint32_t>( 4294967295U ) };
            ::benchmark::DoNotOptimize( value );
        }
    }

    static void BM_DecimalConstructFromUint64( ::benchmark::State& state )
    {
        for ( auto _ : state )
        {
            Decimal value{ static_cast<std::uint64_t>( 9876543210987654321ULL ) };
            ::benchmark::DoNotOptimize( value );
        }
    }

    static void BM_DecimalConstructFromFloat( ::benchmark::State& state )
    {
        for ( auto _ : state )
        {
            Decimal value{ 123.456f };
            ::benchmark::DoNotOptimize( value );
        }
    }

    static void BM_DecimalConstructFromDouble( ::benchmark::State& state )
    {
        for ( auto _ : state )
        {
            Decimal value{ 123456.789012345 };
            ::benchmark::DoNotOptimize( value );
        }
    }

    static void BM_DecimalConstructFromInt128( ::benchmark::State& state )
    {
        Int128 int128Value{ static_cast<std::int64_t>( 1234567890123456789LL ) };
        for ( auto _ : state )
        {
            Decimal value{ int128Value };
            ::benchmark::DoNotOptimize( value );
        }
    }

    static void BM_DecimalCopyConstruct( ::benchmark::State& state )
    {
        Decimal original{ 123456.789 };
        for ( auto _ : state )
        {
            Decimal value{ original };
            ::benchmark::DoNotOptimize( value );
        }
    }

    //----------------------------------------------
    // Arithmetic operators
    //----------------------------------------------

    static void BM_DecimalAddition( ::benchmark::State& state )
    {
        Decimal a{ 123456.789 };
        Decimal b{ 987654.321 };
        for ( auto _ : state )
        {
            Decimal result{ a + b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalSubtraction( ::benchmark::State& state )
    {
        Decimal a{ 987654.321 };
        Decimal b{ 123456.789 };
        for ( auto _ : state )
        {
            Decimal result{ a - b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalMultiplication( ::benchmark::State& state )
    {
        Decimal a{ 123.456 };
        Decimal b{ 789.012 };
        for ( auto _ : state )
        {
            Decimal result{ a * b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalMultiplicationLarge( ::benchmark::State& state )
    {
        Decimal a{ 123456789012345.678 };
        Decimal b{ 987654321098765.432 };
        for ( auto _ : state )
        {
            Decimal result{ a * b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalDivision( ::benchmark::State& state )
    {
        Decimal a{ 987654.321 };
        Decimal b{ 123.456 };
        for ( auto _ : state )
        {
            Decimal result{ a / b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalDivisionHighPrecision( ::benchmark::State& state )
    {
        Decimal a{ 1.0 };
        Decimal b{ 3.0 };
        for ( auto _ : state )
        {
            Decimal result{ a / b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalUnaryMinus( ::benchmark::State& state )
    {
        Decimal value{ 123456.789 };
        for ( auto _ : state )
        {
            Decimal result{ -value };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalAdditionAssignment( ::benchmark::State& state )
    {
        Decimal a{ 123456.789 };
        Decimal b{ 987.654 };
        for ( auto _ : state )
        {
            Decimal temp{ a };
            temp += b;
            ::benchmark::DoNotOptimize( temp );
        }
    }

    static void BM_DecimalSubtractionAssignment( ::benchmark::State& state )
    {
        Decimal a{ 987654.321 };
        Decimal b{ 123.456 };
        for ( auto _ : state )
        {
            Decimal temp{ a };
            temp -= b;
            ::benchmark::DoNotOptimize( temp );
        }
    }

    static void BM_DecimalMultiplicationAssignment( ::benchmark::State& state )
    {
        Decimal a{ 123.456 };
        Decimal b{ 789.012 };
        for ( auto _ : state )
        {
            Decimal temp{ a };
            temp *= b;
            ::benchmark::DoNotOptimize( temp );
        }
    }

    static void BM_DecimalDivisionAssignment( ::benchmark::State& state )
    {
        Decimal a{ 987654.321 };
        Decimal b{ 123.456 };
        for ( auto _ : state )
        {
            Decimal temp{ a };
            temp /= b;
            ::benchmark::DoNotOptimize( temp );
        }
    }

    //----------------------------------------------
    // Parsing
    //----------------------------------------------

    static void BM_DecimalParseInteger( ::benchmark::State& state )
    {
        for ( auto _ : state )
        {
            auto result{ Decimal{ "12345" } };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalParseSmallDecimal( ::benchmark::State& state )
    {
        for ( auto _ : state )
        {
            auto result{ Decimal{ "123.456" } };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalParseLargeDecimal( ::benchmark::State& state )
    {
        for ( auto _ : state )
        {
            auto result{ Decimal{ "123456789012345678901234.567890" } };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalParseHighPrecision( ::benchmark::State& state )
    {
        for ( auto _ : state )
        {
            auto result{ Decimal{ "0.1234567890123456789012345678" } };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalParseNegative( ::benchmark::State& state )
    {
        for ( auto _ : state )
        {
            auto result{ Decimal{ "-987654321.123456789" } };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalFromStringValid( ::benchmark::State& state )
    {
        Decimal result{};
        for ( auto _ : state )
        {
            bool success{ Decimal::fromString( "123456.789", result ) };
            ::benchmark::DoNotOptimize( success );
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalFromStringInvalid( ::benchmark::State& state )
    {
        Decimal result{};
        for ( auto _ : state )
        {
            bool success{ Decimal::fromString( "not_a_decimal", result ) };
            ::benchmark::DoNotOptimize( success );
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Conversion
    //----------------------------------------------

    static void BM_DecimalToDouble( ::benchmark::State& state )
    {
        Decimal value{ 123456.789 };
        for ( auto _ : state )
        {
            double result{ value.toDouble() };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalToBits( ::benchmark::State& state )
    {
        Decimal value{ 123456.789 };
        for ( auto _ : state )
        {
            auto result{ value.toBits() };
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Formatting
    //----------------------------------------------

    static void BM_DecimalToStringInteger( ::benchmark::State& state )
    {
        Decimal value{ static_cast<std::int32_t>( 12345 ) };
        for ( auto _ : state )
        {
            std::string result{ value.toString() };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalToStringSmall( ::benchmark::State& state )
    {
        Decimal value{ 123.456 };
        for ( auto _ : state )
        {
            std::string result{ value.toString() };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalToStringLarge( ::benchmark::State& state )
    {
        Decimal value{ 123456789012345678.901234567 };
        for ( auto _ : state )
        {
            std::string result{ value.toString() };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalToStringNegative( ::benchmark::State& state )
    {
        Decimal value{ -987654321.123456789 };
        for ( auto _ : state )
        {
            std::string result{ value.toString() };
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Comparison
    //----------------------------------------------

    static void BM_DecimalEqualityDecimal( ::benchmark::State& state )
    {
        Decimal a{ 123456.789 };
        Decimal b{ 123456.789 };
        for ( auto _ : state )
        {
            bool result{ a == b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalLessThanDecimal( ::benchmark::State& state )
    {
        Decimal a{ 123456.789 };
        Decimal b{ 987654.321 };
        for ( auto _ : state )
        {
            bool result{ a < b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalGreaterThanDecimal( ::benchmark::State& state )
    {
        Decimal a{ 987654.321 };
        Decimal b{ 123456.789 };
        for ( auto _ : state )
        {
            bool result{ a > b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalEqualityInt32( ::benchmark::State& state )
    {
        Decimal a{ static_cast<std::int32_t>( 12345 ) };
        std::int32_t b{ 12345 };
        for ( auto _ : state )
        {
            bool result{ a == b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalEqualityInt64( ::benchmark::State& state )
    {
        Decimal a{ static_cast<std::int64_t>( 1234567890123456789LL ) };
        std::int64_t b{ 1234567890123456789LL };
        for ( auto _ : state )
        {
            bool result{ a == b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalEqualityUint64( ::benchmark::State& state )
    {
        Decimal a{ static_cast<std::uint64_t>( 9876543210987654321ULL ) };
        std::uint64_t b{ 9876543210987654321ULL };
        for ( auto _ : state )
        {
            bool result{ a == b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalEqualityDouble( ::benchmark::State& state )
    {
        Decimal a{ 123456.789 };
        double b{ 123456.789 };
        for ( auto _ : state )
        {
            bool result{ a == b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalLessThanDouble( ::benchmark::State& state )
    {
        Decimal a{ 123456.789 };
        double b{ 987654.321 };
        for ( auto _ : state )
        {
            bool result{ a < b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalEqualityFloat( ::benchmark::State& state )
    {
        Decimal a{ 123.456f };
        float b{ 123.456f };
        for ( auto _ : state )
        {
            bool result{ a == b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalEqualityInt128( ::benchmark::State& state )
    {
        Decimal a{ static_cast<std::int64_t>( 123456789012345LL ) };
        Int128 b{ static_cast<std::int64_t>( 123456789012345LL ) };
        for ( auto _ : state )
        {
            bool result{ a == b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalLessThanInt128( ::benchmark::State& state )
    {
        Decimal a{ static_cast<std::int64_t>( 123456789012345LL ) };
        Int128 b{ static_cast<std::int64_t>( 987654321098765LL ) };
        for ( auto _ : state )
        {
            bool result{ a < b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Comparison with zero
    //----------------------------------------------

    static void BM_DecimalIsZero( ::benchmark::State& state )
    {
        Decimal value{};
        for ( auto _ : state )
        {
            bool result{ value == 0 };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalIsZeroNonZero( ::benchmark::State& state )
    {
        Decimal value{ 123.456 };
        for ( auto _ : state )
        {
            bool result{ value == 0 };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalIsNegative( ::benchmark::State& state )
    {
        Decimal value{ -123.456 };
        for ( auto _ : state )
        {
            bool result{ value < 0 };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalIsNegativePositive( ::benchmark::State& state )
    {
        Decimal value{ 123.456 };
        for ( auto _ : state )
        {
            bool result{ value < 0 };
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Mathematical operations
    //----------------------------------------------

    static void BM_DecimalAbsPositive( ::benchmark::State& state )
    {
        Decimal value{ 123456.789 };
        for ( auto _ : state )
        {
            Decimal result{ value.abs() };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalAbsNegative( ::benchmark::State& state )
    {
        Decimal value{ -123456.789 };
        for ( auto _ : state )
        {
            Decimal result{ value.abs() };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalSqrt( ::benchmark::State& state )
    {
        Decimal value{ 123456.789 };
        for ( auto _ : state )
        {
            Decimal result{ value.sqrt() };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalSqrtPerfectSquare( ::benchmark::State& state )
    {
        Decimal value{ 10000 };
        for ( auto _ : state )
        {
            Decimal result{ value.sqrt() };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalSqrtLarge( ::benchmark::State& state )
    {
        Decimal value{ "999999999999999999999.999999" };
        for ( auto _ : state )
        {
            Decimal result{ value.sqrt() };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalAbsFreeFunctionPositive( ::benchmark::State& state )
    {
        Decimal value{ 123456.789 };
        for ( auto _ : state )
        {
            Decimal result{ abs( value ) };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalAbsFreeFunctionNegative( ::benchmark::State& state )
    {
        Decimal value{ -123456.789 };
        for ( auto _ : state )
        {
            Decimal result{ abs( value ) };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalSqrtFreeFunction( ::benchmark::State& state )
    {
        Decimal value{ 123456.789 };
        for ( auto _ : state )
        {
            Decimal result{ sqrt( value ) };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalRoundFreeFunction( ::benchmark::State& state )
    {
        Decimal value{ 123456.789 };
        for ( auto _ : state )
        {
            Decimal result{ round( value ) };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalFloorFreeFunction( ::benchmark::State& state )
    {
        Decimal value{ 123456.789 };
        for ( auto _ : state )
        {
            Decimal result{ floor( value ) };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalCeilingFreeFunction( ::benchmark::State& state )
    {
        Decimal value{ 123456.789 };
        for ( auto _ : state )
        {
            Decimal result{ ceil( value ) };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalTruncateFreeFunction( ::benchmark::State& state )
    {
        Decimal value{ 123456.789 };
        for ( auto _ : state )
        {
            Decimal result{ trunc( value ) };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalFormat( ::benchmark::State& state )
    {
        Decimal value{ 123456.789 };
        for ( auto _ : state )
        {
            std::string result{ std::format( "{}", value ) };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalFormatNegative( ::benchmark::State& state )
    {
        Decimal value{ -123456.789 };
        for ( auto _ : state )
        {
            std::string result{ std::format( "{}", value ) };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalTrunc( ::benchmark::State& state )
    {
        Decimal value{ 123456.789 };
        for ( auto _ : state )
        {
            Decimal result{ value.trunc() };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalFloor( ::benchmark::State& state )
    {
        Decimal value{ 123456.789 };
        for ( auto _ : state )
        {
            Decimal result{ value.floor() };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalCeil( ::benchmark::State& state )
    {
        Decimal value{ 123456.789 };
        for ( auto _ : state )
        {
            Decimal result{ value.ceil() };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalRound( ::benchmark::State& state )
    {
        Decimal value{ 123456.789 };
        for ( auto _ : state )
        {
            Decimal result{ value.round() };
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Property accessors
    //----------------------------------------------

    static void BM_DecimalGetScale( ::benchmark::State& state )
    {
        Decimal value{ 123.456 };
        for ( auto _ : state )
        {
            std::uint8_t result{ value.scale() };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalDecimalPlacesCount( ::benchmark::State& state )
    {
        Decimal value{ 123.4500 };
        for ( auto _ : state )
        {
            std::uint8_t result{ value.decimalPlacesCount() };
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Constants
    //----------------------------------------------

    static void BM_DecimalConstantMinValue( ::benchmark::State& state )
    {
        for ( auto _ : state )
        {
            Decimal result{ std::numeric_limits<Decimal>::min() };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DecimalConstantMaxValue( ::benchmark::State& state )
    {
        for ( auto _ : state )
        {
            Decimal result{ std::numeric_limits<Decimal>::max() };
            ::benchmark::DoNotOptimize( result );
        }
    }

    //=====================================================================
    // Benchmarks registration
    //=====================================================================

    //----------------------------------------------
    // Construction
    //----------------------------------------------

    BENCHMARK( BM_DecimalConstructDefault );
    BENCHMARK( BM_DecimalConstructFromInt32 );
    BENCHMARK( BM_DecimalConstructFromInt64 );
    BENCHMARK( BM_DecimalConstructFromUint32 );
    BENCHMARK( BM_DecimalConstructFromUint64 );
    BENCHMARK( BM_DecimalConstructFromFloat );
    BENCHMARK( BM_DecimalConstructFromDouble );
    BENCHMARK( BM_DecimalConstructFromInt128 );
    BENCHMARK( BM_DecimalCopyConstruct );

    //----------------------------------------------
    // Arithmetic operators
    //----------------------------------------------

    BENCHMARK( BM_DecimalAddition );
    BENCHMARK( BM_DecimalSubtraction );
    BENCHMARK( BM_DecimalMultiplication );
    BENCHMARK( BM_DecimalMultiplicationLarge );
    BENCHMARK( BM_DecimalDivision );
    BENCHMARK( BM_DecimalDivisionHighPrecision );
    BENCHMARK( BM_DecimalUnaryMinus );
    BENCHMARK( BM_DecimalAdditionAssignment );
    BENCHMARK( BM_DecimalSubtractionAssignment );
    BENCHMARK( BM_DecimalMultiplicationAssignment );
    BENCHMARK( BM_DecimalDivisionAssignment );

    //----------------------------------------------
    // Parsing
    //----------------------------------------------

    BENCHMARK( BM_DecimalParseInteger );
    BENCHMARK( BM_DecimalParseSmallDecimal );
    BENCHMARK( BM_DecimalParseLargeDecimal );
    BENCHMARK( BM_DecimalParseHighPrecision );
    BENCHMARK( BM_DecimalParseNegative );
    BENCHMARK( BM_DecimalFromStringValid );
    BENCHMARK( BM_DecimalFromStringInvalid );

    //----------------------------------------------
    // Conversion
    //----------------------------------------------

    BENCHMARK( BM_DecimalToDouble );
    BENCHMARK( BM_DecimalToBits );

    //----------------------------------------------
    // Formatting
    //----------------------------------------------

    BENCHMARK( BM_DecimalToStringInteger );
    BENCHMARK( BM_DecimalToStringSmall );
    BENCHMARK( BM_DecimalToStringLarge );
    BENCHMARK( BM_DecimalToStringNegative );

    //----------------------------------------------
    // Comparison
    //----------------------------------------------

    BENCHMARK( BM_DecimalEqualityDecimal );
    BENCHMARK( BM_DecimalLessThanDecimal );
    BENCHMARK( BM_DecimalGreaterThanDecimal );
    BENCHMARK( BM_DecimalEqualityInt32 );
    BENCHMARK( BM_DecimalEqualityInt64 );
    BENCHMARK( BM_DecimalEqualityUint64 );
    BENCHMARK( BM_DecimalEqualityDouble );
    BENCHMARK( BM_DecimalLessThanDouble );
    BENCHMARK( BM_DecimalEqualityFloat );
    BENCHMARK( BM_DecimalEqualityInt128 );
    BENCHMARK( BM_DecimalLessThanInt128 );

    //----------------------------------------------
    // Comparison with zero
    //----------------------------------------------

    BENCHMARK( BM_DecimalIsZero );
    BENCHMARK( BM_DecimalIsZeroNonZero );
    BENCHMARK( BM_DecimalIsNegative );
    BENCHMARK( BM_DecimalIsNegativePositive );

    //----------------------------------------------
    // Mathematical operations
    //----------------------------------------------

    BENCHMARK( BM_DecimalAbsPositive );
    BENCHMARK( BM_DecimalAbsNegative );
    BENCHMARK( BM_DecimalSqrt );
    BENCHMARK( BM_DecimalSqrtPerfectSquare );
    BENCHMARK( BM_DecimalSqrtLarge );
    BENCHMARK( BM_DecimalTrunc );
    BENCHMARK( BM_DecimalFloor );
    BENCHMARK( BM_DecimalCeil );
    BENCHMARK( BM_DecimalRound );

    //----------------------------------------------
    // Free function wrappers
    //----------------------------------------------

    BENCHMARK( BM_DecimalAbsFreeFunctionPositive );
    BENCHMARK( BM_DecimalAbsFreeFunctionNegative );
    BENCHMARK( BM_DecimalSqrtFreeFunction );
    BENCHMARK( BM_DecimalRoundFreeFunction );
    BENCHMARK( BM_DecimalFloorFreeFunction );
    BENCHMARK( BM_DecimalCeilingFreeFunction );
    BENCHMARK( BM_DecimalTruncateFreeFunction );

    //----------------------------------------------
    // Formatting
    //----------------------------------------------

    BENCHMARK( BM_DecimalFormat );
    BENCHMARK( BM_DecimalFormatNegative );

    //----------------------------------------------
    // Property accessors
    //----------------------------------------------

    BENCHMARK( BM_DecimalGetScale );
    BENCHMARK( BM_DecimalDecimalPlacesCount );

    //----------------------------------------------
    // Constants
    //----------------------------------------------

    BENCHMARK( BM_DecimalConstantMinValue );
    BENCHMARK( BM_DecimalConstantMaxValue );
} // namespace nfx::datatypes::benchmark

BENCHMARK_MAIN();
