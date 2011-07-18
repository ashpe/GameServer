#include <stdint.h>
#include <string>
#include <iostream>

typedef struct {
    uint32_t sender_id;
    uint32_t data_length
} PacketHeader;

class Packet {

    public: //naming for static
       Packet();
       int packString();

    private: //naming for members

};
