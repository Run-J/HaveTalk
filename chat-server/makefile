# =======================================================
#                 Chat-Server Makefile
# =======================================================

# Final Binary Target
./bin/chat-server : ./obj/server.o ./obj/connection.o ./obj/clientHandler.o ./obj/clientList.o
	cc ./obj/server.o ./obj/connection.o ./obj/clientHandler.o ./obj/clientList.o -o ./bin/chat-server


# =======================================================
#                     Dependencies
# =======================================================
./obj/server.o : ./src/server.c ./inc/server.h ./inc/clientHandler.h ./inc/clientList.h
	cc -c ./src/server.c -o ./obj/server.o

./obj/connection.o : ./src/connection.c ./inc/connection.h ./inc/clientHandler.h ./inc/clientList.h
	cc -c ./src/connection.c -o ./obj/connection.o

./obj/clientHandler.o : ./src/clientHandler.c ./inc/clientHandler.h ./inc/clientList.h
	cc -c ./src/clientHandler.c -o ./obj/clientHandler.o

./obj/clientList.o : ./src/clientList.c ./inc/clientList.h
	cc -c ./src/clientList.c -o ./obj/clientList.o


clean:
	rm -f ./bin/*
	rm -f ./obj/*.o
