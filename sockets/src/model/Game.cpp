#include "model/Game.h"

#include <algorithm>
#include <cassert>
#include <vector>

#include "model/Character.h"
#include "model/Player.h"

Game::Game(
        const Player & player,
        const Player & other_player,
        State initial_state) :
    m_player(player),
    m_other_player(other_player),
    m_state(initial_state),
    m_characters(this->create_characters()),
    m_current_player(
        this->m_player.id() == 1 ? &this->m_player : &this->m_other_player) {}

Game::~Game() {
    for(auto x : this->m_characters)
        delete x;
}

std::vector<Character *> Game::create_characters() const {
    auto ret = std::vector<Character *>(63, nullptr);
    const Player * player1 = &this->m_player;
    const Player * player2 = &this->m_other_player;
    if(this->m_player.id() != 1)
        std::swap(player1, player2);
#define C(p, o, c) ret[this->index(p)] = new c(o, p)
    C(Point(6, 2), player1, Rat);
    C(Point(0, 6), player2, Rat);
    C(Point(1, 1), player1, Cat);
    C(Point(5, 7), player2, Cat);
    C(Point(5, 1), player1, Wolf);
    C(Point(1, 7), player2, Wolf);
    C(Point(2, 2), player1, Dog);
    C(Point(4, 6), player2, Dog);
    C(Point(4, 2), player1, Panther);
    C(Point(2, 6), player2, Panther);
    C(Point(0, 0), player1, Tiger);
    C(Point(6, 8), player2, Tiger);
    C(Point(6, 0), player1, Lion);
    C(Point(0, 8), player2, Lion);
    C(Point(0, 2), player1, Elephant);
    C(Point(6, 6), player2, Elephant);
#undef C
    return ret;
}

void Game::move_character(Character * character, const Point & dest) {
    assert(this->m_characters.at(this->index(dest)) == nullptr);
    std::swap(
        this->m_characters.at(this->index(character->position())),
        this->m_characters.at(this->index(dest)));
    character->set_position(dest);
}

void Game::remove_character(Character * character) {
    this->m_characters.at(this->index(character->position())) = nullptr;
    delete character;
}

bool Game::water(const Point & p) const {
    return abs(p.y() - 4) < 2 && ((abs(p.x() - 3) + 1) / 2) == 1;
}

bool Game::trap(const Point & p) const {
    return abs(p.y() - 4) - abs(p.x() - 3) == 3;
}

Point Game::can_jump(const Character * c, int dx, int dy) const {
    assert(dx == 0 || dy == 0);
    Point landing = c->position();
    do {
        landing.rx() += dx;
        landing.ry() += dy;
    } while(!this->character_at(landing) && this->water(landing));
    if(!this->water(landing))
        return landing;
    return c->position();
}

std::list<Point> Game::possible_moves(const Character * c) const {
    const auto pos = c->position();
    auto possible_moves = std::list<Point>{
        pos + Point(1, 0),
        pos - Point(1, 0),
        pos + Point(0, 1),
        pos - Point(0, 1),
    };
    if(c->can_jump()) {
        auto jumps = this->jumps(c, possible_moves);
        std::copy(
            std::begin(jumps), std::end(jumps),
            std::back_inserter(possible_moves));
    }
    auto it = possible_moves.begin();
    while(it !=possible_moves.end()) {
        if(it->x() < 0 || 6 < it->x() || it->y() < 0 || 8 < it->y()) {
            it = possible_moves.erase(it);
            continue;
        }
        if(!c->can_swim() && this->water(*it)) {
            it = possible_moves.erase(it);
            continue;
        }
        const Character * dest = this->character_at(*it);
        if(!dest) {
            ++it;
            continue;
        }
        if(dest->owner() == this->current_player()) {
            it = possible_moves.erase(it);
            continue;
        }
        const bool orig_water = this->water(pos);
        const bool dest_water = this->water(*it);
        if((orig_water && !dest_water) || (!orig_water && dest_water)) {
            it = possible_moves.erase(it);
            continue;
        }
        int value_orig = this->trap(pos) ? 0 : c->value();
        int value_dest = this->trap(*it) ? 0 : dest->value();
        if(value_orig == Rat::VALUE && value_dest == Elephant::VALUE) {
            ++it;
            continue;
        }
        if(value_orig == Elephant::VALUE && value_dest == Rat::VALUE) {
            it = possible_moves.erase(it);
            continue;
        }
        if(value_orig < value_dest) {
            it = possible_moves.erase(it);
            continue;
        }
        ++it;
    }
    return possible_moves;
}

std::list<Point>
Game::jumps(const Character * c, const std::list<Point> & moves) const {
    std::list<Point> ret;
    for(auto p : moves) {
        if(!this->water(p))
            continue;
        const auto d = p - c->position();
        const auto landing = this->can_jump(c, d.x(), d.y());
        if(landing != c->position())
            ret.push_back(landing);
    }
    return ret;
}

bool Game::check_victory() const {
    auto id = this->current_player()->id();
    auto c = this->character_at({3, id == 1 ? 8 : 0});
    return c && c->owner()->id() == id;
}
