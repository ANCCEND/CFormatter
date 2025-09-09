#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <vector>
#include <algorithm>
#include "../lexer-parser/Lexer-Paser.hpp"

using namespace std;

int main(int argc, char *argv[]){
    if(argc < 2){
        cerr << "Usage: " << argv[0] << " <source_file.c>" << endl;
        return 1;
    }
    else if(argc > 2){
        cerr << "Error: Too many arguments." << endl;
        cerr << "Usage: " << argv[0] << " <source_file.c>" << endl;
        return 1;
    }
    const char* filename = argv[1];
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return 1;
    }
    try{
        Parser parser(file);
        ASTNode* root = parser.program();
        root->print();
        delete root;
    }catch(const std::runtime_error &e){
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}