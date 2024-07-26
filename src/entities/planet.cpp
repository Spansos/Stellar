#include "entities/planet.hpp"

#include "state.hpp"
#include "order.hpp"
#include "common.hpp"

Planet::Planet(sf::Vector2f position, EntityID id, EntityID owner) :
    Entity(EntityType::Planet, id, owner),
    position(position),
    population(0),
    organizations{}
{ }


UpdateResult Planet::process_order(Order & order, GameState * state) {
    if (order.entity_id.id != id.id || order.entity_id.revision != id.revision)
        ERROR_PRINT("planet (" + std::to_string(id.id) + ") ordered by mis-matched order");

    UpdateResult result;;

    if (order.type != Order::BUILD) {
        ERROR_PRINT("planet (" + std::to_string(id.id) + ") ordered with wrong order type");
        return result;
    }
    
    result.changes.push_back(
        StateChange{
            .entity_id = state->get_unused_id(),
            .property_id = (PropertyID)EntityType::Ship,
            .value = PropertyValue{
                .Vector2f = position,
                .type = PropertyValue::VECTOR2F
            },
            .operation = StateChange::NEW_ENTITY
        }
    );

    return result;
}

void Planet::update_state(StateChange change) {
    if (change.entity_id.id != id.id || change.entity_id.revision != id.revision)
        ERROR_PRINT("planet (" + std::to_string(id.id) + ") updated by mis-matched state change");

    switch (change.property_id) {
        case POSITION:
            if (change.operation != StateChange::SET || change.value.type != PropertyValue::VECTOR2F)
                ERROR_PRINT("planet (" + std::to_string(id.id) + ") given wrong state update");
            else
                position = change.value.Vector2f;
            break;

        case POPULATION:
            if (change.operation != StateChange::SET || change.value.type != PropertyValue::UINT64_T)
                ERROR_PRINT("planet (" + std::to_string(id.id) + ") given wrong state update");
            else
                population = change.value.Uint64_t;
            break;

        case ORGANIZATION_N ... ORGANIZATION_N+49:
            if (change.operation != StateChange::SET || change.value.type != PropertyValue::ENTITYID)
                ERROR_PRINT("planet (" + std::to_string(id.id) + ") given wrong state update");
            else
                organizations[change.property_id-ORGANIZATION_N] = change.value.EntityID_;
            break;

        default:
            ERROR_PRINT("planet (" + std::to_string(id.id) + ") given wrong state update");
    }
}

PropertyValue Planet::get_property(PropertyID property) {
    PropertyValue value{};
    switch (property) {
        case POSITION:
            value.type = PropertyValue::VECTOR2F;
            value.Vector2f = position;
            break;
        case POPULATION:
            value.type = PropertyValue::UINT64_T;
            value.Uint64_t = population;
            break;
        case ORGANIZATION_N ... ORGANIZATION_N+49:
            value.type = PropertyValue::ENTITYID;
            value.EntityID_ = organizations[property-ORGANIZATION_N];
            break;
        default:
            ERROR_PRINT("FRURCK U, P");
            throw;
    }
    return value;
}

UpdateResult Planet::update(GameState * game_state, float delta_time) {
    (void)game_state;
    (void)delta_time;
    
    return UpdateResult{};
}
