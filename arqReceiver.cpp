#include "common.h"
#include "newport.h"
#include<fstream>
#include <iostream>
using namespace std;

int main(int argc, const char * argv[])
{

    try {
        const char* hname = "localhost";  

        //configure receiving port     
        Address * my_addr = new Address(hname, (short)(atoi(argv[1])));

        LossyReceivingPort *my_port = new LossyReceivingPort(0.2);
        my_port->setAddress(my_addr);
        my_port->init();

        //configure a sending port to send ACK
        Address * my_tx_addr = new Address(hname, 3005);
        Address * dst_addr =  new Address(argv[2], (short)(atoi(argv[3])));
        mySendingPort *my_tx_port = new mySendingPort();
        my_tx_port->setAddress(my_tx_addr);
        my_tx_port->setRemoteAddress(dst_addr);
        my_tx_port->init();

        //create an ACK packet format
        Packet * my_ack_packet;
        my_ack_packet = new Packet();
        my_ack_packet->setPayloadSize(0);
        PacketHdr *hdr = my_ack_packet->accessHeader();
        hdr->setOctet('A',0);
        hdr->setOctet('C',1);
        hdr->setOctet('K',2);
        int numPackets;

        string outputFilename = "outputText.dat";
        fstream outputFile;
        outputFile.open(outputFilename.c_str(), fstream::out);
        char* inputstream;

        cout << "begin receiving..." <<endl;
        Packet *p;
        while (1)
        {
            p = my_port->receivePacket(); 
            if (p !=NULL)
            { 

                if(p->accessHeader()->getOctet(0) == 'S')
                {
                    numPackets = p->accessHeader()->getIntegerInfo(3);
                    inputstream = new char[p->accessHeader()->getIntegerInfo(4)];
                    cout<<"Signaling packet received "<<p->accessHeader()->getIntegerInfo(4)<<" "<<endl;
                    hdr->setIntegerInfo(numPackets,3);
                    my_tx_port->sendPacket(my_ack_packet); 
                }
                else
                {
                    for(int i = 0; i < 72; i++)
                    {
                        //                int i = p->accessHeader()->getIntegerInfo(3);
                        if(p->accessHeader()->getIntegerInfo(3) == i)
                        {     cout << "receiving a packet of seq num " << i << "...sending ACK" << endl; 
                            hdr->setIntegerInfo(i,3);
                            my_tx_port->sendPacket(my_ack_packet); 
                            inputstream = p->getPayload();
                            //                        fwrite(*inputstream, sizeof(char), sizeof(*inputstream), outputFile);   
                            outputFile << inputstream;
                        }   
                    }
                } 
            }
        } 

        outputFile.close();

    } catch (const char *reason ) {
        cerr << "Exception:" << reason << endl;
        exit(-1);
    }  

    return 0;
}


