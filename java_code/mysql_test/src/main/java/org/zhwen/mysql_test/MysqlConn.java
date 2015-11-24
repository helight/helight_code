package org.zhwen.mysql_test;

import java.util.ArrayList;
import java.sql.CallableStatement;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import com.mysql.jdbc.PreparedStatement;


public class MysqlConn {
    // public static String host = "jdbc:mysql://localhost:3306/test";//characterEncoding=GBK
    public  String host = "localhost";
    public  String port = "3306";
    public  String dbname = "test";
    public  String user = "root";
    public  String passwd = "root";
    
    public  Connection conn;
    public  Statement stmt;
    private CallableStatement cstmt = null;
    
    public void connect()
    {
    	connect(host, port, dbname, user, passwd);
    }
    
    public void connect(String host, String port, String dbname, String user, String passwd) {
        // 定位驱动
        try {
            Class.forName("com.mysql.jdbc.Driver");
            System.out.println("加载驱动成功!"); 
        } catch (ClassNotFoundException e) {
            System.out.println("加载驱动失败!");
            e.printStackTrace();
        }
        // 建立连接
        try {
        	String conn_url = "jdbc:mysql://" + host + ":" + port + "/" + dbname;
            conn = DriverManager.getConnection(conn_url, user, passwd);
            stmt = conn.createStatement();
            System.out.println("数据库连接成功!"); 
        } catch(SQLException e) {
            System.out.println("数据库连接失败!"); 
        }
    }
    
    public boolean isConnected() {
        return (conn != null);
    }
    
    public ResultSet query(String sql) throws SQLException {
        ResultSet rs = null;
        try {
            if (!isConnected())
                this.connect();
            if (this.stmt == null)
                stmt = conn.createStatement();
            rs = stmt.executeQuery(sql);
            return rs;
        } catch (Exception e) {
            System.err.println("DatabaseBean query error" + e.getMessage());
            e.printStackTrace();
            throw new SQLException("-Query error ");
        }
    }

    public boolean execute(String sql) throws SQLException {
        try {
            if (!isConnected())
                this.connect();
            if (this.stmt == null)
                stmt = conn.createStatement();
            return stmt.execute(sql);
        } catch (Exception e) {
            System.err.println("DatabaseBean execute error" + e.getMessage());
            e.printStackTrace();
            throw new SQLException("-Execute error ");
        }
    }

    public ResultSet queryProcedure(String sql) throws SQLException {
        ResultSet rs = null;
        try {
            if (!isConnected())
                this.connect();
            if (this.cstmt == null)
                cstmt = conn.prepareCall(sql);
            cstmt.executeQuery();
        } catch (Exception e) {
            System.err.println("DatabaseBean callQueryProcedure error" + e.getMessage());
            e.printStackTrace();
            throw new SQLException("-CallQueryProcedure error ");
        }
        return rs;
    }

    public boolean executeProcedure(String sql) throws SQLException {
        try {
            if (!isConnected())
                this.connect();
            if (this.cstmt == null)
                cstmt = conn.prepareCall(sql);
            return cstmt.execute();
        } catch (Exception e) {
            System.err.println("DatabaseBean callExecuteProcedure error"
                    + e.getMessage());
            e.printStackTrace();
            throw new SQLException("-CallExecuteProcedure error ");
        }
    }

    // 批量sql执行
    public void executeBatch(ArrayList<String> sqlBatch) throws SQLException {
        try {
            if (!isConnected())
                this.connect();
            conn.setAutoCommit(false);
            if (this.stmt == null)
                stmt = conn.createStatement();
            for (int i = 0; i < sqlBatch.size(); i++) {
                stmt.addBatch((String) sqlBatch.get(i));
            }
            stmt.executeBatch();
            conn.commit();
            conn.setAutoCommit(true);
        } catch (Exception e) {
            System.err.println("DatabaseBean executeBatch error"
                    + e.getMessage());
            e.printStackTrace();
            throw new SQLException("-ExecuteBatch error ");
        }
    }

    public PreparedStatement createprepareStatement(String paramSQL)
            throws SQLException {
        try {
            if (!isConnected())
                this.connect();
            return (PreparedStatement) conn.prepareStatement(paramSQL);

        } catch (Exception e) {
            System.err.println("DatabaseBean PreparedStatement error");
            e.printStackTrace();
            throw new SQLException("-DatabaseBean PreparedStatement error ");
        }
    }

    public void transactionStart() throws SQLException {
        try {
            conn.setAutoCommit(false);
        } catch (SQLException e) {
            System.err.println("DatabaseBean transactionStart");
            e.printStackTrace();
            throw new SQLException("-Transaction start error ");
        }
    }

    public void transactionEnd() throws SQLException {
        try {
            conn.commit();
            conn.setAutoCommit(true);
        } catch (SQLException e) {
            System.err.println("DatabaseBean transactionEnd error");
            e.printStackTrace();
            throw new SQLException("-Transaction end error:");
        }
    }

    public void transactionRollback() throws SQLException {
        try {
            conn.rollback();
        } catch (SQLException e) {
            System.err.println("DatabaseBean transactionRollback error");
            e.printStackTrace();
            throw new SQLException("-Transaction rollback error ");
        }
    }

    public void disconnect() throws SQLException {
        try {
            if (stmt != null) {
                stmt.close();
                stmt = null;
            }

            if (conn != null) {
                conn.close();
                conn = null;
            }
        } catch (SQLException e) {
            System.err.println("DatabaseBean disconnect error");
            e.printStackTrace();
            throw new SQLException("-Database disConnection error ");
        }
    }
}
