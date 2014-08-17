/*
 * Copyright (C) 2008 Apple Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

/*
* This file was modified by Electronic Arts Inc Copyright © 2009-2010
*/

#ifndef WTF_TypeTraits_h
#define WTF_TypeTraits_h

// We define type traits that allow us to optimize templated array new/delete.
// These type traits match the C++0x working standard defined in section 20.4.2.
// The types (integral_constant, true_type, etc.) defined here are based on 
// the C++0x type_traits types and use the same naming and conventions. 
// Recent versions of some compilers and standard libraries have support for type traits.

#if (defined(__GLIBCXX__) && (__GLIBCXX__ >= 20070724)) || (defined(_MSC_VER) && (_MSC_VER >= 1600))

// GCC's libstdc++ 20070724 and later and VC10 (VS2010) and later support C++ TR1 type_traits.
#include <type_traits>

namespace WTF {
	using std::tr1::is_integral;
    using std::tr1::has_trivial_constructor;
    using std::tr1::has_trivial_destructor;
}

#else

namespace WTF {

    // This compiler doesn't provide type traits, so we provide a set.
    template<typename T> struct is_integral { static const bool value = false; };

    template<> struct is_integral<bool> { static const bool value = true; };
    template<> struct is_integral<const bool> { static const bool value = true; };
    template<> struct is_integral<volatile bool> { static const bool value = true; };
    template<> struct is_integral<const volatile bool> { static const bool value = true; };

    template<> struct is_integral<char> { static const bool value = true; };
    template<> struct is_integral<const char> { static const bool value = true; };
    template<> struct is_integral<volatile char> { static const bool value = true; };
    template<> struct is_integral<const volatile char> { static const bool value = true; };

    template<> struct is_integral<signed char> { static const bool value = true; };
    template<> struct is_integral<const signed char> { static const bool value = true; };
    template<> struct is_integral<volatile signed char> { static const bool value = true; };
    template<> struct is_integral<const volatile signed char> { static const bool value = true; };

    template<> struct is_integral<unsigned char> { static const bool value = true; };
    template<> struct is_integral<const unsigned char> { static const bool value = true; };
    template<> struct is_integral<volatile unsigned char> { static const bool value = true; };
    template<> struct is_integral<const volatile unsigned char> { static const bool value = true; };

    template<> struct is_integral<short> { static const bool value = true; };
    template<> struct is_integral<const short> { static const bool value = true; };
    template<> struct is_integral<volatile short> { static const bool value = true; };
    template<> struct is_integral<const volatile short> { static const bool value = true; };

    template<> struct is_integral<unsigned short> { static const bool value = true; };
    template<> struct is_integral<const unsigned short> { static const bool value = true; };
    template<> struct is_integral<volatile unsigned short> { static const bool value = true; };
    template<> struct is_integral<const volatile unsigned short> { static const bool value = true; };

    template<> struct is_integral<int> { static const bool value = true; };
    template<> struct is_integral<const int> { static const bool value = true; };
    template<> struct is_integral<volatile int> { static const bool value = true; };
    template<> struct is_integral<const volatile int> { static const bool value = true; };

    template<> struct is_integral<unsigned int> { static const bool value = true; };
    template<> struct is_integral<const unsigned int> { static const bool value = true; };
    template<> struct is_integral<volatile unsigned int> { static const bool value = true; };
    template<> struct is_integral<const volatile unsigned int> { static const bool value = true; };

    template<> struct is_integral<long> { static const bool value = true; };
    template<> struct is_integral<const long> { static const bool value = true; };
    template<> struct is_integral<volatile long> { static const bool value = true; };
    template<> struct is_integral<const volatile long> { static const bool value = true; };

    template<> struct is_integral<unsigned long> { static const bool value = true; };
    template<> struct is_integral<const unsigned long> { static const bool value = true; };
    template<> struct is_integral<volatile unsigned long> { static const bool value = true; };
    template<> struct is_integral<const volatile unsigned long> { static const bool value = true; };

