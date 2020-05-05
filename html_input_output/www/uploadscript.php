<?php
if(isset($_POST['submit'])){
    $file = $_FILES['file'];

    $fileName = $_FILES['file']['name'];
    $fileTmpName = $_FILES['file']['tmp_name'];
    $fileSize = $_FILES['file']['size'];
    $fileError = $_FILES['file']['error'];
    $fileType = $_FILES['file']['type'];

    $fileExt = explode('.', $fileName);
    $fileActualExt = strtolower(end($fileExt));

    $allowed = array('txt');

    if(in_array($fileActualExt, $allowed)){
        if($fileError === 0 ){
            if($fileSize < 1000){
                $fileNameNew = "text.txt";
                $fileDestination = 'uploads/'.$fileNameNew;
                move_uploaded_file($fileTmpName,$fileDestination);
                header("Location: home.php?uploadsuccess");
            }else {
                echo "File size is too big!";
            }
        } else {
            echo "an error occured while uploading the file";
        }
    } else {
        echo "You can only upload .txt files!";
    }
}
?>