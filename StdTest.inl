/**
 * \file  StdTest.inl
 * \brief test, using C library and STL
 */


namespace stdtest
{

//-------------------------------------------------------------------------------------------------
Report::Report(
	const char     *a_filePath,   	///< file path
	const long int  a_fileLine,   	///< file line number
	const char     *a_functionName,	///< function name
	const char     *a_expression  	///< expression as string
) :
	_filePath     {a_filePath},
	_fileLine     {a_fileLine},
	_functionName {a_functionName},
	_expression   {a_expression}
{
}
//-------------------------------------------------------------------------------------------------
inline void
Report::print() const
{
	const int          errorCode = errno; // Last error - must be called 1-st !!!
	const std::string &errorMsg  = std::strerror(errorCode);
	const std::string &module    = _modulePath();
	const std::string &dateTime  = _currentDateTime();

    std::cout
        << "\n"
        << "-------------------- STD_TEST --------------------" << "\n"
        << " Module:     " << module                            << "\n"
        << " Expression: " << _expression                       << "\n"
        << " File:       " << _filePath                         << "\n"
        << " Function:   " << _functionName                     << "\n"
        << " Line:       " << _fileLine                         << "\n"
        << " Last error: " << errorCode << " - " << errorMsg    << "\n"
        << " Date time:  " << dateTime                          << "\n"
        << "--------------------------------------------------" << std::endl;
}
//-------------------------------------------------------------------------------------------------
inline std::string
Report::_currentDateTime() const
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
Report::_modulePath() const
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
	auto           buffSize = static_cast<std::uint32_t>(PATH_MAX);
	std::tstring_t buff(buffSize + 1, {});

	int_t iRv = ::_NSGetExecutablePath(&buff[0], &buffSize);
	if (iRv != 0) {
		buff.resize(buffSize);

		iRv = ::_NSGetExecutablePath(&buff[0], &buffSize);
		if (iRv != 0) {
			return "<unknown>";
		}
	}

	sRv = buff.c_str();	// Trim '\0'
#else
    sRv = "<unknown>";
#endif

    return sRv;
}
//-------------------------------------------------------------------------------------------------

} // stdtest
