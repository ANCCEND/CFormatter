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
    string outfilename;
    if (argc < 2)
    {
        cerr << "Usage: " << argv[0] << " <source_file.c>" << endl;
        return 1;
    }
    else if (argc > 4)
    {
        cerr << "Error: Too many arguments." << endl;
        cerr << "Usage: " << argv[0] << " <source_file.c>" << "debug" << endl;
        return 1;
    }
    else if (argc == 3)
    {
        string Flag = argv[2];
        if (Flag == "-debug")
        {
            debug = true;
            const char *infilename = argv[1];
            ifstream infile(infilename);
            if (!infile.is_open())
            {
                cerr << "Error: Could not open file " << infilename << endl;
                return 1;
            }
            try
            {
                Parser parser(infile, debug);
                ASTNode *root = parser.program();
                if (root)
                {
                    root->print();
                }
                cout << "Finished printing AST." << endl;
            }
            catch (const std::runtime_error &e)
            {
                cerr << e.what() << endl;
                return 1;
            }

            return 0;
        }
        else if (Flag == "-o")
        {
            cerr << "Error: Output file not specified." << endl;
            cerr << "Usage: " << argv[0] << " <source_file.c> -o <output_file.c>" << endl;
            return 1;
        }
        else
        {
            cerr << "Error: Unknown argument '" << Flag << "'." << endl;
            cerr << "Usage: " << argv[0] << " <source_file.c>" << "debug" << endl;
            return 1;
        }
    }
    else if (argc == 4)
    {
        string Flag = argv[2];
        if (Flag == "-debug")
        {
            debug = true;
            cerr << "Error: too much argcs" << endl;
        }
        else if (Flag == "-o")
        {
            debug = false;
            outfilename = argv[3];
            const char *infilename = argv[1];
            ifstream infile(infilename);
            ofstream outfile(outfilename);
            if (!infile.is_open())
            {
                cerr << "Error: Could not open file " << infilename << endl;
                return 1;
            }
            if (!outfile.is_open())
            {
                cerr << "Error: Could not open file " << outfilename << endl;
                return 1;
            }

            try
            {
                Parser parser(infile, debug);
                ASTNode *root = parser.program();
                if (root)
                {
                    root->print();
                    root->printToFile(outfile);
                }
                cout << "Finished printing AST." << endl;
            }
            catch (const std::runtime_error &e)
            {
                cerr << e.what() << endl;
                return 1;
            }

            return 0;
        }
        else
        {
            cerr << "Error: Unknown argument '" << Flag << "'." << endl;
            cerr << "Usage: " << argv[0] << " <source_file.c> -o <output_file.c>" << endl;
            return 1;
        }
    }
    return 0;
}