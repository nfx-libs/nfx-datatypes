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

#pragma once

//----------------------------------------------
// Cross-platform 128-bit integer support
//----------------------------------------------

/**
 * @brief Cross-platform 128-bit integer support detection
 * @details Detects native __int128 support for high-performance decimal arithmetic.
 *          - GCC/Clang: Native __int128 support since GCC 4.6+ and Clang 3.1+
 *          - MSVC: No native 128-bit support, requires manual implementation
 */
#if defined( __SIZEOF_INT128__ ) && !defined( _MSC_VER )
// GCC and Clang have native __int128 support
#    define NFX_DATATYPES_HAS_NATIVE_INT128 1
#    define NFX_DATATYPES_NATIVE_INT128 __int128
#else
// MSVC and other compilers without native 128-bit support
#    define NFX_DATATYPES_HAS_NATIVE_INT128 0
// For manual 128-bit implementation, we'll use our custom Int128 struct
#endif

/** @brief Conditional compilation helper for 128-bit specific code paths */
#if NFX_DATATYPES_HAS_NATIVE_INT128
/** @brief Includes code only when native 128-bit integer support is available */
#    define NFX_DATATYPES_IF_INT128( code ) code
/** @brief Includes code only when native 128-bit integer support is NOT available */
#    define NFX_DATATYPES_IF_NO_INT128( code )
#else
/** @brief Includes code only when native 128-bit integer support is available */
#    define NFX_DATATYPES_IF_INT128( code )
/** @brief Includes code only when native 128-bit integer support is NOT available */
#    define NFX_DATATYPES_IF_NO_INT128( code ) code
#endif
