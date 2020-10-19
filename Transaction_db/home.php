<!DOCTYPE html>
<?php
include('session.php');
if(!isset($_SESSION['login_user'])){
header("location: index.php"); // Redirecting To Home Page
}
?>
<html lang="en">
<head>
<title>Welcome to Home Page</title>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>

* {
  box-sizing: border-box;
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
body {font-family: Arial, Helvetica, sans-serif;}

/* Style the footer */
footer {
  background-color: #777;
  padding: 10px;
  text-align: center;
  color: white;
}

body {
  background-repeat: no-repeat;
   background-size: cover;
   background-image: url("/Transaction_db/image/plain.jpg") ;
}


#login {
    background-color: lightgrey;
    }
.button {
  background-color: red;
  border: none;
  color: white;
  padding: 15px 90px;
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
  padding: 15px 50px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
  font-size: 16px;
  margin: 4px 2px;
  cursor: pointer;
  font-family: Arial, Helvetica, sans-serif
}


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
</style>
</head>
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

	<center><br> 
   <a href='/Transaction_db/test_add.php' class="button">Add Account</a>
	<br></br>  
 <a href='/Transaction_db/test_addmoney.php' class="button">Load Card</a>
  <br></br> 
<button id="myBtn" data-toggle="modal" data-target="#myModal4">View/Print Transactions</button></center>
</body>

<div id="myModal" class="modal">>
    <div class="modal-dialog">
    
      <!-- Modal content-->
      <div class="modal-content" align = "center">
        <div class="modal-header">
          <button type="button" class="close" data-dismiss="modal">&times;</button>
          <h4 class="modal-title">Search by:</h4>
        </div>
        <div class="modal-body">
         <a href='/Transaction_db/transactionid.php'><button type="button" class="button" data-toggle="modal" data-target ="#myModal" >Transaction ID</button></a>&nbsp;
		  <a href='/Transaction_db/transactiondatetime.php'><button type="button" class="button" data-toggle="modal" data-target="#myModal">Transaction Date & Time</button></a>&nbsp;
		  <a href='/Transaction_db/rfid.php'><button type="button" class="button" data-toggle="modal" data-target="#myModal">RFID Number</button></a>&nbsp;<br><br>
		   <a href='/Transaction_db/transactionamount.php'><button type="button" class="button" data-toggle="modal" data-target="#myModal">Transaction Amount</button></a>&nbsp;
		   <a href='/Transaction_db/transactiontype.php'><button type="button" class="button" data-toggle="modal" data-target="#myModal">Transaction Type</button></a>&nbsp;

	
        </div>
    
      </div>
      
    </div>
  </div>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>


<footer>
<script>
// Get the modal
var modal = document.getElementById("myModal");

// Get the button that opens the modal
var btn = document.getElementById("myBtn");

// Get the <span> element that closes the modal
var span = document.getElementsByClassName("close")[0];

// When the user clicks the button, open the modal 
btn.onclick = function() {
  modal.style.display = "block";
}

// When the user clicks on <span> (x), close the modal
span.onclick = function() {
  modal.style.display = "none";
}

// When the user clicks anywhere outside of the modal, close it
window.onclick = function(event) {
  if (event.target == modal) {
    modal.style.display = "none";
  }
}
</script>
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
	
	<body onload="display_cdt();"></body>
	    <span id="cdt"></span>
		</footer>


</html>