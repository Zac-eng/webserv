# ベースイメージとしてUbuntuを使用
FROM ubuntu:20.04

# 必要なパッケージのインストール
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    cmake \
    git \
    wget \
    curl \
    vim \
    && rm -rf /var/lib/apt/lists/*

# 作業ディレクトリの設定
WORKDIR /app

# ホストマシンの現在のディレクトリの内容をコンテナの/appにコピー
COPY . /app

# プロジェクトのビルド
RUN make

# デフォルトの実行コマンド（コンテナ起動時に実行される）
CMD ["./webserv"]
