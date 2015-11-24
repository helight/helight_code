/**
 * @author Helight.Xu@gmail.com
 * @date 2012.03.10 ~
 *
 * head file for param_varify.cc
 */

#ifndef	__PARAM_VARIFY_HEAD__
#define	__PARAM_VARIFY_HEAD__
#include <map>
#include <string>
#include <regex.h>

class ParameterVarify
{
public:
    ParameterVarify();
    ~ParameterVarify();
	bool Varify(const char *type, const char *client);
    void Init();

private:
    std::map<std::string, std::string> m_regex_str;
    std::map<std::string, regex_t*> m_regex;
};

#endif
