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

void ClearScreen() {
    std::cout << "\033[H\033[2J";
} // ClearScreen()

void Color(const std::string &Col) {
    std::cout << "\033[" << Col <<"m";
} // Color()

void GotoXY(const unsigned &X, const unsigned &Y) {
    std::cout << "\033[" << Y << ';' << X << "H";
} // GotoXY()

void Delay(const unsigned long long &ms) {
    usleep(ms * 1000);
} // Delay()

void DisplaysDoc (const std::string File, unsigned int X, unsigned int Y) {
    std::fstream ifs (File);
    if (!ifs.is_open())
        std::cerr << "error : file are missing or can't be open" << std::endl;
    else {
        while (!ifs.eof()) {
            std::string Line;
            getline(ifs, Line);
            GotoXY(X, Y++);
            std::cout << Line;
        }
        ifs.close();
    }
} // DisplayDoc()


