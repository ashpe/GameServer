all: server client auth_server 
auth_server: AuthServer.o AuthDatabaseHandler.o PacketHandler.o
	pkg-config --cflags protobuf  # fails if protobuf is not installed
	g++ -o auth_server -g AuthServer.o AuthDatabaseHandler.o PacketHandler.o Packet.pb.o -lsqlite3 `pkg-config --cflags --libs protobuf`  
server: GameServer.o PacketHandler.o Packet.pb.o protoc_middleman
	g++ -o server -g GameServer.o PacketHandler.o Packet.pb.o `pkg-config --cflags --libs protobuf` 
client: GameClient.o
	g++ -o client -g GameClient.o
GameServer.o: GameServer.c GameConf.h GameServer.h PacketHandler.h
	g++ -c -Wall -g GameServer.c
GameClient.o: GameClient.c GameConf.h GameClient.h AuthConf.h
	g++ -c -Wall -g GameClient.c
AuthServer.o: AuthServer.c AuthConf.h AuthServer.h AuthDatabaseHandler.h PacketHandler.h protoc_middleman
	g++ -c -Wall -g AuthServer.c
AuthDatabaseHandler.o: AuthDatabaseHandler.cpp AuthDatabaseHandler.h
	g++ -c -Wall -g AuthDatabaseHandler.cpp -lsqlite3
PacketHandler.o: PacketHandler.cpp PacketHandler.h protoc_middleman
	g++ -c -Wall -g PacketHandler.cpp Packet.pb.cc 
Packet.pb.o: Packet.pb.cc Packet.pb.h protoc_middleman
	g++ -c -Wall -g Packet.pb.cc
protoc_middleman: Packet.proto
	protoc --cpp_out=. Packet.proto
	@touch protoc_middleman