    template<> struct is_integral<long long> { static const bool value = true; };
    template<> struct is_integral<const long long> { static const bool value = true; };
    template<> struct is_integral<volatile long long> { static const bool value = true; };
    template<> struct is_integral<const volatile long long> { static const bool value = true; };

    template<> struct is_integral<unsigned long long> { static const bool value = true; };
    template<> struct is_integral<const unsigned long long> { static const bool value = true; };
    template<> struct is_integral<volatile unsigned long long> { static const bool value = true; };
    template<> struct is_integral<const volatile unsigned long long> { static const bool value = true; };

#if !defined(_MSC_VER) || defined(_NATIVE_WCHAR_T_DEFINED)
    template<> struct is_integral<wchar_t> { static const bool value = true; };
    template<> struct is_integral<const wchar_t> { static const bool value = true; };
    template<> struct is_integral<volatile wchar_t> { static const bool value = true; };
    template<> struct is_integral<const volatile wchar_t> { static const bool value = true; };
#endif


    // This compiler doesn't provide type traits, so we provide a basic has_trivial_constructor
    // and has_trivial_destructor definition. The definitions here include most built-in 
    // scalar types but do not include POD structs and classes. For the intended purposes of 
    // type_traits this results correct but potentially less efficient code.
    template <typename T, T v>
    struct integral_constant {
        static const T value = v;
        typedef T value_type;
        typedef integral_constant<T, v> type;
    };

    typedef integral_constant<bool, true>  true_type;
    typedef integral_constant<bool, false> false_type;

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
    // VC8 (VS2005) and later have built-in compiler support for has_trivial_constructor / has_trivial_destructor, 
    // but for some unexplained reason it doesn't work on built-in types.
    template <typename T> struct has_trivial_constructor : public integral_constant<bool, __has_trivial_constructor(T)> { };
    template <typename T> struct has_trivial_destructor : public integral_constant<bool, __has_trivial_destructor(T)> { };
#else
    template <typename T> struct has_trivial_constructor : public false_type { };
    template <typename T> struct has_trivial_destructor : public false_type { };
#endif

    template <typename T> struct has_trivial_constructor<T*> : public true_type { };
    template <typename T> struct has_trivial_destructor<T*> : public true_type { };

    template <> struct has_trivial_constructor<float> : public true_type { };
    template <> struct has_trivial_constructor<const float> : public true_type { };
    template <> struct has_trivial_constructor<volatile float> : public true_type { };
    template <> struct has_trivial_constructor<const volatile float> : public true_type { };

    template <> struct has_trivial_constructor<double> : public true_type { };
    template <> struct has_trivial_constructor<const double> : public true_type { };
    template <> struct has_trivial_constructor<volatile double> : public true_type { };
    template <> struct has_trivial_constructor<const volatile double> : public true_type { };

    template <> struct has_trivial_constructor<long double> : public true_type { };
    template <> struct has_trivial_constructor<const long double> : public true_type { };
    template <> struct has_trivial_constructor<volatile long double> : public true_type { };
    template <> struct has_trivial_constructor<const volatile long double> : public true_type { };

    template <> struct has_trivial_constructor<unsigned char> : public true_type { };
    template <> struct has_trivial_constructor<const unsigned char> : public true_type { };
    template <> struct has_trivial_constructor<volatile unsigned char> : public true_type { };
    template <> struct has_trivial_constructor<const volatile unsigned char> : public true_type { };

    template <> struct has_trivial_constructor<unsigned short> : public true_type { };
    template <> struct has_trivial_constructor<const unsigned short> : public true_type { };
    template <> struct has_trivial_constructor<volatile unsigned short> : public true_type { };
    template <> struct has_trivial_constructor<const volatile unsigned short> : public true_type { };

    template <> struct has_trivial_constructor<unsigned int> : public true_type { };
    template <> struct has_trivial_constructor<const unsigned int> : public true_type { };
    template <> struct has_trivial_constructor<volatile unsigned int> : public true_type { };
    template <> struct has_trivial_constructor<const volatile unsigned int> : public true_type { };

