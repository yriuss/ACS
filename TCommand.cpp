/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TCommand.cpp
 * Author: daniel
 * 
 * Created on 17 de Março de 2021, 17:22
 */

#include <cstring>

#include "TCommand.h"
#include "TTable.h"

TCommand::TCommand() {
}

TCommand::TCommand(const TCommand& orig) {
}

TCommand::~TCommand() {
}

TCommand& TCommand::parseCmd(const char *cmd, const char *fs) {
    using namespace std;

    int N = strlen(cmd);
    char *str = (char*) malloc((N + 1) * sizeof (char));
    strcpy(str, cmd);
    char *command_line_inputs[10];
    char *ptr = 0;
    ptr = strtok(str, fs);
    command_line_inputs[0] = ptr;
    int k = 1;

    while (ptr != NULL) {
        //printf("'%s'\n", ptr);
        ptr = strtok(NULL, fs);
        command_line_inputs[k] = ptr;
        k++;
        if (k > 9)
            break;
    }

    this->parseCmd(k-1, command_line_inputs);

    free(str);
    return *this;
}

/*
 * Modo comando de linha:
 * acs set -pd <ph1> <ph2> <ph3> [ <at0> <at1> <at2> <at3> ]
 * acs set -pi <ph1> <ph2> <ph3> [ <at0> <at1> <at2> <at3> ]
 * acs -idx <nidx>
 * acs set -pd <ang> <arrangement>
 * acs set -pi <ang> <arrangement>
 * acs -help
 */
TCommand& TCommand::parseCmd(int command_counter, char** command_line_inputs) {
    using namespace std;
	//printf("%d \n\n", command_counter);
    switch(command_counter){
		case 1:
			MSGE << "Not enough arguments!" << endl;
			break;
		case 2:
			if (strcmp(command_line_inputs[1], "-help") == 0) {
				cout << "Usage:" << endl;
				cout << "acs set -pd <ph1> <ph2> <ph3> [ -at <at0> <at1> <at2> <at3> ]" << endl;
				cout << "acs set -pi <ph1> <ph2> <ph3> [ -at <at0> <at1> <at2> <at3> ]" << endl;
				cout << "acs -pi <ang> <arrangement>" << endl;
				cout << "acs -pd <ang> <arrangement>" << endl;
				cout << "acs -help" << endl;
			}else{
				MSGE << "Invalid command!" << endl;
			}
			break;
		case 3:
			if (strcmp(command_line_inputs[1], "-idx") == 0) {
			}else{
				MSGE << "Invalid command!" << endl;
			}
			break;
		case 5:
			if (strcmp(command_line_inputs[2], "-pi") == 0) {
				inputType = 1;
				TTable tab;
				tab.load(strcat(command_line_inputs[4],"_angulos20_60_135.csv"));//"arranjo1_angulos20_60_135.csv");
				short int angle = (short int)strtod(command_line_inputs[3],NULL);
				TTable::TLVector read_line =tab.lineVector(angle);
				
				for(int i=0;i<read_line.size() - 1;++i){
					if(i < 3)
						ph[i] = read_line[i + 1];
					else
						attenuations[i - 3] = read_line[i + 1];
				}
				break;
			}
			if(strcmp(command_line_inputs[2], "-pd") == 0){
				inputType = 0;
				TTable tab;
				tab.load(strcat(command_line_inputs[4],"_angulos20_60_135.csv"));//"arranjo1_angulos20_60_135.csv");
				short int angle = (short int)strtod(command_line_inputs[3],NULL);
				
				TTable::TLVector read_line=tab.lineVector(angle);
				
				for(int i=0;i<read_line.size() - 1;++i){
					if(i < 3)
						ph[i] = read_line[i + 1];
					else
						attenuations[i - 3] = read_line[i + 1];
				break;
				}
			}
			MSGE << "Invalid command!" << endl;
			break;
		case 10:
			if (strcmp(command_line_inputs[2], "-pi") == 0) {
				inputType = 1;
				for(int i = 0; i < 3; i++)
					ph[i] = (float)strtod(command_line_inputs[i + 3],NULL);
				for(int i = 3; i < 7; i++)
					attenuations[i - 3] = (float)strtod(command_line_inputs[i + 3],NULL);
				break;
			}
			if(strcmp(command_line_inputs[2], "-pd") == 0){
				for(int i = 0; i < 3; i++)
					ph[i] = (float)strtod(command_line_inputs[i + 3],NULL);
				for(int i = 3; i < 7; i++)
					attenuations[i - 3] = (float)strtod(command_line_inputs[i + 3],NULL);
				break;
			}
			MSGE << "Invalid command!" << endl;
			break;
		default:
			MSGE << "Invalid command!" << endl;
		
	}
    return *this;
}

void TCommand::printTest() {
	for(int i = 0; i< 7; i++){
		if(i < 3)
			std::cout << this->ph[i] << std::endl;
		else
			std::cout << this->attenuations[i - 3] << std::endl;
	}
}



attenuation_arr& TCommand::getAttenuations(attenuation_arr& out_arr){
	for(int i = 0; i < 4; i++)
		out_arr[i] = this->attenuations[i];
	return out_arr;
}



ph_arr& TCommand::getPhases(ph_arr& out_arr){
	for(int i = 0; i < 3; i++)
		out_arr[i] = this->ph[i];
	return out_arr;
}


bool TCommand::isInt(){
	return this->inputType;
}