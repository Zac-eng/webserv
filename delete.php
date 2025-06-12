#!/usr/bin/php-cgi
<?php
if ($_SERVER['REQUEST_METHOD'] === 'DELETE') {
    // クエリからファイル名を取得
    parse_str(file_get_contents("php://input"), $params);
    $filename = $_GET['file'] ?? null;

    if ($filename && file_exists($filename)) {
        if (unlink($filename)) {
            echo "Deleted $filename";
        } else {
            http_response_code(500);
            echo "Failed to delete $filename";
        }
    } else {
        http_response_code(404);
        echo "File not found: $filename";
    }
} else {
    http_response_code(405); // Method Not Allowed
    echo "Only DELETE allowed";
}
?>