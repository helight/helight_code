#include <vector>
#include <set>
#include <string>
#include "parse_conf.h"

struct MyIdentification
{
	char localIp[20];
	char myRole[20];
	char master_db_ip[20];
	char master_db_user[30];
	char master_db_pass[30];
	char master_db_db[30];
	unsigned int master_db_port;
};


class XAppConfig
{
public:
	XAppConfig(){}
	~XAppConfig(){}

	// 获取单实例
	inline static XAppConfig* GetInstance()
	{
		if(m_pAppConf == NULL)
			m_pAppConf = new XAppConfig;
		return m_pAppConf;
	}

	// 释放单实例
	static void FreeInstance()
	{
		if (m_pAppConf != NULL) {
			delete m_pAppConf;
			m_pAppConf = NULL;
		}
	}

	//加载配置文件
	bool LoadConf(const char* pConf);
	MyIdentification *GetMyIdentification(){ return &myIdentification;}

private:
	static XAppConfig* m_pAppConf;

	CReadIni ini;
	MyIdentification myIdentification;
};

XAppConfig * XAppConfig::m_pAppConf = NULL;
bool XAppConfig::LoadConf(const char* pConf)
{
	int iRet = ini.LoadConfigFile(pConf);
	if (SUCCEEDED!= iRet) {
		return false;
	}

	if (SUCCEEDED!= ini.GetConfigStr("myIdentification","localIp",myIdentification.localIp,20)) {
		return false;
	}
	if (SUCCEEDED!= ini.GetConfigStr("myIdentification","myRole",myIdentification.myRole,20)) {
		return false;
	}
	if (SUCCEEDED!= ini.GetConfigStr("myIdentification","master_db_ip",myIdentification.master_db_ip,20)) {
		return false;
	}
	if (SUCCEEDED!= ini.GetConfigStr("myIdentification","master_db_user",myIdentification.master_db_user,30)) {
		return false;
	}
	if (SUCCEEDED!= ini.GetConfigStr("myIdentification","master_db_pass",myIdentification.master_db_pass,30)) {
		return false;
	}
	if (SUCCEEDED!= ini.GetConfigStr("myIdentification","master_db_db",myIdentification.master_db_db,30)) {
		return false;
	}

	myIdentification.master_db_port = ini.GetConfigInt("myIdentification","master_db_port");
	if (myIdentification.master_db_port <= 0) {
		return false;
	}

	return true;
}

