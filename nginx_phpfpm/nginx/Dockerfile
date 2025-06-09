# nginxのベースイメージを指定
FROM nginx:alpine

# 設定ファイルをコンテナにコピー
COPY ./conf/default.conf /etc/nginx/conf.d/default.conf

COPY ./html/new-page.html /var/www/html/new-page/new-page.html;

# nginxのポートを公開
EXPOSE 80
