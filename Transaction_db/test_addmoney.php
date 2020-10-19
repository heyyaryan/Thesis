<!DOCTYPE html>
<?php
include('session.php');
if(!isset($_SESSION['login_user'])){
header("location: index.php"); // Redirecting To Home Page
}
?>
<html>
<head>
<title>Welcome to Add Money Page</title>
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
  height: 510px;
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


    <?php
$transactionIDErr = $transactionDateErr = $transactionTimeErr = $rfid_numErr = $transactionAmountErr = $transactionTypeErr = "";
$transactionID = $transactionDate = $transactionTime = $rfid_num = $transactionAmount = $transactionType = "";

$errorCount = 0;
$generalMessage = "";
$userid = $fname = $lname = "";
$userid_current = $fname_current = $lname_current = $rfid_num_current = "";
//$_SERVER['REQUEST_METHOD'] == "POST"
if ($_SERVER['REQUEST_METHOD'] == "POST") {

    //check data validity
    if (isset($_POST['verify'])) {
        if (empty($_POST['rfid_num'])) {
            $rfid_numErr = "<font color=\"red\">RFID is empty.</font>";
        } else {
            $rfid_num = clean_input($_POST["rfid_num"]);
        }
        //verifyRFID($rfid_num);
        $sql_st = "SELECT userid,fname,lname FROM user_data WHERE rfid_num='" . $rfid_num . "'";
        $result = $conn->query($sql_st);

        if ($result->num_rows > 0) {
            while ($row = $result->fetch_assoc()) {
                $userid = $row['userid'];
                $fname = $row['fname'];
                $lname = $row['lname'];
            }

            $rfid_num_current = $rfid_num;
            $userid_current = clean_input($userid);
            $fname_current = clean_input($fname);
            $lname_current = clean_input($lname);
        } else {
            $generalMessage = "<font color=\"red\">No matches.</font>";

        }
    }

    if (isset($_POST['submit'])) {
        $rfid_num_current = clean_input($_POST['rfid_num_v']);
        if (empty($_POST['userid_v'])) {
            $generalMessage = "<font color=\"red\">Verify user first.</font>";
            $errorCount += 1;
        } else {
            $userid_current = clean_input($_POST['userid_v']);
        }

        if (empty($_POST['fname_v'])) {
            $generalMessage = "<font color=\"red\">Verify user first.</font>";
            $errorCount += 1;
        } else {
            $fname_current = clean_input($_POST['fname_v']);
        }

        if (empty($_POST['lname_v'])) {
            $generalMessage = "<font color=\"red\">Verify user first.</font>";
            $errorCount += 1;
        } else {
            $lname_current = clean_input($_POST['lname_v']);
        }

        if (empty($_POST['transactionAmount'])) {
            $transactionAmountErr = "<font color=\"red\">Amount is empty.</font>";
            $errorCount += 1;
        } else {
            $transactionAmount = clean_input($_POST["transactionAmount"]);
            $transactionAmountErr = validate_num($transactionAmount);
            if ($transactionAmountErr != "") {
                $errorCount += 1;
            }

        }

        if ($errorCount == 0) {
            //determine last transaction id
            $lastID = 0;
            $prevbal = 0.00;
            $sql_st = "SELECT transactionID FROM transaction_data ORDER BY transactionID DESC LIMIT 1";
            $result = $conn->query($sql_st);
            if ($result->num_rows > 0) {
                while ($row = $result->fetch_assoc()) {
                    $lastID = $row['transactionID'];
                }
                mysqli_free_result($result);
                //add one to last id
                $lastID += 1;
            }
			
				$curdate = "DATE_FORMAT(NOW(), '%e %M %Y %r')";
                //insert record to database
                $sql_st = "INSERT INTO transaction_data (transactionID, transactionDateTime, rfid_num,transactionAmount,transactionType)
                 VALUES ($lastID,$curdate,'$rfid_num_current',$transactionAmount,'Add Money')";

                if ($conn->query($sql_st) === true) {
                    $generalMessage = "<font color=\"green\">Add money successful.</font>";
                    //$rfid_num_current= $fname_current = $lname_current = $userid_current = $transactionAmount = "";

                } else {
                    echo "Error: " . $sql_st . "<br>" . $conn->error;
                }
                //$transactionAmount = $transactionAmount + $prevbal;
                $sql_st = "SELECT balance FROM user_data WHERE rfid_num='".$rfid_num_current."'";

                $result = $conn->query($sql_st);
            if ($result->num_rows > 0) {
                while ($row = $result->fetch_assoc()) {
                    $prevbal = $row['balance'];
                }
                mysqli_free_result($result);
                //add one to last id
                $transactionAmount += $prevbal;
            }

                //update with new value.
                $sql_st = "UPDATE user_data SET balance=" . $transactionAmount . " WHERE rfid_num='" . $rfid_num_current . "' && userid='" . $userid_current . "'";

                if ($conn->query($sql_st) == true) {
                    $generalMessage = "<font color=\"green\">Add money successful.</font>";
                    $rfid_num_current = $fname_current = $lname_current = $userid_current = $transactionAmount = "";
                } else {
                    echo "Error: " . $sql_st . "<br>" . $conn->error;
                }
            

            $conn->close();
        }
    }

}

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
function validate_num($data)
{
    if (!preg_match("/^[0-9]*$/", $data)) {
        $nameErr = "<font color=\"red\">Invalid amount.</font>";

    } else {
        $nameErr = "";

    }
    return $nameErr;
}

