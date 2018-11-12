/**
 * @author Helight.Xu@gmail.com
 * @date 2012.03.10 ~
 */
#ifndef     _X_STRING_TOOL_H__
#define     _X_STRING_TOOL_H__

#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <fstream>
// #include <boost/lexical_cast.hpp>

inline unsigned char FromHex(const unsigned char x);
inline bool IsValidEncodingChar(const unsigned char x);

class StrTools
{
public:
    static void split_string(const std::string& str, const char delim,
                             std::vector<std::string>* result);
	static std::string str_md5(std::string& str);
	static std::string file_md5(std::string& file_name);
	static std::string char2hex(unsigned char ch);
	static std::string int2str(int val);
	static void str_trim(std::string* str);
	static void str_jsontrim(std::string* str);
    static bool decode(std::string& str);
	static void getDatetime(char *tmpbuf);
	static bool hasasic(const char *tmp);
	static void mktm(const char *df, struct tm *t);
	static int daydiff(const char *df, const char *dt);
	static int getday_now();
	static std::string getdate(char *ch);

    template <typename T>
    static bool string_to_number(const std::string txt, T* value)
    {
        bool ret = true;
     /*   try
        {
            T num = boost::lexical_cast<T>(txt);
            *value = num;
        }
        catch (boost::bad_lexical_cast& e)
        {
            ret = false;
        }
    */
        return ret;
    }
};

#endif // _X_STRING_TOOL_H__
