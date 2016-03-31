#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player {
    public:
        Player() : m_id(0) {}
        Player(int id, const std::string & name) : m_id(id), m_name(name) {}

        int id() const {return m_id;}
        std::string name() const {return m_name;}

    private:
        int m_id;
        std::string m_name;
};

#endif // PLAYER_H
