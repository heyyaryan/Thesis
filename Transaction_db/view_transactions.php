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
<?php include 'processing.php'?>
<!doctype html>
<html lang="en">

<head>
    <!-- Required meta tags -->
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">

    <title>View Transactions</title>

    <style>
    .error {
        color: #FF0000;
    }
    </style>
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
</head>

<body onload="display_cdt();">
    <?php
$transactionIDErr = $transactionDateErr = $transactionTimeErr = $rfid_numErr = $transactionAmountErr = $transactionTypeErr = "";
$transactionID = $transactionDate = $transactionTime = $rfid_num = $transactionAmount = $transactionType = "";
$searchFilter="";
$errorCount = 0;
$generalMessage = "";
$userid = $fname = $lname = "";
$userid_current = $fname_current = $lname_current = $rfid_num_current = "";
$queryResult = array(array());
if ($_SERVER['REQUEST_METHOD'] == "POST") {
     if (isset($_POST['search'])){
         viewAll($queryResult);
         print_r($queryResult);
     }
    check data validity
     if (isset($_POST['verify'])) {
         if (empty($_POST['rfid_num'])) {
             $rfid_numErr = "RFID is empty.";
         } else {
             $rfid_num = clean_input($_POST["rfid_num"]);
         }
         verifyRFID($rfid_num);
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
             $generalMessage = "No matches.";

         }
     }

     if (isset($_POST['submit'])) {
         $rfid_num_current = clean_input($_POST['rfid_num_v']);
         if (empty($_POST['userid_v'])) {
             $generalMessage = "Verify user first.";
             $errorCount += 1;
         } else {
             $userid_current = clean_input($_POST['userid_v']);
         }

         if (empty($_POST['fname_v'])) {
             $generalMessage = "Verify user first.";
             $errorCount += 1;
         } else {
             $fname_current = clean_input($_POST['fname_v']);
         }

         if (empty($_POST['lname_v'])) {
             $generalMessage = "Verify user first.";
             $errorCount += 1;
         } else {
             $lname_current = clean_input($_POST['lname_v']);
         }

         if (empty($_POST['transactionAmount'])) {
             $transactionAmountErr = "Amount is empty.";
             $errorCount += 1;
         } else {
             $transactionAmount = clean_input($_POST["transactionAmount"]);
             $transactionAmountErr = validate_num($transactionAmount);
             if ($transactionAmountErr != "") {
                 $errorCount += 1;
             }

         }

         if ($errorCount == 0) {
             determine last transaction id
             $lastID = 0;
             $sql_st = "SELECT transactionID FROM transaction_data ORDER BY transactionID DESC LIMIT 1";
             $result = $conn->query($sql_st);
             if ($result->num_rows > 0) {
                 while ($row = $result->fetch_assoc()) {
                     $lastID = $row['transactionID'];
                 }
                 mysqli_free_result($result);
                 add one to last id
                 $lastID += 1;

                 insert record to database
                 $sql_st = "INSERT INTO transaction_data (transactionID, transactionDateTime, rfid_num,transactionAmount,transactionType)
                  VALUES ($lastID,NOW(),'$rfid_num_current',$transactionAmount,'AM' )";

                 if ($conn->query($sql_st) === true) {
                     $generalMessage = "Add money successful.";
                     $rfid_num_current= $fname_current = $lname_current = $userid_current = $transactionAmount = "";

                 } else {
                     echo "Error: " . $sql_st . "<br>" . $conn->error;
                 }

                 $sql_st = "UPDATE user_data SET balance=" . $transactionAmount . " WHERE rfid_num='" . $rfid_num_current . "' && userid='" . $userid_current . "'";

                 if ($conn->query($sql_st) == true) {
                     $generalMessage = "Add money successful.";
                     $rfid_num_current = $fname_current = $lname_current = $userid_current = $transactionAmount = "";
                 } else {
                     echo "Error: " . $sql_st . "<br>" . $conn->error;
                 }
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
        $nameErr = "Only letters and white space allowed";

    } else {
        $nameErr = "";

    }
    return $nameErr;
}
function validate_num($data)
{
    if (!preg_match("/^[0-9]*$/", $data)) {
        $nameErr = "Invalid amount.";

    } else {
        $nameErr = "";

    }
    return $nameErr;
}

function viewResult($ttype,$filt,$cat,$order){
$servername = "localhost";
$username = "root";
$password = "";
$dbase="rfidcard_db";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbase);
$sql="";
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}
if($filt == 1){
switch($ttype){
    case "*":
        $sql = "SELECT transactionID, transactionDateTime, rfid_num,transactionAmount,transactionType FROM transaction_data ORDER BY transactionDateTime ". $_POST['searchOrder'] . "";
    break;
    case "AM":
        $sql = "SELECT transactionID, transactionDateTime, rfid_num,transactionAmount,transactionType FROM transaction_data WHERE transactionType='AM' ORDER BY transactionDateTime ". $_POST['searchOrder'] . "";
    break;
    case "PR":
        $sql = "SELECT transactionID, transactionDateTime, rfid_num,transactionAmount,transactionType FROM transaction_data WHERE transactionType='PR' ORDER BY transactionDateTime ". $_POST['searchOrder'] . "";
}
}else{
    $sql = "SELECT transactionID, transactionDateTime, rfid_num,transactionAmount,transactionType FROM transaction_data WHERE transactionType='". $ttype ."' && '". $cat . "'='". $filt ."' ORDER BY transactionDateTime ". $_POST['searchOrder'] . "";
    echo $cat;
    echo $filt;
}

