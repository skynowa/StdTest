/**
 * \file  main.cpp
 * \brief
 */


#include <iostream>
#include "StdTest.h"
//-------------------------------------------------------------------------------------------------
int main(int, char **)
{
	//
	{
		const std::size_t value {777};

		STD_TEST_DO(false, std::cout << value << std::endl << std::endl;);
	}

    return EXIT_SUCCESS;
}
//-------------------------------------------------------------------------------------------------


#if OUTPUT



#endif
