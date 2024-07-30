#include "client.hpp"

#include "common.hpp"
#include "entities.hpp"
#include "rendering/ui.hpp"

Client::Client(sf::IpAddress server_address, int server_port) {
    auto status = server.connect( server_address, server_port );
    if (status != sf::Socket::Done)
        ERROR_PRINT("couldn't connect to " + server_address.toString() + ":" + std::to_string(server_port));
    INFO_PRINT("connected to server: " + server.getRemoteAddress().toString() + ":" + std::to_string(server.getRemotePort()));
    window.create({1024, 720}, "Stellarium");
    ui_state.camera = window.getDefaultView();
    window.setFramerateLimit(60);
}

void Client::run() {
    while (window.isOpen()) {
        listen_to_server();
        render_game();
        auto orders = render_ui(get_input());
        for (auto & order : orders)
            send_order(order);
        window.display();
    }
}

std::vector<sf::Event> Client::get_input() {
    std::vector<sf::Event> events;

    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized:
                ui_state.camera.setSize(sf::Vector2f{window.getSize()});
            default:
                break;
        }
        events.push_back(event);
    }
    
    return events;
}

std::vector<Order> Client::render_ui(const std::vector<sf::Event> & events) {
    std::vector<Order> orders;


    Ui ui{window};

    // ui is drawn on the window in absolute coordinates, no camera
    window.setView(sf::View{{{0, 0}, sf::Vector2f{window.getSize()}}});

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        ui_state.camera.move({0, -1});
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        ui_state.camera.move({-1, 0});
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        ui_state.camera.move({0, 1});
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        ui_state.camera.move({1, 0});
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        ui_state.camera.zoom(.95);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        ui_state.camera.zoom(1.05);

    sf::Vector2f mouse_pos = sf::Vector2f{sf::Mouse::getPosition(window)};

    for (const auto & event : events) {
        if (event.type == sf::Event::MouseButtonPressed)
            if (event.mouseButton.button == sf::Mouse::Button::Left)
                ui_state.selection_start = mouse_pos;
        if (event.type == sf::Event::MouseButtonReleased)
            if (event.mouseButton.button == sf::Mouse::Button::Left)
                ui_state.selection_start = {};
        if (event.type == event.MouseWheelScrolled)
            ui_state.camera.zoom(1 - event.mouseWheelScroll.delta * 0.05);
    }
    
    if (ui_state.selection_start) {
        sf::FloatRect selection_rect = {ui_state.selection_start.value(), mouse_pos-ui_state.selection_start.value()};
        ui.draw_box(ui.pixel_to_ui_rect(selection_rect), sf::Color::Transparent, sf::Color{160, 160, 160}, -1);

        ui_state.selected.clear();
        for (auto & entry : state.entities) {
            if (!entry.pointer) continue;
            if (entry.pointer->get_type() == EntityType::Planet) {
                if (selection_rect.contains(entry.pointer->get_property(Planet::POSITION).Vector2f - ui_state.camera.getCenter()+ui_state.camera.getSize()/2.0f)) {
                    ui_state.selected.clear();
                    ui_state.selected.insert(entry.pointer->get_id());
                    break;
                }
            } else if (entry.pointer->get_type() == EntityType::Ship) {
                if (selection_rect.contains(entry.pointer->get_property(Ship::POSITION).Vector2f - ui_state.camera.getCenter()+ui_state.camera.getSize()/2.0f))
                    ui_state.selected.insert(entry.pointer->get_id());
            }
        }
    }



    sf::FloatRect frame{.05, .65, .4f, .3f};
    ui.start_frame(frame);
    {
        const float button_size = .1;
        const sf::FloatRect org_button = {.0, .0, button_size, button_size};
        const sf::FloatRect demo_button = {button_size, .0, button_size, button_size};
        const sf::FloatRect mill_button = {button_size*2, .0, button_size, button_size};
        ui.draw_button(org_button, {48, 128, 64}, {64, 156, 128}, {48, 48, 48});
        if (ui.is_button_pressed(org_button, events))
            ui_state.planet_tab = UiState::PlanetTab::Organization;
        ui.draw_button(demo_button, {64, 48, 128}, {128, 64, 156}, {48, 48, 48});
        if (ui.is_button_pressed(demo_button, events))
            ui_state.planet_tab = UiState::PlanetTab::Demographics;
        ui.draw_button(mill_button, {128, 64, 48}, {156, 128, 64}, {48, 48, 48});
        if (ui.is_button_pressed(mill_button, events))
            ui_state.planet_tab = UiState::PlanetTab::Millitary;

        sf::FloatRect frame{0, button_size, 1, 1-button_size};
        ui.start_frame(frame);
        {
            switch (ui_state.planet_tab) {
                case UiState::PlanetTab::Organization:
                    ui.draw_box({0,0,1,1}, sf::Color::Green);
                    for (int i=0; i<50; ++i) {
                        const sf::Vector2f slot_size = {.1,.2};
                        const float slot_padding = .05;
                        const sf::FloatRect org_slot = {(i%10)*(slot_size.x+slot_padding), (i/10)*(slot_size.y+slot_padding), slot_size.x, slot_size.y};
                        ui.draw_box(org_slot, (!(i%10%2) != !(i/10%2)) ? sf::Color{255, 0, 0, 128} : sf::Color{0, 0, 255, 128});

                    }

                    break;
                case UiState::PlanetTab::Demographics:
                    ui.draw_box({0,0,1,1}, sf::Color::Blue);
                    break;
                case UiState::PlanetTab::Millitary:
                    ui.draw_box({0,0,1,1}, sf::Color::Red);
                    break;
            }
        }
        ui.end_frame();
    }
    ui.end_frame();

    return orders;
}

