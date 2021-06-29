/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TPhShifter.h
 * Author: daniel
 *
 * Created on 17 de Março de 2021, 15:08
 */

#ifndef TPHSHIFTER_H
#define TPHSHIFTER_H

#include <iostream>
#include <sstream>
#include <string>

class TPhShifter {
private:
    float step = -1.4606;
    float offset = 0;
    unsigned short phaseInt = 0;

public:
    unsigned short dat[8];

    TPhShifter();
    TPhShifter(const TPhShifter& orig);
    virtual ~TPhShifter();
    
    TPhShifter(const float off);
    void printPhase();

    inline float int2Deg(const int i) {
        return ((i&0xff) * step + offset);
    }

    inline float deg2Int(const float d) {
        int x = ((d - offset) / step);
        return (x&0xff);
    }

    inline TPhShifter& setOffset(const float offset) {
        this->offset = offset; //em graus
        return *this;
    }

    inline float getOffset() const {
        return offset;
    }

    inline TPhShifter& setPhaseDeg(const float d) {
        phaseInt = deg2Int(d);
        return *this;
    }

    inline TPhShifter& setPhaseInt(const unsigned int i) {
        phaseInt = i&0xff;
        return *this;
    }

    TPhShifter& writeDat();
    
    std::string str();
};


#endif /* TPHSHIFTER_H */

