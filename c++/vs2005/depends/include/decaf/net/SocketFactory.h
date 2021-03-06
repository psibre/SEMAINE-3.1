/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _DECAF_NET_SOCKETFACTORY_H_
#define _DECAF_NET_SOCKETFACTORY_H_

#include <decaf/net/SocketException.h>
#include <decaf/util/Properties.h>
#include <decaf/util/Config.h>

namespace decaf{
namespace net{

    class Socket;

    /**
     * Socket Factory implementation for use in Creating Sockets
     * <p>
     * <p>
     * Property Options: <p>
     * Name                  Value <p>
     * ------------------------------------- <p>
     * inputBufferSize       size in bytes of the buffered input stream buffer.  Defaults to 10000.<p>
     * outputBufferSize      size in bytes of the buffered output stream buffer. Defaults to 10000.<p>
     * soLinger              linger time for the socket (in microseconds). Defaults to 0.<p>
     * soKeepAlive           keep alive flag for the socket (true/false). Defaults to false.<p>
     * soReceiveBufferSize   The size of the socket receive buffer (in bytes). Defaults to 2MB.<p>
     * soSendBufferSize      The size of the socket send buffer (in bytes). Defaults to 2MB.<p>
     * soTimeout             The timeout of socket IO operations (in microseconds). Defaults to 10000<p>
     *
     * @see <code>Socket</code>
     */
    class DECAF_API SocketFactory
    {
    public:

        virtual ~SocketFactory() {}

        /**
         * Creates and returns a Socket derived Object based on the values
         * defined in the Properties Object that is passed in.
         *
         * @param uri
         *      the URI for the Socket Connection.
         * @param properties
         *      A Properties object that contains configuration details.
         *
         * @throws SocketException.
         */
        static Socket* createSocket( const std::string& uri,
                                     const util::Properties& properties )
            throw ( SocketException );

    };

}}

#endif /*_DECAF_NET_SOCKETFACTORY_H_*/
