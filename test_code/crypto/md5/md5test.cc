/**
 * @author Helight.Xu@gmail.com
 * @date 2012.03.10 ~
 */
#include <str_tools.h>

int main()
{
        string file_name = "/root/install.log"; 
	char *requrl="DiscuzChannel.php?action=addDiscuzId&unc=%s&url=%s&sig=%s";
	char *unc = "f400072_d2_1000_1";
	char *url = "http://www.soso.com/?unc=f400072_d2_1000_1";
	
        cout<<str_tools::str_md5(file_name)<<endl; 
	
	return 0;
}
