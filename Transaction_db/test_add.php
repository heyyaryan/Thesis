<!DOCTYPE html>
<?php
include('session.php');
if(!isset($_SESSION['login_user'])){
header("location: index.php"); // Redirecting To Home Page
}
?>
<html>
<head>
<title>Welcome to Add Account Page</title>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>

* {
  box-sizing: border-box;
}

body {
  background-repeat: no-repeat;
   background-size: cover;
   background-image: url("/Transaction_db/image/plain.jpg") ;
}


/* Style the header */
header {
  background-color: red;
  padding: 57px;
  font-size: 15px;
  color: white;
}
p.a {
  font-family: "Times New Roman", Times, serif;
}

p.b {
  font-family: Impact, Charcoal, sans-serif;

}
p {
    line-height: 0px;
}

div.a {
  text-align: right;
} 

/* Clear floats after the columns */
section:after {
  content: "";
  display: table;
  clear: both;
}

/* Style the footer */
footer {
  background-color: #777;
  padding: 10px;
  text-align: center;
  color: white;
}



#login {
    background-color: lightgrey;
    }
.button {
  background-color: red;
  border: none;
  color: white;
  padding: 15px 32px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
  font-size: 16px;
  margin: 4px 2px;
  cursor: pointer;
  font-family: Arial, Helvetica, sans-serif
}
#myBtn {
  background-color: red;
  border: none;
  color: white;
  padding: 15px 32px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
  font-size: 16px;
  margin: 4px 2px;
  cursor: pointer;
  font-family: Arial, Helvetica, sans-serif
}
body {font-family: Arial, Helvetica, sans-serif;}

/* The Modal (background) */
.modal {
  display: none; /* Hidden by default */
  position: fixed; /* Stay in place */
  z-index: 1; /* Sit on top */
  padding-top: 100px; /* Location of the box */
  left: 0;
  top: 0;
  width: 100%; /* Full width */
  height: 100%; /* Full height */
  overflow: auto; /* Enable scroll if needed */
  background-color: rgb(0,0,0); /* Fallback color */
  background-color: rgba(0,0,0,0.4); /* Black w/ opacity */
}

/* Modal Content */
.modal-content {
  background-color: #fefefe;
  margin: auto;
  padding: 20px;
  border: 1px solid #888;
  width: 80%;
}

/* The Close Button */
.close {
  color: #aaaaaa;
  float: right;
  font-size: 28px;
  font-weight: bold;
}

.close:hover,
.close:focus {
  color: #000;
  text-decoration: none;
  cursor: pointer;
}
footer {
  font-family: Arial, Helvetica, sans-serif;
  background-color: #777;
  padding: 10px;
  text-align: left;
  color: white;
}

div {
 div-align: center;
  background-color: lightgrey;
  width: 580px;
  height: 450px;
  padding: 50px;
  margin: 10px;
  
}
</style>
</head>
<?php include 'processing.php'?>
<header>
<img src = "/Transaction_db/image/csab.png"  align="left" width="100" height="100"><h1><p class="a">Colegio San Agustin-Bacolod</p></h1>
<h2><p class="b">Cashless Payment System </p></h2>
&emsp; &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; &emsp;&emsp;&emsp;  
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; &emsp;&emsp;&emsp; &emsp;&emsp;&emsp; &emsp;&emsp;&emsp; &emsp;&emsp;  Developed By:<img src = "/Transaction_db/image/coe.png"  align = "right" width="50" height="50">

<img src = "/Transaction_db/image/icpep.png"  align = "right" width="50" height="50">
</header>
<section id ="login">
<b id="welcome"><font face='verdana'>Welcome : <?php echo $login_session; ?></font></b>
<b id="logout"><a href="logout.php"><font color='#000000' ><font face='verdana'>Log Out</font></a></b>
</section>
<body>
<?php
$fnameErr = $lnameErr = $rfid_numErr = $useridErr = $dept_ofcErr = "";
$fname = $lname = $rfid_num = $dept_ofc = $userid = "";
$errorCount=0;
$generalMessage="";

