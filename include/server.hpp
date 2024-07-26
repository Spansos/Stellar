#pragma once

#include <memory>
#include <list>
#include <vector>
#include <SFML/Network.hpp>
#include "state.hpp"

class Server {
public:
    Server(int ticks_per_second, int port);
    // main loop.
    void run();
private:
    // update the game state one tick
    UpdateResult tick(float deltatime);
    // returns all new packets from all clients.
    // also removes clients form clients if they have disconnected
    std::vector<sf::Packet> get_incoming();
    // adds any incoming connections to clients
    void handle_new_connections();
    sf::TcpListener connection_listener;
    std::list<std::unique_ptr<sf::TcpSocket>> clients;
    GameState state;
    int tps;
};