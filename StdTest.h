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
    std::string sRv;

#if   defined(_MSC_VER)
    sRv.resize(MAX_PATH);

    DWORD stored = ::GetModuleFileName(nullptr, &sRv.at(0), static_cast<DWORD>( sRv.size() ));
    if (stored == 0UL) {
        return "<unknown>";
    }

    sRv.resize(stored);
#elif defined(__linux__)
	const std::string procFile = "/proc/" + std::to_string(::getpid()) + "/exe";

	int         readed {- 1};
	std::string filePath(PATH_MAX, '\0');

	for ( ; ; ) {
		readed = ::readlink(procFile.c_str(), &filePath.at(0), filePath.size());
		if (readed == - 1) {
			return "<unknown>";
		}

		if (filePath.size() > static_cast<size_t>(readed)) {
			break;
		}

		filePath.resize(filePath.size() * 2);
	}

	filePath.resize( static_cast<size_t>(readed) );

	sRv.assign(
		std::find(filePath.rbegin(), filePath.rend(), '/').base(),
		filePath.end());
#elif defined(__FreeBSD__)
    constexpr u_int mibSize            {4};
    int             mib[mibSize]       {CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, - 1};
    char            buff[PATH_MAX + 1] {};
    std::size_t     buffSize           {sizeof(buff) - 1};

    int iRv = ::sysctl(mib, mibSize, buff, &buffSize, nulptr, 0U);
    if (iRv == - 1) {
        return "<unknown>";
    }

    sRv.assign(buff);
#elif defined(__APPLE__)
    // TODO: module path
#else
    sRv = "<unknown>";
#endif

    return sRv;
}
//-------------------------------------------------------------------------------------------------

} // stdtest
