#ifndef __WIN32LETTERCONVERT_HPP__
#define __WIN32LETTERCONVERT_HPP__
#ifdef WIN32LETTERCONVERT_EXPORTS
#define WIN32LETTER __declspec(dllexport)
#else
#define WIN32LETTER __declspec(dllimport)
#endif
#include <string>
#include <tchar.h>

WIN32LETTER std::string WStringToString(const std::wstring str);
WIN32LETTER std::wstring StringToWString(const std::string str);
#endif
