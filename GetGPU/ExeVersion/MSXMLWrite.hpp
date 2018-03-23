#ifndef __MSXMLWRITE_HPP__
#define __MSXMLWRITE_HPP__
#include "ComPtr.hpp"
#include <msxml.h>
#include <string>
#include <type_traits>
#ifdef DLLIMPORT
#include <memory>
#endif

class MSXMLWrite {
private:
#ifdef DLLIMPORT
	std::shared_ptr<std::wstring> FilePath;
#else
	std::wstring FilePath;
#endif
	IXMLDOMDocument* lpXmlDoc;
	IXMLDOMElement* lpRoot;
	bool InitializeSucceeded;
	bool BeginOnANewLinePerAddNewTag;
	IXMLDOMElement* CreateElementNormalPtr(const wchar_t* Tag);
	ComPtr<IXMLDOMElement> CreateElementComPtr(const wchar_t* Tag);
	IXMLDOMText* CreateTextNodeNormalPtr(const wchar_t* Data);
	ComPtr<IXMLDOMText> CreateTextNodeComPtr(const wchar_t* Data);
public:
	MSXMLWrite() = delete;
	MSXMLWrite(const MSXMLWrite&) = delete;
	MSXMLWrite(MSXMLWrite&&) = delete;
	MSXMLWrite& operator = (const MSXMLWrite&) = delete;
	MSXMLWrite& operator = (MSXMLWrite&&) = delete;
	MSXMLWrite(const std::string FilePath, const std::string Root, const bool BeginOnANewLinePerAddNewTag = true);
	MSXMLWrite(const std::wstring FilePath, const std::wstring Root, const bool BeginOnANewLinePerAddNewTag = true);
	~MSXMLWrite();
	ComPtr<IXMLDOMElement> CreateElement(const std::string Tag);
	ComPtr<IXMLDOMElement> CreateElement(const std::wstring Tag);
	ComPtr<IXMLDOMElement> CreateElementIncludedTextData(const std::string Tag, const std::string Text);
	ComPtr<IXMLDOMElement> CreateElementIncludedTextData(const std::wstring Tag, const std::wstring Text);
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	ComPtr<IXMLDOMElement> CreateElementIncludedTextData(const std::string Tag, const T Data) {
		return this->CreateElementIncludedTextData(Tag, std::to_string(Data));
	}
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	ComPtr<IXMLDOMElement> CreateElementIncludedTextData(const std::wstring Tag, const T Data) {
		return this->CreateElementIncludedTextData(Tag, std::to_wstring(Data));
	}
	void AddChildElementToParentElement(ComPtr<IXMLDOMElement> &Parent, const ComPtr<IXMLDOMElement> Child, const int ChildIndentNum = 0);
	void AddChildElementToRoot(ComPtr<IXMLDOMElement> Child, const int ChildTagIndentNum = 0);
};
#endif
