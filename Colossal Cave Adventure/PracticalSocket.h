/**
 *  The .h file for libPracticalSocket.dylib
 *   C++ sockets on Unix and Windows
 *   Copyright (C) 2002
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   Originally Created by Professor Michael J. Donahoo
 *   Modified by Sebastian Norling and Niclas Björner on 2012-09-29.
 **/

#ifndef __PRACTICALSOCKET_INCLUDED__
#define __PRACTICALSOCKET_INCLUDED__

#include <string>            // For string
#include <exception>         // For exception class

using namespace std;

/**
 *   Signals a problem with the execution of a socket call.
 */
class SocketException : public exception {
public:
    /**
     *   Construct a SocketException with a explanatory message.
     */
    SocketException(const string &message, bool inclSysMsg = false) throw();
    
    /**
     *   Provided just to guarantee that no exceptions are thrown.
     */
    ~SocketException() throw();
    
    /**
     *   Get the exception message
     */
    const char *what() const throw();
    
private:
    string userMessage;  // Exception message
};

/**
 *   Base class representing basic communication endpoint
 */
class Socket {
public:
    /**
     *   Close and deallocate this socket
     */
    ~Socket();
    
    /**
     *   Get the local address
     */
    string getLocalAddress() throw(SocketException);
    
    /**
     *   Get the local port
     */
    unsigned short getLocalPort() throw(SocketException);
    
    /**
     *   Set the local port to the specified port and the local address
     *   to any interface
     */
    void setLocalPort(unsigned short localPort) throw(SocketException);
    
    /**
     *   Set the local port to the specified port and the local address
     *   to the specified address.  If you omit the port, a random port
     *   will be selected.
     */
    void setLocalAddressAndPort(const string &localAddress,
                                unsigned short localPort = 0) throw(SocketException);
    
    /**
     *   If WinSock, unload the WinSock DLLs; otherwise do nothing.  We ignore
     *   this in our sample client code but include it in the library for
     *   completeness.
     */
    static void cleanUp() throw(SocketException);
    
    /**
     *   Resolve the specified service for the specified protocol to the
     *   corresponding port number in host byte order
     */
    static unsigned short resolveService(const string &service,
                                         const string &protocol = "tcp");
    
private:
    // Prevent the user from trying to use value semantics on this object
    Socket(const Socket &sock);
    void operator=(const Socket &sock);
    
protected:
    int sockDesc;              // Socket descriptor
    Socket(int type, int protocol) throw(SocketException);
    Socket(int sockDesc);
};

/**
 *   Socket which is able to connect, send, and receive
 */
class CommunicatingSocket : public Socket {
public:
    /**
     *   Establish a socket connection with the given foreign
     *   address and port
     */
    void connect(const string &foreignAddress, unsigned short foreignPort)
    throw(SocketException);
    
    /**
     *   Write the given buffer to this socket.  Call connect() before
     *   calling send()
     */
    void send(const void *buffer, int bufferLen) throw(SocketException);
    
    /**
     *   Read into the given buffer up to bufferLen bytes data from this
     *   socket.  Call connect() before calling recv()
     */
    int recv(void *buffer, int bufferLen) throw(SocketException);
    
    int recvtimeout(char *buffer, int bufferLen, int timeout);
    
    /**
     *   Get the foreign address.  Call connect() before calling recv()
     */
    string getForeignAddress() throw(SocketException);
    
    /**
     *   Get the foreign port.  Call connect() before calling recv()
     */
    unsigned short getForeignPort() throw(SocketException);
    
protected:
    CommunicatingSocket(int type, int protocol) throw(SocketException);
    CommunicatingSocket(int newConnSD);
};

/**
 *   TCP socket for communication with other TCP sockets
 */
class TCPSocket : public CommunicatingSocket {
public:
    /**
     *   Construct a TCP socket with no connection
     */
    TCPSocket() throw(SocketException);
    
    /**
     *   Construct a TCP socket with a connection to the given foreign address
     *   and port
     */
    TCPSocket(const string &foreignAddress, unsigned short foreignPort)
    throw(SocketException);
    
    TCPSocket(int newConnSD);
    
private:
    // Access for TCPServerSocket::accept() connection creation
    friend class TCPServerSocket;
    
};

/**
 *   TCP socket class for servers
 */
class TCPServerSocket : public Socket {
public:
    /**
     *   Construct a TCP socket for use with a server, accepting connections
     *   on the specified port on any interface
     */
    TCPServerSocket(unsigned short localPort, int queueLen = 5)
    throw(SocketException);
    
    /**
     *   Construct a TCP socket for use with a server, accepting connections
     *   on the specified port on the interface specified by the given address
     */
    TCPServerSocket(const string &localAddress, unsigned short localPort,
                    int queueLen = 5) throw(SocketException);
    
    /**
     *   Blocks until a new connection is established on this socket or error
     */
    TCPSocket *accept() throw(SocketException);
    
    int getSockDesc();
    
private:
    void setListen(int queueLen) throw(SocketException);
};

/**
 *   UDP socket class
 */
class UDPSocket : public CommunicatingSocket {
public:
    /**
     *   Construct a UDP socket
     */
    UDPSocket() throw(SocketException);
    
    /**
     *   Construct a UDP socket with the given local port
     */
    UDPSocket(unsigned short localPort) throw(SocketException);
    
    /**
     *   Construct a UDP socket with the given local port and address
     */
    UDPSocket(const string &localAddress, unsigned short localPort)
    throw(SocketException);
    
    /**
     *   Unset foreign address and port
     */
    void disconnect() throw(SocketException);
    
    /**
     *   Send the given buffer as a UDP datagram to the
     *   specified address/port
     */
    void sendTo(const void *buffer, int bufferLen, const string &foreignAddress,
                unsigned short foreignPort) throw(SocketException);
    
    /**
     *   Read read up to bufferLen bytes data from this socket.  The given buffer
     *   is where the data will be placed
     */
    int recvFrom(void *buffer, int bufferLen, string &sourceAddress,
                 unsigned short &sourcePort) throw(SocketException);
    
    /**
     *   Set the multicast TTL
     */
    void setMulticastTTL(unsigned char multicastTTL) throw(SocketException);
    
    /**
     *   Join the specified multicast group
     */
    void joinGroup(const string &multicastGroup) throw(SocketException);
    
    /**
     *   Leave the specified multicast group
     */
    void leaveGroup(const string &multicastGroup) throw(SocketException);
    
private:
    void setBroadcast();
};

#endif