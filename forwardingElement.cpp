#include<iostream>
#include"common.h"
#include"newport.h"
using namespace std;


struct cShared{
    LossyReceivingPort* fwdRecvPort;
    mySendingPort* fwdSendPort;
    //  int max;
};

void* nodeRecProc(void* arg)
{
    cout<<"Created thread"<<endl;
    struct cShared *sh = (struct cShared *)arg;
    Packet* recvPacket;
    while(true)
    {
        recvPacket = sh->fwdRecvPort->receivePacket();
        if(recvPacket != NULL)
        {
            sh->fwdSendPort->sendPacket(recvPacket);
        }
    }
    cout<<"here"<<endl;
    return NULL;
}

void startNode1Thread()
{

   //setup port numbers for Node1  

    Address* recvAddr;  //receive from port corresponding to node2 
    Address* sendAddr; // sending port corresponding to node1
    Address* dstAddr;  //address of node1
    mySendingPort* sendPort; //sending port corr to send_addr
    LossyReceivingPort* recvPort; //receiving port corr to recvAddr;

    try{
        recvAddr = new Address("localhost", 10000);
        sendAddr = new Address("localhost", 11001);
        dstAddr =  new Address("localhost", 4000);

        recvPort = new LossyReceivingPort(0.0);
        recvPort->setAddress(recvAddr);

        sendPort = new mySendingPort();
        sendPort->setAddress(sendAddr);
        sendPort->setRemoteAddress(dstAddr);

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
    pthread_t thread;
    pthread_create(&(thread), 0, &nodeRecProc, sh);
//    pthread_join(thread, NULL);
}

void startNode2Thread()
{

    //setup port numbers for Node2

    Address* recvAddr;  //receive from port corresponding to node1 
    Address* sendAddr; // sending port corresponding to node2
    Address* dstAddr;  //address of node2
    mySendingPort* sendPort; //sending port corr to send_addr
    LossyReceivingPort* recvPort; //receiving port corr to recvAddr;

    try{
        recvAddr = new Address("localhost", 11000);
        sendAddr = new Address("localhost", 10001);
        dstAddr =  new Address("localhost", 4001);

        recvPort = new LossyReceivingPort(0.0);
        recvPort->setAddress(recvAddr);

        sendPort = new mySendingPort();
        sendPort->setAddress(sendAddr);
        sendPort->setRemoteAddress(dstAddr);

        sendPort->init();
        recvPort->init();

    } catch(const char *reason ){
        cerr << "Exception:" << reason << endl;
        return;
    }


    //pthread_create() - with receiver 
    struct cShared *sh;
    sh = (struct cShared*)malloc(sizeof(struct cShared));
    sh->fwdRecvPort = recvPort;
    sh->fwdSendPort = sendPort;
    //    sh->max = n;
    pthread_t thread;
    pthread_create(&(thread), 0, &nodeRecProc, sh);
    pthread_join(thread, NULL);
}


int main(int argc, char* argv[])
{

    //sender 4000
    //receiver localhost 4001 

    startNode1Thread();
    startNode2Thread();
    //void startReceiverThread()

    return 0;
}
