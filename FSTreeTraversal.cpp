/* FSTreeTraversal.cpp
 * Doga Kilinc & Cansu Birsen
 * 12/02/2022
 *
 * CS 15 Proj 4: Gerp
 *
 * Purpose: This file contains the functions that print out the paths of all
 * the accesible files in a given directory by building an FSTree and
 * accessing the file information using the DirNode class. This file also
 * contains the driver of the program which has the usage 
 * ./treeTraversal Directory and exits with failure when called without
 * appropriate command line arguments.
 */

#include "FSTree.h"
#include "DirNode.h"

#include <iostream>
#include <string>

using namespace std;

//function declarations
void pathPrinter(string directoryName);
void pathPrinterHelper(DirNode *root, string pathName);


int main(int argc, char *argv[]) {

    //checks the command line argument number
    if (argc != 2) {
        cerr << "Usage: ./treeTraversal Directory" << endl;
        return EXIT_FAILURE;
    }

    //calls the appropriate function to print the file paths
    pathPrinter(argv[1]);

    return 0;

}

/*
 * name:      pathPrinter
 * purpose:   to print all the path names of the accessible files in the
 *            given directory
 * arguments: the directory name as a string given by the client
 * returns:   none
 * effects:   calls for pathPrinterHelper to traverse the FSTree and access
 *            all the files in the sub directories
 */
void pathPrinter(string directoryName) {

    //builds the FSTree and stores the root
    FSTree tree(directoryName);
    DirNode *root = tree.getRoot();

    //calls the helper function to traverse the FSTree, and prints the path
    //names of all the accessible files from the given root
    pathPrinterHelper(root, directoryName);

}

/*
 * name:      pathPrinterHelper
 * purpose:   prints the file paths in the current directory to the cout,
 *            then visits sub directories and calls itself again
 * arguments: root of the FSTree to retrieve the path names from and a string
 *            that keeps track of the path names so far
 * returns:   none
 * effects:   uses recursion to print the file paths from a given root in the
 *            FSTree to the cout
 */
void pathPrinterHelper(DirNode *root, string pathName) {

    //prints the files in the directory
    if (root->numFiles() != 0) {
        for (int i = 0; i < root->numFiles(); i++) {
            cout << pathName << "/" << root->getFile(i) << endl;
        }
    }

    //gets into the sub directories and calls for the function itself with
    //an updated path name for the sub directories
    if (root->numSubDirs() != 0) {
        for (int i = 0; i < root->numSubDirs(); i++) {
            string newPath = pathName + "/" + root->getSubDir(i)->getName(); 
            pathPrinterHelper(root->getSubDir(i), newPath);
        }
    }

}

