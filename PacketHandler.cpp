/*
 ============================================================================
 Name        : Packet.cpp
 Author      : Ashley Pope
 Version     : -1.0
 Copyright   : ...nobody would copy this (i hope)
 Description : Packets of packets.
 ============================================================================
 */

#include "PacketHandler.h"
#include <google/protobuf/message_lite.h>

PacketHandler::PacketHandler() { }

void PacketHandler::send() {
    puts("sent! (not)");
}

std::string PacketHandler::login(char *username, char *password, int version) {
    Packet::PacketHeader header;

    Packet::PacketHeader::LoginPacket* login = header.mutable_login_packet();
    login->set_username(username);
    login->set_password(password);
    login->set_version(version);

    std::string buf;
    header.SerializeToString(&buf);

    return buf;
}

std::string PacketHandler::auth_code(char *auth_code) {
    Packet::PacketHeader header;

    header.set_auth_code(auth_code);
     
    std::string buf;
    header.SerializeToString(&buf);

    return buf;
}

int PacketHandler::packString(char* packString) {
    printf("Packing string: %s\n", packString);

    newPacket();

    memcpy((char *)m_packet, packString, PACKET_SIZE);
    strcat((char *)m_packetBuffer, (char *)m_packet);

    printf("Result: %s\nSize: %d\n", m_packetBuffer, m_packetHeader->packetLen);

    free(m_packet);
    return 1;
}

int PacketHandler::packFloat(float packFloat) {
    return 1;
}

int PacketHandler::packInt32(uint32_t packInt32) {
    newPacket();
    unsigned char hexBase = 0xff; // A byte of all ones

    m_packet[0] = hexBase & packInt32;
    m_packet[1] = ((hexBase << 8) & packInt32) >> 8;
    m_packet[2] = ((hexBase << 16) & packInt32) >> 16;
    m_packet[3] = ((hexBase << 24) & packInt32) >> 24;
    strcat((char *)m_packetBuffer, (char *)m_packet);

    printf("Buffer: %s\nSize:%d\n", m_packetBuffer, m_packetHeader->packetLen);

    free(m_packet);

    return 1;
}

int PacketHandler::readInt32(unsigned char bytes[INT_PACKET]) {
    int myInt = bytes[INT_PACKET];
    myInt = (myInt << 8) | bytes[2];
    myInt = (myInt << 8) | bytes[1];
    myInt = (myInt << 8) | bytes[0];
    return myInt;
}

void PacketHandler::newPacket() {
    m_packet = (unsigned char*) malloc(PACKET_SIZE);
    m_packetHeader->packetLen += PACKET_SIZE;
}

