//#include "QuadcopterDaemon.h" comment in in real
#include "TCP_TestConnection.h"
#include "ThreadBase.h"
#include "SocketCommunication.h"
#include "JSONParser.h"
#include <string>

class ParameterUpdateThread : public ThreadBase
{
public:
    ParameterUpdateThread(int port);

    void Run(void);
    void CleanUp(void);
private:
    double GetDoubleToken(json_token* tokens, std::string name);
    int mPort;
    UnqLogger mLogger;
    ServerSocket mServer;
};
