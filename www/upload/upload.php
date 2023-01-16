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
    <h3>Upload a File</h1>
    <form action="upload_script.php" method="post" enctype="multipart/form-data">
      Select file to upload:
      <input type="file" />
      <input type="submit" />
    </form>
  </div>
  <br>
  <div class="box background">
    <h3>List of Uploaded Files</h1>
    <ul>
      <?php
        if ($handle = opendir('www/upload/uploads')) {

          while (false !== ($entry = readdir($handle))) {

              if ($entry != "." && $entry != "..") {
       ?>
       <li>
       <?php
                  echo "<a href='upload/uploads/$entry'>$entry</a>";
       ?>
       </li>
       <?php
              }
          }
        closedir($handle);
        }
      ?>
    </ul>
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
