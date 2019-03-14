/**
 * \file  StdTest.h
 * \brief test, using C library and STL
 */


#pragma once

#if   defined(_MSC_VER)
#elif defined(__linux__)
    #include <unistd.h>
    #include <linux/limits.h>
#elif defined(__FreeBSD__)
    #include <sys/sysctl.h>
#elif defined(__APPLE__)
#else
#endif

// C library
#include <cerrno>
#include <ctime>
#include <cstring>

// STL
#include <string>
#include <iostream>
#include <algorithm>
//-------------------------------------------------------------------------------------------------
#define STD_TRACE_POINT \
    { \
        std::cout \
            << "\t::: " \
            << " in "      << __FILE__ \
            << ":"         << __LINE__ \
            << ", error: " << std::strerror(errno) \
            << " :::"      << std::endl; \
    }
    ///< trace point
#define STD_TRACE_FUNC \
    { \
        std::cout << "\t::: " << __FUNCTION__ << " :::" << std::endl; \
    }
    ///< trace function
//-------------------------------------------------------------------------------------------------
#define STD_TEST_DO(expr, do_expr) \
    { \
        if ( !(expr) ) { \
            std::cout \
                << "\n" \
                << "-------------------- STD_TEST --------------------" << "\n" \
                << " Module:     " << stdtest::modulePath()             << "\n" \
                << " Expression: " << #expr                             << "\n" \
                << " File:       " << __FILE__                          << "\n" \
                << " Function:   " << __FUNCTION__                      << "\n" \
                << " Line:       " << __LINE__                          << "\n" \
                << " Last error: " << std::strerror(errno)              << "\n" \
                << " Date time:  " << stdtest::currentDateTime()        << "\n" \
                << "--------------------------------------------------" << "\n" \
                << std::flush; \
            { \
                do_expr; \
            } \
        } \
    }

#define STD_TEST(expr) \
    STD_TEST_DO(expr, {})

#define STD_TEST_RET(expr, ret_expr) \
    STD_TEST_DO(expr, return ret_expr)

#define STD_TEST_FAIL(expr) \
    STD_TEST_DO(!(expr), {})

#define STD_TEST_PTR(ptr) \
    STD_TEST_DO(ptr != nullptr, {})

#define STD_TEST_NA(expr) \
    ;

#define STD_NOT_IMPLEMENTED \
    STD_TEST(false && "Not implemented")
    ///< show not implemented message and return value
//-------------------------------------------------------------------------------------------------
namespace stdtest
{

std::string currentDateTime();
    ///< current date/time, format is %Y-%m-%d.%X
std::string modulePath();
    ///< Get module path

} // stdtest
//-------------------------------------------------------------------------------------------------
#include "StdTest.inl"
