#ifndef GAME_H
#define GAME_H

#include <list>
#include <vector>

#include "model/Point.h"

class Character;
class Player;

class Game {
    public:
        enum class State {
            WAITING_PLAYER,
            WAITING_OTHER,
            VICTORY,
            DEFEAT,
            EXITING
        };

        Game(
            const Player & player,
            const Player & other_player,
            State initial_state);
        ~Game();

        void set_state(State state) {m_state = state;}
        Character * character_at(const Point & p) const
            {return this->m_characters.at(index(p));}
        int index(const Point & p) const {return p.y() * 7 + p.x();}
        const Player * current_player() const {return this->m_current_player;}
        State state() const {return m_state;}

        bool water(const Point & p) const;
        bool trap(const Point & p) const;
        Point can_jump(const Character * c, int dx, int dy) const;
        std::list<Point> possible_moves(const Character * c) const;
        std::list<Point> jumps(
            const Character * c, const std::list<Point> & moves) const;
        bool check_victory() const;

        void move_character(Character * character, const Point & dest);
        void remove_character(Character * character);

        void switch_player() {
            this->m_current_player =
                this->m_current_player == &this->m_player
                    ? &this->m_other_player : &this->m_player;
        }

    private:
        std::vector<Character *> create_characters() const;

        const Player & m_player;
        const Player & m_other_player;
        State m_state;
        std::vector<Character *> m_characters;
        const Player * m_current_player;
};

#endif // GAME_H
