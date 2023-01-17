<?php
    public function curl_del($path, $json = '')
    {
        $url = $this->__url.$path;
        $ch = curl_init();
        curl_setopt($ch, CURLOPT_URL, $url);
        curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "DELETE");
        curl_setopt($ch, CURLOPT_POSTFIELDS, $json);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
        $result = curl_exec($ch);
        $result = json_decode($result);
        curl_close($ch);

        return $result;
    }

    if($_SERVER['REQUEST_METHOD'] == 'DELETE') {
        curl_del($_REQUEST['file'], "", "DELETE");
    }
?>

