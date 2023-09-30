/* MapBuilder.cpp
 * Doga Kilinc & Cansu Birsen
 * 12/04/2022
 *
 * CS 15 Proj 4: Gerp
 *
 * Purpose: MapBuilder is a class that creates a hash map array initialized 
 * empty with a size of 100. It can store a given a word and its path and line 
 * information. It stores the lower case version of the word as the key, and a 
 * list of pointers to CaseSensitive structs as the value. These key-value 
 * pairs are put in ChainNode structs. When a collision occurs, the program 
 * handles the situation by creating a linked list of ChainNode structs at that
 * particular index. When the load factor exceeds 0.7, the program expands the 
 * array of pointers to ChainNode structs by traversing the old array and 
 * rehashing the ChainNodes to a new map with increased capacity. The program
 * also has functions that make it possible to do case sensitive or insensitive
 * searches in the map. Case insensitive search returns a list of pointers to 
 * CaseSensitive structs holding different versions of the word, while the case
 * sensitive search returns a pointer to a particular CaseSensitive struct 
 * holding the information of a required version of a word.
 */
 
#include "MapBuilder.h"

#include <string>
#include <list>
#include <set>
#include <functional>
#include <iostream>
#include <cstring>

using namespace std;

/*
 * name:      default constructer of the MapBuilder class
 * purpose:   initializes the chainedMap to an array of with size 
 *            INITIAL_MAP_SIZE which holds pointers to ChainNodes
 * arguments: none
 * returns:   none
 * effects:   all the elements of the chainedMap are initialized to nullptr
 */
MapBuilder::MapBuilder() {

    currentMapSize = INITIAL_MAP_SIZE;
    numItemsInMap = 0;

    chainedMap = new ChainNode *[currentMapSize];

    for (int i = 0; i < currentMapSize; i++) {
        chainedMap[i] = nullptr;
    }
}

/*
 * name:      destructor of the MapBuilder class
 * purpose:   frees the heap-allocated memory for the chainedMap array
 *            and calls the helper function to free the rest of the 
 *            heap-allocated memory
 * arguments: none
 * returns:   none
 * effects:   utilizes the helper function removeList()
 */
MapBuilder::~MapBuilder() {
    for (int i = 0; i < currentMapSize; i++) {
        removeList(chainedMap[i]);
    }

    delete [] chainedMap;
    chainedMap = nullptr;
}


/*
 * name:      removeList
 * purpose:   frees the heap memory allocated to the value variable of a 
 *            given ChainNode, which is a list of pointers to heap-allocated
 *            CaseSensitive structs. 
 * arguments: pointer to the ChainNode to be visited
 * returns:   none
 * effects:   visits each linked node through recursion
 */
void MapBuilder::removeList(ChainNode *node) {

    //returns if the current node is nullptr
    if (node == nullptr) {
        return;
    } 
    else {
        ChainNode *next = node->next;
        list<CaseSensitive*> *valueL = node->value;
        //traverses the value list and deletes the CaseSensitive structs
        for (auto it = valueL->begin(); it != valueL->end(); ++it) {
            delete *it;
            *it = nullptr;
        }

        //deletes the value list itself
        delete valueL;
        valueL = nullptr;
        
        //deletes the ChainNode
        node->next = nullptr;
        node->value = nullptr;
        delete node;
        node = nullptr;

        //calls itself with the next ChainNode as an argument
        removeList(next);
    }
}

/*
 * name:      insertNewWord
 * purpose:   given a word and its info to be inserted, checks if the word
 *            already exists in the map, and depending on the case, places
 *            the lowercase version of the word as the key and its info
 *            to the appropriate place in the value list of the key 
 * arguments: the original version and the pathAndLine information of the word
 *            to be inserted as strings
 * returns:   none
 * effects:   if the load factor of the map is equal or greater than 0.7, calls
 *            expand function to increase the capacity of the map
 *            uses chaining method to handle the collisions
 */
