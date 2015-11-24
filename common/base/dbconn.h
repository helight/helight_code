#include "mysql/mysql.h"

#ifndef ulong
#define ulong unsigned long
#endif

#ifndef uint
#define uint unsigned int
#endif

#ifndef DATACONN_H_BBS
#define DATACONN_H_BBS
class dbconn
{
public:
	dbconn(char* szHost,char* szDB,char* szUser,char* szPass,uint szPort)//connect to mysql
	{
		this->_szHost=szHost;
		this->_szDB=szDB;
		this->_szUser=szUser;
		this->_szPass=szPass;
		this->_szPort = szPort;
		this->_bConnect = false;
	}
	~dbconn()
	{
		if (_bConnect)
		{
			close();
		}
	}

public:
	bool connect();
	bool FreeResult();
	bool StoreResult();
	char** FetchRow();//获取当前行
	bool close();
	void output();
	ulong GetAffectedRows();//返回结果的行数
	ulong GetResultRows();//获得结果返回的行数
	uint GetFieldsCount();//获得返回的列数
	void DataSeek(uint offset);//将游标移动到offset行
	MYSQL_ROW_OFFSET GetCursor();//得到光标的当前位置，注意不是游标，不是行号
	ulong* GetFieldsLengthArray();//得到返回表的每一列的长度数组
	void GetFieldsName();//获得返回的所有列的名称

	MYSQL_ROW*  SelectEx(const char* szSQL);
	bool Select(const char* szSQL);//根据传入的sql语句进行操作
	bool Insert(const char* szSQL);
	bool Update(const char* szSQL);
	bool Delete(const char* szSQL);
	ulong getVisiteHostCount(char* hostName,char* startYear,char* endYear,char* url);
	ulong getRecordNum(char* szSQL);
	ulong getLimitNum(char* szSQL);
	char* getEarlyAndLateTime(int type,char* url,char* startYear,char* endYear);//0 early, 1 late

private:
	MYSQL			_SQLConn;
	MYSQL_RES		*_SQLResult; //用于保存查询后的结果集
	MYSQL_ROW		_SQLRow;
	MYSQL_FIELD		*_SQLField;
	int				_fieldCount;//查询返回表的列数
	bool			_bConnect;

	char			*_szHost;
	char			*_szDB;
	char			*_szUser;
	char			*_szPass;
	uint			_szPort;//连接端口
};
#endif
