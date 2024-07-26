#include "rendering/ui.hpp"

#include <limits>
#include <cmath>
#include "common.hpp"

Ui::Ui(sf::RenderWindow & window) : window(window) { }

void Ui::draw_button(sf::FloatRect box, sf::Color base_color, sf::Color highlighted_color, sf::Color pressed_color) {
    sf::FloatRect global_box = local_to_global_bounds(box);
    if (global_box.contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))))
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            draw_box(box, pressed_color);
        else
            draw_box(box, highlighted_color);
    else
        draw_box(box, base_color);
}

bool Ui::is_button_pressed(sf::FloatRect box, const std::vector<sf::Event> & events) {
    for (const auto & event : events) {
        if (event.type == sf::Event::MouseButtonReleased) {
            auto global_box = local_to_global_bounds(box);
            if (global_box.contains({static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)}))
                return true;
        }
    }
    return false;
}


void Ui::draw_box(sf::FloatRect box, sf::Color box_color, sf::Color outline_color, float outline_thickness) {
    sf::RectangleShape rect;

    sf::FloatRect global_box = local_to_global_bounds(box);

    rect.setPosition(global_box.getPosition());
    rect.setSize(global_box.getSize());
    rect.setFillColor(box_color);
    rect.setOutlineColor(outline_color);
    rect.setOutlineThickness(outline_thickness);

    window.draw(rect);
}

void Ui::draw_text(sf::FloatRect box, float text_size, std::string string, sf::Color text_color) {
    sf::FloatRect global_box = local_to_global_bounds(box);

    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/TTF/DejaVuSans.ttf"))
        ERROR_PRINT("OOOGA BOOGA CANNOT READ. ILLITERATE");

    sf::Text text{"", font, static_cast<unsigned int>(text_size*std::sqrt(global_box.width*global_box.height))};
    text.setPosition(global_box.getPosition());
    text.setFillColor(text_color);

    std::string disp_text;
    for (std::size_t i=0; i<string.size(); i++) {
        disp_text += string[i];
        text.setString(disp_text);
        if (text.findCharacterPos(std::numeric_limits<std::size_t>::max()).x > global_box.left+global_box.width) {
            disp_text.pop_back();
            disp_text += "\n";
            disp_text += string[i];
            text.setString(disp_text);
            if (text.getGlobalBounds().height > global_box.height) {
                disp_text.pop_back();
                disp_text.pop_back();
                break;
            }
        }
    }
    text.setString(disp_text);

    window.draw(text);
};

void Ui::start_frame(sf::FloatRect box) {
    bounds_stack.push_back(box);
}

void Ui::end_frame() {
    bounds_stack.pop_back();
}

sf::Vector2f Ui::pixel_to_ui_coords(sf::Vector2f position) {
    float window_factor = std::min(window.getSize().x,window.getSize().y);
    return position/window_factor;
}

sf::FloatRect Ui::pixel_to_ui_rect(sf::FloatRect rect) {
    float window_factor = std::min(window.getSize().x,window.getSize().y);
    return {
        rect.getPosition()/window_factor,
        rect.getSize()/window_factor,
    };
}

sf::FloatRect Ui::local_to_global_bounds(sf::FloatRect bounds) {
    sf::FloatRect r = {0, 0, 1, 1};

    for (sf::FloatRect parent_bounds : bounds_stack) {
        r.left   += parent_bounds.left * r.width;
        r.top    += parent_bounds.top  * r.height;
        r.width  *= parent_bounds.width;
        r.height *= parent_bounds.height;
    }

    r.left   += bounds.left * r.width;
    r.top    += bounds.top  * r.height;
    r.width  *= bounds.width;
    r.height *= bounds.height;

    float window_factor = std::min(window.getSize().x,window.getSize().y);
    r.left   *= window_factor;
    r.top    *= window_factor;
    r.width  *= window_factor;
    r.height *= window_factor;

    return r;
}