#ifndef MEMORY_H
#define MEMORY_H

#include <exception>
#include <list>
#include <string>

#include "process.h"

class Memory {
    public:
        Memory(unsigned int size);
        ~Memory() { delete[] memory; }

        /** Create a process. **/
        void create_process(unsigned int pid, unsigned int size, char fill);

        /** Delete process. **/
        void delete_process(unsigned int pid);

        /** Reallocate all processes to eliminate gaps. **/
        void compact();

        /** Return string showing information about one process. **/
        std::string dump_proc(unsigned int pid) const;

        /** Return string showing information about one process. **/
        std::string dump_proc(const Process& p) const;

        /** Return string showing free and occupied spaces. **/
        std::string dump_mem() const;

        /** Return string representation of the constents of memory. **/
        std::string d() const;

    private:
        const static char EMPTY_CHAR = '$';
        const static unsigned int BLOCK_SIZE;

        int calc_digits(int n) const;
        unsigned int get_position(std::list<Process>::const_iterator at) const;
        std::list<Process>::const_iterator
            find_process_by_pid(unsigned int pid) const;

        /**
         * Find a position in memory big enough to store [size] bytes, using
         * the Best-Fit algorithm.
         */
        std::list<Process>::const_iterator
            get_free_position(unsigned int size) const;

        std::string format_index(unsigned int i, unsigned int n_digits) const;

        /** Memory size. **/
        unsigned int size;

        /** Memory content. **/
        char* memory;

        /** Array of processes. **/
        std::list<Process> processes;
};

class MemoryException : public std::exception {
    std::string msg;
    public:
        MemoryException(const std::string & msg) : msg(msg) {}
        virtual ~MemoryException() throw() {}
        virtual const char * what() const throw() {return this->msg.c_str();}
};

#endif
