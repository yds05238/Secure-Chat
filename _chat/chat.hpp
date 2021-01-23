#pragma once

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <iostream>

extern bool serverTaken, clientTaken;  // whether server/client already taken
extern const unsigned int portNumber = 1500;
extern const unsigned int bufferSize = 1024;

