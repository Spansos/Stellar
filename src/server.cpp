#include "server.hpp"

#include "common.hpp"
#include "order.hpp"

Server::Server(int ticks_per_second, int port) :
    tps(ticks_per_second)
{
    connection_listener.setBlocking(false);
    if (connection_listener.listen(port) == sf::Socket::Status::Done)
        INFO_PRINT("server started on port " + std::to_string(connection_listener.getLocalPort()));
    else
        ERROR_PRINT("error opening port for connections");
}

void Server::run() {
    while (true) {
        handle_new_connections();

        std::vector<sf::Packet> packets = get_incoming();
        
        for (auto & packet : packets) {
            Order order;
            packet >> order;

            UpdateResult order_result = state.process_order(order);
            for (auto & change : order_result.changes) {
                state.process_change(change);
                for (auto & client : clients) {
                    sf::Packet packet;
                    packet << change;
                    client->setBlocking(true);
                    client->send(packet);
                }
            }
        }

        UpdateResult tick_result = tick(1.0f/tps);
        for (auto & change : tick_result.changes) {
            state.process_change(change);
            for (auto & client : clients) {
                sf::Packet packet;
                packet << change;
                client->setBlocking(true);
                client->send(packet);
            }
        }

        sf::sleep(sf::microseconds(1'000'000/tps));
    }
}

UpdateResult Server::tick(float deltatime) {
    UpdateResult result;

    for (auto & entityEntry : state.entities) {
        if (entityEntry.pointer) {
            UpdateResult update_result = entityEntry.pointer->update(&state, deltatime);
            result.changes.insert(result.changes.end(), update_result.changes.begin(), update_result.changes.end());
        }
    }

    return result;
}

std::vector<sf::Packet> Server::get_incoming() {
    std::vector<sf::Packet> packets;
    
    // for every client
    for (auto it = clients.begin(); it != clients.end(); ) {
        auto & client = **it;
        client.setBlocking(false);

        sf::Packet packet;
        switch (client.receive(packet)) {
            case sf::Socket::Done:
                packets.push_back(packet);
                break;
            case sf::Socket::NotReady:
                ++it;
                break;
            case sf::Socket::Disconnected:
                INFO_PRINT("connection disconnected: " + client.getRemoteAddress().toString());
                clients.erase(it++);
                break;
            default:
                WARNING_PRINT("error during receiving");
                it++;
        }
    }

    return packets;
}

void Server::handle_new_connections() {
    std::unique_ptr<sf::TcpSocket> new_client;
    sf::Socket::Status status;
    while ((status = connection_listener.accept(*(new_client = std::make_unique<sf::TcpSocket>()))) == sf::Socket::Done) {
        INFO_PRINT("new connection: " + new_client->getRemoteAddress().toString());
        clients.push_back(std::move(new_client));
    }
    if (status != sf::Socket::NotReady)
        WARNING_PRINT("something went wrong handling new connections");
}