# GetGPU
DirectX 11と12を利用した、コンピュータにあるGPUの一覧を表示するプログラム

## プロジェクトの使い方
このリポジトリには、ChooseGPUというC#のプロジェクトが一緒にあります。

これは、GetGPUのExeVersionまたはRundll32ExecuteDllTypeのどちらかを使用して作成したgpu.xmlを読み込み、実際にGUIでユーザーにGPUを選択させるものです。

このGUIをご自身で好きなように作り、GPUの一覧(型：List<GPUInformation>)のデータを表示させてください。表示の仕方、選択のさせ方等についてもご自由にどうぞ。

## ビルド方法
### DirectX 11を使用しての判定をしたい場合
プロジェクトのプロパティからWindows SDKバージョンを「8.1」に変更してビルドしてください。

### DirectX 12を使用しての判定をしたい場合
プロジェクトのプロパティからWindows SDKバージョンを「10」のいずれかに変更してビルドしてください。

## ビルドしたものの使い方
### ExeVersion
GetGPU.exe &lt;xmlを出力するパス&gt;
### Rundll32ExecuteDllType
rundll32.exe GetGPUDll.dll,GetGPUInformation &lt;xmlを出力するパス&gt;

## 使用しているライブラリとプログラムそのものについて
GetGPU内のlibsフォルダ内にあるのは、[あいめぐ(メインペンネーム：神御田)さん](https://github.com/AinoMegumi)が独自に作成したWin32 APIのラッパーライブラリで、MIT Licenseということです。

ライブラリを使いたい人は、わりと癖があるようなので相談してからにしたほうがいいと思います。

## お詫び
現在Windows 10 Fall Creators UpdateのSDK以外のものを使ってビルドするとビルドエラーになる状態です。

早急な修正を行っておりますのでお待ちください。
