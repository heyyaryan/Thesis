<?php
$connect = mysqli_connect("localhost", "root", "", "rfidcard_db");
$output = '';
if(isset($_POST["query"]))
{
	$search = mysqli_real_escape_string($connect, $_POST["query"]);
	$query = "
	SELECT * FROM transaction_data 
	WHERE transactionDateTime LIKE '%".$search."%'
	";
}
else
{
	$query = "
	SELECT * FROM transaction_data ORDER BY transactionID";
}
$result = mysqli_query($connect, $query);
if(mysqli_num_rows($result) > 0)
{
	$output .= '<div class="table-responsive">
					<table class="table table bordered">
						<tr>
							<th>Card ID</th>
							<th>Date &Time</th>
							<th>Amount</th>
							<th>Transaction</th>

						</tr>';
	while($row = mysqli_fetch_array($result))
	{
		$output .= '
			<tr>
			<td>'.$row["rfid_num"].'</td>
			<td>'.$row["transactionDateTime"].'</td>
			<td>'.$row["transactionAmount"].'</td>
			<td>'.$row["transactionType"].'</td>
			</tr>
		';
	}
	echo $output;
}
else
{
	echo 'Data Not Found';
}
?>
<html>
<style>
table, th, tr , td{
  border: 1px solid black;
 text-align: center;
}
</style>
</html>