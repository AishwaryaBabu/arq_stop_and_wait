#include<iostream>
#include"common.h"
#include"newport.h"
#include<vector>
#include<list>
using namespace std;


//Routing table - content id + receiving port(interface) + #hops + time to expire
//Connections table - 2D vector mapping destination address to sending port+"receiving port"(interface)//For broadcast
//Pending request table - Requested id + host id + receiving port(interface) + time to expire

vector<vector<int> >connectionsList;
vector<vector<int> >routingTable;
vector<vector<int> >pendingRequestTable;

struct cShared{
    LossyReceivingPort* fwdRecvPort;
    mySendingPort* fwdSendPort;
    //  int max;
};

static void CreateConnectionsList(int argc, char* argv[])
{
    //Implement formula and build table using config details
    int numberofPorts = argc-1;
    string source(argv[1]);
    int sourceID;
    if(source.at(0)=='r'){
        sourceID=atoi(source.substr(1).c_str())-1;
    }
    else if(source.at(0)=='h'){
        sourceID=atoi(source.substr(1).c_str())+63;
    }
    for(int i=0;i<numberofPorts;i++)
    {
        vector<int> ports;
        string destination(argv[i+2]);
        int destinationID;
        if(destination.at(0)=='r'){
            destinationID=atoi(destination.substr(1).c_str())-1;
        }
        else if(source.at(0)=='h'){
            destinationID=atoi(destination.substr(1).c_str())+63;
        }
        int x,y,zSource,zDestination;
        if(sourceID<destinationID){
            x=sourceID;
            y=destinationID;
            zSource=0;
            zDestination=1;
        }
        else{
            y=sourceID;
            x=destinationID;
            zSource=2;
            zDestination=3;
        }
        int DestinationAddr = x*(512)+y*(4)+zDestination+8000;
        ports.push_back(i+1);
        ports.push_back(DestinationAddr);
        connectionsList.push_back(ports);
    }
}

static void CreateRoutingTable()
{
    //Using config information build routing table
}

static void CreatePendingRequestTable()
{
    //To be monitored by main program

}

void* NodeRecProc(void* arg)
{
    cout<<"Created thread"<<endl;
    struct cShared *sh = (struct cShared *)arg;
    Packet* recvPacket;

    //Packet received : needs to be checked for appropriate forwarding and editing of table
    while(true)
    {
        recvPacket = sh->fwdRecvPort->receivePacket();
        if(recvPacket != NULL)
        {
            //          int dstPortNum = Look up the appopriate table to send the packet
            //             Address* dstAddr = new Address("localhost", dstPortNum);
            //            sh->fwdSendPort->setRemoteAddress(dstAddr);
            sh->fwdSendPort->sendPacket(recvPacket);

            //Request Packet
            if(recvPacket->accessHeader()->getOctet(0) == '0')
            {
                //Look up routing table based on content id


                //Make entry in pending request table                 
            }
            //Response Packet
            else if(recvPacket->accessHeader()->getOctet(0) == '1')
            {
            }
            //Announcement
            else if(recvPacket->accessHeader()->getOctet(0) == '2')
            {

            }
        }
    }
    cout<<"here"<<endl;
    return NULL;
}

void StartNodeThread(pthread_t* thread, vector<int> ports)
{

    //setup ports numbers
    Address* recvAddr;  //receive from port corresponding to node2 
    Address* sendAddr; // sending port corresponding to node1
    Address* dstAddr;  //address of node1 //NEEDS TO GO
    mySendingPort* sendPort; //sending port corr to send_addr
    LossyReceivingPort* recvPort; //receiving port corr to recvAddr;

    try{
        recvAddr = new Address("localhost", ports[0]);
        sendAddr = new Address("localhost", ports[1]);
        dstAddr =  new Address("localhost", ports[2]); //NEEDS TO GO

        recvPort = new LossyReceivingPort(0.0);
        recvPort->setAddress(recvAddr);

        sendPort = new mySendingPort();
        sendPort->setAddress(sendAddr);
        sendPort->setRemoteAddress(dstAddr); //NEEDS TO GO 

        sendPort->init();
        recvPort->init();

    } catch(const char *reason ){
        cerr << "Exception:" << reason << endl;
        return;
    }

    //pthread_create() - with sender
    struct cShared *sh;
    sh = (struct cShared*)malloc(sizeof(struct cShared));
    sh->fwdRecvPort = recvPort;
    sh->fwdSendPort = sendPort;
    //    sh->max = n;
    //    pthread_t thread;
    pthread_create(thread, 0, &NodeRecProc, sh);
    //    pthread_join(thread, NULL);
}

int main(int argc, char* argv[])
{

    //sender 4000
    //receiver localhost 4001 


    int N = 2;

    pthread_t threads[N];

    vector<int> ports;

    ports.push_back(10000);
    ports.push_back(11001);
    ports.push_back(4000);

    connectionsList.push_back(ports);

    ports[0] = 11000;
    ports[1] = 10001 ;
    ports[2] = 4001;

    connectionsList.push_back(ports);

    for(int i = 0; i < N; i++)
    {
        StartNodeThread(&(threads[i]), connectionsList[i]);
    }


    pthread_join(threads[1], NULL);
    pthread_join(threads[2], NULL);
    //void startReceiverThread()

    return 0;
}
