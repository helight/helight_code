#!/bin/bash

sparksubmit="/data/spark/spark_hadoop/bin/spark-submit"
sparkmaster="spark://helight-xu:7077"
sqltask="org.zhwen.sparkproject.SqlTask"
jarfile="./target/sparkproject-0.0.1-jar-with-dependencies.jar"
tables="\"1000015522_qq:tb_1000015522:PrimaryKey,string:,;1000015573_qq_mix:tb_1000015573:PrimaryKey,string:gender,string:regtime,string:qqlevel,int:,\""
sql="\"select tb_1000015522.PrimaryKey,tb_1000015573.gender,tb_1000015573.regtime,tb_1000015573.qqlevel from tb_1000015522 left outer join tb_1000015573 on tb_1000015522.PrimaryKey=tb_1000015573.PrimaryKey order by tb_1000015573.gender asc\""
targetfield="\"PrimaryKey,string:gender,string:regtime,string:qqlevel,int\""
targetpkg="\"10000015572\""

ssql=`echo $sql|sed "s/ /#/g"`
echo $ssql

submitcmd="$sparksubmit --master $sparkmaster --class $sqltask $jarfile -t $tables -s $sql -c $targetfield -o $targetpkg"
echo $submitcmd
$submitcmd

#/data/spark/spark_hadoop/bin/spark-submit --master spark://helight-xu:7077 --class org.zhwen.sparkproject.SqlTask  ./target/sparkproject-0.0.1-jar-with-dependencies.jar 1000015522_qq:tb_1000015522:PrimaryKey,string:,;1000015573_qq_mix:tb_1000015573:PrimaryKey,string:gender,string:regtime,string:qqlevel,int:, select tb_1000015522.PrimaryKey,tb_1000015573.gender,tb_1000015573.regtime,tb_1000015573.qqlevel from tb_1000015522 PrimaryKey,string:gender,string:regtime,string:qqlevel,int 10000015572
