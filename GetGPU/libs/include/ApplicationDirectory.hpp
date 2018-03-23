#ifndef __APPLICATIONDIRECTORY_HPP__
#define __APPLICATIONDIRECTORY_HPP__
#ifdef APPLICATIONDIRECTORY_EXPORTS
#define KGSYSAPPDIR __declspec(dllexport)
#else
#define KGSYSAPPDIR __declspec(dllimport)
#ifdef _DEBUG
#pragma comment(lib, "KgSysApplicationDirectory_d.lib")
#else
#pragma comment(lib, "KgSysApplicationDirectory.lib")
#endif
#endif
#include <string>
#include <memory>

class ApplicationDirectoryA {
private:
	std::shared_ptr<std::string> DefaultDirectory;
	mutable std::shared_ptr<std::string> CurrentDirectory;
	KGSYSAPPDIR ApplicationDirectoryA(const std::string& Directory);
public:
	KGSYSAPPDIR ApplicationDirectoryA() = default;
	// ApplicationDirectory�N���X������������
	KGSYSAPPDIR static ApplicationDirectoryA Initialize(const bool UseModuleFilePath = false);
	// ���s�t�@�C���̃p�X���擾����
	KGSYSAPPDIR static std::string GetModuleFilePath();
	// �J�����g�f�B���N�g�����f�t�H���g�p�X�ɖ߂�
	KGSYSAPPDIR void ChangeCurrentDirectory() const;
	// �J�����g�f�B���N�g�����w�肵���p�X�ɖ߂�
	KGSYSAPPDIR void ChangeCurrentDirectory(const std::string Directory) const;
	// �J�����g�f�B���N�g�����ŐV���ɍX�V����
	KGSYSAPPDIR void UpdateCurrentDirectory() const;
	// �J�����g�f�B���N�g�����擾����
	KGSYSAPPDIR std::string GetApplicationCurrentDirectory() const;
	// �f�t�H���g�p�X���擾����
	KGSYSAPPDIR std::string GetDefaultDirectory() const;
	// �n���ꂽ�p�X���J�����g�f�B���N�g���܂��̓f�t�H���g�p�X�̏��Ƒg�ݍ��킹�邱�ƂŐ�΃p�X�ɕϊ�����B
	// ��΃p�X���n����Ă��鎞�͉������Ȃ�
	KGSYSAPPDIR std::string ChangeFullPath(const std::string Path, const bool UseDefaultDirectory = false) const;
};

class ApplicationDirectoryW {
private:
	std::shared_ptr<std::wstring> DefaultDirectory;
	mutable std::shared_ptr<std::wstring> CurrentDirectory;
	KGSYSAPPDIR ApplicationDirectoryW(const std::wstring& Directory);
public:
	KGSYSAPPDIR ApplicationDirectoryW() = default;
	// ApplicationDirectory�N���X������������
	KGSYSAPPDIR static ApplicationDirectoryW Initialize(const bool UseModuleFilePath = false);
	// ���s�t�@�C���̃p�X���擾����
	KGSYSAPPDIR static std::wstring GetModuleFilePath();
	// �J�����g�f�B���N�g�����f�t�H���g�p�X�ɖ߂�
	KGSYSAPPDIR void ChangeCurrentDirectory() const;
	// �J�����g�f�B���N�g�����w�肵���p�X�ɖ߂�
	KGSYSAPPDIR void ChangeCurrentDirectory(const std::wstring Directory) const;
	// �J�����g�f�B���N�g�����ŐV���ɍX�V����
	KGSYSAPPDIR void UpdateCurrentDirectory() const;
	// �J�����g�f�B���N�g�����擾����
	KGSYSAPPDIR std::wstring GetApplicationCurrentDirectory() const;
	// �f�t�H���g�p�X���擾����
	KGSYSAPPDIR std::wstring GetDefaultDirectory() const;
	// �n���ꂽ�p�X���J�����g�f�B���N�g���܂��̓f�t�H���g�p�X�̏��Ƒg�ݍ��킹�邱�ƂŐ�΃p�X�ɕϊ�����B
	// ��΃p�X���n����Ă��鎞�͉������Ȃ�
	KGSYSAPPDIR std::wstring ChangeFullPath(const std::wstring Path, const bool UseDefaultDirectory = false) const;
};

#endif
