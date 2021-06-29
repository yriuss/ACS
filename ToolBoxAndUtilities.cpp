/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ToolBoxAndUtilities.h"

void ToolBoxAndUtilities::tic() {
    t0 = std::chrono::steady_clock::now();
}

/**
 * Retorna o tempo transcorrido entre uma chamada de um tic() e o utoc() em microsegundos
 * @return intervalo de tempo em microsegundos(int)
 */
int ToolBoxAndUtilities::utoc() {
    t1 = std::chrono::steady_clock::now();
    int dt = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
    return dt;
}

/**
 * Retorna o tempo transcorrido entre uma chamada de um tic() e o mtoc() em milissegundos
 * @return intervalo de tempo em milisegundos(int)
 */
int ToolBoxAndUtilities::mtoc() {
    t1 = std::chrono::steady_clock::now();
    int dt = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    return dt;
}

/**
 * Retorna o tempo transcorrido entre uma chamada de um tic() e o toc() em segundos
 * @return intervalo de tempo em segundos(int)
 */
int ToolBoxAndUtilities::toc() {
    t1 = std::chrono::steady_clock::now();
    int dt = std::chrono::duration_cast<std::chrono::seconds>(t1 - t0).count();
    return dt;
}
