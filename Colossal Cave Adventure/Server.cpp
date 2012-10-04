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

/* The size of the Recive buffer */
const unsigned int RECEIVEBUFFERSIZE = 128;

/* The handling of the Client (Telnetting) */
void HandleTCPClient(TCPSocket *socket);

/* The checker if the Exit Message is sent */
bool exitMsg(char *buffer);


bool exitMsg(char *buffer){
    /* String for char parsing */
    string str;
    
    /* The loop that parses the first 4 characters in the array
        Since the rest of the array will be filled with random data */
    for(int i = 0; i  < 4; i++)
    {
        str+=buffer[i];
    }
    
    /* Check if the Quit string has been sent, and return the appropriate bool-value */
    if(str == "Quit") {
        return true;
    } else {
        return false;
    }
}

/* The Handler for the client */
void HandleTCPClient(TCPSocket *socket) {
    cout << "Client handling ";
    try {
        /* Print the ip-address of the client that connect */
        cout << socket->getForeignAddress() << ":";
    } catch (SocketException e) { 
        cerr << "Network: Unable to get foreign address" << endl;
    }
    try {
        /* Print the socket that the client binds to */
        cout << socket->getForeignPort();
    } catch (SocketException e) {
        cerr << "Network: Unable to get foreign port" << endl;
    }
    
    char buffer[RECEIVEBUFFERSIZE];
    while (1) {
        /* Get the message from the socket */
        socket->recv(buffer, RECEIVEBUFFERSIZE);
        
        /* Check if the Exit Messagge is received*/
        if(exitMsg(buffer)) {
            break;
        }
        
        //get message to send back
    }
        /* Delete the Allocated socket on client quit */
        delete socket;
}