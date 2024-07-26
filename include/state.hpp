#pragma once

#include <vector>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include "entities.hpp"
#include "order.hpp"

struct EntityEntry {
    std::unique_ptr<Entity> pointer;
    std::uint16_t revision;
};

class GameState {
public:
    GameState() {
        StateChange change{};
        change.operation = StateChange::NEW_ENTITY;
        change.value.type = PropertyValue::VECTOR2F;
        change.value.Vector2f = sf::Vector2f{50, 50};
        change.property_id = (PropertyID)EntityType::Planet;
        change.entity_id.id = 0;
        change.entity_id.revision = 0;
        new_entity(change);
    }

    UpdateResult process_order(Order order) {
        INFO_PRINT("order type " + std::to_string(order.type) + " called on entity " + std::to_string(order.entity_id.id));
        return entities[order.entity_id.id].pointer->process_order(order, this);
    }

    void process_change(StateChange change) {
        // INFO_PRINT("change operation " + std::to_string(change.operation) + " on entity " + std::to_string(change.entity_id.id));
        if (change.operation == StateChange::NEW_ENTITY) {
            new_entity(change);
            return;
        }
        entities[change.entity_id.id].pointer->update_state(change);
    }

    void new_entity(StateChange change) {
        if (change.operation != StateChange::NEW_ENTITY) {
            ERROR_PRINT("state change is not NEW_ENTITY, yet it makes a new entity.. curious");
            return;
        }
        if (entities[change.entity_id.id].pointer) {
            ERROR_PRINT("NEW_ENTITY order given would overwrite existing");
            return;
        }

        switch (static_cast<EntityType>(change.property_id)) {
            case EntityType::Ship:
                entities[change.entity_id.id].pointer = std::make_unique<Ship>(change.value.Vector2f, change.entity_id, change.entity_id);
                break;
            case EntityType::Organization:
                break;
            case EntityType::Planet:
                entities[change.entity_id.id].pointer = std::make_unique<Planet>(change.value.Vector2f, change.entity_id, change.entity_id);
                break;
        }

        INFO_PRINT("new entity (" + std::to_string(change.entity_id.id) + ":" + std::to_string(change.entity_id.revision) + ")");
    }

    void kill_entity(EntityID id) {
        if (entities[id.id].revision != id.revision)
            WARNING_PRINT("wrong revision number in id when killing entity");
        entities[id.id].pointer.reset();
        entities[id.id].revision++;
    }

    EntityID get_unused_id() {
        for (std::size_t i=0; i<entities.size(); i++) {
            if (!entities[i].pointer)
                return {static_cast<std::uint16_t>(i), entities[i].revision};
        }
        ERROR_PRINT("All entity slots used. Have a good day !");
        throw "you off a cliff";
        return {};
    }

    std::array<EntityEntry,0xffff> entities;
};