?>
<center>
<div id="register" align = "center">
    <h2>Load Card Form</h2>
	
    <p>Please fill up all fields.</p>
    <!-- <?php echo "Errorcount:" . $errorCount . "<br />"; ?> -->
        <span class="error"><?php echo $generalMessage . "<br />"; ?></span>
    <form method="post" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>">
        RFID: <input type="text" name="rfid_num" value="<?php echo $rfid_num; ?>">
        <span class="error"> <?php echo $rfid_numErr; ?></span>
        <br /><br />
        <input type="submit" name="verify" value="Verify">
		<button onclick="document.getElementByname('rfid_num').value = ''">Clear</button>
        <br /><br />
        <!-- User ID: <input type="text" name="userid" value="<?php echo $userid ?>" disabled="true">
        First Name: <input type="text" name="fname" value="<?php echo $fname ?>" disabled="true">
        Last Name: <input type="text" name="lname" value="<?php echo $lname ?>" disabled="true"> -->
    </form>

    <form method="post" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>">
    <input type="hidden" name="rfid_num_v" value="<?php echo $rfid_num_current ?>">
   User ID:&emsp; <input type="text" name="userid_v" value="<?php echo $userid_current ?>" readonly="true">
    <br /><br />
    First Name: <input type="text" name="fname_v" value="<?php echo $fname_current ?>" readonly="true">
    <br /><br />
    Last Name: <input type="text" name="lname_v" value="<?php echo $lname_current ?>" readonly="true">
    <br /><br />
    Amount: &emsp;<input type="text" name="transactionAmount" value="<?php echo $transactionAmount; ?>">
        <span class="error"> <?php echo $transactionAmountErr; ?></span>
        <br /><br />
        <input type="submit" name="submit" value="Add Money">
			<button type="submit" formaction="/Transaction_db/home.php">Back</button>
        </form>
		</div>
</center>
</body>
<br>
<br>
<br>
<footer>
<body onload="display_cdt();"></body>
	    <span id="cdt"></span>
		</footer>
<?php
//for monitoring variables. Can be deleted.
//echo var_dump(empty($userid))."<br />";
// echo $rfid_num . "<br />";
// echo $userid . "<br />";
// echo $fname . "<br />";
// echo $lname . "<br />";
// echo $transactionAmount . "<br />";
?>
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