if ($_SERVER['REQUEST_METHOD'] == "POST") {

    //check data validity
    if (empty($_POST['fname'])){
        $fnameErr = "<font color=\"red\">First name is empty.</font>";
        $errorCount+=1;
    } else {
        $fname = clean_input($_POST["fname"]);
        $fnameErr = validate_name($fname);
        if($fnameErr !=""){
            $errorCount+=1;
        }

    }
    
    if (empty($_POST['lname'])){
        $lnameErr = "<font color=\"red\">Last name is empty.</font>";
        $errorCount+=1;
    } else {
        $lname = clean_input($_POST["lname"]);
        $lnameErr = validate_name($lname);
        if($lnameErr !=""){
            $errorCount+=1;
        }

    }

    if (empty($_POST['rfid_num'])){
        $rfid_numErr = "<font color=\"red\">RFID is empty.</font>";
        $errorCount+=1;
    } else {
        $rfid_num = clean_input($_POST["rfid_num"]);
    }
    
    if (empty($_POST['userid'])){
        $useridErr = "<font color=\"red\">User ID is empty.</font>";
        $errorCount+=1;
    } else {
        $userid = clean_input($_POST["userid"]);
    }

    if (empty($_POST['dept_ofc'])){
        $dept_ofcErr = "<font color=\"red\">Department/Office is empty.</font>";
        $errorCount+=1;
    } else {
        $dept_ofc = clean_input($_POST["dept_ofc"]);
    }

    
    if ($errorCount == 0){
    //check first if data is existing
    $sql_st = "SELECT rfid_num,userid FROM user_data WHERE rfid_num='" . $rfid_num . "'|| userid='" . $userid . "'";
    $result = $conn->query($sql_st);
    $rows = mysqli_num_rows($result);
    //echo "Record count:" . $rows . "<br/>";
    mysqli_free_result($result);
    

    if ($rows == 0) {
        $sql_st = "INSERT INTO user_data (rfid_num, userid, fname,lname,dept_ofc)
         VALUES ('$rfid_num', '$userid', '$fname','$lname','$dept_ofc' )";

        if ($conn->query($sql_st) === true) {
            $generalMessage = "<font color=\"green\">New record created successfully.</font>";
            $fname = $lname = $rfid_num = $dept_ofc = $userid = "";

        } else {
            echo "Error: " . $sql_st . "<br>" . $conn->error;
        }
    } else {
        $generalMessage = "<font color=\"red\">Duplicate entry. Check data.</font>";
    }
    $conn->close();
}
    //echo "<meta http-equiv='refresh' content='3'>";
}
//unset($_POST['submit']);

function clean_input($data)
{
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}
function validate_name($data)
{
    if (!preg_match("/^[a-zA-Z ]*$/", $data)) {
        $nameErr = "<font color=\"red\">Only letters and white space allowed</font>";

    } else {
        $nameErr = "";

    }
    return $nameErr;
}

// if (isset($_POST['submit'])) {
//     header("Location: register_user.php");
// }

?> 
<center>
<div id="register" align = "center">
  <h2>User Registration Form</h2>
    <p>Please fill up all fields.</p>
    <!-- <?php echo "Errorcount:".$errorCount."<br />"; ?> -->
    <span class="error"><?php echo $generalMessage ."<br />"; ?></span>
    <form method="post" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>">
        RFID:  &emsp;&emsp;&emsp; &emsp;&emsp;<input type="text" name="rfid_num" value="<?php echo $rfid_num; ?>">
        <span class="error"> <?php echo $rfid_numErr; ?></span>
        <br>
		        <br>
        User ID: &emsp;&emsp;&emsp; &emsp; <input type="text" name="userid" value="<?php echo $userid; ?>">
        <span class="error"> <?php echo $useridErr; ?></span>
      <br>        <br>
        First Name:  &emsp;&emsp;&emsp; <input type="text" name="fname" value="<?php echo $fname; ?>">
        <span class="error"> <?php echo $fnameErr; ?></span>
      <br>        <br>
        Last Name:  &emsp;&emsp;&emsp;<input type="text" name="lname" value="<?php echo $lname; ?>">
        <span class="error"> <?php echo $lnameErr; ?></span>
      <br>        <br>
        Department/Office: <input type="text" name="dept_ofc" value="<?php echo $dept_ofc; ?>">
        <span class="error"><?php echo $dept_ofcErr; ?></span>
      <br>
                <br>
        <input type="submit" name="submit" value="Submit">
				  <input name="reset" type="reset" src="images/reset_button.jpg" class="reset_button" />
			<button type="submit" formaction="/Transaction_db/home.php">Back</button>
    </form>
</div>
</center>
</body>
<br>
<br>
<br>
<br>
<br>
<br>
<br>

<footer>
<body onload="display_cdt();"></body>
	    <span id="cdt"></span>
		</footer>
<script type="text/javascript">
    function refresh_cdt() {
        var refresh = 1000; // Refresh rate in milli seconds
        mytime = setTimeout('display_cdt()', refresh)
    }

    function display_cdt() {
        var dateObj = new Date()
        // date part ///
        var dateUTC = dateObj;
        document.getElementById('cdt').innerHTML = dateUTC;
        tt = refresh_cdt();
    }
    </script>

</html>