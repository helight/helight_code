/*
 *  JsonRpc-Cpp - JSON-RPC implementation.
 *  Copyright (C) 2009 Sebastien Vincent <sebastien.vincent@cppextrem.com>
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
 * \file jsonrpc_common.h
 * \brief Common functions and enumerations for JsonRpc-Cpp.
 * \author Sebastien Vincent
 */

#ifndef JSONRPC_COMMON_H
#define JSONRPC_COMMON_H

#ifndef _WIN32

#include <stdint.h>

#else

#ifndef _MSC_VER
#include <stdint.h>
#endif

#endif

namespace json {
namespace rpc {

/**
* \enum ErrorCode
* \brief JSON-RPC error codes.
* \note Value from -32099 to -32000 are reserved for implementation-defined server-errors.
*/
enum ErrorCode
{
	PARSING_ERROR = -32700, /**< Invalid JSON. An error occurred on the server while parsing the JSON text. */
	INVALID_REQUEST = -32600, /**< The received JSON not a valid JSON-RPC Request. */
	METHOD_NOT_FOUND = -32601, /**< The requested remote-procedure does not exist / is not available. */
	INVALID_PARAMS = -32602, /**< Invalid method parameters. */
	INTERNAL_ERROR = -32603 /**< Internal JSON-RPC error. */
};

} /* namespace rpc */
} /* namespace json */

#endif /* JSONRPC_COMMON_H */

