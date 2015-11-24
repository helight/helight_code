#include <stdio.h>

#include "param_varify.h"

ParameterVarify::ParameterVarify()
{
	m_regex_str["thomepage"] = "^http://\\w+([-+.]\\w+)*.\\w+([-.]\\w+)";
	m_regex_str["homepage"] = "^((https|http|ftp|rtsp|mms)?://)[0-9a-z_!~*'().&=+$%?-]*";
	m_regex_str["email"] = "^\\w+((-\\w+)|(\\.\\w+))*\\@[A-Za-z0-9]+((\\.|-)[A-Za-z0-9]+)*\\.[A-Za-z0-9]+$";
	m_regex_str["phone"] = "^1(30|31|32|33|34|35|36|37|38|39|50|51|52|53|55|58|59|80|82|85|86|89)[0-9]{8}$";
	m_regex_str["test"] = "^[0-9]{3,4}$";
	m_regex_str["tname"] = "^[\\u4e00-\\u9fa50]";
}

ParameterVarify::~ParameterVarify()
{
    std::map<std::string, regex_t*>::iterator it = m_regex.begin();
	for (; it!=m_regex.end(); it++)
	{
		if ((*it).second != NULL)
		{
			regfree((*it).second);
			(*it).second = NULL;
		}
	}

	m_regex.clear();
	m_regex_str.clear();
}

void ParameterVarify::Init()
{
	int err_code = 0;
    char err_buf[1024] = {0};
    size_t err_len = 0;

    std::map<std::string, std::string>::iterator it = m_regex_str.begin();
	for (; it!=m_regex_str.end(); it++)
	{
		regex_t *regex = new regex_t;
		if ((err_code = regcomp(regex, (*it).second.c_str(), REG_EXTENDED)) == 0)
		{
			m_regex[(*it).first] = regex;
			continue;
		}

		err_len = regerror(err_code, regex, err_buf, sizeof(err_buf));
		err_len = err_len < sizeof(err_buf) ? err_len : sizeof(err_buf) - 1;
		err_buf[err_len] = '\0';
//		printf("regcomp error: %s\n", err_buf);
	}
}

bool ParameterVarify::Varify(const char *type, const char *client)
{
    regex_t *regex = m_regex[type];
	if (regex == NULL)
		return true;

	if (regexec(regex, client, 0, NULL, 0) == 0)
		return true;

	return false;
}
