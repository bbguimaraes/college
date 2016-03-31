#include <exception>
#include <fstream>
#include <iostream>

#include "Lexer.h"
#include "Parser.h"

void test_lexer(const std::string & code);
std::string read_code(const std::string & filename);

int main(int argc, char ** argv) {
    std::string file;
    bool lexer = false;
    for(int i = 1; i < argc; ++i) {
        if(argv[i] == std::string("--lexer"))
            lexer = true;
        else
            file = argv[i];
    }
    const std::string CODE = read_code(file);
    try{
        if(lexer)
            test_lexer(CODE);
        else
            Parser().parse(CODE);
    } catch(std::exception & ex) {
        std::cerr << ex.what() << std::endl;
        std::exit(1);
    }
    return 0;
}

void test_lexer(const std::string & code) {
    try {
        Lexer lexer;
        lexer.initialize(code);
        std::cout << lexer.current() << std::endl;
        while(!lexer.eof())
            std::cout << lexer.next() << std::endl;
    } catch(std::exception & ex) {
        std::cerr << ex.what() << std::endl;
        std::exit(1);
    }
}

std::string read_code(const std::string & filename) {
    try {
        if(filename.empty())
            return std::string(
                std::istreambuf_iterator<char>(std::cin),
                std::istreambuf_iterator<char>());
        std::ifstream reader(filename.c_str());
        reader.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        return std::string(
            std::istreambuf_iterator<char>(reader),
            std::istreambuf_iterator<char>());
    } catch(std::exception & ex) {
        std::perror(std::string("Could not read file " + filename).c_str());
        std::exit(1);
    }
}
