# FivePoker Client

這是以 Cocos2d-x 開發的 5PK（Five Poker）遊戲前端，包含 Windows Desktop 與 Android 平台專案。程式以 C++ 為主，並使用專案內的 PureMVC 風格框架管理遊戲狀態、畫面與網路命令。

> 專案使用舊版 Cocos2d-x API 與 Android 建置設定。建置前請先確認相容的 Cocos2d-x、Visual Studio、Android SDK／NDK 版本；目前專案目錄未包含 Cocos2d-x SDK。

## 專案結構

| 路徑                                         | 說明                                                             |
| -------------------------------------------- | ---------------------------------------------------------------- |
| `BaseLayer/`                                 | PureMVC 風格核心、網路 Socket、工具程式，以及 Windows 靜態程式庫 |
| `GameLayer/FivePoker/`                       | 5PK 遊戲邏輯、模型、Mediator、Cocos2d-x 畫面及遊戲資源           |
| `GameLayer/ConnectMediator/`                 | 登入／中央／遊戲伺服器的連線、協定事件與命令                     |
| `GameLayer/FivePoker/Platform/proj.win32/`   | Windows Desktop Visual Studio 專案                               |
| `GameLayer/FivePoker/Platform/proj.android/` | Android Java、JNI、NDK 專案                                      |
| `Doc/`                                       | 專案連線流程相關文件                                             |

## 架構概要

- `BaseLayer` 提供 `Facade`、`Command`、`Proxy`、`Mediator` 等 PureMVC 風格元件，並包含 Socket 與通知機制。
- `FivePoker/src/Game/Game.cpp` 初始化遊戲的 Proxy、Command、事件及 Mediator。
- `ConnectMediator` 負責伺服器連線與通訊事件；遊戲 UI 透過通知與命令和網路／模型層互動。
- `FivePoker/src/Game/UI_cocos2dx/FivePoker/` 存放 Cocos2d-x 介面。`GameScene` 組合登入、一般遊戲與 Double Game 畫面，分別由 `LoginLayer`、`NGLayer`、`DGLayer` 呈現。
- `FivePoker/Resources/` 存放遊戲圖片、音效及其他執行期資源。

## Windows Desktop 建置

1. 安裝與專案相容的 Visual Studio，以及 Cocos2d-x 2.x SDK。
2. 在 Visual Studio 開啟 `GameLayer/FivePoker/Platform/proj.win32/FivePoker.win32.sln`。
3. 設定 `COCOS2DX_ROOT` 為 Cocos2d-x SDK 根目錄，並確認專案中的 include、library 路徑可用。
4. 選擇 `Debug` 或 `Release` 組態，以 `Win32` 平台建置並執行。

專案另提供 `Debug_ViewTest` 組態，供不啟用完整遊戲網路流程的畫面測試使用。

## Android 建置

Android 專案採用舊式 NDK／ADT 專案設定：`project.properties` 的目標為 `android-8`，`Application.mk` 使用 `gnustl_static`。新版 Android 工具鏈可能已不支援這些設定，請使用相容的舊版 SDK／NDK 環境。

1. 安裝相容版本的 Android SDK、NDK、Cocos2d-x SDK，以及可匯入舊式 Android 專案的工具。
2. 編輯 `GameLayer/FivePoker/Platform/proj.android/build_native.sh`，將 `NDK_ROOT` 與 `COCOS2DX_ROOT` 改為本機安裝路徑。
3. 從 `GameLayer/FivePoker/Platform/proj.android/` 目錄執行 `build_native.sh`，建置原生程式碼並複製 `Resources` 到 Android assets。
4. 使用相容的 Android 工具鏈匯入／封裝專案，產生 APK。

建置腳本會重新建立 Android `assets` 目錄；若該目錄包含未提交的自訂檔案，請先備份。NDK 原生建置步驟本身不會產生 APK。

## 伺服器連線

遊戲需要搭配相容的後端伺服器與通訊協定。測試前請確認伺服器位址、連接埠及登入／遊戲參數符合目前環境；連線流程及預設參數可從 `GameLayer/FivePoker/src/Game/UI_cocos2dx/FivePoker/LoginLayer.cpp`、`GameLayer/ConnectMediator/src/` 與 [連線方式文件](./Doc/連線方式.txt) 查閱。

## 授權

請參閱 [LICENSE](./LICENSE)。
