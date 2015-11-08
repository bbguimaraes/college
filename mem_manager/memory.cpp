#include "memory.h"

#include <algorithm>
#include <sstream>

const unsigned int Memory::BLOCK_SIZE = 32;

Memory::Memory(unsigned int size) {
    if(size <= 0)
        throw MemoryException("Memory size must be grater than 0.");
    this->size = size;
    this->memory = new char[size + 1];
    memory[size] = 0;
    for(unsigned int i = 0; i < size; i++)
        memory[i] = EMPTY_CHAR;
}

void Memory::create_process(unsigned int pid, unsigned int size, char fill) {
    auto at = get_free_position(size);
    auto p = Process(pid, size, fill);
    p.set_position(get_position(at));
    this->processes.insert(at, p);
    for(unsigned int i = 0; i < size; i++)
        memory[p.position() + i] = fill;
}

void Memory::delete_process(unsigned int pid) {
    auto it = this->find_process_by_pid(pid);
    if(it == this->processes.cend())
        throw MemoryException("Process doesn't exist.");
    for(unsigned int i = 0; i < it->size(); i++)
        memory[it->position() + i] = '$';
    this->processes.erase(it);
}

void Memory::compact(void) {
    auto it = this->processes.begin();
    unsigned int last = 0;
    for(; it != this->processes.cend(); ++it) {
        if(it->position() > last) {
            for(unsigned int i = 0; i < it->size(); ++i)
                memory[it->position() + i] = '$';
            it->set_position(last);
            for(unsigned int i = 0; i < it->size(); ++i)
                memory[it->position() + i] = it->fill();
        }
        last = it->position() + it->size();
    }
}

unsigned int
Memory::get_position(std::list<Process>::const_iterator at) const {
    if(at == this->processes.cbegin())
        return 0;
    if(at == this->processes.cend())
        return this->processes.back().position()
            + this->processes.back().size();
    auto prev = --at;
    return prev->position() + prev->size();
}

std::string Memory::dump_proc(unsigned int pid) const {
    auto it = this->find_process_by_pid(pid);
    if(it == this->processes.cend())
        throw MemoryException("Process doesn't exist.");
    return dump_proc(*it);
}

std::string Memory::dump_proc(const Process& p) const {
    std::stringstream output;
    output << "Process " << p.pid() << ":";
    output << "\n  start: " << p.position();
    output << "\n  size: " << p.size() ;
    output << "\n  last: " << p.position() + p.size() - 1;
    output << "\n  fill: " << memory[p.position()];
    output << "\n";
    return output.str();
}

std::string Memory::dump_mem(void) const {
    std::stringstream output;
    unsigned int last = 0;
    auto it = this->processes.cbegin();
    while(it != this->processes.cend()) {
        if(it->position() != last)
            output << "Empty space: " << (it->position() - last) << "\n";
        output << dump_proc(*it);
        last = it->position() + it->size();
        ++it;
    }
    if(last < size)
        output << "Empty space: " << (size - last) << "\n";
    return output.str();
}

std::string Memory::d() const {
    std::stringstream output;
    const unsigned int n_digits = calc_digits(size);
    for(unsigned int i = 0; i < this->size; i += BLOCK_SIZE) {
        output << this->format_index(i, n_digits);
        const unsigned int n_bits = std::min(BLOCK_SIZE, this->size - i);
        for(unsigned int b = 0; b < n_bits; ++b) {
            if((0 < b) && (b % 8 == 0))
                output << " ";
            output << memory[i + b];
        }
        output << "\n";
    }
    return output.str();
}

int Memory::calc_digits(int n) const {
    int n_digits = 0;
    while(n > 0) {
        n_digits++;
        n /= 10;
    }
    return n_digits;
}

std::string Memory::format_index(unsigned int i, unsigned int n_digits) const {
    std::stringstream str_stream;
    str_stream << i << ":";
    std::string str;
    str_stream >> str;
    str.resize(n_digits + 2, ' ');
    return str;
}

std::list<Process>::const_iterator
Memory::find_process_by_pid(unsigned int pid) const {
    return std::find_if(
        this->processes.cbegin(), this->processes.cend(),
        [&pid](const Process & p){return p.pid() == pid;});
}

std::list<Process>::const_iterator
Memory::get_free_position(unsigned int size) const {
    auto it = this->processes.cbegin();
    auto next = ++this->processes.cbegin();
    auto best = this->processes.cend();
    unsigned int best_size = this->size + 1;
    unsigned int last = 0;
    while(it != this->processes.cend()) {
        unsigned int gap = it->position() - last;
        if(size <= gap && gap < best_size) {
            best_size = gap;
            best = it;
        }
        last = it->position() + it->size();
        it = next;
        ++next;
    }
    unsigned int gap = this->size - last;
    if(size <= gap && gap < best_size) {
        best_size = gap;
        best = it;
    }
    if(best_size > this->size)
        throw MemoryException("Not enought space in memory.");
    return best;
}
