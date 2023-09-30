/* DictionaryBuilder.h
 * Doga Kilinc & Cansu Birsen
 * 12/09/2022
 *
 * CS 15 Proj 4: Gerp
 *
 * Purpose: DictionaryBuilder is a class that reads all accessible files in a 
 * given directory and records each word and its path and line information
 * in a map. A user can use this program with specific commands. If the user 
 * enters the command "@i" or "@insensitive" followed by a word, the program
 * implements a case insensitive search in the files, and prints the 
 * information (the file location, line number, and line itself) about the word
 * to the provided output file. If the word does not exist in any of the files,
 * the program prints an appropriate message instead. The user can also 
 * implement a case sensitive search that provides a similar output by just 
 * providing the word itself as a command. When doing a word search, the 
 * program eliminates the leading and trailing non alphanumeric characters in 
 * the word the user enters. The program also avoids printing duplicate 
 * information if the searched word is found more than once in the same line. 
 * Lastly, the user can change the output file with command "@f" folllowed by 
 * the new output file name and use the command "@q" or "@quit" to end the 
 * program. 
 */

#ifndef __DICTIONARY_BUILDER_H__
#define __DICTIONARY_BUILDER_H__

#include "MapBuilder.h"
#include "DirNode.h"

#include <string>
#include <list>
#include <vector>
#include <set>
#include <iostream>
#include <fstream>

class DictionaryBuilder {

public:

    //constructor and deconstructor
    DictionaryBuilder(std::string directoryName, std::string outputName);
    ~DictionaryBuilder();

    //function
    void query();
    
private:

    //member variables

    //struct used to store a pointer to a string with the path information of a
    //file and another pointer to a vector storing individual lines in that 
    //file
    struct pathsAndFiles {
        std::string *path;
        std::vector<std::string> *fileLines;

        //constructors for struct
        pathsAndFiles() {    
            path = nullptr;
            fileLines = nullptr;
        }
        
        pathsAndFiles(std::string filePath) {    
            path = new std::string(filePath);
            fileLines = new std::vector<std::string>;
        }
    };
    
    MapBuilder map;
    std::vector<pathsAndFiles> files;
    std::string directory;
    std::string outputFile;
    std::ofstream output;


    //private functions
    void pathFinder();
    void fileReader();
    void caseInsensitiveSearch(std::string original);
    void caseSensitiveSearch(std::string original);
    void commandHandler();
    
    //helper functions
    void pathFinderHelper(DirNode *root, std::string pathName);
    template<typename streamtype>
    void openOrDie(streamtype &stream, std::string *filePath);
    void searchPrinter(std::set<std::string> *address, std::ofstream &outfile);
    std::string stripNonAlphaNum(std::string input);
    
};

#endif