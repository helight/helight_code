// helightxu

#include <iostream>
#include <stdexcept>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <ctemplate/template.h>
#include <gflags/gflags.h>
//#include <glog/logging.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/THttpServer.h>

#include "static/static_resource.h"
//#include "system_info.h"
#include "server_status_handler.h"
#include "rpc/ServerStatus.h"

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;
using namespace thrift_httpd;

DEFINE_int32(is_online, 0, "flags for online or testing");

std::string RESOURCE_open_test_html = "<html><head><title>Thrift Httpd index</title>"
    "</head><body><h1>{{PROCESS_NAME}}</h1></body></html>";

// for pages that no needed template
void handleOpenTest(const HttpRequest* request, HttpResponse* response)
{
    ctemplate::TemplateDictionary dict("data");
    dict.SetValue("PROCESS_NAME", "Thrift Httpd For Open Test");
    std::string html_template_filename = "opentest.html";
    ctemplate::StringToTemplateCache(html_template_filename,
                                     response->content.data(), response->content.size(),
                                     ctemplate::STRIP_WHITESPACE);
    std::string output;
    ctemplate::ExpandTemplate(html_template_filename,
                              ctemplate::STRIP_WHITESPACE, &dict, &output);
    response->content = output;
}

// for test.html page
void handleTest(const HttpRequest* request,
                                    HttpResponse* response)
{
    ctemplate::TemplateDictionary dict("data");
    dict.SetValue("PROCESS_NAME", "Thrift Httpd");
    std::string html_template_filename = "test.html";
    ctemplate::StringToTemplateCache(html_template_filename,
                                     response->content.data(),
                                     response->content.size(),
                                     ctemplate::STRIP_WHITESPACE);
    std::string output;
    ctemplate::ExpandTemplate(html_template_filename,
                              ctemplate::STRIP_WHITESPACE, &dict, &output);
    response->content = output;
}
/*
// for status.html page
void handleStatus(const HttpRequest* request,
                                      HttpResponse* response)
{
    ctemplate::TemplateDictionary dict("data");

    dict.SetValue("PROCESS_NAME", binaryPath());

    dict.SetValue("LinuxVersion", getLinuxVersion());

    // Get cpu, mem info.
    int32_t pid = getpid();
    uint64_t vm_size, mem_size;
    dict.ShowSection("CPU_ERROR");
    if (getMemoryUsedBytes(pid, &vm_size, &mem_size)) {
        dict.SetValue("VM_SIZE", boost::lexical_cast<std::string>(vm_size) + " B");
        dict.SetValue("PM_SIZE", boost::lexical_cast<std::string>(mem_size) + " B");
        dict.ShowSection("MEMORY_SUCCESS");
    } else {
        dict.ShowSection("MEMORY_ERROR");
    }

    std::string html_template_filename = "status.html";
    ctemplate::StringToTemplateCache(html_template_filename,
                                     response->content.data(),
                                     response->content.size(),
                                     ctemplate::STRIP_WHITESPACE);
    std::string output;
    ctemplate::ExpandTemplate(html_template_filename,
                              ctemplate::STRIP_WHITESPACE, &dict, &output);

    response->content = output;
}
*/
// for flags.html page
void handleFlags(const HttpRequest* request,
                                     HttpResponse* response)
{
    std::vector<google::CommandLineFlagInfo> flag_info;
    GetAllFlags(&flag_info);

    ctemplate::TemplateDictionary dict("data");
    std::vector<google::CommandLineFlagInfo>::iterator it = flag_info.begin();
    while (it != flag_info.end()) {
        // VLOG(4) << "gflags: " << it->filename;
        const std::string& filename = it->filename;
        ctemplate::TemplateDictionary* file_row_dict =
            dict.AddSectionDictionary("FILE_ROW");
        file_row_dict->SetValue("FILE_NAME", filename);
        while ((it != flag_info.end()) && (filename == it->filename)) {
            ctemplate::TemplateDictionary* flag_row_dict =
                file_row_dict->AddSectionDictionary("FLAG_ROW");
            if (!it->is_default) {
                flag_row_dict->SetValue("FLAG_STYLE", "color:red");
            } else {
                flag_row_dict->SetValue("FLAG_STYLE", "");
            }
            flag_row_dict->SetValue("FLAG_NAME", it->name);
            flag_row_dict->SetValue("FLAG_TYPE", it->type);
            flag_row_dict->SetValue("FLAG_CURRENT_VALUE", it->current_value);
            flag_row_dict->SetValue("FLAG_DEFAULT_VALUE", it->default_value);
            flag_row_dict->SetValue("FLAG_DESCRIPTION", it->description);
            ++it;
        }
    }
    std::string html_template_filename = "flags.html";
    ctemplate::StringToTemplateCache(html_template_filename,
                                     response->content.data(),
                                     response->content.size(),
                                     ctemplate::STRIP_WHITESPACE);
    std::string output;
    ctemplate::ExpandTemplate(html_template_filename,
                              ctemplate::STRIP_WHITESPACE, &dict, &output);
    response->content = output;
}


int main(int argc, char **argv)
{
    google::ParseCommandLineFlags(&argc, &argv, true);
    // google::InitGoogleLogging(argv[0]);

    boost::shared_ptr<TProtocolFactory> protocolFactory(new TJSONProtocolFactory());
    boost::shared_ptr<ServerStatusHandler> handler(new ServerStatusHandler());
    boost::shared_ptr<TProcessor> processor(new ServerStatusProcessor(handler));
    boost::shared_ptr<TServerTransport> serverTransport(new TServerSocket(8080));

    HttpResponseHandle *httpResponseHandle = new HttpResponseHandle();

    // register page and handle
    httpResponseHandle->addHttpHandle("/open_test.html", RESOURCE_open_test_html,
                                      RESOURCE_open_test_html.size(), handleOpenTest);

//    httpResponseHandle->addHttpHandle("/status.html", std::string(RESOURCE_status_html),
//                    RESOURCE_status_html_len, &handleStatus);

    httpResponseHandle->addHttpHandle("/flags.html", std::string(RESOURCE_flags_html),
                    RESOURCE_flags_html_len, &handleFlags);

    httpResponseHandle->addHttpHandle("/test.html", std::string(RESOURCE_test_html),
                    RESOURCE_test_html_len, &handleTest);
    httpResponseHandle->addStaticResource("/ServerStatus_types.js",
                                          std::string(RESOURCE_ServerStatus_types_js),
                                          RESOURCE_ServerStatus_types_js_len);
    httpResponseHandle->addStaticResource("/ServerStatus.js",
                                          std::string(RESOURCE_ServerStatus_js),
                                          RESOURCE_ServerStatus_js_len);
    httpResponseHandle->addStaticResource("/jquery-1.4.2.min.js",
                                          std::string(RESOURCE_jquery_1_4_2_min_js),
                                          RESOURCE_jquery_1_4_2_min_js_len);
    httpResponseHandle->addStaticResource("/thrift.js", std::string(RESOURCE_thrift_js),
                                          RESOURCE_thrift_js_len);

    boost::shared_ptr<THttpServerTransportFactory> transportFactory(
        new THttpServerTransportFactory(httpResponseHandle));

    TThreadedServer server(processor, serverTransport,
                           transportFactory, protocolFactory);

//    VLOG(4) << "is_online: " << FLAGS_is_online;
    printf("Starting the server...\n");
    server.serve();
    printf("done.\n");
    return 0;
}
