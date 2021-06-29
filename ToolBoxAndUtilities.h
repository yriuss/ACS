/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ToolBoxAndUtilities.h
 * Author: Daniel de Filgueiras Gomes
 *
 * Created on 10 de Março de 2021, 17:15
 */

#ifndef TOOLBOXANDUTILITIES_H
#define TOOLBOXANDUTILITIES_H

#include <stdio.h>
#include <iostream>
#include <chrono>
#include <unistd.h>

#define DEBUG_MSG

#ifdef DEBUG_MSG
#define MSG std::cout << __FILE__ << ":" << __func__ << ":" << __LINE__ << ":" 
#define MSGE std::cerr << __FILE__ << ":" << __func__ << ":" << __LINE__ << ":" 
#else
#define MSG
#define MSGE
#endif

#define msleep(x) usleep(x*1000)

namespace ToolBoxAndUtilities {
    static std::chrono::steady_clock::time_point t0;
    static std::chrono::steady_clock::time_point t1;

    void tic();

    /**
     * Retorna o tempo transcorrido entre uma chamada de um tic() e o utoc() em microsegundos
     * @return intervalo de tempo em microsegundos(int)
     */
    int utoc();

    /**
     * Retorna o tempo transcorrido entre uma chamada de um tic() e o mtoc() em milissegundos
     * @return intervalo de tempo em milisegundos(int)
     */
    int mtoc();

    /**
     * Retorna o tempo transcorrido entre uma chamada de um tic() e o toc() em segundos
     * @return intervalo de tempo em segundos(int)
     */
    int toc();
}

#endif /* TOOLBOXANDUTILITIES_H */

