# =======================================================
#                 Chat-Client Makefile
# =======================================================

# Final Binary Target
./bin/chat-client : ./obj/client.o ./obj/network.o ./obj/ui.o ./obj/receiver.o
	cc ./obj/client.o ./obj/network.o ./obj/ui.o ./obj/receiver.o -o ./bin/chat-client -lncurses


# =======================================================
#                     Dependencies
# =======================================================
./obj/client.o : ./src/client.c ./inc/client.h ./inc/network.h ./inc/ui.h
	cc -c ./src/client.c -o ./obj/client.o

./obj/network.o : ./src/network.c ./inc/network.h
	cc -c ./src/network.c -o ./obj/network.o

./obj/ui.o : ./src/ui.c ./inc/ui.h
	cc -c ./src/ui.c -o ./obj/ui.o

./obj/receiver.o : ./src/receiver.c ./inc/receiver.h ./inc/ui.h
	cc -c ./src/receiver.c -o ./obj/receiver.o

# =======================================================
#                     Clean Target
# =======================================================
clean:
	rm -f ./bin/*
	rm -f ./obj/*.o
