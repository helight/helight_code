package org.zhwen.mysql_test;

import java.sql.ResultSet;  


public class App 
{
    public static void main( String[] args )
    {
        System.out.println( "Hello World!" );
        test_mysql();
        System.out.println( "Bye World!" );
    }
    
    
    public static void test_mysql()
    {
        try  
        {  
        	MysqlConn mysql_connConn = new MysqlConn();
        	
        	mysql_connConn.connect("10.222.0.160", "3306", "idata_server_test", "idata","idata");

        	ResultSet rs = mysql_connConn.query("SELECT * from tbJobType");
        	
        	while (rs.next())   
            {   
                 System.out.println(rs.getString(2));  
            }
        }  
        catch(Exception ex)  
        {  
            ex.printStackTrace();  
        } 
    }
}
