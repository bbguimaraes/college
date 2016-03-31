#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include "Controller.h"
#include "Operation.h"
#include "TransactionBuilder.h"
#include "Transaction.h"

std::string read_input(const std::string & filename);
void print_tables(const LockTable * lock_table);
Transaction * get_or_create_transaction(
    std::vector<Transaction *> * v, const std::string & name);

int main(int argc, char ** argv) {
    if(argc < 2)
        return system((
            std::string()
            + "set -euo pipefail;"
            + argv[0]
            + " t | diff -u expected.txt -")
                .c_str());
    std::vector<Transaction *> transactions;
    if(!(std::ifstream(argv[1]) >> transactions)) {
        std::cerr << "Invalid input." << std::endl;
        return 1;
    }
    auto controller = Controller(
        TransactionBuilder::shuffled_operation_list(transactions));
    const LockTable * lock_table = controller.locks();
    print_tables(lock_table);
    while(!controller.done()) {
        std::cout
            << "\n"
            << controller.current()->transaction()->name()
            << "."
            << controller.current()->message()
            << "\n\n";
        controller.step();
        print_tables(lock_table);
    }
    print_tables(lock_table);
    return 0;
}

void print_tables(const LockTable * lock_table) {
    auto locked = lock_table->locked();
    auto locks = lock_table->locks();
    auto sleeping = lock_table->sleeping();
    std::cout << "=== locked   ===";
    for(auto x : *locked)
        std::cout << "\n" << x.first << ": " << x.second;
    std::cout << "\n=== locks    ===";
    for(auto x : *locks) {
        std::cout << "\n" << x.first << ":";
        for(auto y : x.second)
            std::cout << " " << y->name();
    }
    std::cout << "\n=== sleeping ===";
    for(auto x : *sleeping) {
        std::cout << "\n" << x.first << ":";
        for(auto y : x.second)
            std::cout << " " << y->name();
    }
    std::cout << std::endl;
}

std::string read_input(const std::string & filename) {
    std::ifstream reader(filename);
    std::stringstream input;
    std::string temp;
    while(getline(reader, temp))
        input << temp << "\n";
    reader.close();
    return input.str();
}
