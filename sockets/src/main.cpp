#include <string>

#include <QApplication>
#include <QMessageBox>
#include <QTimer>

#include <exception>
#include <iostream>
#include <memory>

#include "gui/GameController.h"
#include "gui/GameDisplay.h"
#include "gui/MainWindow.h"
#include "gui/OpenDialog.h"
#include "model/Game.h"
#include "model/Player.h"
#include "net/TcpSocket.h"
#include "net/SocketException.h"

TcpSocket * init_sockets(
    TcpSocket * socket, bool is_server, const std::string & server_addr);
bool get_initial_data(
    int argc, char ** argv,
    std::string * name, bool * is_server, std::string * addr);
std::string get_other_name(TcpSocket * socket);
void do_handshake(
    QApplication * app, TcpSocket * socket,
    bool is_server, const std::string & player_name,
    Player * player, Player * other_player);
int run(
    bool is_server, const Player & player, const Player & other_player,
    TcpSocket * socket);

bool get_initial_data(
        int argc, char ** argv,
        std::string * name, bool * is_server, std::string * addr) {
    if(argc > 1) {
        *name = std::string(argv[1]);
        *is_server = argc == 2;
        if(argc > 2)
            *addr = std::string(argv[2]);
        return true;
    }
    OpenDialog open_dialog;
    if(!open_dialog.exec())
        return false;
    *name = open_dialog.name();
    *is_server = open_dialog.is_server();
    *addr = open_dialog.addr();
    return true;
}

TcpSocket * init_sockets(
        TcpSocket * socket, bool is_server, const std::string & server_addr) {
    if(is_server) {
        socket->bind(8000);
        socket->listen(1);
        std::cout << "Waiting on port 8000 for other player...\n";
        return socket->accept();
    } else {
        socket->connect(server_addr, 8000);
        return socket;
    }
}

void do_handshake(
        TcpSocket * socket, bool is_server, const std::string & player_name,
        Player * player, Player * other_player) {
    int player_name_len = player_name.size();
    socket->send(&player_name_len, sizeof(int));
    socket->send(player_name.c_str(), player_name_len);
    *player = Player(is_server ? 1 : 2, player_name);
    *other_player = Player(is_server ? 2 : 1, get_other_name(socket));
}

std::string get_other_name(TcpSocket * socket) {
    int other_name_len;
    socket->recv(&other_name_len, sizeof(int));
    auto other_name = new char[other_name_len + 1];
    socket->recv(other_name, other_name_len);
    other_name[other_name_len] = '\0';
    auto ret = std::string(other_name);
    delete [] other_name;
    return ret;
}

int run(
        QApplication * app, bool is_server,
        const Player & player, const Player & other_player,
        TcpSocket * socket) {
    std::unique_ptr<GameDisplay> game_display(new GameDisplay);
    game_display->makeCurrent();
    Game game(
        player, other_player,
        is_server ? Game::State::WAITING_PLAYER : Game::State::WAITING_OTHER);
    GameController game_controller(&game, socket);
    game_display->set_game_controller(&game_controller);
    MainWindow window(0, game_display.release());
    window.setWindowTitle(
        (player.name() + " x " + other_player.name()).c_str());
    window.resize(800, 600);
    window.move(static_cast<int>(!is_server) * 800, 0);
    window.show();
    QObject::connect(
        &game_controller,
        SIGNAL(character_moved(const Character *, const Point &)),
        &window,
        SLOT(character_move(const Character *, const Point &)));
    QObject::connect(
        &game_controller,
        SIGNAL(character_attacked(const Character *, const Point &)),
        &window,
        SLOT(character_attack(const Character *, const Point &)));
    QObject::connect(
        &game_controller,
        SIGNAL(character_select(const Character *)),
        &window,
        SLOT(character_select(const Character *)));
    window.timer()->start(20);
    return app->exec();
}

int main(int argc, char ** argv) {
    QApplication app(argc, argv);
    std::string server_addr, player_name;
    bool is_server;
    if(!get_initial_data(argc, argv, &player_name, &is_server, &server_addr))
        return 0;
    try {
        TcpSocket socket;
        TcpSocket * player_socket =
            init_sockets(&socket, is_server, server_addr);
        Player player, other_player;
        do_handshake(
            player_socket, is_server, player_name, &player, &other_player);
        player_socket->set_blocking(false);
        return run(&app, is_server, player, other_player, player_socket);
    } catch(SocketException ex) {
        QMessageBox::critical(0, "Socket error", ex.what());
        return 1;
    } catch(std::exception ex) {
        QMessageBox::critical(0, "Error", ex.what());
        return 1;
    }
}
