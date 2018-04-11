#include "MSXMLWrite.hpp"
#include "Win32LetterConvert.hpp"
#include "Win32Exception.hpp"
#include <comdef.h>
#ifdef CONSOLEWRITE
#include <iostream>
#endif
#ifdef _DEBUG
#pragma comment(lib, "Win32LetterConvertDebug.lib")
#pragma comment(lib, "Win32ExceptionDebug.lib")
#else
#pragma comment(lib, "Win32LetterConvertRelease.lib")
#pragma comment(lib, "Win32ExceptionRelease.lib")
#endif
#define SAFERELEASE(p) { if (p) p->Release(); p = NULL;}
#define _B(str) SysAllocString(str)
#define _BFREE(str) SysFreeString(str)

static inline void ThrowExceptionIfErrorOccured(const HRESULT hr) {
	if (FAILED(hr)) throw Win32Exception(hr);
}

MSXMLWrite::MSXMLWrite(const std::string FilePath, const std::string Root, const bool BeginOnANewLinePerAddNewTag)
	: MSXMLWrite(StringToWString(FilePath), StringToWString(Root), BeginOnANewLinePerAddNewTag) {}

MSXMLWrite::MSXMLWrite(const std::wstring FilePath, const std::wstring Root, const bool BeginOnANewLinePerAddNewTag) {
	this->InitializeSucceeded = false;
	this->BeginOnANewLinePerAddNewTag = BeginOnANewLinePerAddNewTag;
#ifdef DLLIMPORT
	this->FilePath = std::make_shared<std::wstring>(FilePath);
#else
	this->FilePath = FilePath;
#endif
	CoInitialize(nullptr);
	const HRESULT ErrorCode = CoCreateInstance(CLSID_DOMDocument, nullptr, CLSCTX_INPROC_SERVER, IID_IXMLDOMDocument, (void**)&this->lpXmlDoc);
	if (this->lpXmlDoc == nullptr) throw Win32Exception(ErrorCode);
	IXMLDOMProcessingInstruction* lpProcInst;
	BSTR Target = _B(L"xml");
	BSTR Xml = _B(L"version='1.0' encoding='UTF-8'");
	ThrowExceptionIfErrorOccured(this->lpXmlDoc->createProcessingInstruction(Target, Xml, &lpProcInst));
	ThrowExceptionIfErrorOccured(this->lpXmlDoc->appendChild(lpProcInst, NULL));
	_BFREE(Target);
	_BFREE(Xml);
	SAFERELEASE(lpProcInst);
	this->lpRoot = this->CreateElementNormalPtr(Root.c_str());
	this->CreateTextNodeComPtr(L"\n", this->NewLine);
	this->CreateTextNodeComPtr(L"\t", this->Indent);
	this->InitializeSucceeded = true;
}

MSXMLWrite::~MSXMLWrite() {
	if (this->InitializeSucceeded) {
		try {
			this->lpRoot->appendChild(this->NewLine.Get(), NULL);
			ThrowExceptionIfErrorOccured(this->lpXmlDoc->appendChild(this->lpRoot, NULL));
			ThrowExceptionIfErrorOccured(this->lpXmlDoc->put_async(VARIANT_FALSE));
#ifdef DLLIMPORT
			ThrowExceptionIfErrorOccured(this->lpXmlDoc->save(_variant_t(this->FilePath->c_str())));
#else
			ThrowExceptionIfErrorOccured(this->lpXmlDoc->save(_variant_t(this->FilePath.c_str())));
#endif
		}
		catch (const std::exception& er) {
#ifdef CONSOLEWRITE
			std::cout << er.what() << std::endl;
#else
			MessageBoxA(NULL, er.what(), "Error", MB_ICONERROR | MB_OK);
#endif
		}
		catch (const Win32Exception& wex) {
#ifdef CONSOLEWRITE
			wex.GraphOnConsole();
#else
			wex.GraphOnMessageBox(NULL, TEXT("Error"), MB_OK);
#endif
		}
	}
	CoUninitialize();	
}

ComPtr<IXMLDOMElement> MSXMLWrite::CreateElement(const std::string Tag) {
	return this->CreateElementComPtr(StringToWString(Tag).c_str());
}

ComPtr<IXMLDOMElement> MSXMLWrite::CreateElement(const std::wstring Tag) {
	return this->CreateElementComPtr(Tag.c_str());
}

void MSXMLWrite::AddChildElementToParentElement(ComPtr<IXMLDOMElement> &Parent, const ComPtr<IXMLDOMElement> Child, const int ChildIndentNum) {
	Parent->appendChild(this->NewLine.Get(), NULL);
	for (int i = 0; i < ChildIndentNum; i++) Parent->appendChild(this->Indent.Get(), NULL);
	Parent->appendChild(Child.Get(), NULL);
}

void MSXMLWrite::AddChildElementToRoot(ComPtr<IXMLDOMElement> Child, const int ChildTagIndentNum) {
	this->CreateTextNodeComPtr(L"\n", this->NewLine);
	this->CreateTextNodeComPtr(L"\t", Indent);
	Child->appendChild(this->NewLine.Get(), NULL);
	for (int i = 0; i < ChildTagIndentNum; i++) {
		Child->appendChild(this->NewLine.Get(), NULL);
	}
	this->lpRoot->appendChild(this->NewLine.Get(), NULL);
	for (int i = 0; i < ChildTagIndentNum; i++) this->lpRoot->appendChild(Indent.Get(), NULL);
	this->lpRoot->appendChild(Child.Get(), NULL);
}

IXMLDOMElement* MSXMLWrite::CreateElementNormalPtr(const wchar_t* Tag) {
	IXMLDOMElement* lpElement;
	BSTR str = _B(Tag);
	ThrowExceptionIfErrorOccured(this->lpXmlDoc->createElement(str, &lpElement));
	_BFREE(str);
	return lpElement;
}

ComPtr<IXMLDOMElement> MSXMLWrite::CreateElementComPtr(const wchar_t* Tag) {
	return ComPtr<IXMLDOMElement>(this->CreateElementNormalPtr(Tag));
}

IXMLDOMText* MSXMLWrite::CreateTextNodeNormalPtr(const wchar_t* Data) {
	BSTR DataBuf = _B(Data);
	IXMLDOMText* lpText;
	ThrowExceptionIfErrorOccured(this->lpXmlDoc->createTextNode(DataBuf, &lpText));
	_BFREE(DataBuf);
	return lpText;
}

void MSXMLWrite::CreateTextNodeComPtr(const wchar_t* Data, ComPtr<IXMLDOMText> &Text) {
	Text.Attach(this->CreateTextNodeNormalPtr(Data));
}

ComPtr<IXMLDOMElement> MSXMLWrite::CreateElementIncludedTextData(const std::string Tag, const std::string Text) {
	ComPtr<IXMLDOMElement> lpElem = this->CreateElement(Tag);
	ComPtr<IXMLDOMText> Txt{};
	this->CreateTextNodeComPtr(StringToWString(Text).c_str(), Txt);
	lpElem->appendChild(Txt.Get(), NULL);
	return lpElem;
}

ComPtr<IXMLDOMElement> MSXMLWrite::CreateElementIncludedTextData(const std::wstring Tag, const std::wstring Text) {
	ComPtr<IXMLDOMElement> lpElem = this->CreateElement(Tag);
	ComPtr<IXMLDOMText> Txt{};
	this->CreateTextNodeComPtr(Text.c_str(), Txt);
	lpElem->appendChild(Txt.Get(), NULL);
	return lpElem;
}
