#include "MessageIdentifiers.h"
#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "RakNetStatistics.h"
#include "RakNetTypes.h"
#include "BitStream.h"
#include "RakSleep.h"
#include "OgreString.h"
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <stdlib.h>
#ifdef _WIN32
#include <conio.h>
#endif
#include <stdio.h>
#include <string.h>

#if defined(_CONSOLE_2)
#include "Console2SampleIncludes.h"
#endif

unsigned char GetPacketIdentifier(Packet *p);

#ifdef _CONSOLE_2
_CONSOLE_2_SetSystemProcessParams
#endif

#include "MagixNetworkDefines.h"

class ServerManager
{
protected:
	RakPeerInterface *server;
	RakNetStatistics *rss;
	SystemAddress clientAdd[MAX_CLIENTS];
	unsigned short numClients;
	bool showTraffic;
	Ogre::String mapName;

public:
	ServerManager()
	{
		server=RakNetworkFactory::GetRakPeerInterface();
		//int numAdds = server->GetNumberOfAddresses();
		server->SetIncomingPassword(SERVER_PASSWORD, (int)strlen(SERVER_PASSWORD));

		for(int i=0;i<MAX_CLIENTS;i++)clientAdd[i]=UNASSIGNED_SYSTEM_ADDRESS;
		numClients = 0;
		showTraffic = false;
		mapName = "Default";
	}
	~ServerManager()
	{
		char message[512]="00";
		message[0] = 0;
		message[1] = 0;
		strcat_s(message,"Server Shutdown");
		server->Send(message, (const int) strlen(message)+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
		RakNetworkFactory::DestroyRakPeerInterface(server);
		puts(message);
	}
	bool initialize()
	{
		Ogre::String tMapName[2]={"Default","Waterfall"};
		char input[32];
		puts("Select a map:");
		for(int i=0;i<2;i++)
		{
			printf("%i) ",i);
			puts(tMapName[i].c_str());
		}
		gets_s(input);
		for(int i=0;i<2;i++)
		{
			if(atoi(input)==i)
			{
				mapName = tMapName[i];
				break;
			}
		}

		printf("Starting server with map %s.\n",mapName.c_str());
		SocketDescriptor socketDescriptor(SERVER_PORT,0);
		bool b = server->Startup(MAX_CLIENTS, 30, &socketDescriptor, 1);
		server->SetMaximumIncomingConnections(MAX_CLIENTS);
		if (b)
			puts("Server started, waiting for connections.");
		else
		{ 
			puts("Server failed to start.  Terminating.");
			return false;
		}
		server->SetOccasionalPing(true);

		printf("Server IP is %s\n", server->GetLocalIP(0));
		puts("'quit' to quit. 'stat' to show stats. 'ping' to ping.\n'ban' to ban an IP from connecting.\n'kick to kick a player.\n'spoof' to spoof.\n'traffic' to toggle show traffic.\nType to talk.");
		return true;
	}
	void runLoop()
	{
		//char message[512]="";
		Ogre::String message="";

		// Loop for input
		while (1)
		{

		// This sleep keeps RakNet responsive
		RakSleep(30);

	#ifdef _WIN32
			if (_kbhit())
			{
				// Notice what is not here: something to keep our network running.  It's
				// fine to block on gets or anything we want
				// Because the network engine was painstakingly written using threads.
				char input[512]="";
				gets_s(input);
				message = input;

				if (message=="quit")
				{
					puts("Quitting.");
					break;
				}

				if (message=="stat")
				{
					char temp[2048]="";
					rss = server->GetStatistics(server->GetSystemAddressFromIndex(0));
					StatisticsToString(rss, temp, 2);
					printf("%s", temp);
					printf("Ping %i\n", server->GetAveragePing(server->GetSystemAddressFromIndex(0)));
			
					continue;
				}

				if (message=="ping")
				{
					printf("Enter player ID to ping\n");
					gets_s(input);
					unsigned short iID = atoi(input);
					if(iID<=MAX_CLIENTS && iID>0)
					if(clientAdd[iID-1]!=UNASSIGNED_SYSTEM_ADDRESS)server->Ping(clientAdd[iID-1]);

					continue;
				}

				if (message=="kick")
				{
					printf("Enter player ID to kick\n");
					gets_s(input);
					unsigned short iID = atoi(input);
					if(iID<=MAX_CLIENTS && iID>0)
					if(clientAdd[iID-1]!=UNASSIGNED_SYSTEM_ADDRESS)server->CloseConnection(clientAdd[iID-1], true, 0);

					continue;
				}


				if (message=="ban")
				{
					printf("Enter IP to ban.  You can use * as a wildcard\n");
					gets_s(input);
					server->AddToBanList(input);
					printf("IP %s added to ban list.\n", input);

					continue;
				}

				if (message=="spoof")
				{
					Ogre::String message2="00";
					printf("Enter Spoof ID\n");
					gets_s(input);
					message2[0] = ID_CHAT;
					message2[1] = (char)atoi(input);
					printf("Enter message\n");
					gets_s(input);
					message2 += input;
					server->Send(message2.c_str(), (const int)message2.length()+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
					continue;
				}

				if (message=="traffic")
				{
					showTraffic = !showTraffic;
					continue;
				}

				// Message now holds what we want to broadcast
				Ogre::String message2="00";
				// Append Server: to the message so clients know that it ORIGINATED from the server
				// All messages to all clients come from the server either directly or by being
				// relayed from other clients
				message2[0] = ID_CHAT;
				message2[1] = (char)SERVER_ID;
				message2 += "Server: ";
				message2 += message;
			
				// message2 is the data to send
				// strlen(message2)+1 is to send the null terminator
				// HIGH_PRIORITY doesn't actually matter here because we don't use any other priority
				// RELIABLE_ORDERED means make sure the message arrives in the right order
				// We arbitrarily pick 0 for the ordering stream
				// UNASSIGNED_SYSTEM_ADDRESS means don't exclude anyone from the broadcast
				// true means broadcast the message to everyone connected
				server->Send(message2.c_str(), (const int)message2.length()+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
			}
	#endif

			// Get a packet from either the server or the client

			Packet *p = server->Receive();
			
			/*if (p==0)
				continue; // Didn't get any packets*/
			while(p)
			{

			// We got a packet, get the identifier with our handy function
			unsigned char packetIdentifier = GetPacketIdentifier(p);

			// Check if this is a network message packet
			switch (packetIdentifier)
			{
				case ID_NEW_INCOMING_CONNECTION:
					printf("ID_NEW_INCOMING_CONNECTION from %s ", p->systemAddress.ToString());
					assignToken(p);
					printf("(Player token %i)\n", getOwnerToken(p));

					//first player, create map time and send map info directly
					if(numClients==1)
					{
						message = "00";
						message += "500;";
						message += mapName;
						message[0] = ID_WORLDINFO;
						message[1] = (unsigned char)SERVER_ID;
						server->Send(message.c_str(),(const int)message.length()+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, false);
					}
					else
					{
						//request map time from a player
						message = "00";
						message[0] = ID_REQUESTMAPTIME;
						message[1] = (unsigned char)getOwnerToken(p);
						for(int i=0;i<MAX_CLIENTS;i++)
						{
							if(clientAdd[i]!=UNASSIGNED_SYSTEM_ADDRESS)
							{
								server->Send(message.c_str(),(const int)message.length()+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, clientAdd[i], false);
								break;
							}
						}

						//Ping all old players to send newplayer info
						message = "00";
						message[0] = ID_NEWPLAYERUPDATE;
						message[1] = (unsigned char)getOwnerToken(p);
						server->Send(message.c_str(),(const int)message.length()+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, true);
					}
					break;

				case ID_DISCONNECTION_NOTIFICATION:
					printf("ID_DISCONNECTION_NOTIFICATION from %s ", p->systemAddress.ToString());
					printf("(Player token %i)\n", getOwnerToken(p));

					message = "00";
					message[0] = ID_PLAYERDISCONNECTED;
					message[1] = (unsigned char)getOwnerToken(p);
					server->Send(message.c_str(),(const int)message.length()+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, true);
					
					unassignToken(p);
					break;
				
				case ID_CONNECTION_LOST:
					printf("ID_CONNECTION_LOST from %s ", p->systemAddress.ToString());
					printf("(Player token %i)\n", getOwnerToken(p));

					message = "00";
					message[0] = ID_PLAYERDISCONNECTED;
					message[1] = (unsigned char)getOwnerToken(p);
					server->Send(message.c_str(),(const int)message.length()+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, true);
					
					unassignToken(p);
					break;

				case ID_MODIFIED_PACKET:
					printf("ID_MODIFIED_PACKET from %s ", p->systemAddress.ToString());
					printf("(Player token %i)\n", getOwnerToken(p));
					break;

				case ID_REQUESTMAPTIME:
					//Send map info to new player
					if(p->length>2)
					{
						message = "00";
						message += getPacketData(p);
						message += ";";
						message += mapName;
						message[0] = ID_WORLDINFO;
						message[1] = (unsigned char)SERVER_ID;

						unsigned short targetToken = p->data[1];
						if(targetToken<=MAX_CLIENTS && targetToken>0)
						if(clientAdd[targetToken-1]!= UNASSIGNED_SYSTEM_ADDRESS)
						server->Send(message.c_str(),(const int)message.length()+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, clientAdd[targetToken-1], false);
					}
					break;

				//Reliable_ordered packets
				case ID_CHAT:
				case ID_NEWPLAYER:
				case ID_NEWPLAYERCOLOURS:
				case ID_POSITION:
				case ID_EMOTE:
				case ID_ATTACK:
					if(showTraffic||1)
					{
						printf("%i: ", getOwnerToken(p));
						printf("%s\n", p->data);
					}
					if(p->length>2)
					{
						//no one in particular, broadcast to all except sender
						if(p->data[1]==TOKEN_NULL)
						{
							p->data[1] = (unsigned char)getOwnerToken(p);
							server->Send((char*)p->data,p->bitSize, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, true);
						}
						//target recepient specified, send only to target
						else
						{
							unsigned short targetToken = p->data[1];
							p->data[1] = (unsigned char)getOwnerToken(p);
							if(targetToken<=MAX_CLIENTS && targetToken>0)
							if(clientAdd[targetToken-1]!= UNASSIGNED_SYSTEM_ADDRESS)
							server->Send((char*)p->data,p->bitSize, HIGH_PRIORITY, RELIABLE_ORDERED, 0, clientAdd[targetToken-1], false);
						}
					}
					break;
				//Unreliable packets
				case ID_MOVEPOSITION:
				case ID_LOOKQUATERNION:
				case ID_FACEDIRECTION:
				case ID_ACTIONFLAGS:
					if(showTraffic)
					{
						printf("%i: ", getOwnerToken(p));
						printf("%s\n", p->data);
					}
					if(p->length>2)
					{
						//no one in particular, broadcast to all except sender
						if(p->data[1]==TOKEN_NULL)
						{
							p->data[1] = (unsigned char)getOwnerToken(p);
							server->Send((char*)p->data,p->bitSize, HIGH_PRIORITY, UNRELIABLE, 0, p->systemAddress, true);
						}
						//target recepient specified, send only to target
						else
						{
							unsigned short targetToken = p->data[1];
							p->data[1] = (unsigned char)getOwnerToken(p);
							if(targetToken<=MAX_CLIENTS && targetToken>0)
							if(clientAdd[targetToken-1]!= UNASSIGNED_SYSTEM_ADDRESS)
							server->Send((char*)p->data,p->bitSize, HIGH_PRIORITY, UNRELIABLE, 0, clientAdd[targetToken-1], false);
						}
					}
					break;

				default:
					puts((char*)p->data);
					//server->Send((char*)p->data,p->bitSize, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, true);
					break;
			}

			server->DeallocatePacket(p);
			p = server->Receive();
			}
		}
	}
	unsigned short getOwnerToken(Packet *p)
	{
		for(int i=0;i<MAX_CLIENTS;i++)
		{
			if(p->systemAddress==clientAdd[i])return (i+1);
		}
		return 0;
	}
	void shutdown()
	{
		server->Shutdown(300);
	}
	void assignToken(Packet *p)
	{
		bool assigned = false;
		for(int i=0;i<MAX_CLIENTS;i++)
		{
			if(clientAdd[i] == UNASSIGNED_SYSTEM_ADDRESS)
			{
				clientAdd[i] = p->systemAddress;
				numClients += 1;
				assigned = true;
				break;
			}
		}
		if(!assigned)puts("Failed to assign new token!");
	}
	void unassignToken(Packet *p)
	{
		for(int i=0;i<MAX_CLIENTS;i++)
		{
			if(clientAdd[i] == p->systemAddress)
			{
				clientAdd[i] = UNASSIGNED_SYSTEM_ADDRESS;
				numClients -= 1;
				break;
			}
		}
	}
	const Ogre::String getPacketData(Packet *p)
	{
		if(p->length < 3)return "";
		return Ogre::String((char*)p->data).erase(0,2);
	}
};

int main(void)
{
	ServerManager mServerMgr;
	
	if(!mServerMgr.initialize())
	return 0;

	mServerMgr.runLoop();
	mServerMgr.shutdown();

	return 0;
}

// Copied from Multiplayer.cpp
// If the first byte is ID_TIMESTAMP, then we want the 5th byte
// Otherwise we want the 1st byte
unsigned char GetPacketIdentifier(Packet *p)
{
	if (p==0)
		return 255;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
	{
		assert(p->length > sizeof(unsigned char) + sizeof(unsigned long));
		return (unsigned char) p->data[sizeof(unsigned char) + sizeof(unsigned long)];
	}
	else
		return (unsigned char) p->data[0];
}

