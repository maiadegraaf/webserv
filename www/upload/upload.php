#!/usr/bin/php

<!DOCTYPE html>
<html lang="en">
<head>
  <link rel="icon" href="data:,">
  <link rel="stylesheet" href="../css/css.css">
  <meta charset="UTF-8">
  <link rel="shortcut icon" type="image/x-icon" href="../photo/icon.png">
  <title>about us</title>
</head>
<body>
<div class="subtitle-container">
  <div class="wordart blues">
    <span class="text">file upload</span>
  </div>
</div>

<div class="center-text-box">
  <div class="box background">
    <h3>Upload a File</h3>
    <form method="post" enctype="multipart/form-data">
      Select file to upload:
      <p><input type="file" name="file1">
      <p><button type="submit" value="Send Request">Submit</button>
    </form>
  </div>
  <br>
  <div class="box background">
    <h3>Uploaded Files</h3>
    <table class="filesList">
        <tbody>
          <?php
            if ($handle = opendir('www/upload/uploads')) {

              while (false !== ($entry = readdir($handle))) {

                  if ($entry != "." && $entry != "..") {
            ?>
        <tr>
            <td>
                <?php
                           echo "<a href='upload/uploads/$entry'>$entry</a>";
                ?>
            </td>
            <td>
                <form method="DELETE" action=<?php
                                                 $url = "upload/uploads/$entry";
                                                 $ch = curl_init();
                                                 curl_setopt($ch, CURLOPT_URL, $url);
                                                 curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "DELETE");
                                                 curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
                                                 $headers = [];
                                                 $headers[] = 'Content-Type:application/json';
                                                 curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
                                                 curl_exec($ch);
                                                 curl_close($ch);
                                             ?>>
                    <input type="hidden" id="file" value=<?php echo "\"$entry\"" ?>>
                    <button type="submit" value="Send Request">Delete</button>
                </form>
            </td>
        </tr>
        <?php
              }
          }
        closedir($handle);
        }
        ?>
    </table>
  </div>
</div>
<div class="closing">
  <div class="center">
    <div class="home">
      <a href="/">
        <button type="button" class="fill">home</button>
      </a>
    </div>
  </div>
</div>
</body>
</html>
