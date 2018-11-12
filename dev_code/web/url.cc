#include <string>
#include <stdio.h>
#include <iconv.h>
std::string char2hex(unsigned char ch)                                                                        
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

//URL 编码
std::string urlencode(std::string encode)
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

std::string hex2char(const char hch, const char bch)
{

}
//URL 解码
std::string urldecode(std::string decode)
{
	std::string result;
	for(unsigned int i = 0; i< static_cast<unsigned int>(decode.length()); i++)	{
		switch(decode[i])
		{
			case '+':
				result += ' ';
				break;
			case '%':
				if(isxdigit(decode[i + 1]) && isxdigit(decode[i + 2]))
				{
					result += hex2char(decode[i+1], decode[i+2]);
					i += 2;
				}else {
					result += '%';
				}
				break;
			default:
				result += decode[i];
				break;
		}
	}
	return result;
}

int main(int argc, char *argv[])
{
	std::string str = "http://www.soso.com/?unc=f400072_d2_1000_1";
	std::string tt = urlencode(str);
	printf("%s\n", tt.c_str());
}