void MapBuilder::insertNewWord(string original, string pathAndLine) {

    //calls expands function if the load factor is over 0.7
    if (numItemsInMap / currentMapSize >= 0.7)
        expand();

    //converts the original word to lowercase version
    string keyWord = "";
    for (unsigned int i = 0; i < original.size(); i++) {
        keyWord += tolower(original[i]);
    }

    //finds the hash index to place the word in the map
    size_t index = hash<string>{}(keyWord) % currentMapSize;

    //handles if there is no other key-value pair at that index
    if (chainedMap[index] == nullptr) 
        createChainNode(keyWord, original, pathAndLine, index);

    //if there is a key-value pair, calls the insertAtOccupiedIndex function
    //to place the information about the word appropriately to the chainedMap 
    else {
        insertAtOccupiedIndex(keyWord, original, pathAndLine, index);
    }

    numItemsInMap++;
}

/*
 * name:      expand
 * purpose:   creates a new map with an increased capacity and rehashes all the
 *            ChainNodes in the old map and places them in the new map
 * arguments: none
 * returns:   none
 * effects:   deletes the old chainedMap and then updates it to point to the 
 *            new map
 */
void MapBuilder::expand() {
    
    int oldMapSize = currentMapSize;
    //increases the current map size and creates a new map in the heap
    currentMapSize = currentMapSize * 2 + 2;
    ChainNode **newMap = new ChainNode *[currentMapSize];

    //initializes the new map
    for (int i = 0; i < currentMapSize; i++) {
        newMap[i] = nullptr;
    }

    //traverses the old map and rehashes the key values in the ChainNodes 
    for (int i = 0; i < oldMapSize; i++) {
        ChainNode *curr = chainedMap[i];
        //traverses the linked list of ChainNodes at an index
        while (curr != nullptr) {
            ChainNode *next = curr->next;
            //rehashes and places them appropriately to the new array
            size_t index = hash<string>{}(curr->key) % currentMapSize;
            curr->next = newMap[index];
            newMap[index] = curr;
            curr = next;
        }
        chainedMap[i] = nullptr;
    }
    //deletes the old chainedMap and then updates it to the newMap
    delete [] chainedMap;
    chainedMap = newMap; 
}

/*
 * name:      insertAtOccupiedIndex
 * purpose:   checks whether a ChainNode for the given keyWord already exists
 *            when a particular index of the map is occupied. if a ChainNode 
 *            does not exist, adds a ChainNode for the word to the map. if it 
 *            exists, checks if a CaseSensitive for the original version of the
 *            word exists. depending on the case, either creates a new 
 *            CaseSensitive or adds the pathAndLine info the the infoSet of the
 *            already existing CaseSensitive
 * arguments: the lowercase version and the original version of the word as
 *            strings, pathAndLine info as a string and ChainedMap index to 
 *            insert the word at
 * returns:   none
 * effects:   is called when a particular hash index is already occupied
 */
void MapBuilder::insertAtOccupiedIndex(string keyWord, string original, 
                                       string pathAndLine, int index) {

    //checks if a ChainNode with the given word exists
    if (findChainNode(keyWord, chainedMap[index]) == nullptr) 
        createChainNode(keyWord, original, pathAndLine, index);

    //if such a ChainNode exists, checks if there is CaseSensitive structs
    //with the original version of the word exists
    else {
        ChainNode *n = findChainNode(keyWord, chainedMap[index]);
        if (findCaseSensitive(original, n->value) == nullptr) {

            //creates a new struct holding the original text & path/line
            //and pushes the pointer to this struct in a new list
            CaseSensitive *newVersion = new CaseSensitive(original, 
                                                            pathAndLine);
            n->value->push_back(newVersion);
        }
        
        //if the version exists, adds the new path&line info to the set
        else {
            CaseSensitive *version = findCaseSensitive(original, n->value);
            version->infoSet->insert(pathAndLine);
        }
    }
}

/*
 * name:      createChainNode
 * purpose:   creates a new ChainNode with keyWord as its key and a list of
 *            CaseSensitive struct pointers as its value and inserts this
 *            node to the front of the linked list at the given ChainedMap
 *            index
 * arguments: the lowercase version and the original version of the word as
 *            strings, pathAndLine info as a string and ChainedMap index to 
 *            insert the word at
 * returns:   none
 * effects:   is called when a given keyWord does not exists in the 
 *            ChainedMap
 */
