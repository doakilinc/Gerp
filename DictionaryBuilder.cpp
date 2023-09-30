/* DictionaryBuilder.cpp
 * Doga Kilinc & Cansu Birsen
 * 12/09/2022
 *
 * CS 15 Proj 4: Gerp
 *
 * Purpose: DictionaryBuilder is a class that reads all accessible files in a 
 * given directory by using the FSTree and DirNode classes and retriving the 
 * file paths for each. The class then records each line associated with a 
 * file using a vector of pathsAndFiles structs. Each struct holds a pointer to
 * a file path and a pointer to a vector whose each element represents a line.
 * Then, the program traverses each line and inserts each word in the line and
 * its path and line information in a map using the MapBuilder class.
 * A user can use this program with specific commands. If the user 
 * enters the command "@i" or "@insensitive" followed by a word, the program
 * implements a case insensitive search in the files, and prints the 
 * information (the file location, line number, and line itself) about the word
 * to the provided output file. The user can also implement a case sensitive 
 * search that provides a similar output by just providing the word itself as a
 * command. When doing a word search, the program eliminates the leading and 
 * trailing non alphanumeric characters in the word the user enters. The 
 * program also avoids printing duplicate information if the searched word is
 * found more than once in the same line. Lastly, the user can change the 
 * output file with command "@f" folllowed by the new output file name and use
 * the command "@q" or "@quit" to end the program.  
 */

#include "DictionaryBuilder.h"
#include "MapBuilder.h"
#include "FSTree.h"
#include "DirNode.h"

#include <string>
#include <list>
#include <set>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

/*
 * name:      default constructer of the DictionaryBuilder class
 * purpose:   initializes directory and outputFile variables
 * arguments: none
 * returns:   none
 * effects:   stores the directory name and the output file name that the user
 *            provides, opens an output file stream with the output file name
 */
DictionaryBuilder::DictionaryBuilder(string directoryName, string outputName) {

    directory = directoryName;
    outputFile = outputName;

    openOrDie(output, &outputFile);

}

/*
 * name:      destructor of the DictionaryBuilder class
 * purpose:   frees the heap-allocated memory for each member variable of the
 *            pathsAndFiles struct
 * arguments: none
 * returns:   none
 * effects:   traverses the files vector to access each pathsAndFiles struct
 */
DictionaryBuilder::~DictionaryBuilder() {

    for (unsigned int i = 0; i < files.size(); i++) {
        string *p = files[i].path;
        vector<string>  *l = files[i].fileLines;
        
        delete p;
        p = nullptr;

        delete l;
        l = nullptr;
    }
}

/*
 * name:      pathFinder
 * purpose:   to access all the path names of the accessible files in the
 *            given directory and store them in the files vector using 
 *            pathsAndFiles structs
 * arguments: none
 * returns:   none
 * effects:   calls for pathFinderHelper to traverse the FSTree and access
 *            all the files in the sub directories
 */
void DictionaryBuilder::pathFinder() {

    //builds the FSTree and stores the root
    FSTree tree(directory);
    DirNode *root = tree.getRoot();

    //calls the helper function to traverse the FSTree, and stores the path
    //names of all the accessible files from the given root
    pathFinderHelper(root, directory);

}

/*
 * name:      pathFinderHelper
 * purpose:   stores the file paths in the current directory in the files 
 *            vector using pathsAndFiles structs then visits sub directories 
 *            and calls itself again
 * arguments: root of the FSTree to retrieve the path names from and a string
 *            that keeps track of the path names so far
 * returns:   none
 * effects:   uses recursion to access the file paths from a given root in the
 *            FSTree 
 */
void DictionaryBuilder::pathFinderHelper(DirNode *root, string pathName) {

    //store the paths of the files in the directory in pathsAndFiles structs
    if (root->numFiles() != 0) {
        for (int i = 0; i < root->numFiles(); i++) {
            string finalPath = pathName + "/" + root->getFile(i);
            files.push_back(pathsAndFiles(finalPath));
        }
    }

    //gets into the sub directories and calls for the function itself with
    //an updated path name for the sub directories
    if (root->numSubDirs() != 0) {
        for (int i = 0; i < root->numSubDirs(); i++) {
            string newPath = pathName + "/" + root->getSubDir(i)->getName(); 
            pathFinderHelper(root->getSubDir(i), newPath);
        }
    }

}

