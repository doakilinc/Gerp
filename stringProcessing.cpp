/* stringProcessing.cpp
 * Doga Kilinc & Cansu Birsen
 * 12/02/2022
 *
 * CS 15 Proj 4: Gerp
 *
 * Purpose: This file contains the stripNonAlphaNum function which takes 
 * a string from the client and manipulates it so that there are no
 * leading or trailing non-alpha numeric characters in the string
 */

#include "stringProcessing.h"

#include <string>
#include <cctype>

using namespace std;

 /*
 * name:      stripNonAlphaNum
 * purpose:   modifies the string so that there are no leading or trailing
 *            non-alpha numeric characters in the given string
 * arguments: given user input to be manipulated
 * returns:   the modified version of the string
 * effects:   prepares the given string for a later word search
 */
string stripNonAlphaNum(string input) {
    int length = input.size();
    int index = 0;
    //checks for empty string
    if (length == 0)
        return input;

    //finds the index of first alpha numeric char
    while (not isalnum(input[index])) {
        index++;
        //checks if the whole string consists of non-alpha chars
        if (index == length)
            return "";
    }
    //modifies the given string from the start
    input = input.substr(index, length - index);

    //updates the variables
    length = input.size();
    index = length - 1;

    //find the index of the first alpha numeric char starting from the back
    while (not isalnum(input[index])) {
        index--;
    }
    //modifies the given string from the end
    input = input.substr(0, index + 1);

    return input;
}


        