void MapBuilder::createChainNode(string keyWord, string original,  
                                string pathAndLine, int index) {
    
    //creates a new CaseSensitive struct that holds the original text & path 
    //and line info; pushes the pointer to this struct in a new list
    CaseSensitive *newVersion = new CaseSensitive(original, pathAndLine);
    list<CaseSensitive*> *valueL = new list<CaseSensitive*>;
    valueL->push_back(newVersion);

    //creates a new ChainNode in the map with the lower case word (key),
    //a pointer to the list of CaseSensitive struct pointers (value), and 
    //a pointer to the current front ChainNode at the given index of the map
    ChainNode *node = new ChainNode(keyWord, valueL, chainedMap[index]);

    //places the new node at the given index as the front node
    chainedMap[index] = node;
}

/*
 * name:      findChainNode
 * purpose:   finds the ChainNode in a linked list that has the given keyWord
 *            as its key
 * arguments: a string key to look for in the ChainedMap and a pointer
 *            to the front of the linked list of ChainNodes at particular
 *            index of the ChainedMap
 * returns:   a pointer to the ChainNode which has the string keyWord as its 
 *            key
 *            returns a nullptr if a ChainNode with the given keyWord does not
 *            exist
 * effects:   recursively traverses the linked list of ChainNodes to find the
 *            wanted ChainNode. 
 *            helps to see whether a ChainNode with the given keyWord was 
 *            already present in the linked list 
 */
MapBuilder::ChainNode *MapBuilder::findChainNode(string keyWord, 
                                                 ChainNode *front) {

    if (front == nullptr)
        return nullptr;
    //returns the pointer if the current node has the given key
    else if (front->key == keyWord) 
        return front;
    //calls itself with the same keyWord and next ChainNode as arguments
    else 
        return findChainNode(keyWord, front->next);
}

/*
 * name:      findCaseSensitive
 * purpose:   finds the CaseSensitive struct that has the given original word
 *            as its textOriginal by traversing a list of pointers to 
 *            CaseSensitive structs
 * arguments: original version of a word to be searched as a string, a pointer
 *            to a list of CaseSensitive struct pointers
 * returns:   a pointer to the CaseSensitive struct that has the original
 *            word as its textOriginal member variable
 *            returns a nullptr if a CaseSensitive struct with the given 
 *            original word does not exist
 * effects:   helps to see whether a CaseSensitive struct for the original word
 *            was already created
 */
CaseSensitive *MapBuilder::findCaseSensitive(string original, 
                                            list<CaseSensitive*> *valueL) {
    
    //if there is no value list for the word, none of the versions of the word
    //exists
    if (valueL == nullptr)
        return nullptr; 

    //traverses the list of pointers to CaseSensitive structs
    for (auto it = valueL->begin(); it != valueL->end(); ++it) {
        //returns a pointer to the struct when finds the original word
        if ((*it)->textOriginal == original)
            return *it;
    }

    //returns nullptr otherwise
    return nullptr;
}

/*
 * name:      findCaseInsensitive
 * purpose:   finds the ChainNode in a linked list that has the given keyWord
 *            as its key and accesses to the corresponding value list
 * arguments: a string key to look for in the ChainedMap
 * returns:   a pointer to the list of CaseSensitive struct pointers 
 *            corresponding to the string keyWord 
 *            returns a nullptr if a ChainNode with the given keyWord does not
 *            exist in the map or if the word is an empty string
 * effects:   finds the hash index for the given key word to look for 
 *            the wanted ChainNode and its value list
 *            helps to see whether a ChainNode with the given keyWord is 
 *            present in the map 
 */
list<CaseSensitive*> *MapBuilder::findCaseInsensitive(string keyWord) {
    
    //if the key is an empty string, returns nullptr
    if (keyWord.size() == 0)
        return nullptr;

    //finds the hash index to look for the word in the map
    size_t index = hash<string>{}(keyWord) % currentMapSize;
    ChainNode *n = findChainNode(keyWord, chainedMap[index]);
    
    if (n != nullptr)
        //accesses to the value of the ChainNode with the given key
        return n->value;

    //if such a ChainNode with the keyword does not exist, returns nullptr
    return nullptr;
}

