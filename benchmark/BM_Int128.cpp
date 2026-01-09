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
 * @file BM_Int128.cpp
 * @brief Benchmark Int128 construction, arithmetic, parsing, formatting, and comparison operations
 */

#include <benchmark/benchmark.h>

#include <nfx/datatypes/Decimal.h>
#include <nfx/datatypes/Int128.h>

namespace nfx::datatypes::benchmark
{
    //=====================================================================
    // Int128 benchmark suite
    //=====================================================================

    //----------------------------------------------
    // Construction
    //----------------------------------------------

    static void BM_Int128ConstructDefault( ::benchmark::State& state )
    {
        for ( auto _ : state )
        {
            Int128 value{};
            ::benchmark::DoNotOptimize( value );
        }
    }

    static void BM_Int128ConstructFromInt32( ::benchmark::State& state )
    {
        for ( auto _ : state )
        {
            Int128 value{ 42 };
            ::benchmark::DoNotOptimize( value );
        }
    }

    static void BM_Int128ConstructFromInt64( ::benchmark::State& state )
    {
        for ( auto _ : state )
        {
            Int128 value{ static_cast<std::int64_t>( 1234567890123456789LL ) };
            ::benchmark::DoNotOptimize( value );
        }
    }

    static void BM_Int128ConstructFromUint64( ::benchmark::State& state )
    {
        for ( auto _ : state )
        {
            Int128 value{ static_cast<std::uint64_t>( 9876543210987654321ULL ) };
            ::benchmark::DoNotOptimize( value );
        }
    }

    static void BM_Int128ConstructFromTwoWords( ::benchmark::State& state )
    {
        for ( auto _ : state )
        {
            Int128 value{ 0xEBC2CE4F3C95D6F5ULL, 0x0173DC35270122E8ULL };
            ::benchmark::DoNotOptimize( value );
        }
    }

    static void BM_Int128ConstructFromFloat( ::benchmark::State& state )
    {
        for ( auto _ : state )
        {
            Int128 value{ 123456.789f };
            ::benchmark::DoNotOptimize( value );
        }
    }

    static void BM_Int128ConstructFromDouble( ::benchmark::State& state )
    {
        for ( auto _ : state )
        {
            Int128 value{ 123456789012345.678 };
            ::benchmark::DoNotOptimize( value );
        }
    }

    static void BM_Int128ConstructFromDecimal( ::benchmark::State& state )
    {
        Decimal decimal{ 23456789 };
        for ( auto _ : state )
        {
            Int128 value{ decimal };
            ::benchmark::DoNotOptimize( value );
        }
    }

    static void BM_Int128CopyConstruct( ::benchmark::State& state )
    {
        Int128 original{ 0xEBC2CE4F3C95D6F5ULL, 0x0173DC35270122E8ULL };
        for ( auto _ : state )
        {
            Int128 value{ original };
            ::benchmark::DoNotOptimize( value );
        }
    }

    //----------------------------------------------
    // Arithmetic operators
    //----------------------------------------------

