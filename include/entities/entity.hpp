#pragma once

#include <SFML/System/Vector2.hpp>
#include <cstdint>
#include <vector>

class Order;
class GameState;

// better way to do shit:
//      have a bunch of methods in GameState for entities to use (a bit like subclass sandbox but without the subclass)
//      then have the state have methods and such handling the state changes of those function calls (who would've thunk that having the state handle the state was good)
//      maybe send the functions to call + their operands to the server instead of their result ? could be funky

// more data-oriented way to do things:
//      make a entity-type class to hold information about type
//      replace the EntityType enum with a class-id

// poperties still need a good look-over to decide on more data-oriented design, for the current aproach has too much boilerplate imo (also id like this to be mod-able, but that is just a dream)

struct EntityID {
    std::uint16_t id;
    std::uint16_t revision;
};
typedef std::uint16_t PropertyID;

template<>
struct std::hash<EntityID> {
    std::size_t operator()(const EntityID id) const {
        return std::hash<std::uint32_t>()(static_cast<std::uint32_t>(id.id) + (static_cast<std::uint32_t>(id.revision)<<16));
    }
};

bool operator==(const EntityID lhs, const EntityID rhs);

struct PropertyValue {
    union {
        float Float;
        sf::Vector2f Vector2f;
        std::uint32_t Uint32_t;
        std::uint64_t Uint64_t;
        EntityID EntityID_;
    };
    
    enum : std::uint8_t {FLOAT, VECTOR2F, UINT32_T, UINT64_T, ENTITYID, NONE} type;
};

struct StateChange {
    EntityID entity_id;
    PropertyID property_id;
    PropertyValue value;
    enum : std::uint8_t {SET, POP, PUSH, EMPTY, NEW_ENTITY, KILL_ENTITY} operation;
};

struct UpdateResult {
    std::vector<StateChange> changes;
};

enum class EntityType : std::uint8_t {
    Ship,
    Organization,
    Planet
};


class Entity {
public:
    Entity() = default;
    Entity(EntityType type, EntityID id, EntityID owned_by);
    virtual UpdateResult process_order(Order & order, GameState * state) = 0;
    virtual void update_state(StateChange change) = 0;
    virtual PropertyValue get_property(PropertyID property) = 0;
    virtual UpdateResult update(GameState * game_state, float delta_time) = 0;
    EntityID get_id();
    EntityType get_type();
protected:
    EntityType type;
    EntityID id;
    EntityID owned_by;
};






#include <SFML/Network.hpp>
sf::Packet & operator<<(sf::Packet & packet, StateChange state_change);
sf::Packet & operator>>(sf::Packet & packet, StateChange & state_change);