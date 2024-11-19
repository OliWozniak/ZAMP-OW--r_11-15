#ifndef COMCHANNEL_HH
#define COMCHANNEL_HH

#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#include "AbstractComChannel.hh"
#define PORT  6217


class ComChannel// : AbstractComChannel
{
private:
    int _Socket4Sending;
public:
    ComChannel();
    ~ComChannel();

    bool open_connection();
    int send(const char *sMesg);
};


#endif