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
///\name Test macros
///\{
#define STD_TEST_DO(expr, do_expr) \
	{ \
		if ( !(expr) ) { \
			stdtest::Report report(__FILE__, __LINE__, __FUNCTION__, #expr); \
			report.print(); \
			\
			{ \
				do_expr; \
			} \
		} \
	}
	///< Test, do expression
#define STD_TEST(expr) \
	STD_TEST_DO(expr, {})
	///< Tets only
#define STD_TEST_RET(expr, ret_expr) \
	STD_TEST_DO(expr, return ret_expr)
	///< Test, return expression
#define STD_TEST_FAIL(expr) \
	STD_TEST_DO(!(expr), {}
	///< Test fail expression
#define STD_TEST_PTR(ptr) \
	STD_TEST_DO((ptr) != nullptr, {})
	///< Test pointer
#define STD_TEST_NA(expr) \
	;
	///< Test not applicable (skip)
#define STD_NOT_IMPLEMENTED \
	STD_TEST(false && "Not implemented")
	///< Show not implemented message and return value
///\}

#define STD_UNUSED(arg) \
	{ static_cast<void>( ((true) ? (arg) : (arg)) ); }
	///< Hide "unused variable" warnings
//-------------------------------------------------------------------------------------------------
namespace stdtest
{

//-------------------------------------------------------------------------------------------------
class Report final
	/// Report to std::cout
{
public:
///\name ctors, dtor
///\{
	Report() = delete;
	Report(const Report &) = delete;
	Report(Report &&) = delete;

	Report(const char *filePath, const long int fileLine, const char *functionName,
				const char *expression);
	~Report() = default;

	Report & operator = (const Report &) = delete;
	Report & operator = (Report &&) = delete;
///\}

	void print() const;
		///< print to std::cout

private:
	const char     *_filePath {};
	const long int  _fileLine {};
	const char     *_functionName {};
	const char     *_expression {};

	std::string _currentDateTime() const;
		///< current date/time, format is %Y-%m-%d.%X
	std::string _modulePath() const;
		///< Get module path
};
//-------------------------------------------------------------------------------------------------

} // stdtest
//-------------------------------------------------------------------------------------------------
#include "StdTest.inl"
