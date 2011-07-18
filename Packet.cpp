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

Packet::Packet() {
  m_packetBuffer = (unsigned char*) malloc(1000);
  puts("Packet initiated.");
}

void Packet::send() {
  puts("sent!");
}

int Packet::packString(char* packString) {
  printf("Packing string: %s\n", packString);
  strcat((char *)m_packetBuffer, packString); 
  printf("Result: %s\n", m_packetBuffer);
  return 1;
}

int Packet::packFloat(float packFloat) {
  return 1;
}

int Packet::packInt32(uint32_t packInt32) {
  unsigned char bytes[4]; 
  unsigned char hexBase = 0xff; // A byte of all ones
  
  bytes[0] = hexBase & packInt32;
  bytes[1] = ((hexBase << 8) & packInt32) >> 8;
  bytes[2] = ((hexBase << 16) & packInt32) >> 16;
  bytes[3] = ((hexBase << 24) & packInt32) >> 24;

  printf("Unpacked INT: %d\n", unpackInt32(bytes));
  return 1;
}

int Packet::unpackInt32(unsigned char bytes[4]) {
  int myInt = bytes[3];
  myInt = (myInt << 8) | bytes[2];
  myInt = (myInt << 8) | bytes[1];
  myInt = (myInt << 8) | bytes[0];
  return myInt;
}
