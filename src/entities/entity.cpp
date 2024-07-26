#include "entities/entity.hpp"

#include "common.hpp"


Entity::Entity(EntityType type, EntityID id, EntityID owned_by) :
    type(type),
    id(id),
    owned_by(owned_by)
{ }

EntityID Entity::get_id() {
    return id;
}

EntityType Entity::get_type() {
    return type;
}





bool operator==(const EntityID lhs, const EntityID rhs) {
    return (lhs.id == rhs.id) && (lhs.revision == rhs.revision);
}


sf::Packet & operator<<(sf::Packet & packet, StateChange state_change) {
    packet << state_change.entity_id.id;
    packet << state_change.entity_id.revision;
    packet << state_change.property_id;
    packet << (std::uint8_t)(state_change.operation);
    packet << (std::uint8_t)(state_change.value.type);
    switch (state_change.value.type) {
        case PropertyValue::FLOAT:
            packet << state_change.value.Float;
            break;
        case PropertyValue::VECTOR2F:
            packet << state_change.value.Vector2f.x;
            packet << state_change.value.Vector2f.y;
            break;
        case PropertyValue::UINT32_T:
            packet << state_change.value.Uint32_t;
            break;
        case PropertyValue::UINT64_T:
            packet << (sf::Uint64&)state_change.value.Uint64_t;
            break;
        case PropertyValue::NONE:
            break;
        case PropertyValue::ENTITYID:
            packet << state_change.value.EntityID_.id;
            packet << state_change.value.EntityID_.revision;
            break;
        default:
            WARNING_PRINT("corrupted state change in serialization");
    }
    return packet;
}

sf::Packet & operator>>(sf::Packet & packet, StateChange & state_change) {
    packet >> state_change.entity_id.id;
    packet >> state_change.entity_id.revision;
    packet >> state_change.property_id;
    packet >> (std::uint8_t&)(state_change.operation);
    packet >> (std::uint8_t&)(state_change.value.type);
    switch (state_change.value.type) {
        case PropertyValue::FLOAT:
            packet >> state_change.value.Float;
            break;
        case PropertyValue::VECTOR2F:
            packet >> state_change.value.Vector2f.x;
            packet >> state_change.value.Vector2f.y;
            break;
        case PropertyValue::UINT32_T:
            packet >> state_change.value.Uint32_t;
            break;
        case PropertyValue::UINT64_T:
            packet >> (sf::Uint64&)state_change.value.Uint64_t;
            break;
        case PropertyValue::NONE:
            break;
        case PropertyValue::ENTITYID:
            packet >> state_change.value.EntityID_.id;
            packet >> state_change.value.EntityID_.revision;
            break;
        default:
            WARNING_PRINT("corrupted package in state change deserialization");
    }
    if (!packet)
        WARNING_PRINT("packet invalid in deserialization of state change");
    if (!packet.endOfPacket())
        WARNING_PRINT("order end is not packet end");
    return packet;
}