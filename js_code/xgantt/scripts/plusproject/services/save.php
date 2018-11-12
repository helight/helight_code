<?php
	
if($_POST['project']) {
	$p = $_POST['project'];
	//echo $p,"<br>";
	$qjson = json_decode($p);
	$uid = $qjson->UID;
	$pname = $qjson->Name;
	//var_dump($uid);
		
	$conn = mysql_connect("localhost","root","xxxxxx");
	if (!$conn) {
		die('Could not connect: ' . mysql_error());
	}
	mysql_select_db("helight" ,$conn) or die ("找不到数据源");

	$msql = "select id from project_gantt where id=".$uid;
	$result = mysql_query($msql);

	if (mysql_Num_rows($result) > 0) {
		$msql = "update project_gantt set data='".$p."', name='".$pname."' where id=".$uid;
		$result = mysql_query($msql);
	} else {
		$msql = "INSERT INTO project_gantt (id, name, data) VALUES (".$uid.",'".$pname."','".$p."')";
		$result = mysql_query($msql);
	}
	if (!$result) {   
	　　die('Error: ' . mysql_error()); 
	} else 
		echo "Save OK"; 

	mysql_close($conn);

} else{
	return 0;
}
