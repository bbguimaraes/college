#ifndef PROCESS_H
#define PROCESS_H

class Process {
    unsigned int m_pid;
    unsigned int m_size;
    unsigned int m_position;
    char m_fill;

    public:
        Process(unsigned int pid, unsigned int size, char fill)
            : m_pid(pid), m_size(size), m_fill(fill) {}
        ~Process() {}

        unsigned int pid() const {return m_pid;}
        unsigned int size() const {return m_size;}
        unsigned int position() const{ return m_position;}
        char fill() const {return m_fill;}

        void set_position(unsigned int position) {this->m_position = position;}
};

#endif
