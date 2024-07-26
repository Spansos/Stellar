#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Ui {
public:
    Ui(sf::RenderWindow & window);
    void draw_button(sf::FloatRect box, sf::Color base_color, sf::Color highlighted_color, sf::Color pressed_color);
    bool is_button_pressed(sf::FloatRect box, const std::vector<sf::Event> & events);
    void draw_box(sf::FloatRect box, sf::Color box_color, sf::Color outline_color={}, float outline_thickness=0);
    void draw_text(sf::FloatRect box, float text_size, std::string text_string, sf::Color text_color);
    void start_frame(sf::FloatRect box);
    void end_frame();
    sf::Vector2f pixel_to_ui_coords(sf::Vector2f position);
    sf::FloatRect pixel_to_ui_rect(sf::FloatRect rect);
private:
    sf::FloatRect local_to_global_bounds(sf::FloatRect bounds);
    std::vector<sf::FloatRect> bounds_stack;
    sf::RenderWindow & window;
};