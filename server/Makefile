OBJS_SERVER=server.o ChatTCPServerObserver.o CTCPServer.o

OUTPUT_SERVER=server

CC=g++ -g

main: $(OBJS_SERVER)
	$(CC) -o $(OUTPUT_SERVER) $(OBJS_SERVER) -lpthread
server.o: server.cpp
	$(CC) -c server.cpp
ChatTCPServerObserver.o: ChatTCPServerObserver.cpp
	$(CC) -c ChatTCPServerObserver.cpp
CTCPServer.o: CTCPServer.cpp
	$(CC) -c CTCPServer.cpp
clean:
	rm -f  $(OUTPUT_SERVER) $(OBJS_SERVER)
