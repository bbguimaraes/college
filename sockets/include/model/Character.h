#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>

#include "model/Point.h"

class Player;

class Character {
    public:
        enum class Animal {
            RAT, CAT, DOG, WOLF, PANTHER, TIGER, LION, ELEPHANT, CHARACTER,
        };
        static const Animal ANIMAL = Animal::CHARACTER;

        virtual ~Character() {}

        int value() const {return m_value;}
        std::string name() const {return m_name;}
        const Player * owner() const {return this->m_owner;}
        virtual Animal animal() const {return this->ANIMAL;}

        void set_position(const Point & position) {m_position = position;}
        Point position() const {return m_position;}

        virtual bool can_swim() const {return false;}
        virtual bool can_jump() const {return false;}

    protected:
        Character(
            int value, const std::string & name,
            const Player * owner, const Point & position) :
                m_value(value),
                m_name(name),
                m_owner(owner),
                m_position(position) {}

    private:
        int m_value;
        std::string m_name;
        const Player * m_owner;
        Point m_position;
};

class Rat : public Character {
    public:
        static const Animal ANIMAL = Animal::RAT;
        static const int VALUE = 1;

        Rat(const Player * owner, const Point & position)
            : Character(VALUE, "rat", owner, position) {}
        virtual Animal animal() const {return this->ANIMAL;}
        virtual bool can_swim() const {return true;}
};

class Cat : public Character {
    public:
        static const Animal ANIMAL = Animal::CAT;
        static const int VALUE = 2;

        Cat(const Player * owner, const Point & position)
            : Character(VALUE, "cat", owner, position) {}
        virtual Animal animal() const {return this->ANIMAL;}
};

class Dog : public Character {
    public:
        static const Animal ANIMAL = Animal::DOG;
        static const int VALUE = 3;

        Dog(const Player * owner, const Point & position)
            : Character(VALUE, "dog", owner, position) {}
        virtual Animal animal() const {return this->ANIMAL;}
};

class Wolf : public Character {
    public:
        static const Animal ANIMAL = Animal::WOLF;
        static const int VALUE = 4;

        Wolf(const Player * owner, const Point & position)
            : Character(VALUE, "wolf", owner, position) {}
        virtual Animal animal() const {return this->ANIMAL;}
};

class Panther : public Character {
    public:
        static const Animal ANIMAL = Animal::PANTHER;
        static const int VALUE = 5;

        Panther(const Player * owner, const Point & position)
            : Character(VALUE, "panther", owner, position) {}
        virtual Animal animal() const {return this->ANIMAL;}
};

class Tiger : public Character {
    public:
        static const Animal ANIMAL = Animal::TIGER;
        static const int VALUE = 6;

        Tiger(const Player * owner, const Point & position)
            : Character(VALUE, "tiger", owner, position) {}
        virtual Animal animal() const {return this->ANIMAL;}
        virtual bool can_jump() const {return true;}
};

class Lion : public Character {
    public:
        static const Animal ANIMAL = Animal::LION;
        static const int VALUE = 7;

        Lion(const Player * owner, const Point & position)
            : Character(VALUE, "lion", owner, position) {}
        virtual Animal animal() const {return this->ANIMAL;}
        virtual bool can_jump() const {return true;}
};

class Elephant : public Character {
    public:
        static const Animal ANIMAL = Animal::ELEPHANT;
        static const int VALUE = 8;

        Elephant(const Player * owner, const Point & position)
            : Character(VALUE, "elephant", owner, position) {}
        virtual Animal animal() const {return this->ANIMAL;}
};

#endif // CHARACTER_H
