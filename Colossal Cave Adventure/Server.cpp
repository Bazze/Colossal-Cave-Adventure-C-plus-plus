//
//  Server.cpp
//  Colossal Cave Adventure
//
//  Created by Niclas Björner on 10/4/12.
//  Copyright (c) 2012 SN Solutions. All rights reserved.
//


#include "PracticalSocket.h" // For Socket, ServerSocket and SocketException
#include <iostream>
#include <cstdlib>

const unsigned int RECEIVEBUFFERSIZE = 128;

void HandleTCPClient(TCPSocket *socket);

//Handling for TCP-Client
void HandleTCPClient(TCPSocket *socket) {
    cout << "Client handling ";
    try {
        cout << socket->getForeignAddress() << ":";
    } catch (SocketException e) { 
        cerr << "Network: Unable to get foreign address" << endl;
    }
    try {
        cout << socket->getForeignPort();
    } catch (SocketException e) {
        cerr << "Network: Unable to get foreign port" << endl;
    }
    
    char buffer[RECEIVEBUFFERSIZE];
    int receivMessageSize;
    while ((receivMessageSize = socket->recv(buffer, RECEIVEBUFFERSIZE)) > 0) {
        //send the message for parsing
        
        //get message to send back
    }
        delete socket;
}