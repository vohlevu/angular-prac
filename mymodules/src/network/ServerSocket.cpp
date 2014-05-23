#include "network/ServerSocket.h"

unsigned int ServerSocket::client_id; 

ServerSocket::ServerSocket(void)
{
	printf("\n Start ServerSocket ....\n");
    // id's to assign clients for our table
    client_id = 0;

    // set up the server network to listen 
    network = new ServerNetwork(); 
}

ServerSocket::~ServerSocket(void)
{
	printf("\n Close ServerSocket ....\n");
}

void ServerSocket::update()
{
    // get new clients
   if(network->acceptNewClient(client_id))
   {
        printf("client %d has been connected to the server\n",client_id);

        client_id++;
   }

   receiveFromClients();
}

void ServerSocket::receiveFromClients()
{
    // go through all clients
    std::map<unsigned int, SOCKET>::iterator iter;

    for(iter = network->sessions.begin(); iter != network->sessions.end(); iter++)
    {
        int data_length = network->receiveData(iter->first, network_data);

        if (data_length <= 0) 
        {
            //no data recieved
            continue;
        }
		
		long keyCode = (network_data[1] & 0xFF)  | ((network_data[0] & 0xFF) << 8);
		//int eventId = network_data[2];
		printf("\033[31msmp_tcp: keyCode = 0x%X\033[0m\n", keyCode);
		if (keyCode == 0x7000){
			sendActionPackets(iter->first);
		}
    }
}

void ServerSocket::sendActionPackets(unsigned int client_id)
{
    // send action packet
    char packet_data[20];
	strcpy(packet_data, "NodeJS Server");
	strcat(packet_data, "\n");
	unsigned int packet_size = strlen(packet_data);
	printf("sendActionPackets client_id : %s >> length : %d\n", packet_data, packet_size);
    network->sendToClientById(client_id, packet_data, packet_size);
}

void ServerSocket::sendActionPackets()
{
    // send action packet
    char packet_data[20];
	strcpy(packet_data, "NodeJS Server");
	strcat(packet_data, "\n");
	unsigned int packet_size = strlen(packet_data);
	printf("sendActionPackets : %s >> length : %d\n", packet_data, packet_size);
    network->sendToAll(packet_data, packet_size);
}