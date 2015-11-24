// Copyright (c) 2015, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2015-11-19
// Description:
//

package main

import (
	// 	"bufio"
	"fmt"
	//	"io"
	"os"
	"os/exec"
)

func main() {

	sparksubmit := "/data/spark/spark_hadoop/bin/spark-submit"
	sparkmaster := "spark://helight-xu:7077"
	sqltask := "org.zhwen.sparkproject.SqlTask"
	jarfile := "./target/sparkproject-0.0.1-jar-with-dependencies.jar"
	tables := "1000015522_qq:tb_1000015522:PrimaryKey,string:,;1000015573_qq_mix:tb_1000015573:PrimaryKey,string:gender,string:regtime,string:qqlevel,int:,"
	sql := "select tb_1000015522.PrimaryKey,tb_1000015573.gender,tb_1000015573.regtime,tb_1000015573.qqlevel from tb_1000015522 left outer join tb_1000015573 on tb_1000015522.PrimaryKey=tb_1000015573.PrimaryKey order by tb_1000015573.gender asc"
	targetfield := "PrimaryKey,string:gender,string:regtime,string:qqlevel,int"
	targetpkg := "10000015572"

	params := []string{}

	// submitcmd="$sparksubmit --master $sparkmaster --class $sqltask $jarfile -t $tables -s $sql -c $targetfield -o $targetpkg"
	params = append(params, "--master", sparkmaster, "--class", sqltask)
	params = append(params, jarfile)
	params = append(params, "-t", "\""+tables+"\"", "-s", "\""+sql+"\"")
	params = append(params, "-c", "\""+targetfield+"\"", "-o", "\""+targetpkg+"\"")

	fmt.Println(sparksubmit, " ", params)

	execCommand(sparksubmit, params)
}

func execCommand(commandName string, params []string) bool {
	cmd := exec.Command(commandName, params...)

	// print cmd
	fmt.Println(cmd.Args)

	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr

	cmd.Start()

	cmd.Wait()

	fmt.Println("run ok")

	return true
}
