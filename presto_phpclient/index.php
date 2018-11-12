<?php
/*
Copyright 2013 Xtendsys | xtendsys.net

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at:

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.*/


require_once(__DIR__ . '/PrestoClient.php');

//Create a new connection object. Provide URL and catalog as parameters
$presto = new PrestoClient("http://localhost:8080/v1/statement","mysql");

$str_sql = "select * from mysql.helight.MyClass a, mysql2.helight.myclass b where a.id =b.id";
//Prepare your sql request
try {
    // $presto->Query("select count(*) from hive.default.my_table");
    $presto->Query($str_sql);
} catch (PrestoException $e) {
    var_dump($e);
}

//Execute the request and build the result
if ($presto->WaitQueryExec()) {
    $res = $presto->GetResult()->columns; //[0]->name;
    foreach($res as $key=>$value)  {
        $a[$key]["name"]=$value->name;
        $a[$key]["type"]=$value->type;
    }

    foreach($a as $key=>$value)  {
        //print_r($key."\n".$value["type"]."\n".$value["name"]."\n");
        $colum[$key]=array("DataType"=>"System.".$value["type"],"Index"=>$key,"Name"=>$value["name"],"DisplayName"=>null);
        $colmap[$key]=array("ColumnName"=>$value["name"],"Description"=>"","DisplayName"=>"");
    }

    //print_r($res);
    //Get the result
    $data=array();
    $rows=array();
    $answer = $presto->GetData();
    foreach($answer as $row=>$value1) { //get rows
        foreach($value1 as $col=>$value2)    {
            $data[$col]=array("ColumnName"=>$a[$col]["name"],"Value"=>$value2);
            $item=array("ItemArray"=>$data);
        }
        $rows[$row]=$item;
        //$rows=array("Rows"=&gt;$tmp);
    }
    $table[0]=array("Name"=>"sql","Sql"=>$str_sql,"Status"=>"0","ColumnMapping"=>$colmap,"Columns"=>$colum,"Rows"=>$rows);
    $output=array("HasError"=>"false","Message"=>"","Tables"=>$table);
    print_r(json_encode($output));
} else {
    #$output=array("HasError"=&gt;"true","Message"=&gt;"table is not found!","Tables"=&gt;array());
    $output=array("HasError"=>"true","Message"=>$err,"Tables"=>array());
    print_r(json_encode($output));
}

?>
