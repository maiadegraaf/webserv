<?php
//     parse_str(file_get_contents("php://input"), $delete_data);
//     $file = $delete_data['file'];
//     echo $file;
    $file = $argv[1];
//     $file = substr($file, strpos($file, "=") + 1);
    $url = "localhost:8080/upload/uploads/$file";
//     $url = $file;
    echo $url;
    $ch = curl_init($url);
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "DELETE");
    curl_setopt($ch, CURLOPT_POSTFIELDS, "");
//     curl_setopt($ch, CURLOPT_TIMEOUT_MS, 1);
//     curl_setopt($ch, CURLOPT_NOSIGNAL, 1);
    $result = curl_exec($ch);
    echo $result;
    if(curl_error($ch)) {
        echo curl_error($ch);
    }
//         $result = json_decode($result);
    curl_close($ch);
?>