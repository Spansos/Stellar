#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Network/Packet.hpp>
#include <cstdint>

#include "entities/entity.hpp"
#include "common.hpp"


struct Order {
    struct MoveOrder {
        MoveOrder() = default;
        sf::Vector2f destination;
        bool overwrite;
    };
    struct BuildOrder {
    };

    Order() : entity_id({0,0}), type(MOVE), move(MoveOrder{}) { }
    Order(EntityID id, MoveOrder order) : entity_id(id), type(MOVE), move(order) { }
    Order(EntityID id, BuildOrder order) : entity_id(id), type(BUILD), build(order) { }

    EntityID entity_id;

    enum : std::uint8_t {
        MOVE  = 0,
        BUILD = 1
    } type;

    union {
        MoveOrder move;
        BuildOrder build;
    };
};

sf::Packet& operator<<(sf::Packet& packet, Order order);

sf::Packet& operator>>(sf::Packet& packet, Order &order);