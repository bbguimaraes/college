#include <iostream>
#include <string>
#include <exception>
#include <unistd.h>

#include <boost/lexical_cast.hpp>

#include "memory.h"

Memory * memory;

class ParseException : public std::exception {
    std::string msg;
    public:
        ParseException(const std::string & msg) : msg(msg) {}
        virtual ~ParseException() throw() {}
        const char * what() const throw() {return this->msg.c_str();}
};

unsigned int read_uint(const std::string & name) {
    std::string s;
    std::cin >> s;
    try {
        return boost::lexical_cast<unsigned int>(s);
    } catch(boost::bad_lexical_cast & ex) {
        throw ParseException(name + ": invalid value");
    }
}

char read_char() {
    char c;
    std::cin >> c;
    return c;
}

void log_error(const std::exception & ex) {
    std::cerr << ex.what() << std::endl;
}

void cmd_defmem(void) {
    unsigned int size = read_uint("size");
    if(memory)
        delete memory;
    memory = new Memory(size);
}

void cmd_load(void) {
    unsigned int pid = read_uint("pid");
    unsigned int size = read_uint("size");
    char fill = read_char();
    try {
        memory->create_process(pid, size, fill);
    } catch(std::exception& ex) {
        log_error(ex);
    }
}

void cmd_del(void) {
    unsigned int pid = read_uint("pid");
    try {
        memory->delete_process(pid);
    } catch(std::exception& ex) {
        log_error(ex);
    }
}

void cmd_reset(void) {
    delete memory;
    memory = 0;
}

void cmd_dump_proc(void) {
    unsigned int pid = read_uint("pid");
    try {
        std::cout << memory->dump_proc(pid);
    } catch(std::exception& ex) {
        log_error(ex);
    }
}

void cmd_dump_mem(void) {
    std::cout << memory->dump_mem();
}

void cmd_compact(void) {
    memory->compact();
}

void cmd_d(void) {
    if(memory)
        std::cout << memory->d();
}

bool execute(void) {
    std::string command;
    std::cin >> command;
    if(!std::cin || command == "exit")
        return false;
    else if(memory == 0 && command != "defmem")
        std::cout << "Call defmem first.";
    else if(command == "defmem")
        cmd_defmem();
    else if(command == "load")
        cmd_load();
    else if(command == "del")
        cmd_del();
    else if(command == "reset")
        cmd_reset();
    else if(command == "dump_proc")
        cmd_dump_proc();
    else if(command == "dump_mem")
        cmd_dump_mem();
    else if(command == "compact")
        cmd_compact();
    else if(command == "d")
        cmd_d();
    else
        std::cout << "\nUnknown command.";
    return true;
}

int main(int /*argc*/, char** /*argv*/) {
    bool tty = isatty(0);
    bool run = true;
    do {
        if(tty)
            std::cout << ">> ";
        try {
            run = execute();
        } catch(const ParseException & ex) {
            log_error(ex);
        }
    } while(run);
    delete memory;
}
