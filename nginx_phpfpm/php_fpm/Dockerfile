# PHP-FPMのベースイメージを指定
FROM php:7.4-fpm-alpine

# 必要なPHP拡張をインストール
RUN docker-php-ext-install pdo pdo_mysql

# アップロード用のディレクトリを作成
RUN mkdir -p /var/www/html/uploads

# PHPの設定ファイルをコンテナにコピー
COPY ./upload.php /var/www/html/upload.php
COPY ./delete.php /var/www/html/delete.php
RUN chmod +x /var/www/html/upload.php && chmod +x /var/www/html/delete.php

RUN chown www-data:www-data /var/www/html/uploads

RUN chmod 775 /var/www/html/uploads

# php-fpmを起動
CMD ["php-fpm"]
