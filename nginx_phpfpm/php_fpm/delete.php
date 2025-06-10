<?php
// サーバーにリクエストが来た時に、DELETE メソッドを処理
if ($_SERVER['REQUEST_METHOD'] == 'DELETE') {
    // URLのパスを取得
    $resource = basename($_SERVER['REQUEST_URI']);

    // リソースファイルのパス（例: file_1.txt）
    $file_path = "uploads/" . $resource;

    // ファイルが存在するかチェック
    if (file_exists($file_path)) {
        // ファイルを削除
        if (unlink($file_path)) {
            echo "File deleted successfully!";
        } else {
            echo "Error deleting file.";
        }
    } else {
        // ファイルが存在しない場合、404エラーレスポンス
        http_response_code(404);
        echo "File not found.";
    }
} else {
    // DELETE メソッド以外のリクエストが来た場合、405 Method Not Allowed
    http_response_code(405);
    echo "Method Not Allowed";
}
?>
