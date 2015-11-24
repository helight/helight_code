/**
 * @author Helight.Xu@gmail.com
 * @date 2012.03.10 ~
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "str_tools.h"

inline bool IsDigit(char c)
{
	return (c >= '0' && c <= '9') ? true : false;
}
inline char ToAscii(char c)
{
    return c & 0x7F;
}

inline char ToLower(char c)
{
    return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
}

inline char ToUpper(char c)
{
    return (c >= 'a' && c <= 'z') ? c - ('a' - 'A') : c;
}

inline unsigned char tohex(const unsigned char x)
{
    return "0123456789ABCDEF"[x];
}

void StrTools::split_string(const std::string& str, const char delim,
    std::vector<std::string>* result)
{
    assert(result != NULL);
    const char* p = str.c_str();
    const char* end = p + strlen(p);
    while (p != end)
    {
        if (*p == delim)
        {
            ++p;
        }
        else
        {
            const char* start = p;
            while (++p != end && *p != delim) {}
            result->push_back(std::string(start, p - start));
        }
    }
}
/*
 *
 */
std::string StrTools::char2hex(unsigned char ch)
{
	//if(!ch)		return NULL;
    std::string str("");

    str = tohex(ch / 16);
    str += tohex(ch % 16);
    return str;
}
inline unsigned char FromHex(const unsigned char x)
{
    unsigned char X = ToUpper(x);
    return IsDigit(X) ? X-'0' : X-'A'+10;
}
inline bool IsValidEncodingChar(const unsigned char x)
{
    unsigned char X = ToUpper(x);
    if (IsDigit(X) || (X >= 'A' && X <= 'F'))
        return true;
    return false;
}

/*
 *URL 解码
 */
bool StrTools::decode(std::string& str)
{
    size_t write_pos = 0;
    for (size_t i = 0; i < str.size(); ++i) {
        unsigned char ch = 0;
        if (str[i] == '%') {
            if (i + 2 > str.size()) {
                // 后面的数据不完整了，返回吧
                return false;
            }

            if (!IsValidEncodingChar(str[i+1]) || !IsValidEncodingChar(str[i+2]))
                return false;

            ch = (FromHex(str[i+1])<<4);
            ch |= FromHex(str[i+2]);
            i += 2;
        } else if (str[i] == '+') {
            ch = ' ';
        } else {
            ch = str[i];
        }
        str[write_pos++] = static_cast<char>(ch);
    }
    str.resize(write_pos);
    return true;
}

/*
 * int 转为 string
 */
std::string StrTools::int2str(int val)
{
	char tmp[64];
    std::string str;

	snprintf(tmp, sizeof(tmp), "%d", val);
	str = tmp;

	return str;
}

/*
 * 空格、tab、换行判断
 */
inline bool ispaces(char ch){
	return	(ch==' '||ch=='\t'||ch=='\r'||ch=='\b'||ch=='\f'||ch=='\n');
}

/*
 * 字符串前后空格、tab、换行删除
 */
void StrTools::str_trim(std::string* str)
{   
    size_t start_pos = 0;
    size_t end_pos = str->length();
    while (start_pos != end_pos && ispaces(str->at(start_pos)))
        start_pos++;
    if (start_pos == end_pos)
    {
        str->clear();
        return; 
    }
    end_pos--;
    while (ispaces(str->at(end_pos))) // end_pos always >= 0
        end_pos--;
    *str = str->substr(start_pos, end_pos - start_pos + 1);
}
/*
 *
 * 字符串前后空格、tab、换行删除,
 * 以查找json串为目的
 */
void StrTools::str_jsontrim(std::string* str)
{
    size_t start_pos = 0;
    size_t end_pos = str->length();
    while (start_pos != end_pos && (str->at(start_pos) != '{'))
        start_pos++;
    if (start_pos == end_pos)
    {
        str->clear();
        return;
    }
    end_pos--;
    while (start_pos != end_pos && (str->at(end_pos) != '}')) // end_pos always >= 0
        end_pos--;
    *str = str->substr(start_pos, end_pos - start_pos + 1);
}

void StrTools::getDatetime(char *tmpbuf)
{
	struct tm* newtime;
	time_t lt;
	time(&lt);
	newtime = localtime(&lt);
	strftime(tmpbuf, 128, "%Y-%m-%d %H:%M:%S", newtime);
}

bool StrTools::hasasic(const char *tmp)
{
	unsigned char *p = (unsigned char *)tmp;
	while (*p)
    {
		if(*p < 127)
			return false;
		p++;
	};
	return true;
}

void StrTools::mktm(const char *df, struct tm *t)
{
	char buf[10];
	snprintf(buf, 10, "%s", df);
	t->tm_mday = atoi(buf + 6);
	//printf("mday:%d\n ", atoi(buf + 6));
	*(buf + 6) = '\0';
	t->tm_mon = atoi(buf + 4) - 1;
	//printf("mon:%d\n ", atoi(buf + 4) -1);
	*(buf + 4) = '\0';
	t->tm_year = atoi(buf) - 1900;
	//printf("year:%d\n ", atoi(buf));
	t->tm_hour = 0;
	t->tm_min = 0;
	t->tm_sec = 0;
	//t->tm_isdst = 0;
    //printf("====%s\n", asctime(t));
}

int StrTools::daydiff(const char *df, const char *dt)
{
	int days = 0;
	if(strlen(df) != 8 || strlen(dt) != 8)
		return days;
	struct tm t1, t2;
	time_t tdf, tdt;
	mktm(df, &t1);
	mktm(dt, &t2);
	tdf = mktime(&t1);
	tdt = mktime(&t2);
	days = (int)(difftime(tdt, tdf)/(24*60*60));
	//printf("days:%d \n", days);

	return days+1;
}

int StrTools::getday_now()
{
	time_t t = time(NULL);
	struct tm *tm = gmtime(&t);
	return tm->tm_mday;
}

std::string StrTools::getdate(char *ch)
{
	time_t ntime;
	struct tm *t;
	char str_t[16];
	char tmp[16];

	time(&ntime);
	t = localtime(&ntime);
	snprintf(tmp, sizeof(tmp), "%s%s%s%s%s", "%Y", ch, "%m", ch, "%d");
	strftime(str_t, 16, tmp, t);
	return str_t;
}
/*int main() {
        string file_name = "/root/install.log";
        cout<<file_md5(file_name)<<endl;
        cout<<string_md5(file_name)<<endl;
        return 0;
}
*/
