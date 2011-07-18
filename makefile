all: server client auth_server 
auth_server: AuthServer.o AuthDatabaseHandler.o Packet.o
	g++ -o auth_server -g AuthServer.o AuthDatabaseHandler.o Packet.o -lsqlite3
server: GameServer.o
	g++ -o server -g GameServer.o 
client: GameClient.o
	g++ -o client -g GameClient.o
GameServer.o: GameServer.c GameConf.h GameServer.h
	g++ -c -Wall -g GameServer.c
GameClient.o: GameClient.c GameConf.h GameClient.h AuthConf.h
	g++ -c -Wall -g GameClient.c
AuthServer.o: AuthServer.c AuthConf.h AuthServer.h AuthDatabaseHandler.h Packet.h
	g++ -c -Wall -g AuthServer.c
AuthDatabaseHandler.o: AuthDatabaseHandler.cpp AuthDatabaseHandler.h
	g++ -c -Wall -g AuthDatabaseHandler.cpp -lsqlite3
Packet.o: Packet.cpp Packet.h
	g++ -c -Wall -g AuthDatabaseHandler.cpp
