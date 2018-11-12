/*
 *  JsonRpc-Cpp - JSON-RPC implementation.
 *  Copyright (C) 2008-2009 Sebastien Vincent <sebastien.vincent@cppextrem.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file jsonrpc_handler.cpp
 * \brief JSON-RPC server processor engine.
 * \author Sebastien Vincent
 */

#include "handler.h"

namespace json {
namespace rpc {

CallbackMethod::~CallbackMethod()
{
}

Handler::Handler()
{
    /* add a RPC method that list the actual RPC methods contained in the Handler */
    json::Value root;

    root["description"] = "List the RPC methods available";
    root["parameters"] = json::Value::null;
    root["returns"]
            = "Object that contains description of all methods registered";

    AddMethod(new RpcMethod<Handler> (*this, &Handler::SystemDescribe,
            std::string("system.describe"), root));
}

Handler::~Handler()
{
    /* delete all objects from the list */
    for (std::list<CallbackMethod*>::const_iterator it = m_methods.begin(); it
            != m_methods.end(); it++)
    {
        delete (*it);
    }
    m_methods.clear();
}

void Handler::AddMethod(CallbackMethod* method)
{
    m_methods.push_back(method);
}

void Handler::DeleteMethod(const std::string& name)
{
    /* do not delete system defined method */
    if (name == "system.describe")
    {
        return;
    }

    for (std::list<CallbackMethod*>::iterator it = m_methods.begin(); it
            != m_methods.end(); it++)
    {
        if ((*it)->GetName() == name)
        {
            delete (*it);
            m_methods.erase(it);
            break;
        }
    }
}

bool Handler::SystemDescribe(const json::Value& msg, json::Value& response, void *data)
{
    json::Value methods;
    response["jsonrpc"] = "2.0";
    response["id"] = msg["id"];

    for (std::list<CallbackMethod*>::iterator it = m_methods.begin(); it
            != m_methods.end(); it++)
    {
        methods[(*it)->GetName()] = (*it)->GetDescription();
    }

    response["result"] = methods;
    return true;
}

std::string Handler::GetString(json::Value value)
{
    return m_writer.write(value);
}

bool Handler::Check(const json::Value& root, json::Value& error)
{
    json::Value err;

    /* check the JSON-RPC version => 2.0 */
    if (!root.isObject() || !root.isMember("jsonrpc") || root["jsonrpc"]
            != "2.0")
    {
        error["id"] = json::Value::null;
        error["jsonrpc"] = "2.0";

        err["code"] = INVALID_REQUEST;
        err["message"] = "Invalid JSON-RPC request.";
        error["error"] = err;
        return false;
    }

    if (root.isMember("id") && (root["id"].isArray() || root["id"].isObject()))
    {
        error["id"] = json::Value::null;
        error["jsonrpc"] = "2.0";

        err["code"] = INVALID_REQUEST;
        err["message"] = "Invalid JSON-RPC request.";
        error["error"] = err;
        return false;
    }

    /* extract "method" attribute */
    if (!root.isMember("method") || !root["method"].isString())
    {
        error["id"] = json::Value::null;
        error["jsonrpc"] = "2.0";

        err["code"] = INVALID_REQUEST;
        err["message"] = "Invalid JSON-RPC request.";
        error["error"] = err;
        return false;
    }

    return true;
}

bool Handler::Process(const json::Value& root, json::Value& response, void *data)
{
    json::Value error;
    std::string method;

    if (!Check(root, error))
    {
        response = error;
        return false;
    }

    method = root["method"].asString();

    if (method != "")
    {
        CallbackMethod* rpc = Lookup(method);
        if (rpc)
        {
            return rpc->Call(root, response, data);
        }
    }

    /* forge an error response */
    response["id"] = root.isMember("id") ? root["id"] : json::Value::null;
    response["jsonrpc"] = "2.0";

    error["code"] = METHOD_NOT_FOUND;
    error["message"] = "Method not found.";
    response["error"] = error;

    return false;
}

bool Handler::Process(const std::string& msg, json::Value& response, void *data)
{
    json::Value root;
    json::Value error;
    bool parsing = false;

    /* parsing */
    parsing = m_reader.parse(msg, root);

    if (!parsing)
    {
        /* request or batched call is not in JSON format */
        response["id"] = json::Value::null;
        response["jsonrpc"] = "2.0";

        error["code"] = PARSING_ERROR;
        error["message"] = "Parse error.";
        response["error"] = error;
        return false;
    }

    if (root.isArray())
    {
        /* batched call */
        size_t i = 0;
        size_t j = 0;

        for (i = 0; i < root.size(); i++)
        {
            json::Value ret;
            Process(root[i], ret, data);

            if (ret != json::Value::null)
            {
                /* it is not a notification, add to array of responses */
                response[j] = ret;
                j++;
            }
        }
        return true;
    }
    else
    {
        return Process(root, response, data);
    }
}

bool Handler::Process(const char* msg, json::Value& response, void *data)
{
    std::string str(msg);

    return Process(str, response, data);
}

CallbackMethod* Handler::Lookup(const std::string& name) const
{
    for (std::list<CallbackMethod*>::const_iterator it = m_methods.begin(); it
            != m_methods.end(); it++)
    {
        if ((*it)->GetName() == name)
        {
            return (*it);
        }
    }

    return 0;
}

} /* namespace rpc */
} /* namespace json */

