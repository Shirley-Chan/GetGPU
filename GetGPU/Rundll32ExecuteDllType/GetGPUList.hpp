#ifndef __GETGPULIST_HPP__
#define __GETGPULIST_HPP__
#ifdef GETGPULIST_RUNDLL32EXECUTEDLLTYPE_EXPORTS
#define GETGPU __declspec(dllexport)
#else
#define GETGPU __declspec(dllimport)
#endif
#if (_WIN32_WINNT >= 0x0A00)
#include <d3d12.h>
#include <dxgi1_4.h>
#else
#include <dxgi.h>
#endif
#include <string>
#include <vector>

class GPUInformation {
private:
#if (_WIN32_WINNT >= 0x0A00)
	double ToDouble(const D3D_FEATURE_LEVEL FeatureLevel) {
		switch (FeatureLevel) {
		case D3D_FEATURE_LEVEL_12_1: return 12.1;
		case D3D_FEATURE_LEVEL_12_0: return 12.0;
		case D3D_FEATURE_LEVEL_11_1: return 11.1;
		case D3D_FEATURE_LEVEL_11_0: return 11.0;
		default: throw std::runtime_error("Feature level is wrong.");
		}
	}
#endif
public:
	GPUInformation() = default;
#if (_WIN32_WINNT >= 0x0A00)
	GPUInformation(const DXGI_ADAPTER_DESC1 desc, const unsigned int HardwareID, const D3D_FEATURE_LEVEL FeatureLevel)
		: Description(desc.Description), DedicatedVideoMemory(desc.DedicatedVideoMemory),
		DedicatedSystemMemory(desc.DedicatedSystemMemory), SharedSystemMemory(desc.SharedSystemMemory),
		HardwareID(HardwareID), MaxFeatureLevel(ToDouble(FeatureLevel)) {}
#else
	GPUInformation(const DXGI_ADAPTER_DESC desc, const unsigned int HardwareID)
		: Description(desc.Description), DedicatedVideoMemory(desc.DedicatedVideoMemory),
		DedicatedSystemMemory(desc.DedicatedSystemMemory), SharedSystemMemory(desc.SharedSystemMemory),
		HardwareID(HardwareID), MaxFeatureLevel(11.0) {}
#endif
	std::wstring Description;
	size_t DedicatedVideoMemory;
	size_t DedicatedSystemMemory;
	size_t SharedSystemMemory;
	unsigned int HardwareID;
	double MaxFeatureLevel;
};

GETGPU std::vector<GPUInformation> GetGPUList();
#endif
