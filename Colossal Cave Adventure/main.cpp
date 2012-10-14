//
//  main.cpp
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling and Niclas Björner on 2012-09-29.
//  Copyright (c) 2012. All rights reserved.
//

#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <stdio.h>
#include <syslog.h>
#include "Game.h"
#include "PracticalSocket.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>


const int RECEIVEBUFFERSIZE = 128;
bool  SIGHUPSENT = false;
bool EXITDAEMON = false;
vector<pthread_t> vThreads;


void HandleTCPClient(TCPSocket *socket, Game* game);
bool exitMsg(char *buffer);
void sighupHandler(int sigInt);
void destroy(int sigInt);
void *ThreadMain(void *arg);


void sighupHandler(int sigInt) {
    syslog(LOG_INFO, "SIGHUP Caught");
    sleep(30);
    SIGHUPSENT = true;
    for(int i = 0; i < vThreads.size(); i++){
        pthread_join(vThreads[i], NULL);
    }
    EXITDAEMON = true;
}

int main(int argc, const char * argv[])
{
    signal(SIGHUP,sighupHandler);
    
    /* The Server Code */
    /* A big Thank You to Professor Michael J. Donahoo
     at the School of Engineering & Computer Science
     at Baylor University for the library code and
     inspiration for the Network code! */
    
	/* Process ID and Session ID */
	pid_t pID, sID;
    
	/* Foring oof the process */
    pID = fork();
	
	/* Check for good pID */
	if(pID < 0) {
        cerr << "ERROR: Unable to get a good Process ID" << endl;
        exit(1);
    }
	
	/* Exit the parent process if the pID is good */
    if(pID > 0) {
        std::cout << "Becoming a Daemon!\n";
        exit(0);
    }
    
	/* Create a new Session ID */
	sID = setsid();
    if(sID < 0) {
        cerr << "ERROR: Unable to get a good Session ID" << endl;
        exit(1);
    }
    
    if((chdir("/"))<0) {
        exit(1);
    }
    
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    /* The Server Port, can be replaced by atoi(argv[1]) for usage with args */
    unsigned short serverPort = 10254;
    
    try {
    
        TCPServerSocket serverSocket(serverPort);
        /* This loop will run forever */
        while(!EXITDAEMON) {
            
            /* Old Code */
            //TCPSocket *clntSock = serverSocket.accept();
            
            int n = ::accept(serverSocket.getSockDesc(), NULL, 0);
            
            if(n >= 0) {
                TCPSocket *clntSock = new TCPSocket(n);
                
                /* Create the Thread and get the ID */
                pthread_t threadID;
                if (pthread_create(&threadID, NULL, ThreadMain,
                                   (void *) clntSock) != 0) {
                    cerr << "ERROR: Unable to create thread" << endl;
                    exit(1);
                }
            }
            
        }
    } catch (SocketException &e) {
        cerr << e.what() << endl;
        exit(1);
    }
    /* won't be reached */
    syslog(LOG_INFO, "Closing Daemon");
    sleep(7); // Ensure that the threads has all been closed
    exit(0);
}

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
    if(str == "quit") {
        return true;
    } else {
        return false;
    }
}


// TCP client handling function
void HandleTCPClient(TCPSocket *sock, Game* game) {
    /*cout << "Handling client ";
    try {
        cout << sock->getForeignAddress() << ":";
    } catch (SocketException &e) {
        cerr << "ERROR: Unable to get foreign address" << endl;
    }
    try {
        cout << sock->getForeignPort();
    } catch (SocketException &e) {
        cerr << "ERROR: Unable to get foreign port" << endl;
    }
    cout << " with thread " << pthread_self() << endl;*/
    
    // Send received string and receive again until the end of transmission
    
    string returnMsg = "";
    char clientBuffer[RECEIVEBUFFERSIZE];
    
    while (1) {
        
        /* Check if SIGHUP is caught */
        if(SIGHUPSENT){
            break;
        }
        
        /* Get the message from the socket */
        sock->recvtimeout(clientBuffer, RECEIVEBUFFERSIZE, 5);
        
        
        /* Check if the Exit Messagge is received*/
        if(exitMsg(clientBuffer)) {
            break;
        }
        
        /* Check if SIGHUP is caught */
        if(SIGHUPSENT){
            break;
        }
        
        /* Debugging that prints the string from the client */
        string str;
        for(int i = 0; i < RECEIVEBUFFERSIZE/8; i++){
            if (!::isprint(clientBuffer[i])) break;
            str+=clientBuffer[i];
            clientBuffer[i] = NULL;
        }
        
        if(str != "") {
            //cout << sock->getForeignAddress() << ":" << sock->getForeignPort() << " says: " << str << endl;
            returnMsg = game->parseInput(str);
            if (returnMsg != "") {
                returnMsg += "\n";
            }
            returnMsg += "> ";
            
            const char *returnBuffer = returnMsg.c_str();
            sock->send(returnBuffer, returnMsg.length()+1);
        }
        
        
    }
    
    if(SIGHUPSENT) {
        string str = "Closing socket due to SIGHUP\n\n";
        const char *p = str.c_str();
        sock->send(p, str.length()+1);
    } else {
        string str = "Closing socket due to quitcommand\n\n";
        const char *p = str.c_str();
        sock->send(p, str.length()+1);
    }
    /* Destructor closes socket */
}

void *ThreadMain(void *clntSock) {
    syslog(LOG_INFO, "A player joined the game.");
    
    Game* game = new Game();
    string str = "\n\n\n\n\n\n\n\n\n\n\n" + game->getPlayer()->getCurrentLocation()->getShortDescription() + (game->getPlayer()->getCurrentLocation()->getShortDescription() != "" ? "\n" : "") + game->getPlayer()->getCurrentLocation()->getLongDescription() + "\n> ";
    const char *returnBuffer = str.c_str();
    ((TCPSocket *)clntSock)->send(returnBuffer, str.length()+1);
    
    // Guarantees that thread resources are deallocated upon return
    pthread_detach(pthread_self());
    
    vThreads.push_back(pthread_self());
    // Extract socket file descriptor from argument
    HandleTCPClient((TCPSocket *) clntSock, game);
    
    syslog(LOG_INFO, "A player left the game.");
    
    delete game;
    delete (TCPSocket *) clntSock;
    return NULL;
}

void destroy(int sigint) {
    //
}