/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TLogSystem.h
 * Author: Daniel de Filgueiras Gomes
 *
 * Created on 08 de Março de 2021, 09:29
 */

#ifndef TLOGSYSTEM_H
#define TLOGSYSTEM_H

class TLogSystem {
public:
    enum { FILENAME_FLAG=0x01, DATA_FLAG=0x02, TIME_FLAG=0x04, LINE_FLAG=0x08, STDOUT=0x10, STDERR=0x20, OUTFILE=0x40, ENABLE=0x80};

    TLogSystem();
    TLogSystem(const TLogSystem& orig);
    TLogSystem(const std::string &filaName);
    virtual ~TLogSystem();
    

    TLogSystem& setOutFile(const std::string &fileName);
    std::string& getFileName();
    TLogSystem& setOutFile(bool flag);
    TLogSystem& setStdout(bool flag);
    TLogSystem& setStderr(bool flag);
    TLogSystem& enable(bool flag);
    TLogSystem& config(unsigned short int cfg);
    TLogSystem& msg(const std::string &txt);
    TLogSystem& getuTime();
    TLogSystem& getmTime();
    TLogSystem& getTime();
    
private:
    std::chrono::steady_clock::time_point t0;
    std::chrono::steady_clock::time_point t1;
    std::chrono::steady_clock::time_point t2;
    unsigned short int cfg;
    std::string fileName;
    void init();

};

#endif /* TLOGSYSTEM_H */

