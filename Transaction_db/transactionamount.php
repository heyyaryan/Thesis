<!DOCTYPE html>
<?php
include('session.php');
if(!isset($_SESSION['login_user'])){
header("location: index.php"); // Redirecting To Home Page
}
?>
<html>
<head>
<title>Welcome to Transaction Page</title>
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
body {
  background-repeat: no-repeat;
   background-size: cover;
   background-image: url("/Transaction_db/image/plain.jpg") ;
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

</style>
</head>
<?php include 'processing.php'?>
<header>
<img src = "/Transaction_db/image/csab.png"  align="left" width="100" height="100"><h1><p class="a">Colegio San Agustin-Bacolod</p></h1>
<h2><p class="b">Cashless Payment System </p></h2>
	&emsp; &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; &emsp;&emsp;&emsp;  
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; &emsp;&emsp;&emsp; &emsp;&emsp;&emsp; &emsp;&emsp;&emsp; &emsp;&emsp; Developed By:<img src = "/Transaction_db/image/coe.png"  align = "right" width="50" height="50">

<img src = "/Transaction_db/image/icpep.png"  align = "right" width="50" height="50">
</header>
<section id ="login">
<b id="welcome"><font face='verdana'>Welcome : <?php echo $login_session; ?></font></b>
<b id="logout"><a href="logout.php"><font color='#000000' ><font face='verdana'>Log Out</font></a></b>
</section>
<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />

		<script src="https://ajax.googleapis.com/ajax/libs/jquery/2.1.3/jquery.min.js"></script>
		<script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/js/bootstrap.min.js"></script>
	</head>
	<body>
		<div class="container">
			<br />
			<center> <b>List of User Transactions</b></center>
			<br />
			<br />
			<div class="form-group">
				<div class="input-group">
				<center>
					<span class="input-group-addon">Search</span>
					<input type="text" name="search_text" id="search_text" placeholder="Search by Transaction Amount" class="form-control" />
					</center>
				</div>
			</div>
			
			<br />
			<center>
			<hr>
			<div id="result"></div>
<hr>
		</div>
		<div style="clear:both"></div>
		<br />
		
		<br />
		<br />
		<br />
	</body>
</html>
	<form method="post" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>">
		<center><button onclick="printContent('result')">Print Transaction</button></center><br>
				<center><button type="submit" formaction="/Transaction_db/transactionamount.php">Reload</button></center><br>
			<center><button type="submit" formaction="/Transaction_db/home.php">Back</button></center>
			</form>
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

<script>
$(document).ready(function(){
	load_data();
	function load_data(query)
	{
		$.ajax({
			url:"transactionamountfetch.php",
			method:"post",
			data:{query:query},
			success:function(data)
			{
				$('#result').html(data);
			}
		});
	}
	
	$('#search_text').keyup(function(){
		var search = $(this).val();
		if(search != '')
		{
			load_data(search);
		}
		else
		{
			load_data();			
		}
	});
});
function printContent(el){
	var restorepage = document.body.innerHTML;
	var printcontent = document.getElementById(el).innerHTML;
	document.body.innerHTML = printcontent;
	window.print();
	document.body.innerHTML = restorepage;
}

</script>