$result = $conn->query($sql);

if ($result->num_rows > 0) {
    // output data of each row
    while($row = $result->fetch_assoc()) {
        echo "Transaction ID: " . $row["transactionID"]. " Date and Time " . $row["transactionDateTime"]. " RFID: " . $row["rfid_num"]. " Transaction Amount: ". $row["transactionAmount"] . " Transaction Type: ". $row["transactionType"] . "<br>";
        
            
        }
    
    
} else {
    echo "0 results";
}
$conn->close();

}
?>
    <h2>View Transactions</h2>
    <span id="cdt"></span>
    <p>Modify search parameters.</p>
    <!-- <?php echo "Errorcount:" . $errorCount . "<br />"; ?> -->
        <span class="error"><?php echo $generalMessage . "<br />"; ?></span>
    <br />
    <form method="post" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>">
        View what: 
        <input type="radio" name="transactionType" <?php if (isset($transactionType) && $transactionType == "*") echo "checked";?> value="*" checked="true">All
        <input type="radio" name="transactionType" <?php if (isset($transactionType) && $transactionType == "AM") echo "checked";?> value="AM">Add Money
        <input type="radio" name="transactionType" <?php if (isset($transactionType) && $transactionType == "PR") echo "checked";?> value="PR">Purchases
        <span class="error"> <?php echo $transactionTypeErr; ?></span>
        <br /><br />
        Search Filter:
        <input type="text" name="searchFilter" value="<?php echo $searchFilter; ?>">
        <br />
        Filter by:
        <input type="radio" name="filterCategory" <?php if (isset($filterCategory) && $filterCategory == "*") echo "checked";?> value="1" checked="true">None
        <input type="radio" name="filterCategory" <?php if (isset($filterCategory) && $filterCategory == "transactionID") echo "checked";?> value="transactionID">Transaction ID
        <input type="radio" name="filterCategory" <?php if (isset($filterCategory) && $filterCategory == "rfid_num") echo "checked";?> value="rfid_num">RFID
        <input type="radio" name="filterCategory" <?php if (isset($filterCategory) && $filterCategory == "transactionDate") echo "checked";?> value="transactionDate">Transaction Date
        <br />
        Order by:
        <input type="radio" name="searchOrder" <?php if (isset($searchOrder) && $searchOrder == "DESC") echo "checked";?> value="DESC" checked="true">Descending
        <input type="radio" name="searchOrder" <?php if (isset($searchOrder) && $searchOrder == "ASC") echo "checked";?> value="ASC">Ascending
        <br /><br />
        <input type="submit" name="search" value="Search">
        <br /><br />
       
        <br /><br />
    </form>

    
</body>
<?php
//print_r($queryResult);
if ($_SERVER['REQUEST_METHOD'] == "POST") {
    $transactionType = clean_input($_POST['transactionType']);
    if(empty($_POST['searchFilter'])){
        $searchFilter = 1;
    }else{
        $searchFilter = clean_input($_POST['searchFilter']);
    }
    if(empty($_POST['filterCategory'])){
        $filterCategory = 1;
    } else {
        $filterCategory = clean_input($_POST['filterCategory']);
        echo $filterCategory;
    }
    if(empty($_POST['searchOrder'])){
        $searchOrder = 'DESC';
    }else {
        $searchOrder = clean_input($_POST['searchOrder']);
    }
    if (isset($_POST['search'])){
        viewResult($transactionType,$searchFilter,$filterCategory,$searchOrder);
        
    }
}
?>
</html>