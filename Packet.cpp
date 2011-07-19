/*
 ============================================================================
 Name        : Packet.c
 Author      : Ashley Pope 
 Version     : -1.0
 Copyright   : ...nobody would copy this (i hope)
 Description : Basic game server
 ============================================================================
 */

#include "Packet.h"

Packet::Packet(int packetType) {
  m_packetBuffer = (unsigned char*) malloc(MAX_BUFFER);
  m_packetHeader = new PacketHeader;
  m_packetHeader->packetLen = 32;
  m_packetHeader->packetType = packetType;
  puts("Packet initiated.");
}

void Packet::send() {
  puts("sent! (not)");
}

int Packet::packString(char* packString) {
  printf("Packing string: %s\n", packString);
  
  newPacket();

  memcpy((char *)m_packet, packString, PACKET_SIZE);
  strcat((char *)m_packetBuffer, (char *)m_packet);
  
  printf("Result: %s\nSize: %d\n", m_packetBuffer, m_packetHeader->packetLen);

  free(m_packet);
  return 1;
}

int Packet::packFloat(float packFloat) {
  return 1;
}

int Packet::packInt32(uint32_t packInt32) {
  newPacket(); 
  unsigned char hexBase = 0xff; // A byte of all ones
  
  m_packet[0] = hexBase & packInt32;
  m_packet[1] = ((hexBase << 8) & packInt32) >> 8;
  m_packet[2] = ((hexBase << 16) & packInt32) >> 16;
  m_packet[3] = ((hexBase << 24) & packInt32) >> 24;
    
  printf("Readint: %d\nSize:%d\n", m_packetHeader->packetLen);

  free(m_packet);

  return 1;
}

int Packet::readInt32(unsigned char bytes[INT_PACKET]) {
  int myInt = bytes[INT_PACKET];
  myInt = (myInt << 8) | bytes[2];
  myInt = (myInt << 8) | bytes[1];
  myInt = (myInt << 8) | bytes[0];
  return myInt;
}

void Packet::newPacket() {
    m_packet = (unsigned char*) malloc(PACKET_SIZE);
    m_packetHeader->packetLen += PACKET_SIZE;
}

