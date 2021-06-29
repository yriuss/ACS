/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TCommand.h
 * Author: daniel
 *
 * Created on 17 de Março de 2021, 17:22
 */

#ifndef TCOMMAND_H
#define TCOMMAND_H

#include <iostream>
#include <string>
#include <cstdlib>
#include "ToolBoxAndUtilities.h"
#include <stdio.h>
#include <string.h>

typedef float ph_arr[3];
typedef float attenuation_arr[4];
class TCommand {
public:
    TCommand();
    TCommand(const TCommand& orig);
    virtual ~TCommand();
    virtual TCommand& parseCmd(const char *cmd,const char *fs=" ");
    virtual TCommand& parseCmd(int argc, char** argv);
	void printTest();
	attenuation_arr& getAttenuations(attenuation_arr& out_arr);
	ph_arr& getPhases(ph_arr& out_arr);
	bool isInt();
private:
	float attenuations[4];
	float ph[3];
	bool inputType;
};

#endif /* TCOMMAND_H */