/*
 * name:      fileReader
 * purpose:   opens a file stream for each file stored in the files vector,
 *            stores the lines in the fileLines vector associated with the
 *            appropriate pathsAndFiles struct. Then traverses each word in 
 *            each line and inserts the word to the map with its path and
 *            line info
 * arguments: none
 * returns:   none
 * effects:   calls openOrDie to check if the file is opened correctly. If not,
 *            gives a runtime error and quits
 */
void DictionaryBuilder::fileReader() {
    //opens each file in the directory using the paths from the files vector
    for (unsigned int i = 0; i < files.size(); i++) {
        string *filePath = files[i].path;
        ifstream input;
        openOrDie(input, filePath);

        //gets each line from the opened file
        string line;
        while (getline(input, line)) {
            
            //stores the line in the fileLines vector of the pathsAndFiles 
            //struct
            files[i].fileLines->push_back(line);
            istringstream stream(line);
            
            //gets each word in the current line
            string word;
            while (stream >> word) {
                word = stripNonAlphaNum(word);
                string pathNum = to_string(i + 1);
                string lineNum = to_string(files[i].fileLines->size());
                string address = pathNum + "/" + lineNum;
                //inserts the word into the map with its path and line numbers
                map.insertNewWord(word, address);
            }
        }
        input.close();
    }
}


 /*
 * name:       openOrDie
 * purpose:    opens the given file using the given stream
 * arguments:  streamtype and the filename to be opened
 * returns:    none
 * effects:    throws a runtime error with a message if the file is not
 *             opened correctly
 */
template<typename streamtype>
void DictionaryBuilder::openOrDie(streamtype &stream, string *filePath) {

    //opens the file, if not opened correctly throws a runtime error
    stream.open(*filePath);

    string errorMessage = "Unable to open file " + *filePath;

    if (not stream.is_open()) {
        throw runtime_error(errorMessage);
    }
    
}

/*
 * name:      caseInsensitiveSearch
 * purpose:   calls findCaseInsensitive to access the list of CaseSensitive
 *            structs storing the versions of the word, and calls searchPrinter
 *            to print the results of the query to the output stream
 * arguments: the original version of the word to be searched as a string
 * returns:   none
 * effects:   if there is no version of the word that is searched, prints
 *            a "not found" message to the output stream
 */
void DictionaryBuilder::caseInsensitiveSearch(string original) {
    
    //removes the nonalphanumerics, converts the original to lowercase version
    original = stripNonAlphaNum(original);
    string keyWord = "";
    for (unsigned int i = 0; i < original.size(); i++) {
        keyWord += tolower(original[i]);
    }

    //calls findCaseInsensitive to access the list of CaseSensitive struct 
    //pointers of the given keyword
    list<CaseSensitive*> *valueL = map.findCaseInsensitive(keyWord);
    
    //if keyword is not found, prints the message to output
    if (valueL == nullptr) {
        output << original << " Not Found." << endl;
        return;
    }
   
   //creates a new set and inserts the path and line info to avoid duplicates
    set<string> address;
    for (auto it1 = valueL->begin(); it1 != valueL->end(); ++it1) {
        set<std::string> *setAdd = (*it1)->infoSet;
        for (auto it2 = setAdd->begin(); it2 != setAdd->end(); ++it2) {
            address.insert(*it2);
        }
    }
    //calls searchPrinter to print the query result to output
    searchPrinter(&address, output);
}

/*
 * name:      searchPrinter
 * purpose:   traverses the strings in the address set and obtains the line 
 *            and path indices. Using these, prints file path, line number
 *            and the line itself to the output stream for a particular word
 * arguments: a pointer to a set of string that holds path and line 
 *            information of a particular word, a reference to the output 
 *            stream to print the results
 * returns:   none
 * effects:   separates the strings in the set using "/" as a separator
 *            prints the query result to the output stream
 */