    template <> struct has_trivial_constructor<unsigned long> : public true_type { };
    template <> struct has_trivial_constructor<const unsigned long> : public true_type { };
    template <> struct has_trivial_constructor<volatile unsigned long> : public true_type { };
    template <> struct has_trivial_constructor<const volatile unsigned long> : public true_type { };

    template <> struct has_trivial_constructor<unsigned long long> : public true_type { };
    template <> struct has_trivial_constructor<const unsigned long long> : public true_type { };
    template <> struct has_trivial_constructor<volatile unsigned long long> : public true_type { };
    template <> struct has_trivial_constructor<const volatile unsigned long long> : public true_type { };

    template <> struct has_trivial_constructor<signed char> : public true_type { };
    template <> struct has_trivial_constructor<const signed char> : public true_type { };
    template <> struct has_trivial_constructor<volatile signed char> : public true_type { };
    template <> struct has_trivial_constructor<const volatile signed char> : public true_type { };

    template <> struct has_trivial_constructor<signed short> : public true_type { };
    template <> struct has_trivial_constructor<const signed short> : public true_type { };
    template <> struct has_trivial_constructor<volatile signed short> : public true_type { };
    template <> struct has_trivial_constructor<const volatile signed short> : public true_type { };

    template <> struct has_trivial_constructor<signed int> : public true_type { };
    template <> struct has_trivial_constructor<const signed int> : public true_type { };
    template <> struct has_trivial_constructor<volatile signed int> : public true_type { };
    template <> struct has_trivial_constructor<const volatile signed int> : public true_type { };

    template <> struct has_trivial_constructor<signed long> : public true_type { };
    template <> struct has_trivial_constructor<const signed long> : public true_type { };
    template <> struct has_trivial_constructor<volatile signed long> : public true_type { };
    template <> struct has_trivial_constructor<const volatile signed long> : public true_type { };

    template <> struct has_trivial_constructor<signed long long> : public true_type { };
    template <> struct has_trivial_constructor<const signed long long> : public true_type { };
    template <> struct has_trivial_constructor<volatile signed long long> : public true_type { };
    template <> struct has_trivial_constructor<const volatile signed long long> : public true_type { };

    template <> struct has_trivial_constructor<bool> : public true_type { };
    template <> struct has_trivial_constructor<const bool> : public true_type { };
    template <> struct has_trivial_constructor<volatile bool> : public true_type { };
    template <> struct has_trivial_constructor<const volatile bool> : public true_type { };

    template <> struct has_trivial_constructor<char> : public true_type { };
    template <> struct has_trivial_constructor<const char> : public true_type { };
    template <> struct has_trivial_constructor<volatile char> : public true_type { };
    template <> struct has_trivial_constructor<const volatile char> : public true_type { };

    #if !defined(_MSC_VER) || defined(_NATIVE_WCHAR_T_DEFINED)
        template <> struct has_trivial_constructor<wchar_t> : public true_type { };
        template <> struct has_trivial_constructor<const wchar_t> : public true_type { };
        template <> struct has_trivial_constructor<volatile wchar_t> : public true_type { };
        template <> struct has_trivial_constructor<const volatile wchar_t> : public true_type { };
    #endif


    template <> struct has_trivial_destructor<float> : public true_type { };
    template <> struct has_trivial_destructor<const float> : public true_type { };
    template <> struct has_trivial_destructor<volatile float> : public true_type { };
    template <> struct has_trivial_destructor<const volatile float> : public true_type { };

    template <> struct has_trivial_destructor<double> : public true_type { };
    template <> struct has_trivial_destructor<const double> : public true_type { };
    template <> struct has_trivial_destructor<volatile double> : public true_type { };
    template <> struct has_trivial_destructor<const volatile double> : public true_type { };

    template <> struct has_trivial_destructor<long double> : public true_type { };
    template <> struct has_trivial_destructor<const long double> : public true_type { };
    template <> struct has_trivial_destructor<volatile long double> : public true_type { };
    template <> struct has_trivial_destructor<const volatile long double> : public true_type { };

    template <> struct has_trivial_destructor<unsigned char> : public true_type { };
    template <> struct has_trivial_destructor<const unsigned char> : public true_type { };
    template <> struct has_trivial_destructor<volatile unsigned char> : public true_type { };
    template <> struct has_trivial_destructor<const volatile unsigned char> : public true_type { };

