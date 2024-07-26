#include "entities/ship.hpp"

#include <cmath>
#include "order.hpp"
#include "common.hpp"
#include "state.hpp"

Ship::Ship(sf::Vector2f position, EntityID id, EntityID owner) :
    Entity(EntityType::Ship, id, owner),
    position(position)
{
    type = EntityType::Ship;
}

UpdateResult Ship::process_order(Order & order, GameState * state) {
    (void)state;

    if (order.entity_id.id != id.id || order.entity_id.revision != id.revision)
        ERROR_PRINT("ship (" + std::to_string(id.id) + ") ordered by mis-matched order");

    UpdateResult result;;

    if (order.type != Order::MOVE) {
        ERROR_PRINT("ship (" + std::to_string(id.id) + ") ordered with wrong order type");
        return result;
    }
    if (order.move.overwrite)
        result.changes.push_back(
            StateChange{
                .entity_id = id,
                .property_id = DESTINATION,
                .value = {
                    .Uint32_t = 0,
                    .type = PropertyValue::NONE
                },
                .operation = StateChange::EMPTY
            }
        );

    result.changes.push_back(
        StateChange{
            .entity_id = id,
            .property_id = DESTINATION,
            .value = PropertyValue{
                .Vector2f = order.move.destination,
                .type = PropertyValue::VECTOR2F
            },
            .operation = StateChange::PUSH
        }
    );

    return result;
}

UpdateResult Ship::update(GameState * game_state, float delta_time) {
    (void)game_state;

    UpdateResult result;

    if (!destinations.empty()) {
        sf::Vector2f dest = destinations.front();

        sf::Vector2f relative_dest = dest - position;
        float dest_distance = std::sqrt(relative_dest.x*relative_dest.x+relative_dest.y*relative_dest.y);
        sf::Vector2f dest_direction;
        if (dest_distance)
            dest_direction = relative_dest / dest_distance;

        sf::Vector2f movement = dest_direction * std::min(speed*delta_time, dest_distance);

        result.changes.push_back(
            StateChange{
                .entity_id = id,
                .property_id = POSITION,
                .value = PropertyValue{
                    .Vector2f = position + movement,
                    .type = PropertyValue::VECTOR2F
                },
                .operation = StateChange::SET
            }
        );
        
        if (std::abs(dest_distance) < 1) {
            result.changes.push_back(
                StateChange{
                    .entity_id = id,
                    .property_id = DESTINATION,
                    .value = {
                        .Uint32_t = 0,
                        .type = PropertyValue::NONE
                    },
                    .operation = StateChange::POP
                }
            );
        }
    }
    
    return result;
}

void Ship::update_state(StateChange change) {
    if (change.entity_id.id != id.id || change.entity_id.revision != id.revision)
        ERROR_PRINT("ship (" + std::to_string(id.id) + ") updated by mis-matched state change");

    switch (change.property_id) {
        case SPEED:
            if (change.operation != StateChange::SET || change.value.type != PropertyValue::FLOAT)
                ERROR_PRINT("ship (" + std::to_string(id.id) + ") given wrong state update");
            else
                speed = change.value.Float;
            break;

        // case VELOCITY:
        //     if (change.operation != StateChange::SET || change.value.type != PropertyValue::VECTOR2F)
        //         ERROR_PRINT("ship (" + std::to_string(id.id) + ") given wrong state update");
        //     else
        //         velocity = change.value.Vector2f;
        //     break;

        case POSITION:
            if (change.operation != StateChange::SET || change.value.type != PropertyValue::VECTOR2F)
                ERROR_PRINT("ship (" + std::to_string(id.id) + ") given wrong state update");
            else
                position = change.value.Vector2f;
            break;

        case DESTINATION:
            if (change.operation == StateChange::POP)
                destinations.pop_front();
            else if (change.operation == StateChange::EMPTY)
                destinations.clear();
            else if (change.operation == StateChange::PUSH && change.value.type == PropertyValue::VECTOR2F)
                destinations.push_back(change.value.Vector2f);
            else
                ERROR_PRINT("ship (" + std::to_string(id.id) + ") given wrong state update");
            break;

        default:
            ERROR_PRINT("ship (" + std::to_string(id.id) + ") given wrong state update");
    }
}

PropertyValue Ship::get_property(PropertyID property) {
    PropertyValue value{};
    switch (property) {
        case SPEED:
            value.type = PropertyValue::FLOAT;
            value.Float = speed;
            break;
        // case VELOCITY:
        //     value.type = PropertyValue::VECTOR2F;
        //     value.Vector2f = velocity;
        //     break;
        case POSITION:
            value.type = PropertyValue::VECTOR2F;
            value.Vector2f = position;
            break;
        default:
            ERROR_PRINT("FRURCK U");
            throw;
    }
    return value;
}