#include "dbconn.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

bool dbconn::connect()
{
	mysql_init(&this->_SQLConn);

	if (mysql_real_connect(&this->_SQLConn, this->_szHost, this->_szUser,
                           this->_szPass,this->_szDB, this->_szPort, NULL, 0)==NULL)
	{
		return false;
	}
	mysql_query(&_SQLConn, "SET NAMES utf8");

	this->_bConnect = true;

	return true;
}

bool dbconn::Select(const char* szSQL)
{
	bool success = false;
	if (szSQL == NULL) return false;
	if (connect()){
		int Ret = mysql_query(&this->_SQLConn,szSQL);
		if (Ret != 0) {//Ê§°Ü
			//printf("Content-type:text/html;charset=utf-8 \n\n");
			//printf("<MySQL-WARNING> DB real query fail:%s</MySQL-WARNING>\n", mysql_error(&this->_SQLConn));
			success = false;
		} else {
			success = true;
		}
	}
	return success;
}

ulong dbconn::getRecordNum(char* szSQL)
{
	ulong count = 0;
	if(Select(szSQL))
	{
		StoreResult();
		if(FetchRow())
		{
			count = strtoul(_SQLRow[0], NULL, 10);
		}
		FreeResult();
		close();
	}
	return count;
}

void dbconn::output()
{
	if (this->_SQLRow != NULL)
	{
		while (FetchRow())
		{
			for (int j = 0; j<_fieldCount; ++j)
			{
				printf("%s",_SQLRow[j]);
			}
			printf("\n");
		}
	}
}

bool dbconn::FreeResult()
{
	if (this->_SQLResult != NULL)
	{
		mysql_free_result(this->_SQLResult);
		this->_SQLResult = NULL ;
		this->_fieldCount = 0;
	}

	return true;
}

bool dbconn::StoreResult()
{
	bool b = false;
	this->_SQLResult = mysql_store_result(&this->_SQLConn);
	if (this->_SQLResult == NULL)
	{
		//printf ("<***MySQL ERROR***> DBSaveResult ERR:%s!\r", mysql_error(&this->_SQLConn));
		_fieldCount = 0;
	}
	else
	{
		_fieldCount = mysql_nu_fields(this->_SQLResult);
		b = true;
	}
	return b;
}

char** dbconn::FetchRow()
{
	this->_SQLRow = 0;
	if(this->_SQLResult != NULL)
		this->_SQLRow = mysql_fetch_row(this->_SQLResult);
	return this->_SQLRow;
}

bool dbconn::Insert(const char* szSQL)
{
	bool b = false;
	if (szSQL == NULL) return false;
	if (connect())
	{
		int Ret = mysql_query(&this->_SQLConn,szSQL);
		if (Ret != 0)//Ê§°Ü
		{
			printf("<MySQL-WARNING> insert into db fail:%s</MySQL-WARNING>\n", mysql_error(&this->_SQLConn));
		}
		b = true;
		close();
	}
	return b;
}

bool dbconn::Update(const char* szSQL)
{
	bool b = false;
	if (szSQL == NULL) return false;
	if (connect())
	{
		int Ret = mysql_query(&this->_SQLConn,szSQL);
		if (Ret != 0)//Ê§°Ü
		{
			printf("<MySQL-WARNING> insert into db fail:%s</MySQL-WARNING>\n", mysql_error(&this->_SQLConn));
		}
		b = true;
		close();
	}
	return b;
}

bool dbconn::Delete(const char* szSQL)
{
	bool b = false;
	if (szSQL == NULL) return false;
	if (connect())
	{
		int Ret = mysql_query(&this->_SQLConn,szSQL);
		if (Ret != 0)//Ê§°Ü
		{
			printf("<MySQL-WARNING> insert into db fail:%s</MySQL-WARNING>\n", mysql_error(&this->_SQLConn));
		}
		b = true;
		close();
	}

	return b;
}

ulong* dbconn::GetFieldsLengthArray()
{
	if (_SQLResult != NULL)
	{
		return mysql_fetch_lengths(_SQLResult);
	}

	return NULL;
}

void dbconn::DataSeek(uint offset)
{
	if (_SQLResult != NULL && offset >= 0)
	{
		mysql_data_seek(_SQLResult,offset);
	}
}

MYSQL_ROW_OFFSET dbconn::GetCursor()
{
	if (_SQLResult != NULL)
	{
		return mysql_row_tell(_SQLResult);
	}

	return NULL;
}

uint dbconn::GetFieldsCount()
{
	if (_SQLResult != NULL)
	{
		return mysql_nu_fields(_SQLResult);
	}

	return 0;
}

void dbconn::GetFieldsName()
{
	if (_SQLResult != NULL)
	{
		_SQLField = mysql_fetch_fields(_SQLResult);
		for (uint i = 0;i < GetFieldsCount(); i++)
		{
			printf("Field %u is %s\n",i,_SQLField[i].name);
		}
	}
}

ulong dbconn::GetAffectedRows()
{
	if (&_SQLConn != NULL)
	{
		return (ulong)mysql_affected_rows(&_SQLConn);
	}
	return 0;
}

ulong dbconn::getLimitNum(char* szSQL)
{
	ulong count = 0;
	if(szSQL == NULL)
	{
		return count;
	}
	if (Select(szSQL))
	{
		StoreResult();
		count =  GetResultRows();
		FreeResult();
		close();
	}

	return count;
}

ulong dbconn::GetResultRows()
{
	if (_SQLResult != NULL)
	{
		return (ulong)mysql_nu_rows(_SQLResult);
	}

	return 0;
}

bool dbconn::close()
{
	if (&this->_SQLConn == NULL) return false;
	mysql_close(&this->_SQLConn);
	this->_bConnect=false;

	return true;
}
