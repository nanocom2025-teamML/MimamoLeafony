# みまも Leafony

## 作品概要

本作品は，遠隔から家族を見守るためのぬいぐるみ型デバイスである．
音声認識などを用いて外出や帰宅を検知し，帰宅予定時間を超過した場合は家族が所持するスマートフォンに通知する．
デバイスは音声の入出力装置を持つため，スマートフォンが扱えない人でも離れたところにいる家族との連絡を取り合うことができる．

## 環境構築

### 1. pyenv のインストール

ターミナル上で次のコマンドを実行して，pyenv の有無を確認する．

```
pyenv --version
```

pyenv がインストールされていない場合，次のコマンドを実行する．

```
brew update
brew install pyenv
```

pyenv がインストールされている場合，次のコマンドを実行する．

```
brew upgrade pyenv
```

pyenv を zsh で有効にするために，次の文字列を `Users/.zshrc` ファイルに記述する．

```
# pyenv の初期化コマンド
eval "$(pyenv init --path)"
eval "$(pyenv init -)"
```

再度 `pyenv --version` を実行し，バージョンが表示されることを確認をする．

※ conda 環境が読み込まれている場合，pyenv 環境と競合する恐れがあります．

### 2. Python のインストール

ターミナル上で次のコマンドを実行

```
pyenv install 3.13.0
```

他のプログラムと環境が競合しないように，今回の開発環境の専用のディレクトリを用意する（例：`Users/src/nanocom2025/`）．

専用のディレクトリ内でのみ Python3.13.0 が有効になるように pyenv を設定する．

```
cd $HOME/src/nanocom2025

pyenv local 3.13.0
python -V  # もしくは "python3 -V"
```

バージョンが正しく表示されることを確認する．

### 3. リポジトリのクローン

ディレクトリ `Users/src/nanocom2025/` 内に本リポジトリをクローンする．

### 4. ライブラリのインストール

VSCode にてクローンしたプロジェクトを開き，ターミナル上で次のコマンドを実行する．

```
pip install -r requirements.txt
```

### 5. 実行

```
uvicorn app.main:app --host 0.0.0.0 --port 10000 --reload
```

## APIへのリクエスト

```
curl -X POST "http://0.0.0.0:10000/targets/" \
    -H "Content-Type: application/json" \
    -d '{"name": "aomi", "curfew_time": "22:00:00"}'
```

```
curl -X POST "http://0.0.0.0:10000/accesses/" \
     -H "Content-Type: application/json" \
     -d '{"target_id": 1, "gone_at": "2025-04-01T08:00:00"}'
```

```
curl -X POST "http://0.0.0.0:10000/messages/" \
     -H "Content-Type: application/json" \
     -d '{"target_id": 1, "content": "Hello!"}'
```
