/**
 * \file  StdTest.inl
 * \brief test, using C library and STL
 */

namespace stdtest
{

//-------------------------------------------------------------------------------------------------
inline std::string
currentDateTime()
{
    std::string sRv;

    constexpr std::size_t buffSize           {80};
    char                  buff[buffSize + 1] {};

    const time_t timeNow = std::time(nullptr);

    const std::tm *timeLocal = std::localtime(&timeNow);
    if (timeLocal == nullptr) {
        return {};
    }

    size_t uiRv = std::strftime(buff, buffSize, "%Y-%m-%d.%X", timeLocal);
    if (uiRv == 0) {
        return {};
    }

    sRv.assign(&buff[0], uiRv);

    return sRv;
}
//-------------------------------------------------------------------------------------------------
inline std::string
modulePath()
{
    std::string sRv;

#if   defined(_WIN32)
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
		std::find(filePath.crbegin(), filePath.crend(), '/').base(),
		filePath.cend());
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
inline void
printReport(
	const char *filePath,		///< file path
	const int   fileLine,		///< file line number
	const char *functionName,	///< function name
	const char *expression		///< expression as string
)
{
    std::cout
        << "\n"
        << "-------------------- STD_TEST --------------------" << "\n"
        << " Module:     " << modulePath()                      << "\n"
        << " Expression: " << expression                        << "\n"
        << " File:       " << filePath                          << "\n"
        << " Function:   " << functionName                      << "\n"
        << " Line:       " << fileLine                          << "\n"
        << " Last error: " << std::strerror(errno)              << "\n"
        << " Date time:  " << currentDateTime()                 << "\n"
        << "--------------------------------------------------" << std::endl;
}
//-------------------------------------------------------------------------------------------------

} // stdtest
