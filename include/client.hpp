#pragma once

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <unordered_set>
#include <optional>
#include "order.hpp"
#include "state.hpp"

struct UiState {
    enum class PlanetTab {Organization, Demographics, Millitary} planet_tab;
    std::unordered_set<EntityID> selected;
    std::optional<sf::Vector2f> selection_start;
    sf::View camera;
};

class Client {
public:
    Client(sf::IpAddress server_address, int server_port);
    void run();
private:
    // listen to and return a list of events.
    // automatically handels non-game events, such as resizing and closing the window
    std::vector<sf::Event> get_input();
    // do ui, returns orders based on ui input
    std::vector<Order> render_ui(const std::vector<sf::Event> & events);
    // listen to server for updates.
    // updates the state
    void listen_to_server();
    // render the current game state (ui excluded)
    void render_game();
    // send order to server
    void send_order(Order order);
    GameState state;
    UiState ui_state;
    sf::TcpSocket server;
    sf::RenderWindow window;
};