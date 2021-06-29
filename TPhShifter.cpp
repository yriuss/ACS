/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TPhShifter.cpp
 * Author: daniel
 * 
 * Created on 17 de Março de 2021, 15:08
 */

#include "TPhShifter.h"

TPhShifter::TPhShifter() {
    this->offset = 0;
    this->step = -1.4606;
    for (int i = 0; i < 8; ++i) dat[i] = 0;    
}

TPhShifter::TPhShifter(const TPhShifter& orig) {
    offset=orig.offset;
    step=orig.step;
    phaseInt=orig.phaseInt;
    for (int i = 0; i < 8; ++i) dat[i] = orig.dat[i];
}

TPhShifter::~TPhShifter() {
}

TPhShifter::TPhShifter(const float off) {
    this->offset = off;
    this->step = -1.4606;
    for (int i = 0; i < 8; ++i) dat[i] = 0;
}

TPhShifter& TPhShifter::writeDat() {
    dat[0] = (phaseInt & 0x01);
    dat[1] = (phaseInt >> 1 & 0x01);
    dat[2] = (phaseInt >> 2 & 0x01);
    dat[3] = (phaseInt >> 3 & 0x01);
    dat[4] = (phaseInt >> 4 & 0x01);
    dat[5] = (phaseInt >> 5 & 0x01);
    dat[6] = (phaseInt >> 6 & 0x01);
    dat[7] = (phaseInt >> 7 & 0x01);

#ifdef DEBUG_MODE
    cout << "***************" << endl;
    cout << str();
    cout << endl << "***************" << endl;
#endif
    return *this;
}

std::string TPhShifter::str() {
    std::stringstream ss;
    ss << "[" << phaseInt << "] ";
    for (int i = 0; i < 8; ++i) {
        ss << dat[i] << " ";
    }
    return ss.str();
}

void TPhShifter::printPhase() {
    printf("\nphase is %hu\n", this->phaseInt);
}