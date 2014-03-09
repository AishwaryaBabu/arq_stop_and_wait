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

        LossyReceivingPort *my_port = new LossyReceivingPort(0.0);
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
        int packetCounter = 0; 
        int payloadSize;
        string outputFilename = "outputText.dat";
        fstream outputFile;
        outputFile.open(outputFilename.c_str(), fstream::out);
        //        outputFile.open("opTxt.dat", ios::out | ios::app);
        char* inputstream;
        int expectedNumPackets;
        cout << "begin receiving..." <<endl;
        Packet *p;
        //Packet *last_packet;
        while (1)
        {

            p = my_port->receivePacket();
          //  cout<<p->getPayload()<<endl;
            if (p !=NULL)
            { 

                if(p->accessHeader()->getOctet(0) == 'S')
                {
                    //                        inputstream = new char[p->accessHeader()->getIntegerInfo(7)];
                    expectedNumPackets = p->accessHeader()->getIntegerInfo(3);
                    payloadSize = p->accessHeader()->getIntegerInfo(7);
                    cout<<"Signaling packet received "<<endl;
                    my_tx_port->sendPacket(my_ack_packet); 
                }
                else
                {

                    if(p->getPayloadSize() < payloadSize)
                    {
                        cout << "receiving a packet of seq num " << packetCounter << "...sending ACK" << endl; 
                        hdr->setIntegerInfo(packetCounter,3);
                        my_tx_port->sendPacket(my_ack_packet); 
                        if(p->accessHeader()->getIntegerInfo(3) == packetCounter)
                        {
                            
                            int paySize = p->getPayloadSize();
                            cout<<paySize<<endl;
                            char* tempstream;
                           // tempstream = new char[paySize];
                            tempstream = p->getPayload();
//                            cout<<tempstream<<endl;
//                            cout<<p->getPayload()<<endl;
                            
                            for(int i = 0; i < paySize; i++)
                            {
                                outputFile << tempstream[i]; 
                                cout << tempstream[i];
                            }
                            my_tx_port->sendPacket(my_ack_packet); 
                            my_tx_port->sendPacket(my_ack_packet); 
                            my_tx_port->sendPacket(my_ack_packet); 

                            packetCounter = 0;
                            outputFile.close();
                            exit(-1);

                        }

                    }

                    else
                    {    

                        //      cout<<p->accessHeader()->getIntegerInfo(3)<<endl;
                        //           cout<<p->accessHeader()->getIntegerInfo(7)<<endl;
                        // inputstream = new char[p->accessHeader()->getIntegerInfo(7)];
                        cout << "receiving a packet of seq num " << packetCounter << "...sending ACK" << endl; 
                        hdr->setIntegerInfo(packetCounter,3);
                        my_tx_port->sendPacket(my_ack_packet); 
                        //inputstream = p->getPayload();
                        //cout<<inputstream<<endl;
                        if(p->accessHeader()->getIntegerInfo(3) == packetCounter)
                        {   
                            //                        fwrite(*inputstream, sizeof(char), sizeof(*inputstream), outputFile);   
                            outputFile << p->getPayload();
                            packetCounter++;
                        }
                    }

                } 
            }


        } 
//        outputFile.close();

    } catch (const char *reason ) {
        cerr << "Exception:" << reason << endl;
        exit(-1);
    }  

    return 0;
}


