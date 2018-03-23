#ifndef __WIN32API_HPP__
#define __WIN32API_HPP__
#ifdef WIN32FUNCTION_EXPORTS
#define WIN32API __declspec(dllexport)
#else
#define WIN32API __declspec(dllimport)
#ifdef _DEBUG
#pragma comment(lib, "KgWin32Func_d.lib")
#else
#pragma comment(lib, "KgWin32Func.lib")
#endif
#endif
#include <string>
#include <vector>
#include <utility>

WIN32API std::string GetExecutionUserNameA();
WIN32API std::string WStringToString(std::wstring oWString);
WIN32API bool FileExists(const std::string FilePath);
WIN32API bool DirectoryExists(const std::string Path);
WIN32API std::vector<std::string> GetFilePathList(const std::string Directory);
WIN32API std::string GetSIDStringA(const std::string ComputerName, const std::string UserName);
WIN32API std::string GetSIDStringA(const std::string UserName);
WIN32API std::string GetSIDStringA();
WIN32API std::string ConvertUserNameToFullName(const std::string UserName);
WIN32API std::string ConvertFullNameToUserName(const std::string FullName);
WIN32API std::vector<std::pair<std::string, std::string>> GetAllUserNameA();
WIN32API std::vector<std::string> GetLocalgroupMembers(const std::string ComputerName, const std::string GroupName);
WIN32API std::vector<std::string> GetLocalgroupMembers(const std::string GroupName);
WIN32API bool IsReservedLocalgroupMembers(const std::string ComputerName, const std::string GroupName, const std::string UserName);
WIN32API bool IsReservedLocalgroupMembers(const std::string GroupName, const std::string UserName);
WIN32API bool IsReservedLocalgroupMembers(const std::string GroupName);

WIN32API std::wstring GetExecutionUserNameW();
WIN32API std::wstring StringToWString(std::string oWString);
WIN32API bool FileExists(const std::wstring FilePath);
WIN32API bool DirectoryExists(const std::wstring Path);
WIN32API std::vector<std::wstring> GetFilePathList(const std::wstring Directory);
WIN32API std::wstring GetSIDStringW(const std::wstring ComputerName, const std::wstring UserName);
WIN32API std::wstring GetSIDStringW(const std::wstring UserName);
WIN32API std::wstring GetSIDStringW();
WIN32API std::wstring ConvertUserNameToFullName(const std::wstring UserName);
WIN32API std::wstring ConvertFullNameToUserName(const std::wstring FullName);
WIN32API std::vector<std::pair<std::wstring, std::wstring>> GetAllUserNameW();
WIN32API std::vector<std::wstring> GetLocalgroupMembers(const std::wstring ComputerName, const std::wstring GroupName);
WIN32API std::vector<std::wstring> GetLocalgroupMembers(const std::wstring GroupName);
WIN32API bool IsReservedLocalgroupMembers(const std::wstring ComputerName, const std::wstring GroupName, const std::wstring UserName);
WIN32API bool IsReservedLocalgorupMembers(const std::wstring GroupName, const std::wstring UserName);
WIN32API bool IsReservedLocalgorupMembers(const std::wstring GroupName);
#endif
