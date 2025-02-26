class expression

server class
サーバーを管理する責務
クライアント接続の管理、リクエストとレスポンス管理

Nginx Class
Nginxの設定ファイル

Socket class
ソケットの情報を持つ
Listen fdをもち、set upまで

Client class
client fdの一つ一つの情報保持

Request class
リクエスト情報を持つ
リクエストのParse

Response class
レスポンスの情報を保持
レスポンスを生成


POST example
POST /post HTTP/1.1
Host: localhost
Content-Type: application/json
Content-Length: 19

{"name":"Alice"}