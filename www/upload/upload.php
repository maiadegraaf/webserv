<script>
    function deleteSomething(id) {
      fetch('upload/uploads/' + id,  {
        method: 'DELETE'
      })
    }
</script>

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
                <button
                  onclick='deleteSomething("<?php echo $entry?>")'
                >
                  delete
                </button>

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
