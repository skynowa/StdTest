/**
 * \file  StdTest.h
 * \brief test, using C library and STL
 */


#pragma once

#if   defined(_WIN32)
    // n/a
#elif defined(__linux__)
    #include <unistd.h>
    #include <linux/limits.h>
#elif defined(__FreeBSD__)
    #include <sys/sysctl.h>
#elif defined(__APPLE__)
    // n/a
#else
    #warning StdTest: unsupported OS environment
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
#define STD_TEST_DO(expr, do_expr) \
    { \
        if ( !(expr) ) { \
            stdtest::printReport(__FILE__, __LINE__, __FUNCTION__, #expr); \
            \
            { \
                do_expr; \
            } \
        } \
    }
	///<
#define STD_TEST(expr) \
    STD_TEST_DO(expr, {})
	///<
#define STD_TEST_RET(expr, ret_expr) \
    STD_TEST_DO(expr, return ret_expr)
	///<
#define STD_TEST_FAIL(expr) \
    STD_TEST_DO(!(expr), {})
	///<
#define STD_TEST_PTR(ptr) \
    STD_TEST_DO(ptr != nullptr, {})
	///<
#define STD_TEST_NA(expr) \
    ;
	///<
#define STD_NOT_IMPLEMENTED \
    STD_TEST(false && "Not implemented")
    ///< show not implemented message and return value

#define STD_UNUSED(arg) \
	{ static_cast<void>( ((true) ? (arg) : (arg)) ); }
	///< hide "unused variable" warnings
//-------------------------------------------------------------------------------------------------
namespace stdtest
{
std::string currentDateTime();
    ///< current date/time, format is %Y-%m-%d.%X
std::string modulePath();
    ///< Get module path
void        printReport(const char *filePath, const int fileLine, const char *functionName,
				const char *expression);
	///< print report message

} // stdtest
//-------------------------------------------------------------------------------------------------
#include "StdTest.inl"
