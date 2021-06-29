/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TTable.cpp
 * Author: Daniel de Filgueiras Gomes
 * 
 * Created on 17 de Março de 2021, 11:19
 */

#include "TTable.h"

TTable::TTable() {
}

TTable::TTable(const TTable& orig) {
    memTab=orig.memTab;
}

TTable::~TTable() {
}

int TTable::load(const char *fileName) {
    using namespace std;
    memTab.clear();

    ifstream file(fileName);
    
    string line;
    string cell;
    bool successful=false;

    while (getline(file, line)) {
        std::istringstream is(line);
        TLine v;
        
        while (getline(is, cell, ',')) {
            v.push_back(cell);

        }
        
        memTab.push_back(v);
        successful=true;
    }

    file.close();
    
    return successful;
}

std::string TTable::str() {
    using namespace std;
    
    int M=memTab.size();
    
    stringstream ss;
    
    for(int i=0;i<M;++i) {
        int N=memTab[i].size();
        
        for(int j=0;j<N;++j) {
           ss << " " << memTab[i][j];
        }
        ss << endl;
    }
    
    return ss.str();
}

std::string TTable::text(unsigned int i, unsigned j) {
    using namespace std;
    
    if(i>=0 && i<memTab.size()) {
        if(j>=0 && j<memTab[i].size())
            return memTab[i][j];
        else
            MSGE << "Invalid column number index!" << endl;
    }
    else
        MSGE << "Inlavid line number index!" << endl;
    
    return "";
}

float TTable::number(unsigned int i, unsigned j) {
    return atof(text(i,j).c_str());
}

TTable::TLVector TTable::lineVector(const char* lb, unsigned int c) {
    using namespace std;
    TLVector v;
    
    TTab::iterator it;
    for(it=memTab.begin();it!=memTab.end();++it) {
        int x=(*it)[c].compare(lb);
        MSG << x << " " << (*it)[c] << endl;
        if(x==0) {
            int N=it->size();
            
            for(int j=0;j<N;++j) {
                v.push_back(atof((*it)[j].c_str()));
            }
            break;
        }
    }
    
    return v;
}

TTable::TLVector TTable::lineVector(const float k, unsigned int c) {
    using namespace std;
    TLVector v;
    
    TTab::iterator it;
    for(it=memTab.begin();it!=memTab.end();++it) {
        if(atof((*it)[c].c_str())==k) {
            int N=it->size();
            
            for(int j=0;j<N;++j) {
                v.push_back(atof((*it)[j].c_str()));
            }
            break;
        }
    }
    
    return v;
}