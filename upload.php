#!/usr/bin/php-cgi
<?php
// $input = file_get_contents("php://stdin");
// fwrite(STDERR, "child content: $input");
$file = $_FILES["file"]["name"];
echo "filename:::$file";
if ($_SERVER['REQUEST_METHOD'] == 'POST' && isset($_FILES['file'])) {
    // アップロードされたファイルの保存先ディレクトリ
    $target_dir = "/home/hmiyazak/Dev/webserv/uploads/";
    $target_file = $target_dir . basename($_FILES["file"]["name"]);

    // ファイルを保存
    if (move_uploaded_file($_FILES["file"]["tmp_name"], $target_file)) {
        echo "ファイルがアップロードされました: " . htmlspecialchars(basename($_FILES["file"]["name"]));
    } else {
        echo "ファイルのアップロードに失敗しました。";
    }
}
?>
