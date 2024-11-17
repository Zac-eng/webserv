<?php
// dynamic.php

// Parse query string (e.g., ?name=John)
$name = isset($_GET['name']) ? htmlspecialchars($_GET['name']) : "Guest";

// Respond dynamically
echo "Hello, " . $name . "!\n";
echo "The current server time is: " . date("Y-m-d H:i:s") . "\n";
