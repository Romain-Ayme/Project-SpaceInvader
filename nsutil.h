/**
*
* @file    nsutil.h
*
* @authors Romain AYME
*
* @version V1.0
* @date    2/1/2020
*
* @brief   Set of usefull functions
*
*/

#ifndef NSUTIL_H
#define NSUTIL_H

#include <string>

#include <iostream>
#include <unistd.h>
#include <fstream>

/** alias to the reset color display */
const std::string    KReset    = "0";
/** alias to the black color */
const std::string    KBlack    = "30";
/** alias to the red color */
const std::string    KRed      = "31";
/** alias to the green color */
const std::string    KGreen    = "32";
/** alias to the yellow color */
const std::string    KYellow   = "33";
/** alias to the blue color */
const std::string    KBlue     = "34";
/** alias to the mangenta color */
const std::string    KMagenta  = "35";
/** alias to the cyan color */
const std::string    KCyan     = "36";

/** alias to the black color */
const std::string    KBBlack    = "40";
/** alias to the red color */
const std::string    KBRed      = "41";
/** alias to the green color */
const std::string    KBGreen    = "42";
/** alias to the yellow color */
const std::string    KBYellow   = "43";
/** alias to the blue color */
const std::string    KBBlue     = "44";
/** alias to the mangenta color */
const std::string    KBMagenta  = "45";
/** alias to the cyan color */
const std::string    KBCyan     = "46";

/**
*
* @brief    Clear the terminal
*
*/
void ClearScreen();

/**
*
* @brief    Display color in the terminal
* @param    [in] Col the color in which the next text will be displayed in
*
*/
void Color(const std::string &Col);

/**
*
* @brief    Go to the position on terminal
* @param    [in] X : Line
* @param    [in] Y : Column
*
*/
void GotoXY(const unsigned &X, const unsigned &Y);

/**
*
* @brief    Add delay
* @param    [in] ms : the time to wait in millisecond
*
*/
void Delay(const unsigned long long &ms);

/**
*
* @brief    Display document on terminal with coordinates
* @param    [in] File : Name of document
* @param    [in] X : Line
* @param    [in] Y : Column
*
*/
void DisplaysDoc (const std::string File, unsigned int X = 0, unsigned int Y = 0);

#endif // NSUTIL_H