    template <> struct has_trivial_destructor<unsigned short> : public true_type { };
    template <> struct has_trivial_destructor<const unsigned short> : public true_type { };
    template <> struct has_trivial_destructor<volatile unsigned short> : public true_type { };
    template <> struct has_trivial_destructor<const volatile unsigned short> : public true_type { };

    template <> struct has_trivial_destructor<unsigned int> : public true_type { };
    template <> struct has_trivial_destructor<const unsigned int> : public true_type { };
    template <> struct has_trivial_destructor<volatile unsigned int> : public true_type { };
    template <> struct has_trivial_destructor<const volatile unsigned int> : public true_type { };

    template <> struct has_trivial_destructor<unsigned long> : public true_type { };
    template <> struct has_trivial_destructor<const unsigned long> : public true_type { };
    template <> struct has_trivial_destructor<volatile unsigned long> : public true_type { };
    template <> struct has_trivial_destructor<const volatile unsigned long> : public true_type { };

    template <> struct has_trivial_destructor<unsigned long long> : public true_type { };
    template <> struct has_trivial_destructor<const unsigned long long> : public true_type { };
    template <> struct has_trivial_destructor<volatile unsigned long long> : public true_type { };
    template <> struct has_trivial_destructor<const volatile unsigned long long> : public true_type { };

    template <> struct has_trivial_destructor<signed char> : public true_type { };
    template <> struct has_trivial_destructor<const signed char> : public true_type { };
    template <> struct has_trivial_destructor<volatile signed char> : public true_type { };
    template <> struct has_trivial_destructor<const volatile signed char> : public true_type { };

    template <> struct has_trivial_destructor<signed short> : public true_type { };
    template <> struct has_trivial_destructor<const signed short> : public true_type { };
    template <> struct has_trivial_destructor<volatile signed short> : public true_type { };
    template <> struct has_trivial_destructor<const volatile signed short> : public true_type { };

    template <> struct has_trivial_destructor<signed int> : public true_type { };
    template <> struct has_trivial_destructor<const signed int> : public true_type { };
    template <> struct has_trivial_destructor<volatile signed int> : public true_type { };
    template <> struct has_trivial_destructor<const volatile signed int> : public true_type { };

    template <> struct has_trivial_destructor<signed long> : public true_type { };
    template <> struct has_trivial_destructor<const signed long> : public true_type { };
    template <> struct has_trivial_destructor<volatile signed long> : public true_type { };
    template <> struct has_trivial_destructor<const volatile signed long> : public true_type { };

    template <> struct has_trivial_destructor<signed long long> : public true_type { };
    template <> struct has_trivial_destructor<const signed long long> : public true_type { };
    template <> struct has_trivial_destructor<volatile signed long long> : public true_type { };
    template <> struct has_trivial_destructor<const volatile signed long long> : public true_type { };

    template <> struct has_trivial_destructor<bool> : public true_type { };
    template <> struct has_trivial_destructor<const bool> : public true_type { };
    template <> struct has_trivial_destructor<volatile bool> : public true_type { };
    template <> struct has_trivial_destructor<const volatile bool> : public true_type { };

    template <> struct has_trivial_destructor<char> : public true_type { };
    template <> struct has_trivial_destructor<const char> : public true_type { };
    template <> struct has_trivial_destructor<volatile char> : public true_type { };
    template <> struct has_trivial_destructor<const volatile char> : public true_type { };

    #if !defined(_MSC_VER) || defined(_NATIVE_WCHAR_T_DEFINED)
        template <> struct has_trivial_destructor<wchar_t> : public true_type { };
        template <> struct has_trivial_destructor<const wchar_t> : public true_type { };
        template <> struct has_trivial_destructor<volatile wchar_t> : public true_type { };
        template <> struct has_trivial_destructor<const volatile wchar_t> : public true_type { };
    #endif

} // namespace WTF


#endif  // __GLIBCXX__, etc.



#endif // WTF_TypeTraits_h
