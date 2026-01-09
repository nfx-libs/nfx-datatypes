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
 * @file Sample_Datatypes.cpp
 * @brief Demonstrates comprehensive usage of nfx-datatypes library
 * @details This sample shows how to use Int128 and Decimal for high-precision
 *          mathematical operations, exact arithmetic, and business calculations
 */

#include <iomanip>
#include <iostream>
#include <string>

#include <nfx/DataTypes.h>

int main()
{
    using namespace nfx::datatypes;

    std::cout << "=== nfx-datatypes Library ===\n\n";

    //=====================================================================
    // 1. Int128: Basic construction
    //=====================================================================
    {
        std::cout << "1. Int128: Basic construction\n";
        std::cout << "--------------------------------\n";

        Int128 small{ 12345 };
        Int128 large{ static_cast<std::int64_t>( 9223372036854775807LL ) };
        Int128 huge{ "123456789012345678901234567890" };

        std::cout << "Small: " << small << "\n";
        std::cout << "Large (max int64): " << large << "\n";
        std::cout << "Huge (from string): " << huge << "\n";
        std::cout << "\n";
    }

    //=====================================================================
    // 2. Int128: Arithmetic & comparisons
    //=====================================================================
    {
        std::cout << "2. Int128: Arithmetic & comparisons\n";
        std::cout << "---------------------------------------\n";

        Int128 a{ 1000 };
        Int128 b{ 7 };
        auto sum = a + b;
        auto prod = a * Int128{ 2 };
        auto div = a / b;
        auto mod = a % b;

        std::cout << "1000 + 7 = " << sum << "\n";
        std::cout << "1000 * 2 = " << prod << "\n";
        std::cout << "1000 / 7 = " << div << "\n";
        std::cout << "1000 % 7 = " << mod << "\n";

        std::cout << std::boolalpha;
        std::cout << "Is 1000 > 7? " << ( a > b ) << "\n";

        std::cout << "\n";
    }

    //=====================================================================
    // 3. Decimal: Basic construction and formatting
    //=====================================================================
    {
        std::cout << "3. Decimal: Basic construction and formatting\n";
        std::cout << "------------------------------------------------\n";

        Decimal price{ "999.99" };
        Decimal qty{ 1000000 };
        Decimal rate{ "0.0825" };

        std::cout << "Price: " << price << "\n";
        std::cout << "Quantity: " << qty << "\n";
        std::cout << "Rate: " << rate << "\n";

        std::cout << "\n";
    }

    //=====================================================================
    // 4. Decimal: Exact arithmetic, rounding and scale
    //=====================================================================
    {
        std::cout << "4. Decimal: Exact arithmetic, rounding and scale\n";
        std::cout << "---------------------------------------------------\n";

        Decimal price{ "19.95" };
        Decimal quantity{ "3" };
        auto subtotal = price * quantity;
        auto tax = subtotal * Decimal{ "0.0825" };
        auto total = subtotal + tax;

        std::cout << "Subtotal: " << subtotal << "\n";
        std::cout << "Tax: " << tax << "\n";
        std::cout << "Total: " << total << "\n";

        Decimal unrounded{ "123.456789" };
        auto rounded2 = unrounded.round( 2 );
        auto truncated = unrounded.trunc();

        std::cout << std::defaultfloat;
        std::cout << "Original: " << unrounded << "\n";
        std::cout << "Rounded (2 dp): " << rounded2 << "\n";
        std::cout << "Truncated: " << truncated << "\n";

        std::cout << "Scale of price: " << static_cast<int>( price.scale() ) << "\n";

        std::cout << "\n";
    }

    //=====================================================================
    // 5. Advanced: Compound interest (demonstration)
    //=====================================================================
    {
        std::cout << "5. Advanced: Compound interest (demonstration)\n";
        std::cout << "--------------------------------------------------\n";

        Decimal principal{ "10000.00" };
        Decimal annualRate{ "0.05" };
        Decimal n{ "12" }; // periods per year
        Decimal t{ "10" }; // years

        auto ratePerPeriod = annualRate / n;
        auto totalPeriods = n * t;
        auto base = Decimal{ "1.0" } + ratePerPeriod;

        std::cout << "Principal: " << principal << "\n";
        std::cout << "Rate per period: " << ratePerPeriod << "\n";
        std::cout << "Periods: " << totalPeriods << "\n";
        std::cout << "Base: " << base << "\n";

        std::cout << "Note: full exponentiation using Decimal is left as an exercise; this demonstrates precision-friendly steps.\n";
        std::cout << "\n";
    }

    //=====================================================================
    // 6. Integration: Int128 factorial and Fibonacci
    //=====================================================================
    {
        std::cout << "6. Integration: Int128 factorial and Fibonacci\n";
        std::cout << "------------------------------------------------\n";

        Int128 factN{ 20 };
        Int128 fact{ 1 };
        for ( Int128 i{ 1 }; i <= factN; i = i + Int128{ 1 } )
            fact = fact * i;
        std::cout << "20! = " << fact << "\n";

        Int128 f0{ 0 }, f1{ 1 };
        std::cout << "Fibonacci (first 15):\n";
        std::cout << "F(0) = " << f0 << "\n";
        std::cout << "F(1) = " << f1 << "\n";
        for ( int i = 2; i < 15; ++i )
        {
            auto next = f0 + f1;
            std::cout << "F(" << i << ") = " << next << "\n";
            f0 = f1;
            f1 = next;
        }

        std::cout << "\n";
    }

    //=====================================================================
    // 7. Financial precision: Decimal vs double
    //=====================================================================
    {
        std::cout << "7. Financial precision: Decimal vs double\n";
        std::cout << "--------------------------------------------\n";

        double dPrice{ 0.1 }, dQty{ 3.0 };
        double dResult = dPrice * dQty;

        Decimal decPrice{ "0.1" };
        Decimal decQty{ "3" };
        auto decResult = decPrice * decQty;

        std::cout << std::fixed << std::setprecision( 17 );
        std::cout << "double: 0.1 * 3 = " << dResult << "\n";
        std::cout << std::defaultfloat; // Reset stream formatting
        std::cout << "Decimal: 0.1 * 3 = " << decResult.toString() << "\n";
        std::cout << std::boolalpha;
        std::cout << "Decimal exact == 0.3 ? " << ( decResult == Decimal{ "0.3" } ) << "\n";

        std::cout << "\n";
    }

    //=====================================================================
    // 8. Edge cases: Very large Int128 values
    //=====================================================================
    {
        std::cout << "8. Edge cases: Very large Int128 values\n";
        std::cout << "------------------------------------------\n";

        // Maximum Int128 value (2^127 - 1)
        Int128 maxInt128{ "170141183460469231731687303715884105727" };
        std::cout << "Max Int128: " << maxInt128 << "\n";

        // Minimum Int128 value (-2^127)
        Int128 minInt128{ "-170141183460469231731687303715884105728" };
        std::cout << "Min Int128: " << minInt128 << "\n";

        // Large arithmetic
        Int128 huge1{ "123456789012345678901234567890" };
        Int128 huge2{ "987654321098765432109876543210" };
        auto hugeSum = huge1 + huge2;
        auto hugeProd = huge1 * Int128{ 1000 };

        std::cout << "Huge + Huge: " << hugeSum << "\n";
        std::cout << "Huge * 1000: " << hugeProd << "\n";

        // Square root of large numbers
        Int128 bigSquare{ "10000000000000000" };
        auto sqrtResult = bigSquare.isqrt();
        std::cout << "isqrt(10^16): " << sqrtResult << "\n";
        std::cout << "Verification: " << sqrtResult << "² = " << ( sqrtResult * sqrtResult ) << "\n";

        std::cout << "\n";
    }

    //=====================================================================
    // 9. Edge cases: Maximum precision Decimal values
    //=====================================================================
    {
        std::cout << "9. Edge cases: Maximum precision Decimal values\n";
        std::cout << "--------------------------------------------------\n";

        // Maximum Decimal value (96-bit mantissa)
        Decimal maxDecimal{ "79228162514264337593543950335" };
        std::cout << "Max Decimal: " << maxDecimal << "\n";

        // Minimum Decimal value
        Decimal minDecimal{ "-79228162514264337593543950335" };
        std::cout << "Min Decimal: " << minDecimal << "\n";

        // Maximum precision (28 digits total, scale up to 28)
        Decimal maxPrecision{ "0.1234567890123456789012345678" };
        std::cout << "Max precision (28 digits): " << maxPrecision << "\n";
        std::cout << "Scale: " << static_cast<int>( maxPrecision.scale() ) << "\n";

        // Very small value
        Decimal verySmall{ "0.0000000000000000000000000001" };
        std::cout << "Very small (10^-28): " << verySmall << "\n";

        // Complex calculation with high precision
        Decimal pi{ "3.1415926535897932384626433832" };
        Decimal e{ "2.7182818284590452353602874713" };
        auto piPlusE = pi + e;
        auto piTimesE = pi * e;

        std::cout << "π ≈ " << pi << "\n";
        std::cout << "e ≈ " << e << "\n";
        std::cout << "π + e ≈ " << piPlusE << "\n";
        std::cout << "π × e ≈ " << piTimesE << "\n";

        std::cout << "\n";
    }

    //=====================================================================
    // 10. Edge cases: Rounding modes with extreme values
    //=====================================================================
    {
        std::cout << "10. Edge cases: Rounding modes with extreme values\n";
        std::cout << "-----------------------------------------------------\n";

        Decimal testValue{ "123456789012345678901234.56789" };
        std::cout << "Original: " << testValue << "\n";

        auto rounded = testValue.round( 2 );
        auto floored = testValue.floor();
        auto ceiled = testValue.ceil();
        auto truncated = testValue.trunc();

        std::cout << "Rounded (2 dp): " << rounded << "\n";
        std::cout << "Floor: " << floored << "\n";
        std::cout << "Ceiling: " << ceiled << "\n";
        std::cout << "Truncate: " << truncated << "\n";

        // Banker's rounding demonstration
        Decimal halfDown{ "2.5" };
        Decimal halfUp{ "3.5" };
        std::cout << "Banker's rounding:\n";
        std::cout << "  2.5 → " << halfDown.round( 0 ) << " (round to even)\n";
        std::cout << "  3.5 → " << halfUp.round( 0 ) << " (round to even)\n";

        // Alternative rounding mode
        auto halfAway = halfDown.round( 0, Decimal::RoundingMode::ToNearestTiesAway );
        std::cout << "  2.5 → " << halfAway << " (ties away from zero)\n";

        std::cout << "\n";
    }

    //=====================================================================
    // 11. Integration: Int128 ↔ Decimal conversions with edge cases
    //=====================================================================
    {
        std::cout << "11. Integration: Int128 ↔ Decimal conversions with edge cases\n";
        std::cout << "----------------------------------------------------------------\n";

        // Int128 to Decimal (within range)
        Int128 largeInt{ "12345678901234567890123456" };
        Decimal fromInt{ largeInt };
        std::cout << "Int128: " << largeInt << "\n";
        std::cout << "As Decimal: " << fromInt << "\n";

        // Int128 to Decimal (overflow - will throw)
        Int128 tooLarge{ "170141183460469231731687303715884105727" };
        try
        {
            Decimal overflowed{ tooLarge };
            std::cout << "Int128 (too large): " << tooLarge << "\n";
            std::cout << "Converted to Decimal: " << overflowed << "\n";
        }
        catch ( const std::overflow_error& e )
        {
            std::cout << "Int128 (too large): " << tooLarge << "\n";
            std::cout << "Overflow exception: " << e.what() << "\n";
        }

        // Decimal with fractional part to Int128 (truncates)
        Decimal withFraction{ "123456789.987654321" };
        Int128 truncatedInt{ withFraction };
        std::cout << "Decimal: " << withFraction << "\n";
        std::cout << "As Int128 (truncated): " << truncatedInt << "\n";

        // High-precision square root comparison
        Int128 perfect{ "144" };
        auto intSqrt = perfect.isqrt();
        auto decSqrt = sqrt( perfect ); // Returns Decimal with high precision
        std::cout << "sqrt(144) as Int128: " << intSqrt << "\n";
        std::cout << "sqrt(144) as Decimal: " << decSqrt << "\n";

        Int128 notPerfect{ "150" };
        auto notPerfectSqrt = sqrt( notPerfect );
        std::cout << "sqrt(150) as Decimal: " << notPerfectSqrt << "\n";

        std::cout << "\n";
    }

    return 0;
}
