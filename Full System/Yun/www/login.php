<?php 
    session_start();
    $username =$_POST['user']; //get the entered username and password using post method
    $password = $_POST['pass'];
    $username = stripcslashes($username); //get rid of the backslashes. 
    $password = stripcslashes($password); //since the code is for the local networks security isnt too big of an issue

    $db = new SQLite3("/mnt/sda1/arduino/www/html_input_output/databases/authentification.db"); //query into a database thats stored on the atheros chip
    $result = $db->query('select * from login');
    
    $row = $result->fetchArray();

    if($password === $row['pass']&&$username === $row['user']){ //check if the user and password exist in the database and if the match
        //echo "Login success";
        session_regenerate_id(); //generate a session id
        $_SESSION['loggedin'] = TRUE; //set the session logged in key to a value of TRUE 
        $_SESSION['name'] = $username;
        header('Location: home.php'); //head to the home.php for the control panel
        exit();
    }else {
        header("Location: index.html"); //if passwords and username do not match or dont exist, send the user back to the login screen
        exit();
    }
?>