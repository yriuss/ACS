/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TCAT.h
 * Author: daniel
 *
 * Created on 17 de Março de 2021, 15:30
 */

#ifndef TACS_H
#define TACS_H


#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <math.h>
#include <string.h>
#include "TPhShifter.h"
#include "TCommand.h"
#include "Attenuator.h"
/*
 * No Raspberry:
 Habilitar -> ARCH_RASP
 * defasador.cpp

   gcc -o defasador defasador.c -lpigpio -lrt -lpthread

   sudo ./defasador
 */


//#define ARCH_RASP 1
#define DEBUG_MODE 1
#define OPEN_DNP3_LIB 1

#ifdef ARCH_RASP
#include <pigpio.h>
#endif


/* Sockets buffers length */
#define LEN 512 //tamanho da string do payload
#define LSBy(num) \
	(char) num & 0x00FF
#define MSBy(num) \
	(char) (num >> 8)

#define CRC16_DNP 0xA6BC ///Reversed 0x3D65
typedef unsigned char data_arr[16];

#include "TTcpIPServer.h"
#include "TTcpIPClient.h"
#include <memory>
#include <csignal>
#include <stdio.h>
#include <stdint.h>
#include <vector>


//#define LEN 10000


#include "stdlib.h"
#include <iostream>
#include "PcapLiveDeviceList.h"
#include "SystemUtils.h"
#include <chrono>
#include <PayloadLayer.h>
#include "stdlib.h"
#include "SystemUtils.h"
#include "Packet.h"
#include "EthLayer.h"
#include "IPv4Layer.h"
#include "TcpLayer.h"
#include "HttpLayer.h"
#include "PcapFileDevice.h"
#include "SystemUtils.h"
#include <string>

#define revertCRC(num) \
    (uint16_t) ((num >> 8) << 8) + (((num << 8) >> 8) & 0xFF)

uint16_t crc16(uint8_t const *data, size_t size);

class TAcs: public TCommand{
private:
	TPhShifter ps[3];
	Attenuator att0{"R3160950384"},att1{"R3160950386"},att2{"R3160950383"},att3{"R3160950387"};
	bool manual = 0;
    TTcpIPServer server;
    int COIPort;
#ifdef OPEN_DNP3_LIB
    char DNP3Frame[LEN];
    unsigned short int DNP3FrameLen;
    char control;
#endif
    char DNP3Msg[LEN];
    unsigned short int DNP3MsgLen;
    int DNP3Addr;
    TTcpIPClient client;
public:
    TAcs(const TAcs& orig);
    TAcs();
    virtual ~TAcs();

    void interceptPacket();
	/* Communication*/
#ifdef OPEN_DNP3_LIB
    void calculateMsgLen();
#endif
	int connectToCOI(const char* COIAddr, int COIPort);
    int read_dnp_frame ();
	int read_dnp_msg ();
	int getDNP3Address ();
    int connectToOutstation (char* OutstationAddr,int port);
	int talkToOutstation (char* outstationAddr,int port,pthread_t t1);
    void gen_header();
	void closeConnection();
    uint32_t generateCRC();
    
    /* Setting attenuations and phase shifts*/
	void setPhases();

	void connectAttenuators();
	void AttenuatePot();
	void unconnectAttenuators();

    TAcs& loadParameters(std::string file = "offset.txt") {
        using namespace std;

        ifstream phOffset;

        phOffset.open(file, std::ifstream::in);

        if (!phOffset) {
            cerr << "Arquivo " << file << " não encontrado!!!";
            cout << "Utilizando fases padrões." << endl;
        } else {
#ifdef DEBUG_MODE
            cout << "Lendo arquivo de offsets." << endl;
#endif
            float x;
            for (int i = 0; i < 3; ++i) {
                phOffset >> x;
                ps[i].setOffset(x);

#ifdef DEBUG_MODE
                cout << "D(" << i << ")_off=" << ps[i].getOffset() << endl;
#endif
            }
        }

        phOffset.close();
        return *this;
    }

