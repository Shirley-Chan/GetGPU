﻿#include "GetGPUList.hpp"
#include "MSXMLWrite.hpp"
#include "ApplicationCurrentDirectoryManager.hpp"
#include "Win32LetterConvert.hpp"
#include "Win32Exception.hpp"
#include <array>
#ifdef _DEBUG
#pragma comment(lib, "Win32LetterConvertDebug.lib")
#pragma comment(lib, "Win32ExceptionDebug.lib")
#pragma comment(lib, "ApplicationCurrentDirectoryManagerDebug.lib")
#else
#pragma comment(lib, "Win32LetterConvertRelease.lib")
#pragma comment(lib, "Win32ExceptionRelease.lib")
#pragma comment(lib, "ApplicationCurrentDirectoryManagerRelease.lib")
#endif
#if (_WIN32_WINNT >= 0x0A00)
#pragma comment(lib, "d3d12.lib")
#else
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#endif
#pragma comment(lib, "dxgi.lib")

#define SAFERELEASE(p) { if(p) { (p)->Release(); (p) = NULL; } }

#if (_WIN32_WINNT >= 0x0A00)
std::vector<GPUInformation> GetGPUList() {
	std::vector<GPUInformation> DeviceList;
	ComPtr<IDXGIFactory4> Factory;
	if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&Factory)))) throw std::runtime_error("Failed to create factory.");
	static const std::array<D3D_FEATURE_LEVEL, 4> Buffer = { D3D_FEATURE_LEVEL_12_1, D3D_FEATURE_LEVEL_12_0, D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };
	ComPtr<IDXGIAdapter1> HardwareAdapter;
	for (unsigned int i = 0; DXGI_ERROR_NOT_FOUND != Factory->EnumAdapters1(i, &HardwareAdapter); i++) {
		DXGI_ADAPTER_DESC1 desc{};
		HardwareAdapter->GetDesc1(&desc);
		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;
		for (auto& j : Buffer) {
			if (SUCCEEDED(D3D12CreateDevice(HardwareAdapter.Get(), j, __uuidof(ID3D12Device), nullptr))) {
				DeviceList.emplace_back(desc, i, j);
				break;
			}
		}
	}
	if (DeviceList.empty()) throw std::runtime_error(
		"This computer is installed no hardware device compatibled DirectX 12 use hardware adapter."
		"Warning : This tool can not judge compatibled warp device mode."
	);
	return DeviceList;
}
#else
std::vector<GPUInformation> GetGPUList() {
	std::vector<GPUInformation> DeviceList;
	IDXGIFactory* Factory = NULL;
	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&Factory)))
		throw std::runtime_error("failed to create dxgi factory.");
	IDXGIAdapter* HardwareAdapter = NULL;
	ID3D11Device* Dx11Device = NULL;
	ID3D11DeviceContext* Dx11DevContext = NULL;
	unsigned int CreationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	CreationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	for (unsigned int i = 0; DXGI_ERROR_NOT_FOUND != Factory->EnumAdapters(i, &HardwareAdapter); i++) {
		const D3D_FEATURE_LEVEL Level[] = { D3D_FEATURE_LEVEL_11_0 };
		if (SUCCEEDED(
			D3D11CreateDevice(HardwareAdapter, HardwareAdapter != NULL ? D3D_DRIVER_TYPE_UNKNOWN : D3D_DRIVER_TYPE_HARDWARE, NULL, CreationFlags, Level, ARRAYSIZE(Level), D3D11_SDK_VERSION, &Dx11Device, nullptr, &Dx11DevContext)
		)) {
			DXGI_ADAPTER_DESC desc;
			HardwareAdapter->GetDesc(&desc);
			DeviceList.emplace_back(desc, i);
		}
	}
	SAFERELEASE(Dx11DevContext);
	SAFERELEASE(Dx11Device);
	SAFERELEASE(HardwareAdapter);
	SAFERELEASE(Factory);
	if (DeviceList.empty()) throw std::runtime_error(
		"This computer is installed no hardware device compatibled DirectX 11 use hardware adapter.\n"
		"Warning : This tool can not judge compatibled warp device mode."
	);
	return DeviceList;
}
#endif

void CALLBACK GetGPUInformation(HWND, HINSTANCE, LPSTR lpCmdLine, int) {
	try {
		const ApplicationCurrentDirectoryManagerW AppPath = ApplicationCurrentDirectoryManagerW::Initialize();
		const std::wstring FileSavePoint = AppPath.ChangeFullPath(std::strlen(lpCmdLine) == 0 ? L".\\gpu.xml" : StringToWString(lpCmdLine) + L"\\gpu.xml");
		const std::vector<GPUInformation> GPUList = GetGPUList();
		MSXMLWrite xml(FileSavePoint, L"gpulist");
		for (auto& i : GPUList) {
			ComPtr<IXMLDOMElement> Parent = xml.CreateElement("gpuinfo");
			xml.AddChildElementToParentElement(Parent, xml.CreateElementIncludedTextData(L"name", i.Description), 2);
			xml.AddChildElementToParentElement(Parent, xml.CreateElementIncludedTextData(L"dedicatedvideomemory", i.DedicatedVideoMemory), 2);
			xml.AddChildElementToParentElement(Parent, xml.CreateElementIncludedTextData(L"dedicatedsystemmemory", i.DedicatedSystemMemory), 2);
			xml.AddChildElementToParentElement(Parent, xml.CreateElementIncludedTextData(L"sharedsystemmemory", i.SharedSystemMemory), 2);
			xml.AddChildElementToParentElement(Parent, xml.CreateElementIncludedTextData(L"hardwareid", i.HardwareID), 2);
			xml.AddChildElementToParentElement(Parent, xml.CreateElementIncludedTextData(L"maxfeaturelevel", i.MaxFeatureLevel), 2);
			xml.AddChildElementToRoot(Parent, 1);
		}
	}
	catch (const std::exception& er) {
		MessageBoxA(NULL, er.what(), "エラー", MB_ICONERROR | MB_OK);
	}
	catch (const Win32Exception& wex) {
		wex.GraphOnMessageBox(NULL, TEXT("エラー"), MB_OK);
	}
}