void DictionaryBuilder::searchPrinter(set<string> *address, 
                                      ofstream &outfile) {
    //obtains the true path and line indices by reading and separating 
    //path and line info from a set of strings
    for (auto it = address->begin(); it != address->end(); ++it) {
        string pathNumber = "";
        string lineNumber= "";

        //separates the string at the "/" character
        int i = 0;
        char a = (*it)[i];
        //the substring before "/" represents the path number
        while (a != '/') {
            pathNumber += a;
            i++;
            a = (*it)[i];
        }

        //the substring after "/" represents the line number
        lineNumber = it->substr(i + 1, it->size() - i - 1);

        //finds the true path and line indices from the numbers
        int pathIndex = stoi(pathNumber) - 1;
        int lineIndex = stoi(lineNumber) - 1;

        //prints the path info, line number, and line itself to output
        outfile << *(files[pathIndex].path) << ":" << (lineIndex + 1) << ": ";
        outfile << files[pathIndex].fileLines->at(lineIndex) << endl;
    }
}

/*
 * name:      caseSensitiveSearch
 * purpose:   calls findCaseInsensitive to access the list of CaseSensitive
 *            structs storing the versions of the word, and calls 
 *            findCaseSensitive to find the specific CaseSensitive structs from
 *            that list and calls searchPrinter to print the results of the 
 *            query to the output stream
 * arguments: the original version of the word to be searched as a string
 * returns:   none
 * effects:   if there is no version of the word that is searched, prints
 *            a "not found" message to the output stream
 */
void DictionaryBuilder::caseSensitiveSearch(string original) {

    original = stripNonAlphaNum(original);
    string keyWord = "";
    for (unsigned int i = 0; i < original.size(); i++) {
        keyWord += tolower(original[i]);
    }

    //calls findCaseInsensitive to access the list of CaseSensitive struct 
    //pointers of the given keyword, and calls findCaseSensitive to access to
    //the CaseSensitive with the specific version of the word
    list<CaseSensitive*> *valueL = map.findCaseInsensitive(keyWord);
    CaseSensitive *version = map.findCaseSensitive(original, valueL);

    //if keyword is not found, prints the message to output
    if (version == nullptr) {
        output << original << " Not Found. Try with @insensitive or @i.";
        output << endl;
        return;
    }

    //calls searchPrinter to print the query result to output
    searchPrinter(version->infoSet, output);
}

 /*
 * name:      stripNonAlphaNum
 * purpose:   modifies the string so that there are no leading or trailing
 *            non-alpha numeric characters in the given string
 * arguments: given user input to be manipulated
 * returns:   the modified version of the string
 * effects:   prepares the given string for a later word search
 */
string DictionaryBuilder::stripNonAlphaNum(string input) {
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

/*
 * name:      commandHandler
 * purpose:   asks for and takes input until user quits or end of file is 
 *            reached. Depending on the command, implements a word search 
 *            (case sensitive or insesitive), closes the current output file
 *            and opens new one, or quits
 * arguments: none
 * returns:   none
 * effects:   prints a message asking for a command and prints a goodbye 
 *            message when the user quits or the end of file is reached
 */
void DictionaryBuilder::commandHandler() {
    string c1;
    cout  << "Query? ";
    //asks for and takes input until user quits or end of file is reached
    while (cin >> c1) {
        c1 = stripNonAlphaNum(c1);
        //depending on the command, calls the appropriate functions
        if (c1 == "i" or c1 == "insensitive") {
            string c2;
            cin >> c2;
            caseInsensitiveSearch(c2);
        }
        else if (c1 == "q" or c1 == "quit") {
            cout << "Goodbye! Thank you and have a nice day." << endl;
            return;
        }
        else if (c1 == "f") {
            string c2;
            cin >> c2;
            output.close();
            openOrDie(output, &c2);
        }
        else {
            caseSensitiveSearch(c1);
        }
        cout  << "Query? ";
    }
    output.close();
    cout << "Goodbye! Thank you and have a nice day." << endl;
}

/*
 * name:      query
 * purpose:   builds the dictionary of words by calling pathFinder and 
 *            fileReader, and asks for user input and handles the commands
 *            by calling the commandHandler function
 * arguments: none
 * returns:   none
 * effects:   utilizes private functions pathFinder, fileReader, and 
 *            commandHandler.
 */
void DictionaryBuilder::query() {

    pathFinder();
    fileReader();
    commandHandler();

}