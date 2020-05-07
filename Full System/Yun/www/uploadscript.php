<?php
if(isset($_POST['submit'])){ //initialize and accept a file through a  POST method
    $file = $_FILES['file'];

    $fileName = $_FILES['file']['name']; //file name
    $fileTmpName = $_FILES['file']['tmp_name']; 
    $fileSize = $_FILES['file']['size'];
    $fileError = $_FILES['file']['error'];
    $fileType = $_FILES['file']['type'];

    $fileExt = explode('.', $fileName); //find an extention of the file
    $fileActualExt = strtolower(end($fileExt)); //TXT = .txt

    $allowed = array('txt'); //array of allowed file formats 

    if(in_array($fileActualExt, $allowed)){ //check if the file format is allowed
        if($fileError === 0 ){ //check if file upload was successful
            if($fileSize < 1000){ //current file size limit is set to 1000 bytes! 
                $fileNameNew = "text.txt"; //all files will be saved into a single file just to simplify file access and not to over load the sd card
                $fileDestination = 'uploads/'.$fileNameNew; 
                move_uploaded_file($fileTmpName,$fileDestination);
                header("Location: home.php?uploadsuccess"); //return to home screen and update header saying operation successful
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