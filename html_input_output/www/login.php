<?php 
    session_start();
    $username =$_POST['user'];
    $password = $_POST['pass'];
    $username = stripcslashes($username);
    $password = stripcslashes($password);

    $db = new SQLite3("/mnt/sda1/arduino/www/html_input_output/databases/authentification.db");
    $result = $db->query('select * from login');
    
    $row = $result->fetchArray();

    //if(array_key_exists($username, $row)){
    if($password === $row['pass']&&$username === $row['user']){
        //echo "Login success";
        session_regenerate_id();
        $_SESSION['loggedin'] = TRUE;
        $_SESSION['name'] = $username;
        header('Location: home.php');
        exit();
    }else {
        header("Location: index.html");
        exit();
    }
    //}else{
    //    echo "Error, no such user exists in the database\n";
    //    echo $username."\n";
    //    echo $row[0];
    //}
?>