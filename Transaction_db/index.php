<?php
include('login.php'); // Includes Login Script
if(isset($_SESSION['login_user'])){
header("location: home.php"); // Redirecting To Profile Page
}
?>
<!DOCTYPE html>
<html lang="en">
<head>
<title>Welcome to Login Page</title>
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

footer {
  font-family: Arial, Helvetica, sans-serif;
  background-color: #777;
  padding: 10px;
  text-align: left;
  color: white;
}


div {
  background-color: lightgrey;
  width: 300px;
  height: 307px;
  border: 15px;
  padding: 50px;
  margin: 20px;
  
}
body {font-family: Arial, Helvetica, sans-serif;}

</style>
</head>
<body>


<header>
<img src = "/Transaction_db/image/csab.png"  align="left" width="100" height="100"><h1><p class="a">Colegio San Agustin-Bacolod</p></h1>
<h2><p class="b">Cashless Payment System </p></h2>
&emsp; &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; &emsp;&emsp;&emsp;  
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; &emsp;&emsp;&emsp; &emsp;&emsp;&emsp; &emsp;&emsp;&emsp; &emsp;&emsp;  Developed By:<img src = "/Transaction_db/image/coe.png"  align = "right" width="50" height="50">

<img src = "/Transaction_db/image/icpep.png"  align = "right" width="50" height="50">
</header>
<body >
<center><div id="login" align = "center">
<font face='verdana'><h2>LOGIN</h2>
<form action="" method="post">
<label>UserName </label>
<input id="name" name="username" placeholder="username" type="text">
<label>Password </label></font>

<input id="password" name="password" placeholder="**********" type="password"><br><br>
<input name="submit" type="submit" value=" Login ">

<span><?php echo $error; ?></span>


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
<br>

<footer>
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