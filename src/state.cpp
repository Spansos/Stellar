#include "state.hpp"

// void GameState::perform_action(Action action) {
//     switch (action.type) {
//         case Action::ActionType::KILL:
//             entities.erase(entities.begin()+action.kill.entity_id);
//             break;
//         case Action::ActionType::MAKE:
//             entities.push_back(action.make.position);
//             break;
//         case Action::ActionType::MOVE:
//             entities[action.move.entity_id] += action.move.movement;
//             break;
//     }
// }