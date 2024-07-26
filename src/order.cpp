#include "order.hpp"

sf::Packet& operator<<(sf::Packet& packet, Order order) {
    packet << order.entity_id.id;
    packet << order.entity_id.revision;
    packet << (std::uint8_t)order.type;
    switch (order.type) {
        case Order::MOVE:
            packet << order.move.overwrite;
            packet << order.move.destination.x;
            packet << order.move.destination.y;
            break;
        case Order::BUILD:
            break;
        default:
            WARNING_PRINT("uninitialized packet type in serialization");
    }
    return packet;
}

sf::Packet& operator>>(sf::Packet& packet, Order &order) {
    packet >> order.entity_id.id;
    packet >> order.entity_id.revision;
    packet >> (std::uint8_t&)order.type;
    switch (order.type) {
        case Order::MOVE:
            packet >> order.move.overwrite;
            packet >> order.move.destination.x;
            packet >> order.move.destination.y;
            break;
        case Order::BUILD:
            break;
        default:
            WARNING_PRINT("corrupted package in order deserialization");
    }
    if (!packet)
        WARNING_PRINT("packet invalid in deserialization of order of type " + std::to_string((int)order.type));
    if (!packet.endOfPacket())
        WARNING_PRINT("order end is not packet end");
    return packet;
}