    TAcs& saveParameters(std::string file = "offset.txt") {
        using namespace std;
        ofstream phOffsetOut;
        cout << "Criando arquivo " << file << endl;
        phOffsetOut.open(file, std::ofstream::out);

#ifdef DEBUG_MODE
        cout << "Gravando os offsets:" << endl;
#endif
        for (int i = 0; i < 3; ++i) {
            phOffsetOut << ps[i].getOffset() << " ";

#ifdef DEBUG_MODE
            cout << "D(" << i << ")_off=" << ps[i].getOffset() << endl;
#endif
        }

        phOffsetOut.close();

        return *this;
    }

    TAcs& writePhases() {
        for (int i = 0; i < 3; ++i){
		ps[i].writeDat();
	}
#ifdef ARCH_RASP
        for (int j = 0; j < 8; ++j) {
            gpioWrite(mapPort0[j], ps[0].dat[j]); /* on */
            gpioWrite(mapPort1[j], ps[1].dat[j]); /* on */
            gpioWrite(mapPort2[j], ps[2].dat[j]); /* on */
        }
#endif
        return *this;
    }

    const short unsigned int mapPort0[8] = {22, 10, 9, 11, 0, 27, 17, 4};
    const short unsigned int mapPort1[8] = {19, 26, 21, 20, 16, 13, 6, 5};
    const short unsigned int mapPort2[8] = {8, 25, 24, 23, 18, 7, 1, 12};
};

class PacketStats
{
	
	//pcpp::Packet& cb_packet;
	
public:
	int ethPacketCount;
	int ipv4PacketCount;
	int ipv6PacketCount;
	int icmpPacketCount;
	int icmpPacketCount_cb;
	int tcpPacketCount;
	int udpPacketCount;
	int dnsPacketCount;
	int httpPacketCount;
	int sslPacketCount;
	int sshPacketCount;
	pcpp::Packet cb_packet;
	pcpp::RawPacket* cb_rawpacket;
	/**
	 * Clear all stats
	 */
	void clear() { icmpPacketCount_cb = 0; icmpPacketCount = 0; sshPacketCount = 0; ethPacketCount = 0; ipv4PacketCount = 0; ipv6PacketCount = 0; tcpPacketCount = 0; udpPacketCount = 0; tcpPacketCount = 0; dnsPacketCount = 0; httpPacketCount = 0; sslPacketCount = 0; }

	/**
	 * C'tor
	 */
	PacketStats() { clear(); }

	/**
	 * Collect stats from a packet
	 */
	void consumePacket(pcpp::Packet& packet)
	{
		//this->cb_packet = packet;
        
		if (packet.isPacketOfType(pcpp::Ethernet))
			ethPacketCount++;
		if (packet.isPacketOfType(pcpp::SSH))
			sshPacketCount++;
		if (packet.isPacketOfType(pcpp::ICMP)){
            icmpPacketCount_cb++;
			icmpPacketCount++;
			
		}
		if (packet.isPacketOfType(pcpp::IPv4))
			ipv4PacketCount++;
		if (packet.isPacketOfType(pcpp::IPv6))
			ipv6PacketCount++;
		if (packet.isPacketOfType(pcpp::TCP))
			tcpPacketCount++;
		if (packet.isPacketOfType(pcpp::UDP))
			udpPacketCount++;
		if (packet.isPacketOfType(pcpp::DNS))
			dnsPacketCount++;
		if (packet.isPacketOfType(pcpp::HTTP))
			httpPacketCount++;
		if (packet.isPacketOfType(pcpp::SSL))
			sslPacketCount++;
	}

	/**
	 * Print stats to console
	 */
	void printToConsole()
	{
		printf("Ethernet packet count: %d\n", ethPacketCount);
		printf("IPv4 packet count:     %d\n", ipv4PacketCount);
		printf("IPv6 packet count:     %d\n", ipv6PacketCount);
		printf("TCP packet count:      %d\n", tcpPacketCount);
		printf("UDP packet count:      %d\n", udpPacketCount);
		printf("DNS packet count:      %d\n", dnsPacketCount);
		printf("HTTP packet count:     %d\n", httpPacketCount);
		printf("SSL packet count:      %d\n", sslPacketCount);
		printf("ICMP packet count:      %d\n", icmpPacketCount);
	}
};

static void onPacketArrives(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* cookie);
void calculateMsgLen(char* msg);

#endif /* TACS_H */

