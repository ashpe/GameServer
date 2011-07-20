all: server client auth_server
clean:
	rm AuthServer.o AuthDatabaseHandler.o PacketHandler.o Packet.pb.o GameServer.o GameClient.o \
    	Packet.pb.cc Packet.pb.h
protoc_middleman: Packet.proto
	protoc --cpp_out=. Packet.proto
	@touch Packet.proto
auth_server: AuthServer.o AuthDatabaseHandler.o PacketHandler.o
	g++ -o auth_server -g AuthServer.o AuthDatabaseHandler.o Packet.pb.o PacketHandler.o -lsqlite3 `pkg-config --cflags --libs protobuf`  
server: GameServer.o PacketHandler.o Packet.pb.o AuthDatabaseHandler.o
	g++ -o server -g GameServer.o AuthDatabaseHandler.o Packet.pb.o PacketHandler.o -lsqlite3 `pkg-config --cflags --libs protobuf` 
client: GameClient.o PacketHandler.o Packet.pb.o
	g++ -o client -g GameClient.o Packet.pb.o PacketHandler.o `pkg-config --cflags --libs protobuf`
GameServer.o: GameServer.c GameConf.h GameServer.h PacketHandler.h
	g++ -c -Wall -g GameServer.c
GameClient.o: GameClient.c GameConf.h GameClient.h AuthConf.h
	g++ -c -Wall -g GameClient.c
AuthServer.o: AuthServer.c AuthConf.h AuthServer.h AuthDatabaseHandler.h PacketHandler.h
	g++ -c -Wall -g AuthServer.c 
AuthDatabaseHandler.o: AuthDatabaseHandler.cpp AuthDatabaseHandler.h
	g++ -c -Wall -g AuthDatabaseHandler.cpp
PacketHandler.o: PacketHandler.cpp PacketHandler.h protoc_middleman
	g++ -c -Wall -g PacketHandler.cpp 
Packet.pb.o: Packet.pb.cc Packet.pb.h protoc_middleman
	g++ -c -Wall -g Packet.pb.cc

