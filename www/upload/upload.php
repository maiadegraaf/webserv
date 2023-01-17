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
    <form action="upload_script.php" method="post" enctype="multipart/form-data">
      Select file to upload:
      <input type="file" />
      <input type="submit" />
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
                <form method="DELETE" action="<?php
                                                  $ch = curl_init();

                                                  // Set the cURL options
                                                  curl_setopt($ch, CURLOPT_URL, '$url');
                                                  curl_setopt($ch, CURLOPT_CUSTOMREQUEST, 'DELETE');
                                                  curl_setopt($ch, CURLOPT_POSTFIELDS, '');
                                                  curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);

                                                  // Set the headers
                                                  curl_setopt($ch, CURLOPT_HTTPHEADER, array(
                                                      'DELETE upload/uploads/$entry HTTP/1.1',
                                                      'Content-Type: application/json',
                                                      'Content-Length: '
                                                  ));

                                                  // Execute the cURL request
                                                  $response = curl_exec($ch);

                                                  // Close the cURL session
                                                  curl_close($ch);
                                             ?>">
                    <input type="hidden" id="file" value=<?php echo "$entry" ?>>
                    <input type="submit" value="delete">
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
