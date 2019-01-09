#include "ChatTCPServerObserver.h"
#include "CTCPServer.h"

int main()
{
    ChatTCPServerObserver observer;

    CTCPServer myserver(&observer, 4000);
    myserver.Run();
    
    return 0;
}

