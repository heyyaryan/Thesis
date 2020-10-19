<?php

if(isset($_POST['search']))
{
    $valueToSearch = $_POST['valueToSearch'];
    // search in all table columns
    // using concat mysql function
    $query = "SELECT * FROM `transaction_data` WHERE CONCAT( `id_base`, `transactionID`, `transactionDateTime`, `rfid_num`, `transactionAmount`) LIKE '%".$valueToSearch."%'";
    $search_result = filterTable($query);
    
}
 else {
    $query = "SELECT * FROM `transaction_data`";
    $search_result = filterTable($query);
}

// function to connect and execute the query
function filterTable($query)
{
    $connect = mysqli_connect("localhost", "root", "", "rfidcard_db");
    $filter_Result = mysqli_query($connect, $query);
    return $filter_Result;
}

?>

<!DOCTYPE html>
<html>
    <head>
        <title>PHP HTML TABLE DATA SEARCH</title>
        <style>
            table,tr,th,td
            {
                border: 1px solid black;
            }
        </style>
    </head>
    <body>
        
        <form action="view_transactions1.php" method="post">
            <input type="text" name="valueToSearch" placeholder="Value To Search"><br><br>
            <input type="submit" name="search" value="Filter"><br><br>
            
            <table>
                <tr>
                    <th>ID Base</th>
                    <th>Transaction ID</th>
                    <th>Transaction Date and Time</th>
                    <th>RFID Number</th>
					<th>Transaction Amount</th>
                </tr>

      <!-- populate table from mysql database -->
                <?php while($row = mysqli_fetch_array($search_result)):?>
                <tr>
                    <td><?php echo $row['id_base'];?></td>
                    <td><?php echo $row['transactionID'];?></td>
                    <td><?php echo $row['transactionDateTime'];?></td>
                    <td><?php echo $row['rfid_num'];?></td>
					<td><?php echo $row['transactionAmount'];?></td>
                </tr>
                <?php endwhile;?>
            </table>
        </form>
        
    </body>
</html>
