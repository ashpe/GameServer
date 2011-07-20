//Includes
#include <stdint.h>
#include <string>
#include <iostream>

//defines

#define PACKET_SIZE 32
#define INT_PACKET 3
#define MAX_BUFFER 960 //this is packet * 30

//packet types

enum PacketTypes {
    PingPacketType = 1,
    // Sends one string (ping/pong) to check sockets alive
    LoginPacketType = 2,
    // Sends username (string) then password (string)
};

//packet header
typedef struct {
    uint32_t packetType;
    uint32_t packetLen;
} PacketHeader;

//class definition

class PacketHandler {

    public: //naming for static
       PacketHandler(int packetType);
       void Login(char *username, char *password, int version);
       void send();
       int packString(char* packString);
       int packFloat(float packFloat);
       int packInt32(uint32_t packInt32);
       int readInt32(unsigned char bytes[INT_PACKET]);

    private: //naming for members
       unsigned char *m_packetBuffer;
       unsigned char *m_packet;
       PacketHeader *m_packetHeader;
       void newPacket();
};
