#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <vector>
#include <algorithm>
#include "../lexer-parser/Lexer-Paser.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    bool debug = false;
    if (argc < 2)
    {
        cerr << "Usage: " << argv[0] << " <source_file.c>" << endl;
        return 1;
    }
    else if (argc > 3)
    {
        cerr << "Error: Too many arguments." << endl;
        cerr << "Usage: " << argv[0] << " <source_file.c>" << "debug" << endl;
        return 1;
    }
    else if (argc == 3)
    {
        string debugFlag = argv[2];
        if (debugFlag != "debug")
        {
            cerr << "Error: Unknown argument '" << debugFlag << "'." << endl;
            cerr << "Usage: " << argv[0] << " <source_file.c>" << "debug" << endl;
            return 1;
        }
        debug = true;
    }
    const char *filename = argv[1];
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Could not open file " << filename << endl;
        return 1;
    }
    try
    {
        Parser parser(file, debug);
        ASTNode *root = parser.program();
        if (root)
            root->print();
        delete root;
    }
    catch (const std::runtime_error &e)
    {
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}