    static void BM_Int128Addition( ::benchmark::State& state )
    {
        Int128 a{ 0x1234567890ABCDEFULL, 0x0000000012345678ULL };
        Int128 b{ 0xFEDCBA0987654321ULL, 0x0000000087654321ULL };
        for ( auto _ : state )
        {
            Int128 result{ a + b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128Subtraction( ::benchmark::State& state )
    {
        Int128 a{ 0xFEDCBA0987654321ULL, 0x0000000087654321ULL };
        Int128 b{ 0x1234567890ABCDEFULL, 0x0000000012345678ULL };
        for ( auto _ : state )
        {
            Int128 result{ a - b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128Multiplication( ::benchmark::State& state )
    {
        Int128 a{ static_cast<std::int64_t>( 123456789012345LL ) };
        Int128 b{ static_cast<std::int64_t>( 987654321098765LL ) };
        for ( auto _ : state )
        {
            Int128 result{ a * b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128MultiplicationLarge( ::benchmark::State& state )
    {
        Int128 a{ 0x1234567890ABCDEFULL, 0x0000000012345678ULL };
        Int128 b{ 0xFEDCBA0987654321ULL, 0x0000000087654321ULL };
        for ( auto _ : state )
        {
            Int128 result{ a * b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128Division( ::benchmark::State& state )
    {
        Int128 a{ 0xFEDCBA0987654321ULL, 0x0000000087654321ULL };
        Int128 b{ static_cast<std::int64_t>( 123456789LL ) };
        for ( auto _ : state )
        {
            Int128 result{ a / b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128DivisionLarge( ::benchmark::State& state )
    {
        Int128 a{ 0xFEDCBA0987654321ULL, 0x0000000087654321ULL };
        Int128 b{ 0x1234567890ABCDEFULL, 0x0000000000000001ULL };
        for ( auto _ : state )
        {
            Int128 result{ a / b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128Modulo( ::benchmark::State& state )
    {
        Int128 a{ 0xFEDCBA0987654321ULL, 0x0000000087654321ULL };
        Int128 b{ static_cast<std::int64_t>( 123456789LL ) };
        for ( auto _ : state )
        {
            Int128 result{ a % b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128UnaryMinus( ::benchmark::State& state )
    {
        Int128 value{ 0x1234567890ABCDEFULL, 0x0000000012345678ULL };
        for ( auto _ : state )
        {
            Int128 result{ -value };
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Parsing
    //----------------------------------------------

    static void BM_Int128ParseSmallNumber( ::benchmark::State& state )
    {
        for ( auto _ : state )
        {
            auto result{ Int128{ "42" } };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128ParseMediumNumber( ::benchmark::State& state )
    {
        for ( auto _ : state )
        {
            auto result{ Int128{ "123456789012345678" } };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128ParseLargeNumber( ::benchmark::State& state )
    {
        for ( auto _ : state )
        {
            auto result{ Int128{ "123456789012345678901234567890123456789" } };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128ParseNegativeNumber( ::benchmark::State& state )
    {
        for ( auto _ : state )
        {
            auto result{ Int128{ "-987654321098765432109876543210" } };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128FromStringValid( ::benchmark::State& state )
    {
        Int128 result{};
        for ( auto _ : state )
        {
            bool success{ Int128::fromString( "123456789012345678901234567890", result ) };
            ::benchmark::DoNotOptimize( success );
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128FromStringInValid( ::benchmark::State& state )
    {
        Int128 result{};
        for ( auto _ : state )
        {
            bool success{ Int128::fromString( "not_a_number", result ) };
            ::benchmark::DoNotOptimize( success );
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Conversion
    //----------------------------------------------

    static void BM_Int128ToLow( ::benchmark::State& state )
    {
        Int128 value{ 0x1234567890ABCDEFULL, 0x0000000012345678ULL };
        for ( auto _ : state )
        {
            std::uint64_t result{ value.toLow() };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128ToHigh( ::benchmark::State& state )
    {
        Int128 value{ 0x1234567890ABCDEFULL, 0x0000000012345678ULL };
        for ( auto _ : state )
        {
            std::uint64_t result{ value.toHigh() };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128ToBits( ::benchmark::State& state )
    {
        Int128 value{ 0x1234567890ABCDEFULL, 0x0000000012345678ULL };
        for ( auto _ : state )
        {
            auto result{ value.toBits() };
            ::benchmark::DoNotOptimize( result );
        }
    }

#if NFX_DATATYPES_HAS_NATIVE_INT128
    static void BM_Int128ToNative( ::benchmark::State& state )
    {
        Int128 value{ 0x1234567890ABCDEFULL, 0x0000000012345678ULL };
        for ( auto _ : state )
        {
            NFX_DATATYPES_NATIVE_INT128 result{ value.toNative() };
            ::benchmark::DoNotOptimize( result );
        }
    }

#endif

    //----------------------------------------------
    // Formatting
    //----------------------------------------------

    static void BM_Int128ToStringSmall( ::benchmark::State& state )
    {
        Int128 value{ 42 };
        for ( auto _ : state )
        {
            std::string result{ value.toString() };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128ToStringMedium( ::benchmark::State& state )
    {
        Int128 value{ static_cast<std::int64_t>( 1234567890123456789LL ) };
        for ( auto _ : state )
        {
            std::string result{ value.toString() };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128ToStringLarge( ::benchmark::State& state )
    {
        Int128 value{ 0xEBC2CE4F3C95D6F5ULL, 0x0173DC35270122E8ULL };
        for ( auto _ : state )
        {
            std::string result{ value.toString() };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128ToStringNegative( ::benchmark::State& state )
    {
        Int128 value{ 0xEBC2CE4F3C95D6F5ULL, 0x0173DC35270122E8ULL };
        Int128 negative{ -value };
        for ( auto _ : state )
        {
            std::string result{ negative.toString() };
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Comparison
    //----------------------------------------------

    static void BM_Int128EqualityInt128( ::benchmark::State& state )
    {
        Int128 a{ 0x1234567890ABCDEFULL, 0x0000000012345678ULL };
        Int128 b{ 0x1234567890ABCDEFULL, 0x0000000012345678ULL };
        for ( auto _ : state )
        {
            bool result{ a == b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128LessThanInt128( ::benchmark::State& state )
    {
        Int128 a{ 0x1234567890ABCDEFULL, 0x0000000012345678ULL };
        Int128 b{ 0xFEDCBA0987654321ULL, 0x0000000087654321ULL };
        for ( auto _ : state )
        {
            bool result{ a < b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128GreaterThanInt128( ::benchmark::State& state )
    {
        Int128 a{ 0xFEDCBA0987654321ULL, 0x0000000087654321ULL };
        Int128 b{ 0x1234567890ABCDEFULL, 0x0000000012345678ULL };
        for ( auto _ : state )
        {
            bool result{ a > b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128EqualityInt64( ::benchmark::State& state )
    {
        Int128 a{ static_cast<std::int64_t>( 1234567890123456789LL ) };
        std::int64_t b{ 1234567890123456789LL };
        for ( auto _ : state )
        {
            bool result{ a == b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128LessThanInt64( ::benchmark::State& state )
    {
        Int128 a{ static_cast<std::int64_t>( 1234567890123456789LL ) };
        std::int64_t b{ 8765432109876543210LL };
        for ( auto _ : state )
        {
            bool result{ a < b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128EqualityUint64( ::benchmark::State& state )
    {
        Int128 a{ static_cast<std::uint64_t>( 9876543210987654321ULL ) };
        std::uint64_t b{ 9876543210987654321ULL };
        for ( auto _ : state )
        {
            bool result{ a == b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128EqualityDouble( ::benchmark::State& state )
    {
        Int128 a{ static_cast<std::int64_t>( 123456789012345LL ) };
        double b{ 123456789012345.0 };
        for ( auto _ : state )
        {
            bool result{ a == b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128LessThanDouble( ::benchmark::State& state )
    {
        Int128 a{ static_cast<std::int64_t>( 123456789012345LL ) };
        double b{ 987654321098765.0 };
        for ( auto _ : state )
        {
            bool result{ a < b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128EqualityFloat( ::benchmark::State& state )
    {
        Int128 a{ 123456 };
        float b{ 123456.0f };
        for ( auto _ : state )
        {
            bool result{ a == b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128EqualityDecimal( ::benchmark::State& state )
    {
        Int128 a{ 123456789 };
        Decimal b{ 123456789 };
        for ( auto _ : state )
        {
            bool result{ a == b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128LessThanDecimal( ::benchmark::State& state )
    {
        Int128 a{ 123456789 };
        Decimal b{ 987654321 };
        for ( auto _ : state )
        {
            bool result{ a < b };
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Comparison with zero
    //----------------------------------------------

    static void BM_Int128IsZero( ::benchmark::State& state )
    {
        Int128 value{ 0 };
        for ( auto _ : state )
        {
            bool result{ value == 0 };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128IsZeroNonZero( ::benchmark::State& state )
    {
        Int128 value{ 0x1234567890ABCDEFULL, 0x0000000012345678ULL };
        for ( auto _ : state )
        {
            bool result{ value == 0 };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128IsNegative( ::benchmark::State& state )
    {
        Int128 value{ static_cast<std::int64_t>( -123456789LL ) };
        for ( auto _ : state )
        {
            bool result{ value < 0 };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128IsNegativePositive( ::benchmark::State& state )
    {
        Int128 value{ static_cast<std::int64_t>( 123456789LL ) };
        for ( auto _ : state )
        {
            bool result{ value < 0 };
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Mathematical operations
    //----------------------------------------------

    static void BM_Int128AbsPositive( ::benchmark::State& state )
    {
        Int128 value{ 0x1234567890ABCDEFULL, 0x0000000012345678ULL };
        for ( auto _ : state )
        {
            Int128 result{ value.abs() };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128AbsNegative( ::benchmark::State& state )
    {
        Int128 value{ 0x1234567890ABCDEFULL, 0x0000000012345678ULL };
        Int128 negative{ -value };
        for ( auto _ : state )
        {
            Int128 result{ negative.abs() };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128Isqrt( ::benchmark::State& state )
    {
        Int128 value{ 1000000 };
        for ( auto _ : state )
        {
            Int128 result{ value.isqrt() };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128IsqrtLarge( ::benchmark::State& state )
    {
        Int128 value{ 0x1234567890ABCDEFULL, 0x0000000000001234ULL };
        for ( auto _ : state )
        {
            Int128 result{ value.isqrt() };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128IsqrtPerfectSquare( ::benchmark::State& state )
    {
        Int128 value{ 10000 };
        for ( auto _ : state )
        {
            Int128 result{ value.isqrt() };
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Free function wrappers
    //----------------------------------------------

    static void BM_Int128AbsFreeFunctionPositive( ::benchmark::State& state )
    {
        Int128 value{ 0x1234567890ABCDEFULL, 0x0000000012345678ULL };
        for ( auto _ : state )
        {
            Int128 result{ abs( value ) };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128AbsFreeFunctionNegative( ::benchmark::State& state )
    {
        Int128 value{ 0x1234567890ABCDEFULL, 0x0000000012345678ULL };
        Int128 negative{ -value };
        for ( auto _ : state )
        {
            Int128 result{ abs( negative ) };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128IsqrtFreeFunction( ::benchmark::State& state )
    {
        Int128 value{ 1000000 };
        for ( auto _ : state )
        {
            Int128 result{ isqrt( value ) };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128SqrtFreeFunction( ::benchmark::State& state )
    {
        Int128 value{ 1000000 };
        for ( auto _ : state )
        {
            Decimal result{ sqrt( value ) };
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Formatting
    //----------------------------------------------

    static void BM_Int128Format( ::benchmark::State& state )
    {
        Int128 value{ 0x1234567890ABCDEFULL, 0x0000000012345678ULL };
        for ( auto _ : state )
        {
            std::string result{ std::format( "{}", value ) };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_Int128FormatNegative( ::benchmark::State& state )
    {
        Int128 value{ 0x1234567890ABCDEFULL, 0x0000000012345678ULL };
        Int128 negative{ -value };
        for ( auto _ : state )
        {
            std::string result{ std::format( "{}", negative ) };
            ::benchmark::DoNotOptimize( result );
        }
    }

    //=====================================================================
    // Benchmarks registration
    //=====================================================================

    //----------------------------------------------
    // Construction
    //----------------------------------------------

    BENCHMARK( BM_Int128ConstructDefault );
    BENCHMARK( BM_Int128ConstructFromInt32 );
    BENCHMARK( BM_Int128ConstructFromInt64 );
    BENCHMARK( BM_Int128ConstructFromUint64 );
    BENCHMARK( BM_Int128ConstructFromTwoWords );
    BENCHMARK( BM_Int128ConstructFromFloat );
    BENCHMARK( BM_Int128ConstructFromDouble );
    BENCHMARK( BM_Int128ConstructFromDecimal );
    BENCHMARK( BM_Int128CopyConstruct );

    //----------------------------------------------
    // Arithmetic operators
    //----------------------------------------------

    BENCHMARK( BM_Int128Addition );
    BENCHMARK( BM_Int128Subtraction );
    BENCHMARK( BM_Int128Multiplication );
    BENCHMARK( BM_Int128MultiplicationLarge );
    BENCHMARK( BM_Int128Division );
    BENCHMARK( BM_Int128DivisionLarge );
    BENCHMARK( BM_Int128Modulo );
    BENCHMARK( BM_Int128UnaryMinus );

    //----------------------------------------------
    // Parsing
    //----------------------------------------------

    BENCHMARK( BM_Int128ParseSmallNumber );
    BENCHMARK( BM_Int128ParseMediumNumber );
    BENCHMARK( BM_Int128ParseLargeNumber );
    BENCHMARK( BM_Int128ParseNegativeNumber );
    BENCHMARK( BM_Int128FromStringValid );
    BENCHMARK( BM_Int128FromStringInValid );

    //----------------------------------------------
    // Conversion
    //----------------------------------------------

    BENCHMARK( BM_Int128ToLow );
    BENCHMARK( BM_Int128ToHigh );
    BENCHMARK( BM_Int128ToBits );

#if NFX_DATATYPES_HAS_NATIVE_INT128
    BENCHMARK( BM_Int128ToNative );
#endif

    //----------------------------------------------
    // Formatting
    //----------------------------------------------

    BENCHMARK( BM_Int128ToStringSmall );
    BENCHMARK( BM_Int128ToStringMedium );
    BENCHMARK( BM_Int128ToStringLarge );
    BENCHMARK( BM_Int128ToStringNegative );

    //----------------------------------------------
    // Comparison
    //----------------------------------------------

    BENCHMARK( BM_Int128EqualityInt128 );
    BENCHMARK( BM_Int128LessThanInt128 );
    BENCHMARK( BM_Int128GreaterThanInt128 );
    BENCHMARK( BM_Int128EqualityInt64 );
    BENCHMARK( BM_Int128LessThanInt64 );
    BENCHMARK( BM_Int128EqualityUint64 );
    BENCHMARK( BM_Int128EqualityDouble );
    BENCHMARK( BM_Int128LessThanDouble );
    BENCHMARK( BM_Int128EqualityFloat );
    BENCHMARK( BM_Int128EqualityDecimal );
    BENCHMARK( BM_Int128LessThanDecimal );

    //----------------------------------------------
    // Comparison with zero
    //----------------------------------------------

    BENCHMARK( BM_Int128IsZero );
    BENCHMARK( BM_Int128IsZeroNonZero );
    BENCHMARK( BM_Int128IsNegative );
    BENCHMARK( BM_Int128IsNegativePositive );

    //----------------------------------------------
    // Mathematical operations
    //----------------------------------------------

    BENCHMARK( BM_Int128AbsPositive );
    BENCHMARK( BM_Int128AbsNegative );
    BENCHMARK( BM_Int128Isqrt );
    BENCHMARK( BM_Int128IsqrtLarge );
    BENCHMARK( BM_Int128IsqrtPerfectSquare );

    //----------------------------------------------
    // Free function wrappers
    //----------------------------------------------

    BENCHMARK( BM_Int128AbsFreeFunctionPositive );
    BENCHMARK( BM_Int128AbsFreeFunctionNegative );
    BENCHMARK( BM_Int128IsqrtFreeFunction );
    BENCHMARK( BM_Int128SqrtFreeFunction );

    //----------------------------------------------
    // Formatting
    //----------------------------------------------

    BENCHMARK( BM_Int128Format );
    BENCHMARK( BM_Int128FormatNegative );
} // namespace nfx::datatypes::benchmark

BENCHMARK_MAIN();
