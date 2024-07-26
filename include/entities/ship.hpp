#pragma once

#include <deque>
#include "entities/entity.hpp"

class Ship : public Entity {
public:
    Ship(sf::Vector2f position, EntityID id, EntityID owner);
    UpdateResult process_order(Order & order, GameState * state) override;
    void update_state(StateChange change) override;
    PropertyValue get_property(PropertyID property) override;
    UpdateResult update(GameState * game_state, float delta_time) override;
    enum Properties {SPEED, /*VELOCITY,*/ POSITION, DESTINATION};
private:
    float speed = 100;
    sf::Vector2f velocity = {};
    sf::Vector2f position;
    std::deque<sf::Vector2f> destinations = {};
};