#include<iostream>
#include<vector>
#include<fstream>
#include"common.h"
#include"newport.h"
using namespace std;


int main(int argc, char* argv[])
{
    vector<Packet* > myPackets;
    int packetSize = 1500;
    int hdrSize = 100;
    int payloadSize = packetSize - hdrSize;    
    int numPackets = 0;
    char* inputstream;
    inputstream = new char[payloadSize];

    /* Read file into packets */
    string inputFilename("text2.dat"); 
    fstream inputFile;
    inputFile.open(inputFilename.c_str(), fstream::in);

    //    while((inputFile.read(inputstream, payloadSize)).good())
    while(true)
    {

        inputFile.read(inputstream, payloadSize);
//               cout<<inputstream;

        myPackets.push_back(new Packet(packetSize));
        myPackets.back()->fillPayload(payloadSize, inputstream); //Last pushed packet

        PacketHdr* hdr = myPackets.back()->accessHeader();    
        //        hdr->setHeaderSize(hdrSize);
        hdr->setOctet('D', 0); 
        hdr->setOctet('A',1);
        hdr->setOctet('T',2);
        hdr->setIntegerInfo(numPackets, 3);
        numPackets++;

        if(inputFile.eof())
            break;

    }
    delete[] inputstream;
//    cout<<numPackets<<endl;

    try {

        const char* hname = "localhost";       
        Address * my_tx_addr = new Address(hname, 3000);

        //configure sending port
        Address * dst_addr =  new Address(argv[1], (short)(atoi(argv[2])));
        mySendingPort *my_port = new mySendingPort();
        my_port->setAddress(my_tx_addr);
        my_port->setRemoteAddress(dst_addr);
        my_port->init();

        //configure receiving port to listen to ACK frames
        Address * my_rx_addr = new Address(hname, (short)(atoi(argv[3])));
        LossyReceivingPort *my_rx_port = new LossyReceivingPort(0.2);
        my_rx_port->setAddress(my_rx_addr);
        my_rx_port->init();

        //create a single signal packet
        Packet * my_packet;
        my_packet = new Packet();
        PacketHdr *hdr = my_packet->accessHeader();
        hdr->setOctet('S',0);
        hdr->setOctet('I',1);
        hdr->setOctet('G',2);
        hdr->setIntegerInfo(numPackets, 3);
        hdr->setIntegerInfo(payloadSize, 4);        

        my_port->sendPacket(my_packet);
        cout<<"Signaling Packet sent"<<endl;
        my_port->setACKflag(false);
        my_port->timer_.startTimer(2.5);
        cout << "begin waiting for ACK..." <<endl;
        Packet *pAck1;
        while (!my_port->isACKed()){
            pAck1 = my_rx_port->receivePacket();
            if (pAck1!= NULL)
            {
                my_port->setACKflag(true);
                my_port->timer_.stopTimer();
                cout << "Signaling packet acknowelged" <<endl;

            }
        }; 

        for(int i = 0; i < numPackets; i++)
        {
            my_packet = myPackets[i];

            //sending it
            my_port->sendPacket(my_packet);
            my_port->lastPkt_ = my_packet; 
//            cout << "packet is first sent!" <<endl;
            my_port->setACKflag(false);
            //schedule retransmit
            my_port->timer_.startTimer(2.5);  

//            cout << "begin waiting for ACK..." <<endl;
            Packet *pAck;
            pAck = new Packet();
            while (!my_port->isACKed()){
                pAck = my_rx_port->receivePacket();
                if (pAck!= NULL)
                {
                    my_port->setACKflag(true);
                    my_port->timer_.stopTimer();
                    cout << "The packet  "<<i<<" has been acknowledged." <<endl;

                }
            };
            delete[] pAck; 
        } 


    } catch (const char *reason ) {
        cerr << "Exception:" << reason << endl;
        exit(-1);
    }  

    return 0;

}
