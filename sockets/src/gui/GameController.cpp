#include "gui/GameController.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include <QMessageBox>

#include "model/Character.h"
#include "model/Game.h"
#include "model/Player.h"
#include "net/TcpSocket.h"

GameController::GameController(Game * game, TcpSocket * socket)
        : m_game(game), m_socket(socket) {
    this->clear_highlighted();
    this->clear_selected();
}

void GameController::set_selected(const Point & p) {
    this->m_selected = p;
    this->m_possible_moves =
        this->m_game->possible_moves(this->m_game->character_at(p));
}

void GameController::clear_selected() {
    this->m_selected = Point(-1, -1);
    this->m_possible_moves.clear();
}

const Character * GameController::character_at(const Point & p) const {
    return this->m_game->character_at(p);
}

bool GameController::move_possible(const Point & p) {
    const auto & m = this->m_possible_moves;
    return std::find(std::begin(m), std::end(m), p) != std::end(m);
}

void GameController::click(const Point & p) {
    if(this->m_game->state() != Game::State::WAITING_PLAYER
            && this->m_game->state() != Game::State::WAITING_OTHER)
        return;
    if((!this->has_selected() || !this->move_possible(p)))
        this->handle_selection(p);
    else if(!this->m_game->character_at(p))
        this->handle_move(p);
    else
        this->handle_attack(p);
}

void GameController::check_for_other() {
    Message msg;
    if(!this->m_socket->recv(&msg, sizeof(Message)))
        return;
    this->m_socket->set_blocking(true);
    switch(msg) {
        case Message::QUIT:
            this->m_game->set_state(Game::State::EXITING);
            break;
        case Message::MOVEMENT: {
            int buffer[4];
            this->m_socket->recv(buffer, 4 * sizeof(int));
            const Point orig_point(buffer[0], buffer[1]);
            const Point dest_point(buffer[2], buffer[3]);
            this->set_selected(orig_point);
            if(this->m_game->character_at(dest_point))
                this->handle_attack(dest_point);
            else
                this->handle_move(dest_point);
            this->m_game->set_state(Game::State::WAITING_PLAYER);
            break;
        }
        case Message::VICTORY: {
            int buffer[4];
            this->m_socket->recv(buffer, 4 * sizeof(int));
            const Point orig_point(buffer[0], buffer[1]);
            const Point dest_point(buffer[2], buffer[3]);
            this->clear_highlighted();
            this->clear_selected();
            this->click(orig_point);
            this->handle_move(dest_point);
            this->m_game->set_state(Game::State::DEFEAT);
            break;
        }
    }
    this->m_socket->set_blocking(false);
}

void GameController::send_quit_message() {
    Message msg = Message::QUIT;
    this->m_socket->send(&msg, sizeof(Message));
    this->m_game->set_state(Game::State::EXITING);
}

void GameController::update() {
    this->popup_on_gl_error();
    this->check_for_other();
}

void GameController::popup_on_gl_error() {
    GLenum error = glGetError();
    if(error == GL_NO_ERROR)
        return;
    QMessageBox::critical(
        nullptr,
        "glGetError",
        reinterpret_cast<const char *>(gluErrorString(error)));
}

void GameController::handle_selection(const Point & p) {
    auto new_selected = this->m_game->character_at(p);
    if(!new_selected)
        return;
    if(new_selected->position() == this->selected())
        return;
    if(new_selected->owner() != this->m_game->current_player())
        return;
    this->set_selected(new_selected->position());
    emit character_select(new_selected);
}

void GameController::handle_move(const Point & p) {
    auto selected = this->m_game->character_at(this->selected());
    this->m_game->move_character(selected, p);
    emit character_moved(selected, this->selected());
    switch(this->m_game->state()) {
        case Game::State::WAITING_OTHER:
            this->m_game->set_state(Game::State::WAITING_PLAYER);
            this->m_game->switch_player();
            break;
        case Game::State::WAITING_PLAYER:
            if(!this->m_game->check_victory()) {
                this->send_move_message(this->selected(), p);
                this->m_game->set_state(Game::State::WAITING_OTHER);
                this->m_game->switch_player();
            } else {
                this->m_game->set_state(Game::State::VICTORY);
                this->send_victory_message(this->selected(), p);
            }
            break;
        default:
            abort();
    }
    this->clear_highlighted();
    this->clear_selected();
}

void GameController::handle_attack(const Point & p) {
    auto orig = this->m_game->character_at(this->selected());
    auto dest = this->m_game->character_at(p);
    this->m_game->remove_character(dest);
    this->m_game->move_character(orig, p);
    emit character_attacked(orig, this->selected());
    auto state = this->m_game->state();
    if(state == Game::State::WAITING_OTHER) {
        this->m_game->set_state(Game::State::WAITING_PLAYER);
        this->m_game->switch_player();
    } else if(state == Game::State::WAITING_PLAYER) {
        this->send_move_message(this->selected(), p);
        this->m_game->set_state(Game::State::WAITING_OTHER);
        this->m_game->switch_player();
    }
    this->clear_highlighted();
    this->clear_selected();
}

void
GameController::send_move_message(const Point & orig, const Point & dest) {
    Message msg = Message::MOVEMENT;
    this->m_socket->send(&msg, sizeof(msg));
    int buffer[4] = {orig.x(), orig.y(), dest.x(), dest.y()};
    this->m_socket->send(buffer, sizeof(buffer));
}

void
GameController::send_victory_message(const Point & orig, const Point & dest) {
    Message msg = Message::VICTORY;
    this->m_socket->send(&msg, sizeof(msg));
    int buffer[4] = {orig.x(), orig.y(), dest.x(), dest.y()};
    this->m_socket->send(buffer, sizeof(buffer));
}
