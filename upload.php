#!/usr/bin/php
<?php
$method = $_FILES['file'];
fwrite(STDERR, "method: $method");
if ($_SERVER['REQUEST_METHOD'] == 'POST' && isset($_FILES['file'])) {
    fwrite("inside the block\n");
    // アップロードされたファイルの保存先ディレクトリ
    $target_dir = "/home/hmiyazak/Dev/42/webserv/uploads/";
    $target_file = $target_dir . basename($_FILES["file"]["name"]);
    fwrite(STDERR, "target: $target_file");

    // ファイルを保存
    if (move_uploaded_file($_FILES["file"]["tmp_name"], $target_file)) {
        echo "ファイルがアップロードされました: " . htmlspecialchars(basename($_FILES["file"]["name"]));
    } else {
        echo "ファイルのアップロードに失敗しました。";
    }
}
?>
