/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SystemTests.h
 * Author: daniel
 *
 * Created on 17 de Março de 2021, 09:55
 */

#ifndef SYSTEMTESTS_H
#define SYSTEMTESTS_H

#include <iostream>
#include  <string>
#include "Attenuator.h"
//#define NDEBUG
#include <assert.h>
#include "ToolBoxAndUtilities.h"
#include "TTable.h"
#include "TCommand.h"

void testTCommand() {
    TCommand com;
    
    com.parseCmd("cmd -pi 1 2 3 4 5 6 7");
    
}

void testTTablet() {
    using namespace std;
    TTable tab;
    
    tab.load("arranjo1_angulos20_60_135.csv");
    
    cout << tab.str() << endl;
    cout << "m(1,0)="  << tab.text(1,0) << endl;
    cout << "m(2,0)="  << tab.text(2,0) << endl;
    cout << "m(3,0)="  << tab.text(3,0) << endl;
    
    TTable::TLVector v=tab.lineVector(60);
    
    for(int i=0;i<v.size();++i) {
        cout << v[i] << " ";
    }
    cout << endl << flush;
    cout << "v.size()=" << v.size() << endl;
}

void testToolBoxAndUtilities() {

    using namespace std;

    MSG << "Teste de mensagem de debug" << endl;
    ToolBoxAndUtilities::tic();
    char buf[100];
    int i=100;
    
#ifdef NDEBUG
    cout << "O sistema não está no modo DEBUG!!!!" << endl;
#else
    cout << "O sistema está no modo DEBUG." << endl;
#endif
    
    assert(i<100);
    cout << "mem=" << (int)buf[i] << endl;
    


    MSGE<< "Testes de saída de erro" << endl;
    auto dt = ToolBoxAndUtilities::utoc();

    cout << "dt=" << dt << endl;
}

void testTSerial() {

    using namespace std;


    TSerial usb0;
  
    usb0.sopen("/dev/ttyUSB0");
    usb0.sflush();
    msleep(3500);
    usb0.swrite("INFO\n");
    msleep(40);
    stringstream ss;
    while(usb0.sread()>0){
        ss << usb0.read_buf;
    }

    string s = ss.str();


    //Attenuator  attenuator3;//, attenuator4("R3160950387");

    string SN = "R3160950387";
    string test = " Teste R316 fim";

    cout << "***********************************" << endl;
    cout << "SN:" << SN << endl;
    cout << "***********************************" << endl;
    cout << s << endl;
    cout << "***********************************" << endl;
    
    usb0.sclose();

}

void testGetSN() {
    using namespace std;
    cout << getSN("/dev/ttyUSB1",115200) << endl;
}

void testAttenuator() {
    using namespace std;
    Attenuator att0("R3160950387");
    att0.connect();
    if(att0.isConnected()) 
        cout << "conexão ok! " << att0.getSerialPort() << endl;
}

#endif /* SYSTEMTESTS_H */

