#ifndef __KGWINEXCEPTION_HPP__
#define __KGWINEXCEPTION_HPP__
#ifdef EXCEPTION_EXPORTS
#define EXCEPTIONLIB __declspec(dllexport)
#else
#define EXCEPTIONLIB __declspec(dllimport)
#ifdef _DEBUG
#pragma comment(lib, "KgWinException_d.lib")
#else
#pragma comment(lib, "KgWinException.lib")
#endif
#endif
#include <string>
#include <memory>

class KgWinException {
private:
	std::shared_ptr<std::string> Message;
	EXCEPTIONLIB std::string GetErrorMessage() const;
	EXCEPTIONLIB std::string GetErrorMessage(const unsigned long ErrorCode) const;
	EXCEPTIONLIB std::string GetErrorMessage(const long ErrorCode) const;
	EXCEPTIONLIB std::shared_ptr<std::string> GetErrorMessagePtr() const;
	EXCEPTIONLIB std::shared_ptr<std::string> GetErrorMessagePtr(const unsigned long ErrorCode) const;
	EXCEPTIONLIB std::shared_ptr<std::string> GetErrorMessagePtr(const long ErrorCode) const;
public:
	EXCEPTIONLIB KgWinException();
	EXCEPTIONLIB KgWinException(const unsigned long ErrorCode);
	EXCEPTIONLIB KgWinException(const long ErrorCode);
	EXCEPTIONLIB KgWinException(const std::string ExceptionAddMessage, const unsigned long ErrorCode);
	EXCEPTIONLIB KgWinException(const std::string ExceptionMessage);
	EXCEPTIONLIB KgWinException(const char* ExceptionMessage);
	EXCEPTIONLIB int GraphErrorMessageOnMessageBox(const char* Caption, const int Button = 0) const;
	EXCEPTIONLIB int GraphErrorMessageOnMessageBox(const std::string Caption, const int Button = 0) const;
	EXCEPTIONLIB void GraphErrorMessageOnConsole() const;
	EXCEPTIONLIB const char* what() const;
};

#endif
