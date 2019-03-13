/**
 * \file  StdTest.h
 * \brief test, using C library and STL
 */


#pragma once

// FreeBSD
#if defined(__FreeBSD__)
    #include <sys/sysctl.h>
#endif

// C library
#include <cerrno>
#include <ctime>
#include <cstring>

// STL
#include <string>
#include <iostream>
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
#define STD_NOT_IMPLEMENTED \
    { \
        STD_TEST(false && "Not implemented"); \
    }
    ///< show not implemented message and return value
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
    ///< test macros
#define STD_TEST(expr) \
    STD_TEST_DO(expr, {})
    ///< test macros
#define STD_TEST_RET(expr, ret_expr) \
    STD_TEST_DO(expr, return ret_expr)
    ///< test macros

#define STD_TEST_NA(expr) \
    ;
    ///< test macros
//-------------------------------------------------------------------------------------------------


namespace stdtest
{

//-------------------------------------------------------------------------------------------------
/**
 * current date/time, format is %Y-%m-%d.%X
 */
inline std::string
currentDateTime()
{
    std::string sRv;
    char        buff[80 + 1] {};
    time_t      timeNow      {std::time(nullptr)};
    std::tm    *timeLocal    {};

    timeLocal = std::localtime(&timeNow);
    if (timeLocal == nullptr) {
        return {};
    }

    size_t uiRv = std::strftime(buff, sizeof(buff) - 1, "%Y-%m-%d.%X", timeLocal);
    if (uiRv == 0) {
        return {};
    }

    sRv.assign(&buff[0], uiRv);

    return sRv;
}
//-------------------------------------------------------------------------------------------------
/**
 * Get module path
 */
inline std::string
modulePath()
{
    std::string srv;

#if defined(KERN_PROC_PATHNAME)
    const u_int mibSize            {4};
    int         mib[mibSize]       {CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, - 1};
    char        buff[PATH_MAX + 1] {};
    std::size_t buffSize           {sizeof(buff) - 1};

    int iRv = ::sysctl(mib, mibSize, buff, &buffSize, nuulptr, 0U);
    if (iRv == - 1) {
        return "<unknown>";
    }

    srv.assign(buff);
#else
    srv = "<unknown>";
#endif

    return srv;
}
//-------------------------------------------------------------------------------------------------

} // stdtest
