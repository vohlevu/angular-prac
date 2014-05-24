#pragma once
#include "network/ServerNetwork.h"

class ServerSocket
{

public:

    ServerSocket(void);
    ~ServerSocket(void);

    void update();

	void receiveFromClients();

	void sendActionPackets(unsigned int client_id);
	
	void sendActionPackets();
	
	void setSocketCallbackFunc(void (*func_pointer)(char*));
	
	void callback(char* data);

private:

	void (*mySocketCallbackFunc)(char*);
   // IDs for the clients connecting for table in ServerNetwork 
    static unsigned int client_id;

   // The ServerNetwork object 
    ServerNetwork* network;

	// data buffer
   char network_data[MAX_PACKET_SIZE];
};