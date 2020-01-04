/**
*
* @file    nsutil.cpp
*
* @authors Romain AYME
*
* @version V1.0
* @date    2/1/2020
*
* @brief   Set of usefull functions
*
*/

#include "nsutil.h"

void ClearScreen () {
    std::cout << "\033[H\033[2J";
}

void Color (const std::string & Col) {
    std::cout << "\033[" << Col <<"m";
}


void Delay(unsigned long ms) {
    usleep(ms * 1000);
}


