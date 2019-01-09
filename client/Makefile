OBJS=Main.o Utils.o Config.o CPluginController.o CPluginEnumerator.o
OBJS_CLIENT=ChatTCPClientObserver.o CTCPClient.o

OUTPUT_CLIENT=client

CC=g++ -g

main: $(OBJS) $(OBJS_CLIENT)
	$(CC) -fpic -shared -o ./plugins/UI1.so UIPlugin1.cpp IUIPlugin.cpp
	$(CC) -fpic -shared -o ./plugins/UI2.so UIPlugin2.cpp IUIPlugin.cpp -lpthread
	$(CC) -o $(OUTPUT_CLIENT) $(OBJS_CLIENT) $(OBJS) -lpthread -ldl
CPluginEnumerator.o: CPluginEnumerator.cpp
	$(CC) -c CPluginEnumerator.cpp
CPluginController.o: CPluginController.cpp
	$(CC) -c CPluginController.cpp
CTCPClient.o: CTCPClient.cpp
	$(CC) -c CTCPClient.cpp
ChatTCPClientObserver.o: ChatTCPClientObserver.cpp
	$(CC) -c ChatTCPClientObserver.cpp
Main.o: Main.cpp
	$(CC) -c Main.cpp
Config.o: Config.cpp
	$(CC) -c Config.cpp
Utils.o: Utils.cpp
	$(CC) -c Utils.cpp
clean:
	rm -f $(OUTPUT_CLIENT) $(OBJS_CLIENT) $(OBJS)
	rm -f ./plugins/UI1.so
