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
    camera = window.getDefaultView();
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
                camera.setSize(sf::Vector2f{window.getSize()});
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
        camera.move({0, -1});
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        camera.move({-1, 0});
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        camera.move({0, 1});
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        camera.move({1, 0});
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        camera.zoom(.95);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        camera.zoom(1.05);

    sf::Vector2f mouse_pos = sf::Vector2f{sf::Mouse::getPosition(window)};

    for (const auto & event : events) {
        if (event.type == sf::Event::MouseButtonPressed)
            if (event.mouseButton.button == sf::Mouse::Button::Left)
                selection_start = mouse_pos;
        if (event.type == sf::Event::MouseButtonReleased)
            if (event.mouseButton.button == sf::Mouse::Button::Left)
                selection_start = {};
        if (event.type == event.MouseWheelScrolled)
            camera.zoom(1 - event.mouseWheelScroll.delta * 0.05);
    }
    
    if (selection_start) {
        sf::FloatRect selection_rect = {selection_start.value(), mouse_pos-selection_start.value()};
        ui.draw_box(ui.pixel_to_ui_rect(selection_rect), sf::Color::Transparent, sf::Color{160, 160, 160}, -1);

        selected.clear();
        for (auto & entry : state.entities) {
            if (!entry.pointer) continue;
            if (entry.pointer->get_type() == EntityType::Planet) {
                if (selection_rect.contains(entry.pointer->get_property(Planet::POSITION).Vector2f - camera.getCenter()+camera.getSize()/2.0f)) {
                    selected.clear();
                    selected.insert(entry.pointer->get_id());
                    break;
                }
            } else if (entry.pointer->get_type() == EntityType::Ship) {
                if (selection_rect.contains(entry.pointer->get_property(Ship::POSITION).Vector2f - camera.getCenter()+camera.getSize()/2.0f))
                    selected.insert(entry.pointer->get_id());
            }
        }
    }



    sf::FloatRect frame{.05, .05, .3f, .2f};
    ui.draw_box(frame, {48, 64, 128});
    ui.start_frame(frame);
    {
        ui.draw_text({0, 0, 1, .5}, .1f, "hello greetings fellow me i hope the this works first try else i WILL have to debug thios shit, which is an activity which is my hobby so im actually quite neutral about the whole thing", sf::Color::White);
        ui.draw_button({0.4, .6, .2, .2}, {64, 64, 64}, {128, 128, 128}, {48, 48, 48});
        if (ui.is_button_pressed({0.4, .6, .2, .2},events))
            std::cout << "BUTTTONON" << std::endl;
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
    window.setView(camera);

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
                if (selected.find(ship.get_id()) != selected.end())
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
                if (selected.find(planet.get_id()) != selected.end())
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