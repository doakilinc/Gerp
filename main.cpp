/* main.cpp
 * Doga Kilinc & Cansu Birsen
 * 12/09/2022
 *
 * CS 15 Proj 4: Gerp
 *
 * Purpose: This is the driver file, which creates an instance of the 
 * DictionaryBuilder class. Command line arguments should be 
 * "./gerp inputDirectory outputFile" to invoke this program. If invalid
 * command line arguments are given, it throws and error message and
 * terminates. If the command line arguments are valid, this file calls query
 * function of the to DictionaryBuilder class to ask user for commands and 
 * perform neccessary operations.
 */


#include<string>
#include<iostream>

#include "MapBuilder.h"
#include "DictionaryBuilder.h"

using namespace std;

int main(int argc, char *argv[]) {

    //includes the correct usage of the program
    std::string message = "Usage: ./gerp inputDirectory outputFile";

    //checks the number of arguments, if wrong, prints a cerr message & exits
    if (argc != 3) {
        cerr << message << endl;
        exit(EXIT_FAILURE); //quits
    }

    //constructs the DictionaryBuilder class with the input and ouput files
    DictionaryBuilder d(argv[1], argv[2]);
    d.query();
    
    return 0;
}
