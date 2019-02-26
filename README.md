cedit++
=======

マルチプラットフォームで使いやすいソースコードエディタ

## ヴァージョン
1.0.2

## 主な機能
* さまざまな言語に対応したシンタックスハイライト
* 改行時や括弧の挿入時に自動でインデント
* 対応する括弧への色づけ
* 保存時に構文を自動でチェック(Flymake)
* コマンド１つで登録したライブラリを呼び出し
* 複数の文字コードに対応(JIS, Shift_JIS, EUC-JP, UTF-8, UTF-16)
* 日本語と英語のメニュー表示

## インストール

### Windows
`windows/installer`以下にあるインストーラを使ってください

### Linux
```bash
$ sudo apt-get install libgtkmm-3.0-dev libgtksourceviewmm-3.0-dev
$ make
$ make install
```

### Mac
```bash
$ brew install gtkmm3 gtksourceviewmm3 adwaita-icon-theme
$ make
$ make install
```
