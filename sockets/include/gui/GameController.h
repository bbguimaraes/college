#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include <algorithm>
#include <list>

#include <QObject>

#include "model/Game.h"

class Character;
class Tile;
class TcpSocket;

class GameController : public QObject {
    Q_OBJECT

    public:
        enum class Message {MOVEMENT, VICTORY, QUIT};

        GameController(Game * game, TcpSocket * socket);

        Game::State state() const {return this->m_game->state();}
        Point highlighted() {return this->m_highlighted;}
        const Point selected() {return this->m_selected;}
        bool has_selected() {return this->m_selected != Point(-1, -1);}
        bool has_highlighted() {return this->m_highlighted != Point(-1, -1);}

        void set_socket(TcpSocket * socket) {this->m_socket = socket;}
        void set_selected(const Point & p);
        void set_highlighted(const Point & p) {this->m_highlighted = p;}
        void clear_selected();
        void clear_highlighted() {this->set_highlighted({-1, -1});}

        const Character * character_at(const Point & p) const;
        bool move_possible(const Point & p);
        void click(const Point & p);
        void check_for_other();
        void send_quit_message();
        void update();

    signals:
        void character_select(const Character * c);
        void character_moved(const Character * c, const Point & orig);
        void character_attacked(const Character * c, const Point & orig);

    private:
        void popup_on_gl_error();
        void handle_selection(const Point & p);
        void handle_move(const Point & p);
        void handle_attack(const Point & p);
        void send_move_message(const Point & orig, const Point & dest);
        void send_victory_message(const Point & orig, const Point & dest);

        Game * m_game;
        Point m_highlighted;
        Point m_selected;
        std::list<Point> m_possible_moves;
        TcpSocket * m_socket;
};

#endif // GAME_CONTROLLER_H
