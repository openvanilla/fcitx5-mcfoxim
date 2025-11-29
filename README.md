# fcitx5-mcfoxim: 小麥族語輸入法 fcitx5 版本

本專案提供在 Linux 上的小麥族語輸入法，基於 fcitx5 輸入法框架開發，使用的資料來源是原住民語言研究發展基金會（原語會）的族語學習詞表系統，支援 42 種台灣南島語系語言。

## 安裝方式

以下說明如何在 Ubuntu 24.04 LTS（或是 22.04 LTS）上編譯安裝。

請先安裝 fcitx5, CMake, 以及以下開發用模組：

```bash
sudo apt install \
    pkg-config fcitx5 libfcitx5core-dev libfcitx5config-dev libfcitx5utils-dev fcitx5-modules-dev \
    cmake extra-cmake-modules
```

然後在本專案的 git 目錄下執行以下指令：

```bash
cmake -B build -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release
cmake --build build
sudo cmake --install build

# 初次安裝後，執行以下指令，小麥注音 icon 就會出現在 fcitx5 選單中
sudo update-icon-caches /usr/share/icons/*
```

## 社群公約

歡迎小麥注音 Linux 用戶回報問題與指教，也歡迎大家參與小麥注音開發。

我們採用了 GitHub 的[通用社群公約](https://github.com/openvanilla/fcitx5-mcfoxim/blob/master/CODE_OF_CONDUCT.md)。公約的中文版請參考[這裡的翻譯](https://www.contributor-covenant.org/zh-tw/version/1/4/code-of-conduct/)。我們以上述公約，作為維護小麥注音社群的準則。
