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
	// ApplicationDirectoryクラスを初期化する
	KGSYSAPPDIR static ApplicationDirectoryA Initialize(const bool UseModuleFilePath = false);
	// 実行ファイルのパスを取得する
	KGSYSAPPDIR static std::string GetModuleFilePath();
	// カレントディレクトリをデフォルトパスに戻す
	KGSYSAPPDIR void ChangeCurrentDirectory() const;
	// カレントディレクトリを指定したパスに戻す
	KGSYSAPPDIR void ChangeCurrentDirectory(const std::string Directory) const;
	// カレントディレクトリを最新情報に更新する
	KGSYSAPPDIR void UpdateCurrentDirectory() const;
	// カレントディレクトリを取得する
	KGSYSAPPDIR std::string GetApplicationCurrentDirectory() const;
	// デフォルトパスを取得する
	KGSYSAPPDIR std::string GetDefaultDirectory() const;
	// 渡されたパスをカレントディレクトリまたはデフォルトパスの情報と組み合わせることで絶対パスに変換する。
	// 絶対パスが渡されている時は何もしない
	KGSYSAPPDIR std::string ChangeFullPath(const std::string Path, const bool UseDefaultDirectory = false) const;
};

class ApplicationDirectoryW {
private:
	std::shared_ptr<std::wstring> DefaultDirectory;
	mutable std::shared_ptr<std::wstring> CurrentDirectory;
	KGSYSAPPDIR ApplicationDirectoryW(const std::wstring& Directory);
public:
	KGSYSAPPDIR ApplicationDirectoryW() = default;
	// ApplicationDirectoryクラスを初期化する
	KGSYSAPPDIR static ApplicationDirectoryW Initialize(const bool UseModuleFilePath = false);
	// 実行ファイルのパスを取得する
	KGSYSAPPDIR static std::wstring GetModuleFilePath();
	// カレントディレクトリをデフォルトパスに戻す
	KGSYSAPPDIR void ChangeCurrentDirectory() const;
	// カレントディレクトリを指定したパスに戻す
	KGSYSAPPDIR void ChangeCurrentDirectory(const std::wstring Directory) const;
	// カレントディレクトリを最新情報に更新する
	KGSYSAPPDIR void UpdateCurrentDirectory() const;
	// カレントディレクトリを取得する
	KGSYSAPPDIR std::wstring GetApplicationCurrentDirectory() const;
	// デフォルトパスを取得する
	KGSYSAPPDIR std::wstring GetDefaultDirectory() const;
	// 渡されたパスをカレントディレクトリまたはデフォルトパスの情報と組み合わせることで絶対パスに変換する。
	// 絶対パスが渡されている時は何もしない
	KGSYSAPPDIR std::wstring ChangeFullPath(const std::wstring Path, const bool UseDefaultDirectory = false) const;
};

#endif
