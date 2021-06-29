/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TTable.h
 * Author: daniel
 *
 * Created on 17 de Março de 2021, 11:19
 */

#ifndef TTABLE_H
#define TTABLE_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "ToolBoxAndUtilities.h"

class TTable {
public:
    
    typedef std::string TCell;
    typedef std::vector<TCell> TLine;
    typedef std::vector<float> TLVector;
    typedef std::vector<TLine> TTab;
    
    TTable();
    TTable(const TTable& orig);
    virtual ~TTable();
    
    int load(const char *fileName);
    std::string str();
    std::string text(unsigned int i, unsigned j);
    TLVector lineVector(const char* lb, unsigned int c=0);
    TLVector lineVector(const float k, unsigned int c=0);
    float number(unsigned int i, unsigned j);
    
private:
    TTab memTab;

};

#endif /* TTABLE_H */