void Client::listen_to_server() {
    server.setBlocking(false);

    sf::Packet packet;
    sf::Socket::Status status;
    while ( (status = server.receive(packet)) == sf::Socket::Done ) {
        StateChange change{};
        packet >> change;
        state.process_change(change);
    }

    if (status == sf::Socket::Disconnected)
        ERROR_PRINT("server closed");

    if (status == sf::Socket::Error)
        WARNING_PRINT("error on receiving packets");
}

void Client::render_game() {
    // game is rendered with game camera
    window.setView(ui_state.camera);

    window.clear(sf::Color{16, 16, 24});

    for (auto & entityEntry : state.entities) {
        if (!entityEntry.pointer)
            continue;

        switch (entityEntry.pointer->get_type()) {
            case EntityType::Ship: {
                Ship &ship = *static_cast<Ship*>(entityEntry.pointer.get());

                static sf::CircleShape shape;
                shape.setFillColor(sf::Color::Red);
                shape.setPosition(ship.get_property(Ship::POSITION).Vector2f - sf::Vector2f(16,16));
                shape.setOutlineColor(sf::Color::White);
                if (ui_state.selected.find(ship.get_id()) != ui_state.selected.end())
                    shape.setOutlineThickness(-2);
                else
                    shape.setOutlineThickness(0);
                shape.setRadius(16);

                window.draw(shape);
            } break;

            case EntityType::Planet: {
                Planet &planet = *static_cast<Planet*>(entityEntry.pointer.get());

                sf::CircleShape shape;
                shape.setFillColor(sf::Color::Blue);
                shape.setPosition(planet.get_property(Planet::POSITION).Vector2f - sf::Vector2f(32,32));
                shape.setOutlineColor(sf::Color::White);
                if (ui_state.selected.find(planet.get_id()) != ui_state.selected.end())
                    shape.setOutlineThickness(-2);
                else
                    shape.setOutlineThickness(0);
                shape.setRadius(32);

                window.draw(shape);
            } break;

            default:
                throw;
        }
    }
}

void Client::send_order(Order order) {
    sf::Packet packet;
    packet << order;

    server.setBlocking(true);

    if (server.send(packet) != sf::Socket::Done)
        ERROR_PRINT("no fucking ding dong");
}