<!DOCTYPE html>
<?php
include('session.php');
if(!isset($_SESSION['login_user'])){
header("location: index.php"); // Redirecting To Home Page
}
?>
<html>
<head>
<link href="style.css" rel="stylesheet" type="text/css">
</head>
<body>
<div id="profile">
<img src='https://vignette.wikia.nocookie.net/fallout/images/c/c0/VaultBoyFO3.png/revision/latest/scale-to-width-down/181?cb=20110809182235'
		width='60' height='60' /><b id="welcome"><font face='verdana'>Welcome : <?php echo $login_session; ?></font></b>
<b id="logout"><a href="logout.php"><font color='#000000' ><font face='verdana'>Log Out</font></a></b>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
body {font-family: Arial;}

/* Style the tab */
.tab {
  overflow: hidden;
  border: 1px solid #ccc;
  background-color: #f1f1f1;
}

/* Style the buttons inside the tab */
.tab button {
  background-color: inherit;
  float: left;
  border: none;
  outline: none;
  cursor: pointer;
  padding: 14px 16px;
  transition: 0.3s;
  font-size: 17px;
}

/* Change background color of buttons on hover */
.tab button:hover {
  background-color: #ddd;
}

/* Create an active/current tablink class */
.tab button.active {
  background-color: #ccc;
}

/* Style the tab content */
.tabcontent {
  display: none;
  padding: 6px 12px;
  border: 1px solid #ccc;
  border-top: none;
}
</style>
</head>
<?php include 'processing.php';?>
<!doctype html>
<html lang="en">

<head>
    <!-- Required meta tags -->
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">

    <title>Home Page</title>
    <script>function recCreate{alert("Record added.");}</script>
    <style>
    .error {
        color: #FF0000;
    }
    </style>
</head>

<body>
    <?php
// define variables and set to empty values
$fnameErr = $lnameErr = $rfid_numErr = $useridErr = $dept_ofcErr = "";
$fname = $lname = $rfid_num = $dept_ofc = $userid = "";
$onError = 0;
// $_SERVER["REQUEST_METHOD"] == "POST"
if ($_SERVER["REQUEST_METHOD"] == "POST") {

    if (empty($_POST["fname"])) {
        $fnameErr = "data required";
    } else {
        $fname = test_input($_POST["fname"]);
        // check if name only contains letters and whitespace
        $fnameErr = validate_name($fname);
    }
    if (empty($_POST["lname"])) {
        $lnameErr = "data required";
        
    } else {
        $lname = test_input($_POST["lname"]);
        // check if name only contains letters and whitespace
        $lnameErr = validate_name($lname);
        

    }

    if (empty($_POST["rfid_num"])) {
        $rfid_numErr = "RFID is required";
        
    } else {
        $rfid_num = test_input($_POST["rfid_num"]);
        
    }

    if (empty($_POST["userid"])) {
        $useridErr = "Missing User ID";
        
    } else {
        $userid = test_input($_POST["userid"]);
        
    }

    if (empty($_POST["dept_ofc"])) {
        $dept_ofcErr = "Missing data";
        
    } else {
        $dept_ofc = test_input($_POST["dept_ofc"]);
        
    }
   
}

function test_input($data)
{
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}
function validate_name($data)
{
    if (!preg_match("/^[a-zA-Z ]*$/", $data)) {
        $nameErr = "Only letters and white space allowed";
        
    } else {
        $nameErr = "";
        
    }
    return $nameErr;

}

function recCreated(){
    alert("Record Created");
}
if ($fnameErr == "" && $lnameErr == "" && $rfid_numErr == "" && $useridErr == "" && $dept_ofcErr == ""){
    if($fname != "" && $lname != "" && $rfid_num != "" && $dept_ofc != "" && $userid != ""){
    echo "<br>No Error. <br />";
    $onError = 0;
    }
    
} else {
    echo "<br>Errors found. <br />";
    $onError = 1;
}
echo $onError."<br />";
// if ($fnameErr == "" && $lnameErr == "" && $rfid_numErr == "" && $useridErr == "" && $dept_ofcErr == ""){
//     //echo "yeet <br />";
//     $sql_st = "INSERT INTO user_data (rfid_num, userid, fname,lname,dept_ofc)
//     VALUES ('$rfid_num', '$userid', '$fname','$lname','$dept_ofc' )";
    
//             if ($conn->query($sql_st) === true) {
//                 echo "New record created successfully";
//             } else {
//                 echo "Error: " . $sql_st . "<br>" . $conn->error;
//             }
            
// }
if($onError == 1){
    echo "";
}else{
    echo "";
    if(isset($_POST['submit'])){
        //check first if data is existing
        $sql_st = "SELECT rfid_num, userid, fname,lname,dept_ofc FROM user_data";
        $result = $conn->query($sql_st);
        if($result->num_rows == 0){
        $sql_st = "INSERT INTO user_data (rfid_num, userid, fname,lname,dept_ofc)
        VALUES ('$rfid_num', '$userid', '$fname','$lname','$dept_ofc' )";
    
            if ($conn->query($sql_st) === true) {
                //echo "New record created successfully";
                
                
            } else {
                echo "Error: " . $sql_st . "<br>" . $conn->error;
            }
        }
    
    
    if(isset($_POST['submit'])){
        header("Location: register_user.php");
    }
    $fname = $lname = $rfid_num = $dept_ofc = $userid = "";
    
   
    //header("Location: register_user.php");
}}
?>

    <h2>User Registration Form</h2>
    <br />
    <form method="post" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>">
        RFID: <input type="text" name="rfid_num" value="<?php echo $rfid_num; ?>">
        <span class="error"> <?php echo $rfid_numErr; ?></span>
        <br /><br />
        User ID: <input type="text" name="userid" value="<?php echo $userid; ?>">
        <span class="error"> <?php echo $useridErr; ?></span>
        <br /><br />
        First Name: <input type="text" name="fname" value="<?php echo $fname; ?>">
        <span class="error"> <?php echo $fnameErr; ?></span>
        <br /><br />
        Last Name: <input type="text" name="lname" value="<?php echo $lname; ?>">
        <span class="error"> <?php echo $lnameErr; ?></span>
        <br /><br />
        Deptartmen or Office: <input type="text" name="dept_ofc" value="<?php echo $dept_ofc; ?>">
        <span class="error"><?php echo $dept_ofcErr; ?></span>
        <br /><br />

        
        <input type="submit" name="submit" value="Submit">
    </form>

   
</body>

</html>