<?php
//$uid = $_GET['UID'];
if(isset($_GET['projectuid'])){
	$uid = $_GET['projectuid'];
	//var_dump($uid);
	$conn = mysql_connect("localhost","root","xxxxxx");
	if (!$conn) {
		die('Could not connect: ' . mysql_error());
	}
	mysql_select_db("helight" ,$conn) or die ("找不到数据源");

	$msql = "select data from project_gantt where id=".$uid;
	$result = mysql_query($msql);
	//var_dump($result);
	if (mysql_Num_rows($result) > 0) {
		$row = mysql_fetch_array($result);
		
		echo $row["data"];
	} else {  
	　　die('Error: ' . mysql_error()); 
	} 

	mysql_close($conn);
}
