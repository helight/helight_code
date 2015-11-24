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
	char** FetchRow();//��ȡ��ǰ��
	bool close();
	void output();
	ulong GetAffectedRows();//���ؽ��������
	ulong GetResultRows();//��ý�����ص�����
	uint GetFieldsCount();//��÷��ص�����
	void DataSeek(uint offset);//���α��ƶ���offset��
	MYSQL_ROW_OFFSET GetCursor();//�õ����ĵ�ǰλ�ã�ע�ⲻ���α꣬�����к�
	ulong* GetFieldsLengthArray();//�õ����ر��ÿһ�еĳ�������
	void GetFieldsName();//��÷��ص������е�����

	MYSQL_ROW*  SelectEx(const char* szSQL);
	bool Select(const char* szSQL);//���ݴ����sql�����в���
	bool Insert(const char* szSQL);
	bool Update(const char* szSQL);
	bool Delete(const char* szSQL);
	ulong getVisiteHostCount(char* hostName,char* startYear,char* endYear,char* url);
	ulong getRecordNum(char* szSQL);
	ulong getLimitNum(char* szSQL);
	char* getEarlyAndLateTime(int type,char* url,char* startYear,char* endYear);//0 early, 1 late

private:
	MYSQL			_SQLConn;
	MYSQL_RES		*_SQLResult; //���ڱ����ѯ��Ľ����
	MYSQL_ROW		_SQLRow;
	MYSQL_FIELD		*_SQLField;
	int				_fieldCount;//��ѯ���ر������
	bool			_bConnect;

	char			*_szHost;
	char			*_szDB;
	char			*_szUser;
	char			*_szPass;
	uint			_szPort;//���Ӷ˿�
};
#endif
