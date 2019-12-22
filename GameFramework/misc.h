#pragma once
#include <string>
#include <strstream>

//
//	Define needed for microsoft C to export functions across DLLs
//
#ifdef _MSC_VER
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif


/*
* This little utility formations the specified value to a string
* using the std::stream insertion operator.
*/
template<class T> std::string fmtStr(T vVal)
{
	char _zBuf[1024];
	std::ostrstream vOstr(_zBuf, int(sizeof(_zBuf) - 1));
	vOstr << vVal;
	_zBuf[vOstr.pcount()] = 0;
	return std::string(_zBuf);
}
