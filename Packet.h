#include <stdint.h>
#include <string>
#include <iostream>

typedef struct {
    uint32_t packetType;
    uint32_t packetLen
} PacketHeader;

class Packet {

    public: //naming for static
       Packet();
       void* packString(string packString);
       void* packFloat(float packFloat);
       void* packInt32(uint32_t packInt32);

    private: //naming for members
       unsigned char *m_packetBuffer;
       unsigned char *m_packet[32];
};
