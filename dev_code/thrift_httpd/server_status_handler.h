// helightxu

#include <string>
#include "rpc/ServerStatus.h"


using namespace thrift_httpd;

class ServerStatusHandler : public ServerStatusIf
{
public:
    ServerStatusHandler() {}
    ~ServerStatusHandler() {}

    void ping();

    void hello(std::string& retValue, const std::string& ret);

    int32_t add(const int32_t n1, const int32_t n2);

    int32_t calculate(const int32_t logid, const Work &work);

    void getStruct(SharedStruct &ret, const int32_t logid);


protected:
    std::map<int32_t, SharedStruct> log;
};
