/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TimeMeasurement.h
 * Author: Daniel de Filgueiras Gomes
 *
 * Created on 10 de Julho de 2020, 12:26
 */

#ifndef TIMEMEASUREMENT_H
#define TIMEMEASUREMENT_H

#include <ctime>
#include <sys/time.h>
#include <climits>

namespace TimeMeas {
    //A incerteza nas medidas de tempo desta função está na faixa de 0.1ms
    double t0;
    double t1;
    
    double getCurrentRealTimer(void)
    {
        struct timeval t;
        gettimeofday(&t, 0);
        return t.tv_sec + t.tv_usec * 1.0e-6;
    }
    
    inline void tic(void) {
        t0=getCurrentRealTimer();
    }
    
    inline double toc(void) {
        t1=getCurrentRealTimer();
        return (t1-t0);
    }
}

#endif /* TIMEMEASUREMENT_H */

