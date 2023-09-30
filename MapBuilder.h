/* MapBuilder.h
 * Doga Kilinc & Cansu Birsen
 * 12/04/2022
 *
 * CS 15 Proj 4: Gerp
 *
 * Purpose: MapBuilder is a class that creates an empty hash map that can store
 * a given word and its path and line information. As the key, the map stores 
 * the lower case version of the word. As the value, it stores a list of 
 * pointers to CaseSensitive structs that hold the original version of the word
 * and path&line information for that version.
 * This file also contains the CaseSensitive struct, which holds the original
 * version of a given word and a pointer to a string set of path&line 
 * information for a particular version of a word.
 */

#ifndef __MAP_BUILDER_H__
#define __MAP_BUILDER_H__

#include <string>
#include <list>
#include <set>
#include <iostream>

//This struct holds the original version of a given word and a pointer to a
//set of strings which represents the path and line number information of a 
//given word
struct CaseSensitive {

    std::string textOriginal;
    std::set<std::string> *infoSet;

    //constructor for struct
    CaseSensitive(std::string original, std::string pathAndLine) {
        textOriginal = original;
        std::set<std::string> *newSetAdd = new std::set<std::string>;
        newSetAdd->insert(pathAndLine);
        infoSet = newSetAdd;
    }

    //destructor for the struct
    ~CaseSensitive() {
        delete infoSet;
        infoSet = nullptr;   
    }

};

class MapBuilder {

public:

    //constructor and deconstructor
    MapBuilder();
    ~MapBuilder();

    //functions for insertion and retrieval
    void insertNewWord(std::string original, std::string pathAndLine);
    std::list<CaseSensitive*> *findCaseInsensitive(std::string keyWord);
    CaseSensitive *findCaseSensitive(std::string original, 
                                            std::list<CaseSensitive*> *front);
    
private:

    //struct used to create a chained map to handle collision in hashing
    struct ChainNode {
        std::string key;
        std::list<CaseSensitive*> *value;
        ChainNode *next;

        //constructors for struct
        ChainNode() {    
            key = "";
            value = nullptr;
            next = nullptr;
        }

        //create a new Chain Node with specific member variables
        ChainNode(std::string word, 
                        std::list<CaseSensitive*> *versions, ChainNode *n) {
            key = word;
            value = versions;
            next = n;
        }
        
    };
    
    //Chaining map is an array of pointer to ChainNodes.
    ChainNode **chainedMap;

    static const int INITIAL_MAP_SIZE = 1;
    
    //member variables
    int numItemsInMap;
    int currentMapSize;
    

    //helper functions
    void removeList(ChainNode *node);
    void createChainNode(std::string keyWord, std::string original, 
                         std::string pathAndLine, int index);
    ChainNode *findChainNode(std::string keyWord, ChainNode *front);
    void insertAtOccupiedIndex(std::string keyWord, std::string original, 
                               std::string pathAndLine, int index);
    void expand();
    
};

#endif