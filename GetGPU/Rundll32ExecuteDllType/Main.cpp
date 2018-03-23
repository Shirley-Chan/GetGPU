#include "MSXMLWrite.hpp"
#include <d3d11.h>
#include <vector>
#include <array>
#pragma comment(lib, "d3d11.lib")
#define SAFERELEASE(p) { if(p) { (p)->Release(); (p) = NULL; } }

class GPUInformation  {
private:
	double ToDouble(const D3D_FEATURE_LEVEL FeatureLevel) {
#if (_WIN32_WINNT >= 0x0A00)
		switch (FeatureLevel) {
			case D3D_FEATURE_LEVEL_12_1: return 12.1;
			case D3D_FEATURE_LEVEL_12_0: return 12.0;
			case D3D_FEATURE_LEVEL_11_1: return 11.1;
			case D3D_FEATURE_LEVEL_11_0: return 11.0;
			default: throw std::runtime_error("Feature level is wrong.");
		}
#else 
		UNREFERENCED_PARAMETER(FeatureLevel);
		return 11.0;
#endif
	}
public:
	GPUInformation() = default;
	GPUInformation(const DXGI_ADAPTER_DESC desc, const unsigned int HardwareID, const D3D_FEATURE_LEVEL FeatureLevel)
		: Description(desc.Description), DedicatedVideoMemory(std::to_wstring(desc.DedicatedVideoMemory)),
		DedicatedSystemMemory(std::to_wstring(desc.DedicatedSystemMemory)), SharedSystemMemory(std::to_wstring(desc.SharedSystemMemory)),
		HardwareID(std::to_wstring(HardwareID)), MaxFeatureLevel(ToDouble(FeatureLevel)) {}
	GPUInformation(const DXGI_ADAPTER_DESC1 desc, const unsigned int HardwareID, const D3D_FEATURE_LEVEL FeatureLevel)
		: Description(desc.Description), DedicatedVideoMemory(std::to_wstring(desc.DedicatedVideoMemory)),
		DedicatedSystemMemory(std::to_wstring(desc.DedicatedSystemMemory)), SharedSystemMemory(std::to_wstring(desc.SharedSystemMemory)),
		HardwareID(std::to_wstring(HardwareID)), MaxFeatureLevel(ToDouble(FeatureLevel)) {}
	std::wstring Description;
	std::wstring DedicatedVideoMemory;
	std::wstring DedicatedSystemMemory;
	std::wstring SharedSystemMemory;
	std::wstring HardwareID;
	double MaxFeatureLevel;
};

std::vector<GPUInformation> Dx11GetGPUList() {
	std::vector<GPUInformation> DeviceList;
	IDXGIFactory* Factory;
	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&Factory)))
		throw std::runtime_error("failed to create dxgi factory.");
	IDXGIAdapter* HardwareAdapter;
	for (unsigned int i = 0; DXGI_ERROR_NOT_FOUND != Factory->EnumAdapters(i, &HardwareAdapter); i++) {
		const D3D_FEATURE_LEVEL Level = D3D_FEATURE_LEVEL_11_0;
		if (SUCCEEDED(D3D11CreateDevice(HardwareAdapter, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &Level, 0, D3D11_SDK_VERSION, nullptr, nullptr, nullptr))) {
			DXGI_ADAPTER_DESC desc;
			HardwareAdapter->GetDesc(&desc);
			DeviceList.emplace_back(desc, i, Level);
		}
	}
	SAFERELEASE(HardwareAdapter);
	SAFERELEASE(Factory);
	return DeviceList;
}

#if (_WIN32_WINNT >= 0x0A00)
#include <d3d12.h>
#include <dxgi1_4.h>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
std::vector<GPUInformation> Dx12GetGPUList(const bool UseWarpDevice) {
	std::vector<GPUInformation> DeviceList;
	Microsoft::WRL::ComPtr<IDXGIFactory4> Factory;
	if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&Factory)))) return Dx11GetGPUList();
	static const std::array<D3D_FEATURE_LEVEL, 4> Buffer = { D3D_FEATURE_LEVEL_12_1, D3D_FEATURE_LEVEL_12_0, D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };
	Microsoft::WRL::ComPtr<IDXGIAdapter1> HardwareAdapter;
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
	if (DeviceList.empty()) return Dx12GetGPUList(true);
	return DeviceList;
}
#endif

#include "ApplicationDirectory.hpp"
#include "Win32API.hpp"
#include "KgWinException.hpp"

__declspec(dllexport) void CALLBACK GetGPUInformation(HWND, HINSTANCE, LPSTR lpCmdLine, int) {
	try {
		const ApplicationDirectoryW AppPath = ApplicationDirectoryW::Initialize(true);
		const std::wstring FileSavePoint = AppPath.ChangeFullPath(std::strlen(lpCmdLine) == 0 ?  L".\\gpu.xml" : StringToWString(lpCmdLine) + L"\\gpu.xml");
		const std::vector<GPUInformation> GPUList = Dx12GetGPUList(false);
		MSXMLWrite xml(FileSavePoint, L"gpulist");
		for (auto& i : GPUList) {
			ComPtr<IXMLDOMElement> Parent = xml.CreateElement("gpuinfo");
			xml.AddChildElementToParentElement(Parent, xml.CreateElementIncludedTextData(L"name", i.Description));
			xml.AddChildElementToParentElement(Parent, xml.CreateElementIncludedTextData(L"dedicatedvideomemory", i.DedicatedVideoMemory));
			xml.AddChildElementToParentElement(Parent, xml.CreateElementIncludedTextData(L"dedicatedsystemmemory", i.DedicatedSystemMemory));
			xml.AddChildElementToParentElement(Parent, xml.CreateElementIncludedTextData(L"sharedsystemmemory", i.SharedSystemMemory));
			xml.AddChildElementToParentElement(Parent, xml.CreateElementIncludedTextData(L"hardwareid", i.HardwareID));
			xml.AddChildElementToRoot(Parent);
		}
	}
	catch (const std::exception& er) {
		MessageBoxA(NULL, er.what(), "エラー", MB_ICONERROR | MB_OK);
	}
	catch (const KgWinException& kex) {
		kex.GraphErrorMessageOnMessageBox("エラー", MB_OK);
	}
}
