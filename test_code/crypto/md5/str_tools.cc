/**
 * @author Helight.Xu@gmail.com
 * @date 2012.03.10 ~
 */
#include <str_tools.h>

/**
 * 字符串md5
 */
string str_tools::str_md5(string str)
{
	unsigned char md[MD5_DIGEST_LENGTH];
	char tmp[33] = {};
	string hash = "";
	MD5((const unsigned char*)str.c_str(), str.size(), md);
	for(int i = 0; i < MD5_DIGEST_LENGTH; i++){
		sprintf(tmp, "%02x", md[i]);
		hash+=(string)tmp;
	}
	return hash;
}
/**
 * 文件 md5
 */
string str_tools::file_md5(string file_name)
{
	MD5_CTX md5;
	unsigned char md[MD5_DIGEST_LENGTH];
	char tmp[33]={};
	int length,i;
	char buffer[1024];
	string hash="";
	MD5_Init(&md5);
	ifstream fin(file_name.c_str(), ios::in|ios::binary);
	while (!fin.eof()) {
		fin.read(buffer, 1024);
		length = fin.gcount();
		if (length > 0) {
			MD5_Update(&md5, buffer, length);
		}
	}
	MD5_Final(md,&md5);
	for(i = 0; i < MD5_DIGEST_LENGTH; i++){
		sprintf(tmp, "%02x", md[i]);
		hash+=(string)tmp;
	}
	return hash;
}
/*
 *
 */
string str_tools::char2hex(unsigned char ch)
{
    std::string str("");
    char high;
    char low;

    high = ch / 16;
    if(high > 9)
        high = 'A' + high - 10;
    else
        high = '0' + high;

    low  = ch % 16;
    if(low > 9)
        low = 'A' + low - 10;
    else
        low = '0' + low;

    str += high;
    str += low;

    return str;
}
/*
 *URL 编码
 */
string str_tools::urlencode(std::string encode)
{
	std::string result;
	for(unsigned int i = 0; i< static_cast<unsigned int>(encode.length()); i++)	{
		char ch = encode[i];
		if(ch == ' '){
			result += '+';
		}else if(ch >= 'A' && ch <= 'Z'){
			result += ch;
		}else if(ch >= 'a' && ch <= 'z'){
			result += ch;
		}else if(ch >= '0' && ch <= '9'){
			result += ch;
		}else if(ch == '-' || ch == '-' || ch == '.' || ch == '!' || ch == '~' || ch == '*' || ch == '\'' || ch == '(' || ch == ')' ){
			result += ch;
		}else{
			result += '%';
			result += char2hex(ch);
		}
	}
	return result;
}
/*int main() {
        string file_name = "/root/install.log";
        cout<<file_md5(file_name)<<endl;
        cout<<string_md5(file_name)<<endl;
        return 0;
}
*/
