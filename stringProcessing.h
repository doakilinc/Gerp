/* stringProcessing.h
 * Doga Kilinc & Cansu Birsen
 * 12/02/2022
 *
 * CS 15 Proj 4: Gerp
 *
 * Purpose: This file contains the stripNonAlphaNum function which takes 
 * a string from the client and manipulates it so that there are no
 * leading or trailing non-alpha numeric characters in the string.
 */

#ifndef __STRING_PROCESSING_H__
#define __STRING_PROCESSING_H__

#include <string>

//function declaration
std::string stripNonAlphaNum(std::string input);

#endif